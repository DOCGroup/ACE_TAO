// $Id$

#include "ThreadPool_Task.h"

#if ! defined (__ACE_INLINE__)
#include "ThreadPool_Task.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(RT_Notify, TAO_NS_ThreadPool_Task, "$Id$")

#include "tao/debug.h"
#include "Properties.h"
#include "Method_Request_Shutdown.h"
#include "AdminProperties.h"

TAO_NS_ThreadPool_Task::TAO_NS_ThreadPool_Task (void)
  : msg_queue_ (*msg_queue ()), buffering_strategy_ (0), queue_length_ (0)
{
}

TAO_NS_ThreadPool_Task::~TAO_NS_ThreadPool_Task ()
{
  delete this->buffering_strategy_;
}

int
TAO_NS_ThreadPool_Task::init (int argc, char **argv)
{
  return this->ACE_Task<ACE_SYNCH>::init (argc, argv);
}

void
TAO_NS_ThreadPool_Task::init (TAO_NS_AdminProperties& admin_properties)
{
  TAO_NS_Worker_Task::init (admin_properties);
}

void
TAO_NS_ThreadPool_Task::init (const NotifyExt::ThreadPoolParams& tp_params, TAO_NS_AdminProperties& admin_properties  ACE_ENV_ARG_DECL)
{
  long flags = THR_SCHED_DEFAULT|THR_BOUND|THR_NEW_LWP;

  flags |=
    TAO_NS_PROPERTIES::instance()->scope_policy () |
    TAO_NS_PROPERTIES::instance()->sched_policy ();

  // Become an active object.
  if (this->ACE_Task <ACE_SYNCH>::activate (flags,
                                            tp_params.static_threads,
                                            0,
                                            ACE_THR_PRI_OTHER_DEF) == -1)
    {
      if (TAO_debug_level > 0)
        {
          if (ACE_OS::last_error () == EPERM)
            ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Insufficient privilege.\n")));
          else
            ACE_DEBUG ((LM_ERROR,
                        ACE_TEXT ("(%t) task activation at priority %d failed\n")
                        ACE_TEXT ("exiting!\n%a"),
                        tp_params.default_priority));
        }

        ACE_THROW (CORBA::BAD_PARAM ());
    }

  // Store the admin properties...
  this->queue_length_ = &admin_properties.queue_length ();

  ACE_NEW_THROW_EX (this->buffering_strategy_,
                                        TAO_NS_Buffering_Strategy (this->msg_queue_, admin_properties),
                                        CORBA::NO_MEMORY ());
}

void
TAO_NS_ThreadPool_Task::exec (TAO_NS_Method_Request& method_request)
{
  TAO_NS_Method_Request& request_copy = *method_request.copy ();

  ACE_Time_Value tv;
  this->buffering_strategy_->execute (request_copy, &tv);
}

int
TAO_NS_ThreadPool_Task::svc (void)
{
  int done = 0;
  while (!done)
    {
      ACE_TRY_NEW_ENV
        {
          ACE_Message_Block *mb;
          if (this->getq (mb) == -1)
            if (ACE_OS::last_error () == ESHUTDOWN)
              return 0;
            else
              ACE_ERROR ((LM_ERROR,
                          "EC (%P|%t) getq error in Dispatching Queue\n"));

          // Decrement the global event count.
          (*this->queue_length_)--;

          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG, "removing from queue, queue_length = %d\n",this->queue_length_->value () ));
          TAO_NS_Method_Request *request =
            ACE_dynamic_cast (TAO_NS_Method_Request*, mb);

          int result = 0;

          if (request != 0)
            {
              result = request->execute (ACE_ENV_SINGLE_ARG_PARAMETER);
              ACE_TRY_CHECK;
            }

          ACE_Message_Block::release (mb);

          if (result == -1)
            done = 1;
        }
      ACE_CATCHANY
        {
          ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                               "EC (%P|%t) exception in dispatching queue");
        }
      ACE_ENDTRY;
    }
  return 0;
}

void
TAO_NS_ThreadPool_Task::shutdown (void)
{
  this->msg_queue_.enqueue (new TAO_NS_Method_Request_Shutdown (this));

  // We can not wait for ourselves to quit
  if (this->thr_mgr ())
    {
      // call this->thr_mgr ()->task () in the main thread will assert ()
      // fail in ACE_Thread_Manager::thread_desc_self (void) so I get
      // task this way.
      ACE_Thread_Descriptor *mydesc = this->thr_mgr ()->thread_descriptor (ACE_OS::thr_self ());

      if (mydesc && mydesc->task () == this)
        return;
    }

  this->wait ();
  return;
}
