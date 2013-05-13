// $Id$

#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/Notify/ThreadPool_Task.h"
#include "orbsvcs/Notify/Properties.h"
#include "orbsvcs/Notify/Timer_Queue.h"
#include "orbsvcs/Notify/Buffering_Strategy.h"

#include "tao/debug.h"
#include "tao/ORB_Core.h"

#include "ace/OS_NS_errno.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Notify_ThreadPool_Task::TAO_Notify_ThreadPool_Task (void)
: shutdown_ (false)
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

TAO_Notify_Buffering_Strategy*
TAO_Notify_ThreadPool_Task::buffering_strategy (void)
{
  return this->buffering_strategy_.get ();
}

void
TAO_Notify_ThreadPool_Task::init (const NotifyExt::ThreadPoolParams& tp_params,
                                  const TAO_Notify_AdminProperties::Ptr& admin_properties)
{
  ACE_ASSERT (this->timer_.get() == 0);

  TAO_Notify_Timer_Queue* timer = 0;
  ACE_NEW_THROW_EX (timer,
                    TAO_Notify_Timer_Queue (),
                    CORBA::NO_MEMORY ());
  this->timer_.reset (timer);

  TAO_Notify_Buffering_Strategy* buffering_strategy = 0;
  ACE_NEW_THROW_EX (buffering_strategy,
                    TAO_Notify_Buffering_Strategy (*msg_queue (), admin_properties),
                    CORBA::NO_MEMORY ());
  this->buffering_strategy_.reset (buffering_strategy);

  long flags = THR_NEW_LWP | THR_DETACHED;
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

      if (ACE_OS::last_error () == EPERM)
        ORBSVCS_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) Insufficient privilege.\n")));
      else if (ACE_OS::last_error () == EAGAIN)
        {
          ORBSVCS_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) task activation at priority %d failed %p\n"),
                      tp_params.default_priority, "activate"));
          throw CORBA::NO_RESOURCES ();
        }

      throw CORBA::BAD_PARAM ();
    }
}

void
TAO_Notify_ThreadPool_Task::execute (TAO_Notify_Method_Request& method_request)
{
  if (!shutdown_)
    {
      TAO_Notify_Method_Request_Queueable* request_copy = method_request.copy ();

      if (this->buffering_strategy_->enqueue (request_copy) == -1)
        {
          ACE_Message_Block::release (request_copy);

          if (TAO_debug_level > 0)
            ORBSVCS_DEBUG ((LM_DEBUG, "NS_ThreadPool_Task (%P|%t) - "
                        "failed to enqueue\n"));
        }
    }
}

int
TAO_Notify_ThreadPool_Task::svc (void)
{
  TAO_Notify_Method_Request_Queueable* method_request = 0;

  while (!shutdown_)
    {
      try
        {
          ACE_Time_Value* dequeue_blocking_time = 0;
          ACE_Time_Value earliest_time;

          if (!this->timer_->impl().is_empty ())
            {
              earliest_time = this->timer_->impl().earliest_time ();
              dequeue_blocking_time = &earliest_time;
            }

          // Dequeue 1 item
          int const result = buffering_strategy_->dequeue (method_request, dequeue_blocking_time);

          if (result > 0)
            {
              method_request->execute ();

              ACE_Message_Block::release (method_request);
            }
          else if (errno == ETIME)
            {
              this->timer_->impl ().expire ();
            }
          else
            {
              if (TAO_debug_level > 0)
                ORBSVCS_DEBUG ((LM_DEBUG, "(%P|%t)ThreadPool_Task dequeue failed\n"));
            }
        }
      catch (const CORBA::Exception& ex)
        {
          ex._tao_print_exception (
                                   "ThreadPool_Task (%P|%t) exception in method request\n");
        }
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
}

void
TAO_Notify_ThreadPool_Task::release (void)
{
  delete this;
}

int
TAO_Notify_ThreadPool_Task::close (u_long)
{
  // _incr_refcnt() for each spawned thread in init()
  this->_decr_refcnt();
  return 0;
}

void
TAO_Notify_ThreadPool_Task::update_qos_properties (const TAO_Notify_QoSProperties& qos_properties)
{
  this->buffering_strategy_->update_qos_properties (qos_properties);
}

TAO_END_VERSIONED_NAMESPACE_DECL
