// $Id$

#include "Method_Request_Shutdown.h"
#include "ThreadPool_Task.h"

#if ! defined (__ACE_INLINE__)
#include "Method_Request_Shutdown.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Notify, TAO_Notify_Method_Request_Shutdown, "$Id$")

TAO_Notify_Method_Request_Shutdown::TAO_Notify_Method_Request_Shutdown (TAO_Notify_ThreadPool_Task* task)
  : task_ (task)
{
}

TAO_Notify_Method_Request_Shutdown::~TAO_Notify_Method_Request_Shutdown ()
{
}

TAO_Notify_Method_Request*
TAO_Notify_Method_Request_Shutdown::copy (void)
{
  return new TAO_Notify_Method_Request_Shutdown (this->task_);
}

int
TAO_Notify_Method_Request_Shutdown::execute (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  // Shut down the scheduler by deactivating the activation queue's
  // underlying message queue - should pop all worker threads off their
  // wait and they'll exit.
  this->task_->msg_queue ()->deactivate ();
  return -1;
}
