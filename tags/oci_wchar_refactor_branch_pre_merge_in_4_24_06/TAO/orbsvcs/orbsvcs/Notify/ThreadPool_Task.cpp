// $Id$

#include "ThreadPool_Task.h"

ACE_RCSID(Notify, TAO_Notify_ThreadPool_Task, "$Id$")

#include "Properties.h"
#include "Timer_Queue.h"
#include "Buffering_Strategy.h"

#include "tao/debug.h"
#include "tao/ORB_Core.h"

#include "ace/OS_NS_errno.h"

TAO_Notify_ThreadPool_Task::TAO_Notify_ThreadPool_Task (void)
  : shutdown_ (false)
  , shutdown_handler_(this)
{
}

TAO_Notify_ThreadPool_Task::~TAO_Notify_ThreadPool_Task ()
{
}

int
TAO_Notify_ThreadPool_Task::init (int argc, ACE_TCHAR **argv)
{
  return this->ACE_Task<ACE_NULL_SYNCH>::init (argc, argv);
}

TAO_Notify_Timer*
TAO_Notify_ThreadPool_Task::timer (void)
{
  return this->timer_.get();
}

void
TAO_Notify_ThreadPool_Task::init (const NotifyExt::ThreadPoolParams& tp_params, TAO_Notify_AdminProperties::Ptr& admin_properties  ACE_ENV_ARG_DECL)
{
  ACE_ASSERT (this->timer_.get() == 0);

  TAO_Notify_Timer_Queue* timer = 0;
  ACE_NEW_THROW_EX (timer,
                    TAO_Notify_Timer_Queue (),
                    CORBA::NO_MEMORY ());
  ACE_CHECK;
  this->timer_.reset (timer);


  TAO_Notify_Buffering_Strategy* buffering_strategy = 0;
  ACE_NEW_THROW_EX (buffering_strategy,
                    TAO_Notify_Buffering_Strategy (*msg_queue (), admin_properties),
                    CORBA::NO_MEMORY ());
  this->buffering_strategy_.reset (buffering_strategy);
  ACE_CHECK;

  long flags = THR_NEW_LWP | THR_JOINABLE;

  CORBA::ORB_var orb =
    TAO_Notify_PROPERTIES::instance()->orb ();

  flags |=
    orb->orb_core ()->orb_params ()->thread_creation_flags ();

  // Guards the thread for auto-deletion; paired with close.
  // This is done in the originating thread before the spawn to
  // avoid any race conditions.
  for ( CORBA::ULong i = 0; i < tp_params.static_threads; ++i )
  {
    this->_incr_refcnt();
  }

  // Become an active object.
  if (this->ACE_Task <ACE_NULL_SYNCH>::activate (flags,
                                                 tp_params.static_threads,
                                                 0,
                                                 ACE_THR_PRI_OTHER_DEF) == -1)
    {
      // Undo the ref counts on error
      for ( CORBA::ULong i = 0; i < tp_params.static_threads; ++i )
      {
        this->_decr_refcnt();
      }

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
}

void
TAO_Notify_ThreadPool_Task::execute (TAO_Notify_Method_Request& method_request ACE_ENV_ARG_DECL)
{
  if (!shutdown_)
  {
    TAO_Notify_Method_Request_Queueable& request_copy = *method_request.copy (ACE_ENV_SINGLE_ARG_PARAMETER);

    // Ignore the return value, because we already print an error
    // message if enqueing fails.
    this->buffering_strategy_->enqueue (request_copy);
  }
}

int
TAO_Notify_ThreadPool_Task::svc (void)
{
  TAO_Notify_Method_Request_Queueable* method_request;

  while (!shutdown_)
    {
      ACE_TRY_NEW_ENV
        {
          ACE_Time_Value* dequeue_blocking_time = 0;
          ACE_Time_Value earliest_time;

          if (!this->timer_->impl().is_empty ())
            {
              earliest_time = this->timer_->impl().earliest_time ();
              dequeue_blocking_time = &earliest_time;
            }

          // Dequeue 1 item
          int result = buffering_strategy_->dequeue (method_request, dequeue_blocking_time);

          if (result > 0)
            {
              method_request->execute (ACE_ENV_SINGLE_ARG_PARAMETER);
              ACE_TRY_CHECK;

              ACE_Message_Block::release (method_request);
            }
          else if (errno == ETIME)
            {
              this->timer_->impl ().expire ();
            }
          else if (result == -1)
            {
              if (TAO_debug_level > 0)
                ACE_DEBUG ((LM_DEBUG, "ThreadPool_Task dequeue failed\n"));
            }
        }
      ACE_CATCHANY
        {
          ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                               "ThreadPool_Task (%P|%t) exception in method request\n");
        }
      ACE_ENDTRY;
    } /* while */

  return 0;
}

void
TAO_Notify_ThreadPool_Task::shutdown (void)
{
  if (this->shutdown_)
  {
  	return;
  }

  this->shutdown_ = true;

  this->buffering_strategy_->shutdown ();

  // be sure this object is not deleted until wait() returns
  this->_incr_refcnt ();

  // get another thread to wait() for the thread(s) running svc() to exit
  // otherwise the thread is a zombie on Solaris and just hangs around
  // on windows.
  TAO_Notify_PROPERTIES::instance()
           ->orb ()->orb_core ()->reactor ()->notify (&shutdown_handler_);
}

void
TAO_Notify_ThreadPool_Task::release (void)
{
  delete this;
}

int
TAO_Notify_ThreadPool_Task::close (u_long /*flags*/)
{
  // Undo the thread spawn guard. close is called per thread spawned.
  this->_decr_refcnt();
  return 0;
}

void
TAO_Notify_ThreadPool_Task::wait_for_shutdown ()
{
  // wait for thread(s) running svc() to return.
  this->wait ();

  // Undo the shutdown request guard.
  this->_decr_refcnt ();
}

void
TAO_Notify_ThreadPool_Task::update_qos_properties (const TAO_Notify_QoSProperties& qos_properties)
{
  this->buffering_strategy_->update_qos_properties (qos_properties);
}
