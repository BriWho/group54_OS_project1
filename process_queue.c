#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/queue.h>
#include "process.h"
#include "process_queue.h"

TAILQ_HEAD(process_head , entry) head;
struct entry{
	int index;
	struct process* process_p;
	TAILQ_ENTRY(entry) entries;
} *np;

int is_queue_empty(){ return head.tqh_first == NULL; }

void init_queue(struct process* process_p, int n_size){
	TAILQ_INIT(&head);
	np = (struct entry*)malloc(sizeof(struct entry) * n_size);
	for(int i = 0 ; i < n_size ; i++){
		np[i].process_p = &process_p[i];
		np[i].index = i;
	}
}

void free_queue(){
	free(np);
}

int process_enqueue(int process_index){
	TAILQ_INSERT_TAIL(&head , &np[process_index] , entries);
	return 0;
}

int process_dequeue(){
	int process_index = head.tqh_first -> index;
	TAILQ_REMOVE(&head , head.tqh_first , entries);
	return process_index;
}

int low_exec_prior_enqueue(int process_index){
	struct entry* prev = NULL;
	for(struct entry* p = head.tqh_first; p != NULL ; prev = p , p = p -> entries.tqe_next)
		if(p -> process_p -> r_exec_time > np[process_index].process_p -> r_exec_time)
			break;
	if(prev == NULL)
		TAILQ_INSERT_HEAD(&head , &np[process_index] , entries);
	else
		TAILQ_INSERT_AFTER(&head, prev ,  &np[process_index] , entries);
	return 0;
}

/*
void queue_init(Queue* q , int n_element , size_t size){
	q -> elements = malloc(size * (n_element + 1));
	q -> tail = q -> head = 0;
	q -> n_element = n_element;
	q -> size = size;
}

int is_queue_empty(Queue* q){ return q -> head == q -> tail; } 
int is_queue_full(Queue* q){ return (q -> head + 1)%(q -> n_element + 1) == q -> tail; }


int cyclic_enqueue(Queue* q , void* element){
	if(!is_queue_full(q)){
		memcpy(q -> elements + q -> size * q -> head , element , q -> size);
		q -> head = (q -> head + 1)%(q -> n_element + 1);
		return 1;
	}
	return 0;
}

void* cyclic_dequeue(Queue* q){
	if(!is_queue_empty(q)){
		void* ret = q -> elements + q -> size * q -> tail;
		q -> tail = (q -> tail + 1)%(q -> n_element + 1);
		return ret;
	}
	return NULL;
}

int prior_enqueue(Queue* q , void* element , int (*cmp)(const void* , const void*)){
#define swap(a , b) { void* }
	if(cyclic_enqueue(q , element)){
		int t_head = q -> head;
		while(th)
		return 1;
	}
	return 0;
}
*/
