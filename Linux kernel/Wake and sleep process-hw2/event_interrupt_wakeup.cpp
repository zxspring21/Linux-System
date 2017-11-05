project2:

wait_event_interruptible(wq, condition) /*等效沒有考慮返回值*/
{
         if (!(condition))
             {
                     wait_queue_t _ _wait;
                     init_waitqueue_entry(&_ _wait, current);
                     add_wait_queue(&wq, &_ _wait);
                     for (;;)
                         {
                                set_current_state(TASK_INTERRUPTIBLE);
                                if (condition)
                                    break;
                                schedule();  /* implicit call: del_from_runqueue(current)*/
                             }
                     current->state = TASK_RUNNING;
                     remove_wait_queue(&wq, &_ _wait);
                  }
}

void wake_up(wait_queue_head_t *q)
{
          struct list_head *tmp;
          wait_queue_t *curr;
          list_for_each(tmp, &q->task_list)
          { 
                curr = list_entry(tmp, wait_queue_t, task_list); 
                wake_up_process(curr->task); 
                /* implicit call: add_to_runqueue(curr->task);*/ 
                if (curr->flags) 
                      break; 
              } 
} 

