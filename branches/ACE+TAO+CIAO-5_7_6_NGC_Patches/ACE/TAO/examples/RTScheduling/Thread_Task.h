//$Id$
#ifndef THREAD_TASK_H
#define THREAD_TASK_H

#include "JobC.h"
#include "rtschedtestlib_export.h"

#include "tao/RTScheduling/RTScheduler.h"
#include "ace/Task.h"

class Task_Stats;
class DT_Creator;

class RTSCHEDTESTLIB_Export Thread_Task : public ACE_Task <ACE_SYNCH>
{
 public:

  Thread_Task (void);

  virtual int activate_task (RTScheduling::Current_ptr current,
                             CORBA::Policy_ptr sched_param,
                             long flags,
                             ACE_Time_Value* base_time) = 0;

  virtual int perform_task (void);

  int importance (void);

  time_t start_time (void);

  /// = Job get/set
  /// Returns the name of the Job exec'ed by this Task.
  const char* job (void);

  /// Sets the Job to exec.
  void job (Job_ptr job);

  int dist (void);

  void dump_stats (void);

 protected:
  /// task svc
  virtual int svc (void);
  RTScheduling::Current_var current_;
  CORBA::Policy_var sched_param_;
  time_t start_time_;
  int load_;
  int iter_;
  size_t count_;
  int importance_;
  DT_Creator *dt_creator_;
  //Task *task_;
  ACE_Time_Value* base_time_;
  int dist_;
  char *job_name_;
  Job_var job_;
  Task_Stats *task_stats_;
};

#endif /* THREAD_TASK_H */
