// $Id$

#define ACE_BUILD_DLL
#include "ace/Reactor.h"
#include "ace/Reactor_Impl.h"
#include "ace/Handle_Set.h"
#if !defined (ACE_HAS_WINCE)
#  include "ace/Service_Config.h"
#  include "ace/WFMO_Reactor.h"
#  include "ace/Msg_WFMO_Reactor.h"
#endif /* !ACE_HAS_WINCE */
#include "ace/Select_Reactor.h"
#include "ace/TP_Reactor.h"
#include "ace/Object_Manager.h"

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
      ACE_NEW (impl, ACE_TP_Reactor);
  #else  
      ACE_NEW (impl, ACE_Select_Reactor);
  #endif /* ACE_USE_TP_REACTOR_FOR_REACTOR_IMPL */
#else /* We are on Win32 and we have winsock and ACE_USE_SELECT_REACTOR_FOR_REACTOR_IMPL is not defined */
  #if defined (ACE_USE_MSG_WFMO_REACTOR_FOR_REACTOR_IMPL)
      ACE_NEW (impl, ACE_Msg_WFMO_Reactor);
  #else
      ACE_NEW (impl, ACE_WFMO_Reactor);
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

// Terminate the eventloop.
sig_atomic_t ACE_Reactor::end_event_loop_ = 0;

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
          ACE_NEW_RETURN (ACE_Reactor::reactor_, ACE_Reactor, 0);
          ACE_Reactor::delete_reactor_ = 1;
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

// Run the event loop until the <ACE_Reactor::handle_events> method
// returns -1 or the <end_event_loop> method is invoked.

int
ACE_Reactor::run_event_loop (void)
{
  ACE_TRACE ("ACE_Reactor::run_event_loop");

  while (ACE_Reactor::end_event_loop_ == 0)
    {
      int result = ACE_Reactor::instance ()->handle_events ();

#if !defined (ACE_HAS_WINCE)
      if (ACE_Service_Config::reconfig_occurred ())
        ACE_Service_Config::reconfigure ();
      else
#endif /* !ACE_HAS_WINCE */
        if (result == -1)
          return -1;
    }
  /* NOTREACHED */
  return 0;
}

// Run the event loop until the <ACE_Reactor::handle_events>
// method returns -1, the <end_event_loop> method
// is invoked, or the <ACE_Time_Value> expires.

int
ACE_Reactor::run_event_loop (ACE_Time_Value &tv)
{
  ACE_TRACE ("ACE_Reactor::run_event_loop");

  while (ACE_Reactor::end_event_loop_ == 0)
    {
      int result = ACE_Reactor::instance ()->handle_events (tv);

#if !defined (ACE_HAS_WINCE)
      if (ACE_Service_Config::reconfig_occurred ())
        ACE_Service_Config::reconfigure ();
      else
#endif /* !ACE_HAS_WINCE */
        if (result <= 0)
          return result;
    }

  /* NOTREACHED */
  return 0;
}

// Run the event loop until the <ACE_Reactor::alertable_handle_events> method
// returns -1 or the <end_event_loop> method is invoked.

int
ACE_Reactor::run_alertable_event_loop (void)
{
  ACE_TRACE ("ACE_Reactor::run_event_loop");

  while (ACE_Reactor::end_event_loop_ == 0)
    {
      int result = ACE_Reactor::instance ()->alertable_handle_events ();

#if !defined (ACE_HAS_WINCE)
      if (ACE_Service_Config::reconfig_occurred ())
        ACE_Service_Config::reconfigure ();
      else
#endif /* !ACE_HAS_WINCE */

        if (result == -1)
          return -1;
    }
  /* NOTREACHED */
  return 0;
}

// Run the event loop until the <ACE_Reactor::alertable_handle_events>
// method returns -1, the <end_event_loop> method
// is invoked, or the <ACE_Time_Value> expires.

int
ACE_Reactor::run_alertable_event_loop (ACE_Time_Value &tv)
{
  ACE_TRACE ("ACE_Reactor::run_event_loop");

  while (ACE_Reactor::end_event_loop_ == 0)
    {
      int result = ACE_Reactor::instance ()->alertable_handle_events (tv);

#if !defined (ACE_HAS_WINCE)
      if (ACE_Service_Config::reconfig_occurred ())
        ACE_Service_Config::reconfigure ();
      else
#endif /* !ACE_HAS_WINCE */
        if (result <= 0)
          return result;
    }

  /* NOTREACHED */
  return 0;
}

void
ACE_Reactor::reset_event_loop (void)
{
  ACE_TRACE ("ACE_Reactor::reset_event_loop");

  ACE_Reactor::end_event_loop_ = 0;
}

int
ACE_Reactor::end_event_loop (void)
{
  ACE_TRACE ("ACE_Reactor::end_event_loop");

  ACE_Reactor::end_event_loop_ = 1;

  // Wakeup all threads waiting in the Reactor.
  ACE_Reactor::instance ()->wakeup_all_threads ();

  return 0;
}

int
ACE_Reactor::event_loop_done (void)
{
  ACE_TRACE ("ACE_Reactor::event_loop_done");
  return ACE_Reactor::end_event_loop_ != 0;
}

void
ACE_Reactor::dump (void) const
{
  ACE_TRACE ("ACE_Reactor::dump");

  implementation_->dump ();
}
