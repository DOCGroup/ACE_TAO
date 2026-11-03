//=============================================================================
/**
 *  @file    ClientTask.h
 *
 *  @author  Tim Bradley <bradley_t@ociweb.com>
 */
//=============================================================================

#ifndef CLIENT_TASK_H
#define CLIENT_TASK_H

#include "FooC.h"
#include "CallbackC.h"
#include "ace/Task.h"

class ClientTask : public ACE_Task_Base
{
public:
  ClientTask(CORBA::ORB_ptr orb,
              Foo_ptr foo,
              Callback_ptr callback,
              bool         collocated = false);

  ~ClientTask () override;

  int open(void* x = 0) override;
  int svc() override;
  int close(u_long) override;

private:
  bool validate_connection ();

  CORBA::ORB_var orb_;
  Foo_var foo_;
  Callback_var callback_;
  bool collocated_;
};

#endif
