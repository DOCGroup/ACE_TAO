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

  ~ClientTask () override = default;

  ClientTask (ClientTask const &) = delete;
  ClientTask (ClientTask &&) = delete;
  ClientTask &operator= (ClientTask const &) = delete;
  ClientTask &operator= (ClientTask &&) = delete;

  int open (void * = nullptr) override;
  int svc() override;
  int close(u_long) override;

private:
  bool validate_connection () const;

  CORBA::ORB_var const orb_;
  Foo_var const foo_;
  Callback_var const callback_;
  bool const collocated_;
};

#endif
