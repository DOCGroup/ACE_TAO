//$Id$
#ifndef THREAD_TASK_H
#define THREAD_TASK_H

#include "tao/RTScheduling/RTSchedulerC.h"
#include "ace/Task.h"


class Thread_Task : public ACE_Task <ACE_SYNCH>
{
 public:

  Thread_Task (RTScheduling::Current_ptr current,
	       CORBA::Policy_ptr sched_param,
	       int start_time,
	       int load,
	       long flags,
	       ACE_Barrier* barrier);
  
  int perform_task (int times);

  int activate_task (void);


 protected:
  /// task svc
  virtual int svc (void);
 private:
  RTScheduling::Current_var current_;
  CORBA::Policy_var sched_param_;
  int start_time_;
  ACE_Barrier* barrier_;
};

#endif /* THREAD_TASK_H */
