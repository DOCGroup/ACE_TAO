//$Id$
#ifndef THREAD_TASK_H
#define THREAD_TASK_H

#include "tao/RTScheduling/RTSchedulerC.h"
#include "ace/Task.h"


class Thread_Task : public ACE_Task <ACE_SYNCH>
{
 public:
  int activate_task (CORBA::ORB_ptr orb,
		     int thr_count);

 protected:
  /// task svc
  virtual int svc (void);
 private:
  CORBA::ORB_var orb_;
 RTScheduling::Current_var current_;
 int active_thread_count_;
 ACE_Lock* shutdown_lock_;
};

#endif /* THREAD_TASK_H */
