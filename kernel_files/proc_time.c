#include <linux/linkage.h>
#include <linux/kernel.h>
#include <linux/ktime.h>
#include <linux/timekeeping.h>

asmlinkage long sys_proc_time(pid_t pid , struct timespec* start, int f)
{
	struct timespec end;
	if(f == 0) getnstimeofday(start);
	else{
		getnstimeofday(&end);
		printk("[Project1] %ld %ld.%ld %ld.%ld\n" , (long)pid ,start-> tv_sec , start -> tv_nsec ,  end.tv_sec , end.tv_nsec );
	}
	return 0;

}
