// $Id$

#include "ace/Reactor.h"
#include "ace/Reactor_Impl.h"
#include "ace/Handle_Set.h"
#if !defined (ACE_HAS_WINCE)
#  if !defined (ACE_LACKS_ACE_SVCCONF)
#    include "ace/Service_Config.h"
#  endif /* !ACE_LACKS_ACE_SVCCONF */
#  include "ace/WFMO_Reactor.h"
#  include "ace/Msg_WFMO_Reactor.h"
#endif /* ! ACE_HAS_WINCE */
#include "ace/Select_Reactor.h"
#include "ace/TP_Reactor.h"
#include "ace/Object_Manager.h"
#include "ace/Framework_Component.h"

#if !defined (__ACE_INLINE__)
#include "ace/Reactor.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(ace, Reactor, "$Id$")

ACE_ALLOC_HOOK_DEFINE(ACE_Reactor)

ACE_Reactor::ACE_Reactor (ACE_Reactor_Impl *impl,
                          int delete_implementation)
  : implementation_ (0),
    delete_implementation_ (delete_implementation)
{
  this->implementation (impl);

  if (this->implementation () == 0)
    {
#if !defined (ACE_WIN32) \
      || !defined (ACE_HAS_WINSOCK2) || (ACE_HAS_WINSOCK2 == 0) \
      || defined (ACE_USE_SELECT_REACTOR_FOR_REACTOR_IMPL) \
      || defined (ACE_USE_TP_REACTOR_FOR_REACTOR_IMPL)
  #if defined (ACE_USE_TP_REACTOR_FOR_REACTOR_IMPL)
      ACE_NEW (impl,
               ACE_TP_Reactor);
  #else
      ACE_NEW (impl,
               ACE_Select_Reactor);
  #endif /* ACE_USE_TP_REACTOR_FOR_REACTOR_IMPL */
#else /* We are on Win32 and we have winsock and ACE_USE_SELECT_REACTOR_FOR_REACTOR_IMPL is not defined */
  #if defined (ACE_USE_MSG_WFMO_REACTOR_FOR_REACTOR_IMPL)
      ACE_NEW (impl,
               ACE_Msg_WFMO_Reactor);
  #else
      ACE_NEW (impl,
               ACE_WFMO_Reactor);
  #endif /* ACE_USE_MSG_WFMO_REACTOR_FOR_REACTOR_IMPL */
#endif /* !defined (ACE_WIN32) || !defined (ACE_HAS_WINSOCK2) || (ACE_HAS_WINSOCK2 == 0) || defined (ACE_USE_SELECT_REACTOR_FOR_REACTOR_IMPL) */
      this->implementation (impl);
      this->delete_implementation_ = 1;
    }
}

ACE_Reactor::~ACE_Reactor (void)
{
  if (this->delete_implementation_)
    delete this->implementation ();
}

// Process-wide ACE_Reactor.
ACE_Reactor *ACE_Reactor::reactor_ = 0;

// Controls whether the Reactor is deleted when we shut down (we can
// only delete it safely if we created it!)
int ACE_Reactor::delete_reactor_ = 0;

ACE_Reactor *
ACE_Reactor::instance (void)
{
  ACE_TRACE ("ACE_Reactor::instance");

  if (ACE_Reactor::reactor_ == 0)
    {
      // Perform Double-Checked Locking Optimization.
      ACE_MT (ACE_GUARD_RETURN (ACE_Recursive_Thread_Mutex, ace_mon,
                                *ACE_Static_Object_Lock::instance (), 0));

      if (ACE_Reactor::reactor_ == 0)
        {
          ACE_NEW_RETURN (ACE_Reactor::reactor_,
                          ACE_Reactor,
                          0);
          ACE_Reactor::delete_reactor_ = 1;
          ACE_REGISTER_FRAMEWORK_COMPONENT(ACE_Reactor, ACE_Reactor::reactor_)
        }
    }
  return ACE_Reactor::reactor_;
}

ACE_Reactor *
ACE_Reactor::instance (ACE_Reactor *r,
                       int delete_reactor)
{
  ACE_TRACE ("ACE_Reactor::instance");

  ACE_MT (ACE_GUARD_RETURN (ACE_Recursive_Thread_Mutex, ace_mon,
                            *ACE_Static_Object_Lock::instance (), 0));
  ACE_Reactor *t = ACE_Reactor::reactor_;
  if (delete_reactor != 0)
    ACE_Reactor::delete_reactor_ = 1;
  else
    // We can't safely delete it since we don't know who created it!
    ACE_Reactor::delete_reactor_ = 0;

  ACE_Reactor::reactor_ = r;
  ACE_REGISTER_FRAMEWORK_COMPONENT(ACE_Reactor, ACE_Reactor::reactor_);
  return t;
}

void
ACE_Reactor::close_singleton (void)
{
  ACE_TRACE ("ACE_Reactor::close_singleton");

  ACE_MT (ACE_GUARD (ACE_Recursive_Thread_Mutex, ace_mon,
                     *ACE_Static_Object_Lock::instance ()));

  if (ACE_Reactor::delete_reactor_)
    {
      delete ACE_Reactor::reactor_;
      ACE_Reactor::reactor_ = 0;
      ACE_Reactor::delete_reactor_ = 0;
    }
}

int
ACE_Reactor::check_reconfiguration (ACE_Reactor *)
{
#if !defined (ACE_HAS_WINCE)  &&  !defined (ACE_LACKS_ACE_SVCCONF)
  if (ACE_Service_Config::reconfig_occurred ())
    {
      ACE_Service_Config::reconfigure ();
      return 1;
    }
#endif /* ! ACE_HAS_WINCE || ! ACE_LACKS_ACE_SVCCONF */
  return 0;
}

int
ACE_Reactor::run_reactor_event_loop (REACTOR_EVENT_HOOK eh)
{
  ACE_TRACE ("ACE_Reactor::run_reactor_event_loop");

  if (this->reactor_event_loop_done ())
    return 0;

  while (1)
    {
      int result = this->implementation_->handle_events ();

      if (eh != 0 && (*eh)(this))
        continue;
      else if (result == -1 && this->implementation_->deactivated ())
        return 0;
      else if (result == -1)
        return -1;
    }

  ACE_NOTREACHED (return 0;)
}

int
ACE_Reactor::run_alertable_reactor_event_loop (REACTOR_EVENT_HOOK eh)
{
  ACE_TRACE ("ACE_Reactor::run_alertable_reactor_event_loop");

  if (this->reactor_event_loop_done ())
    return 0;

  while (1)
    {
      int result = this->implementation_->alertable_handle_events ();

      if (eh != 0 && (*eh)(this))
        continue;
      else if (result == -1 && this->implementation_->deactivated ())
        return 0;
      else if (result == -1)
        return -1;
    }

  ACE_NOTREACHED (return 0;)
}

int
ACE_Reactor::run_reactor_event_loop (ACE_Time_Value &tv,
                                     REACTOR_EVENT_HOOK eh)
{
  ACE_TRACE ("ACE_Reactor::run_reactor_event_loop");

  if (this->reactor_event_loop_done ())
    return 0;

  while (1)
    {
      int result = this->implementation_->handle_events (tv);

      if (eh != 0 && (*eh) (this))
        continue;
      else if (result == -1)
        {
          if (this->implementation_->deactivated ())
            result = 0;
          return result;
        }
      else if (result == 0)
        {
          // The <handle_events> method timed out without dispatching
          // anything.  Because of rounding and conversion errors and
          // such, it could be that the wait loop (WFMO, select, etc.)
          // timed out, but the timer queue said it wasn't quite ready
          // to expire a timer. In this case, the ACE_Time_Value we
          // passed into handle_events won't have quite been reduced
          // to 0, and we need to go around again. If we are all the
          // way to 0, just return, as the entire time the caller
          // wanted to wait has been used up.
          if (tv.usec () > 0)
            continue;
          return 0;
        }
      // Else there were some events dispatched; go around again
    }

  ACE_NOTREACHED (return 0;)
}

int
ACE_Reactor::run_alertable_reactor_event_loop (ACE_Time_Value &tv,
                                               REACTOR_EVENT_HOOK eh)
{
  ACE_TRACE ("ACE_Reactor::run_alertable_reactor_event_loop");

  if (this->reactor_event_loop_done ())
    return 0;

  for (;;)
    {
      int result = this->implementation_->alertable_handle_events (tv);

      if (eh != 0 && (*eh)(this))
        continue;
      else if (result == -1 && this->implementation_->deactivated ())
        return 0;
      else if (result <= 0)
        return result;
    }

  ACE_NOTREACHED (return 0;)
}

int
ACE_Reactor::end_reactor_event_loop (void)
{
  ACE_TRACE ("ACE_Reactor::end_reactor_event_loop");

  this->implementation_->deactivate (1);

  return 0;
}

void
ACE_Reactor::reset_reactor_event_loop (void)
{
  ACE_TRACE ("ACE_Reactor::reset_reactor_event_loop");

  this->implementation_->deactivate (0);
}


int
ACE_Reactor::resumable_handler (void)
{
  return this->implementation ()->resumable_handler ();
}

void
ACE_Reactor::dump (void) const
{
  ACE_TRACE ("ACE_Reactor::dump");

  implementation_->dump ();
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Framework_Component_T<ACE_Reactor>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Framework_Component_T<ACE_Reactor>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
