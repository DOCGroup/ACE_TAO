//$Id$
#ifndef THREAD_TASK_H
#define THREAD_TASK_H

#include "tao/RTScheduling/RTSchedulerC.h"
#include "FP_Scheduler.h"
#include "ace/Task.h"


class Thread_Task : public ACE_Task <ACE_SYNCH>
{
 public:

  Thread_Task (RTScheduling::Current_ptr current,
	       FP_Scheduling::SegmentSchedulingParameterPolicy_ptr sched_param,
	       int start_time,
	       int load,
	       long flags);
  
  int perform_task (int times);

  int activate_task (ACE_Barrier* barrier);


 protected:
  /// task svc
  virtual int svc (void);
 private:
  RTScheduling::Current_var current_;
  FP_Scheduling::SegmentSchedulingParameterPolicy_var sched_param_;
  int start_time_;
  int load_;
  int count_;
  ACE_Barrier* barrier_;
};

#endif /* THREAD_TASK_H */
