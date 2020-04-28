#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/syscall.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <time.h>
#include <assert.h>
#include <sched.h>
#include "process.h"
#include "process_queue.h"

#define QUANTUM 500
const char* schedulers[4] = { "FIFO" , "PSJF" , "RR" , "SJF" };
enum SCHEDULERS { FIFO , PSJF , RR , SJF };

int assign_proc2cpu(pid_t pid , int cpu){
	cpu_set_t cpu_set;
	CPU_ZERO(&cpu_set);
	CPU_SET(cpu , &cpu_set);
	return sched_setaffinity(pid , sizeof(cpu_set_t) , &cpu_set);
}

int main(int argc, char* argv[]){

	char S[8];
	int N;
	int scheduling = -1;

	scanf("%s%d" ,S , &N );
	struct process* process_list = (struct process*)malloc(sizeof(struct process) * N);
	for(int i = 0 ; i < N ; i++)
		scanf("%s%d%d" , process_list[i].proc_name , &process_list[i].start_time , &process_list[i].r_exec_time);
	for(int i = 0 ; i < 4 ; i++)
		if(strcmp(S , schedulers[i]) == 0)
			scheduling = i;

	assert(assign_proc2cpu(0 , 0) == 0);

	int start_index = 0;
	int end_proc_cnt = 0;
	int timer = 0, rr_timer = 0;
	int running_index = -1;

	init_queue(process_list , N);
	int (*enqueue)(int) = (scheduling == PSJF || scheduling == SJF)? low_exec_prior_enqueue:process_enqueue;
	int (*dequeue)() = process_dequeue;

	while(end_proc_cnt < N){
		while(start_index < N && process_list[start_index].start_time == timer){

			pid_t pid = fork();
			if(pid == 0){
				struct timespec start;
				syscall(333 , getpid() , &start , 0 );

				MULTI_UNIT_TIME(process_list[start_index].r_exec_time);

				syscall(333 , getpid() , &start , 1);
				printf("%s %ld\n" , process_list[start_index].proc_name , (long)getpid());
				return 0;
			}
			assert(assign_proc2cpu(pid , 1) == 0);

			process_list[start_index].pid = pid;
			(*enqueue)(start_index);
			PROCESS_IDLE(pid);
			start_index++;
			if(scheduling == PSJF){  	//PSJF
				if(running_index > -1){
					(*enqueue)(running_index);
					PROCESS_IDLE(process_list[running_index].pid);
					running_index = -1;
				}
			}
		}

		if(scheduling == RR){   //RR
			if(running_index > -1 && rr_timer >= QUANTUM){
				(*enqueue)(running_index);
				PROCESS_IDLE(process_list[running_index].pid);
				//printf("%s %d\n" , process_list[running_index].proc_name , process_list[running_index].r_exec_time);			
				running_index = -1;
			}
		}

		if(running_index < 0 && !is_queue_empty()){
			running_index = (*dequeue)();
			PROCESS_RUN(process_list[running_index].pid);
			rr_timer = 0;
			//printf("%s %d\n" , process_list[running_index].proc_name , process_list[running_index].r_exec_time);
		}

		UNIT_TIME();

		if(running_index > -1){
			process_list[running_index].r_exec_time --;
			if(process_list[running_index].r_exec_time == 0){
				running_index = -1;
				end_proc_cnt++;
				waitpid(process_list[running_index].pid , NULL , 0);
			}
		}
		timer++ , rr_timer++;;
	}

	free_queue();	
	free(process_list);
	return 0;
}

