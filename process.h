
struct process{
	char proc_name[32];
	int start_time;
	int r_exec_time;
	pid_t pid;
};

#define UNIT_TIME() { \
	volatile unsigned long i; \
	for(i = 0 ; i < 1000000UL ; i++); \
}

#define MULTI_UNIT_TIME(N) { \
	volatile unsigned long i; \
	for(i = 0 ; i < N ; i++) UNIT_TIME() \
}

#define PROCESS_IDLE(pid) { \
	struct sched_param param = { .sched_priority = 0 }; \
	sched_setscheduler(pid , SCHED_IDLE ,&param ); \
}

#define PROCESS_RUN(pid) { \
	struct sched_param param = { .sched_priority = 0 }; \
	sched_setscheduler(pid , SCHED_OTHER , &param);\
}
