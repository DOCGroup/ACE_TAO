//$Id$
#ifndef THREAD_TASK_H
#define THREAD_TASK_H

#include "tao/RTScheduling/RTSchedulerC.h"
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
};

#endif /* THREAD_TASK_H */
