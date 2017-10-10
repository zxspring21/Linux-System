#include <linux/linkage.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/spinlock.h>
#include <linux/list.h>


extern wait_queue_head_t my_wait_queue;
extern int my_wait_queue_flags;

static int __wake_up_common_own(wait_queue_head_t* q, unsigned int mode, 
				int nr_exclusive, int wake_flags, void* key, pid_t pid){
	wait_queue_t *curr, *next;
	struct task_struct* process_descriptor;
	int exist = -1;	
	//printk(KERN_EMERG "start wake up\n");
	list_for_each_entry_safe(curr, next, &q->task_list, task_list){
		//unsigned flags = curr->flags;
		process_descriptor = (struct task_struct*)curr->private;
		printk(KERN_EMERG "stop pid = %d, tgid=%d, pid=%d\n", pid, process_descriptor->tgid, process_descriptor->pid);
		if(process_descriptor->tgid == pid && curr->func(curr, mode, wake_flags, key)){
			exist = 1;
			break;
		}
	}
	//printk(KERN_EMERG "end wake up\n");
	return exist;
}

int __wake_up_own(wait_queue_head_t* q, unsigned int mode, int nr_exclusive, void* key, pid_t pid){
	unsigned long flags;	
	int exist;	
	//printk(KERN_EMERG "start wake up\n");
	spin_lock_irqsave(&q->lock, flags);
	exist =	__wake_up_common_own(q, mode, nr_exclusive, 0, key, pid);
	spin_unlock_irqrestore(&q->lock, flags);
	//printk(KERN_EMERG "end wake up\n");
	return exist;
}

asmlinkage int sys_wake_up_my_process(int pid){
	int exist;
	pid_t wake_up_pid = (pid_t)pid;
	my_wait_queue_flags = 1;
	
	printk(KERN_EMERG "start wake up\n");
	exist = __wake_up_own(&my_wait_queue, TASK_UNINTERRUPTIBLE, 0, NULL, wake_up_pid);	
	printk(KERN_EMERG "end wake up\n");
	return exist;
}
