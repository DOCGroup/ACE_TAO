//$Id$
#ifndef THREAD_TASK_H
#define THREAD_TASK_H

#include "tao/RTScheduling/RTSchedulerC.h"
#include "FP_Scheduler.h"
#include "ace/Task.h"


class Thread_Task : public ACE_Task <ACE_SYNCH>
{
 public:

  Thread_Task (int importance,
	       int start_time,
	       int load);
  
  int perform_task (int times);

  int activate_task (RTScheduling::Current_ptr current,
		     long flags,
		     ACE_Barrier* barrier);


 protected:
  /// task svc
  virtual int svc (void);
 private:
  RTScheduling::Current_var current_;
  int start_time_;
  int load_;
  int count_;
  ACE_Barrier* barrier_;
    int importance_;
};

#endif /* THREAD_TASK_H */
