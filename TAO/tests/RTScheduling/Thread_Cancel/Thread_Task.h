//$Id$
#ifndef THREAD_TASK_H
#define THREAD_TASK_H

#include "tao/RTScheduling/RTScheduler.h"
#include "ace/Task.h"


class Thread_Task : public ACE_Task <ACE_SYNCH>
{
 public:
  int activate_task (CORBA::ORB_ptr orb);

  RTScheduling::Current::IdType* guids (void);
  
 protected:
  /// task svc
  virtual int svc (void);
 private:
  CORBA::ORB_var orb_;
  RTScheduling::Current_var current_;
  RTScheduling::Current::IdType guid_ [4];
  ACE_Lock* shutdown_lock_;
  ACE_Lock* lock_;
  int active_thread_count_;
};

#endif /* THREAD_TASK_H */
