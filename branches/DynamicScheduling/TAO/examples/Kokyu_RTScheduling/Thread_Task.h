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

/*    Thread_Task (int importance, */
/*  	       int start_time, */
/*  	       int load, */
/*  	       DT_Creator *dt_creator); */

  Thread_Task (void);

  int activate_task (RTScheduling::Current_ptr current,
		     CORBA::Policy_ptr sched_param,
		     long flags,
		     ACE_Time_Value* base_time,
		     ACE_Barrier* barrier
		     ACE_ENV_ARG_DECL_WITH_DEFAULTS);

  virtual int perform_task (void);
  
  int importance (void);

  int start_time (void);

  virtual void pre_activate (void);
  virtual void post_activate (void);

 protected:
  /// task svc
  virtual int svc (void);
  RTScheduling::Current_var current_;
  CORBA::Policy_var sched_param_;
  int start_time_;
  int load_;
  int count_;
  ACE_Barrier* barrier_;
  int importance_;
  DT_Creator *dt_creator_;
  //Task *task_;
  ACE_Time_Value* base_time_;
};

#endif /* THREAD_TASK_H */
