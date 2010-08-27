// $Id$

#include "orbsvcs/Notify/Method_Request_Shutdown.h"
#include "orbsvcs/Notify/ThreadPool_Task.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Notify_Method_Request_Shutdown::TAO_Notify_Method_Request_Shutdown (TAO_Notify_ThreadPool_Task* task)
  : task_ (task)
{
}

TAO_Notify_Method_Request_Shutdown::~TAO_Notify_Method_Request_Shutdown ()
{
}

TAO_Notify_Method_Request_Queueable*
TAO_Notify_Method_Request_Shutdown::copy (void)
{
  return new TAO_Notify_Method_Request_Shutdown (this->task_);
}

int
TAO_Notify_Method_Request_Shutdown::execute (void)
{
  // Shut down the scheduler by deactivating the activation queue's
  // underlying message queue - should pop all worker threads off their
  // wait and they'll exit.
  this->task_->msg_queue ()->deactivate ();
  return -1;
}

TAO_END_VERSIONED_NAMESPACE_DECL
