// $Id$

#include "Method_Request_Shutdown.h"
#include "ThreadPool_Task.h"

#if ! defined (__ACE_INLINE__)
#include "Method_Request_Shutdown.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(RT_Notify, TAO_NS_Method_Request_Shutdown, "$Id$")

TAO_NS_Method_Request_Shutdown::TAO_NS_Method_Request_Shutdown (TAO_NS_ThreadPool_Task* task)
  : task_ (task)
{
}

TAO_NS_Method_Request_Shutdown::~TAO_NS_Method_Request_Shutdown ()
{
}

int
TAO_NS_Method_Request_Shutdown::call (void)
{
  // Shut down the scheduler by deactivating the activation queue's
  // underlying message queue - should pop all worker threads off their
  // wait and they'll exit.
  this->task_->msg_queue ()->deactivate ();
  return -1;
}
