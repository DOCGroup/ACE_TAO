//
// $Id$
//
#ifndef SERVER_TASK_H
#define SERVER_TASK_H

#include "ace/pre.h"

#include "tao/corba.h"
#include "ace/Task.h"

class Server_Task : public ACE_Task_Base
{
public:
  Server_Task (CORBA::ORB_ptr orb,
               ACE_Thread_Manager *thr_mgr);

  int svc (void);

private:
  CORBA::ORB_var orb_;
};

#include "ace/post.h"
#endif /* SERVER_TASK_H */
