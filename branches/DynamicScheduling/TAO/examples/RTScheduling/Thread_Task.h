//$Id$
#ifndef THREAD_TASK_H
#define THREAD_TASK_H

#include "tao/RTScheduling/RTScheduler.h"
//#include "FP_Scheduler.h"
#include "ace/Task.h"
#include "DT_Creator.h"
#include "Task.h"

class Task_Stats;
class DT_Creator;

class Thread_Task : public ACE_Task <ACE_SYNCH>
{
 public:

  Thread_Task (int importance,
	       int start_time,
	       int load,
	       DT_Creator *dt_creator,
	       Task *task);
  
  int activate_task (RTScheduling::Current_ptr current,
		     CORBA::Policy_ptr sched_param,
		     long flags,
		     ACE_Barrier* barrier
		     ACE_ENV_ARG_DECL_WITH_DEFAULTS);

  int importance (void);

  int start_time (void);

 protected:
  /// task svc
  virtual int svc (void);
 private:
  RTScheduling::Current_var current_;
  CORBA::Policy_var sched_param_;
  int start_time_;
  int load_;
  int count_;
  ACE_Barrier* barrier_;
  int importance_;
  DT_Creator *dt_creator_;
  Task *task_;
};

#endif /* THREAD_TASK_H */
