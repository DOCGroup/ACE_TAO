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
  ClientTask (Foo_ptr foo, Callback_ptr callback);

  ~ClientTask () override = default;

  ClientTask (ClientTask const &) = delete;
  ClientTask (ClientTask &&) = delete;
  ClientTask & operator= (ClientTask const &) = delete;
  ClientTask & operator= (ClientTask &&) = delete;

  int open (void * = nullptr) override;
  int svc () override;
  int close (u_long) override;

private:
  Foo_var foo_;
  Callback_var callback_;
};

#endif
