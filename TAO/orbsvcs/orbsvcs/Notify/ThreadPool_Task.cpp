// $Id$

#include "tao/debug.h"
#include "ThreadPool_Task.h"
#include "Properties.h"
#include "Method_Request_Shutdown.h"

#if ! defined (__ACE_INLINE__)
#include "ThreadPool_Task.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(RT_Notify, TAO_NS_ThreadPool_Task, "$Id$")

TAO_NS_ThreadPool_Task::TAO_NS_ThreadPool_Task (void)
 : activation_queue_ (msg_queue ())
{
}

TAO_NS_ThreadPool_Task::~TAO_NS_ThreadPool_Task ()
{
}

void
TAO_NS_ThreadPool_Task::init (NotifyExt::ThreadPoolParams* tp_params ACE_ENV_ARG_DECL)
{
  long flags = THR_NEW_LWP | THR_JOINABLE;

  flags |=
    TAO_NS_PROPERTIES::instance()->scope_policy () |
    TAO_NS_PROPERTIES::instance()->sched_policy ();

  // Become an active object.
  if (this->ACE_Task <ACE_SYNCH>::activate (flags,
                                            tp_params->static_threads,
                                            0,
                                            tp_params->default_priority) == -1)
    {
      if (TAO_debug_level > 0)
        {
          if (ACE_OS::last_error () == EPERM)
            ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Insufficient privilege.\n")));
          else
            ACE_DEBUG ((LM_ERROR,
                        ACE_TEXT ("(%t) task activation at priority %d failed\n")
                        ACE_TEXT ("exiting!\n%a"),
                        tp_params->default_priority));
        }

        ACE_THROW (CORBA::BAD_PARAM ());
    }
}

void
TAO_NS_ThreadPool_Task::exec (TAO_NS_Method_Request& method_request)
{
  /// Use Buffering Strategy

  TAO_NS_Method_Request* request_copy = method_request.copy ();

  this->activation_queue_.enqueue (request_copy);
}

int
TAO_NS_ThreadPool_Task::svc (void)
{
  for (;;)
    {
      // Dequeue the next method request (we use an auto pointer in
      // case an exception is thrown in the <call>).
      ACE_Method_Request *mo_p = this->activation_queue_.dequeue ();

      if (0 == mo_p)
        {
          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,
                        ACE_LIB_TEXT ("(%t) activation queue shut down\n")));
          break;
        }
      auto_ptr<ACE_Method_Request> mo (mo_p);

      // Call it.
      if(mo->call () == -1)
        break;

      // Destructor automatically deletes it.
    }

  return 0;
}


void
TAO_NS_ThreadPool_Task::shutdown (void)
{
  this->activation_queue_.enqueue (new TAO_NS_Method_Request_Shutdown (this));
  this->wait ();
  return;
}
