#include <linux/linkage.h>
#include <linux/kernel.h>
//#include <linux/wait.h>
#include <linux/sched.h>


wait_queue_head_t my_wait_queue;
int my_wait_queue_flags;
int my_wait_queue_head = 0;

asmlinkage void sys_go_to_sleep_now(void){
	if(my_wait_queue_head == 0) 
		init_waitqueue_head(&my_wait_queue);

	my_wait_queue_head = 1;

	my_wait_queue_flags = 0;
	printk(KERN_EMERG "process sleep now");
	wait_event(my_wait_queue, my_wait_queue_flags);
	printk(KERN_EMERG "process wake up now");
}
