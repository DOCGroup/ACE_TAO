// $Id$

#define ACE_BUILD_DLL

#include "ace/Reactor.h"

#include "ace/Reactor_Impl.h"
#include "ace/Handle_Set.h"
#include "ace/Service_Config.h"
#include "ace/Select_Reactor.h"
#include "ace/WFMO_Reactor.h"

#if !defined (__ACE_INLINE__)
#include "ace/Reactor.i"
#endif /* __ACE_INLINE__ */

ACE_ALLOC_HOOK_DEFINE(ACE_Reactor)

ACE_Reactor::ACE_Reactor (ACE_Reactor_Impl *impl)
  : implementation_ (0),
    delete_implementation_ (0)
{
  this->implementation (impl);

  if (this->implementation () == 0)
    {
#if defined (ACE_WIN32) && !defined (ACE_USE_SELECT_REACTOR_FOR_REACTOR_IMPL)
      ACE_NEW (impl, ACE_WFMO_Reactor);
#else /* ACE_WIN32 && !ACE_USE_SELECT_REACTOR_AS_REACTOR */
      ACE_NEW (impl, ACE_Select_Reactor);
#endif /* ACE_WIN32 && !ACE_USE_SELECT_REACTOR_AS_REACTOR */
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
	  ACE_NEW_RETURN (ACE_Reactor::reactor_, ACE_Reactor, NULL);
	  ACE_Reactor::delete_reactor_ = 1;
	}
    }
  return ACE_Reactor::reactor_;
}

ACE_Reactor *
ACE_Reactor::instance (ACE_Reactor *r)
{
  ACE_TRACE ("ACE_Reactor::instance");

  ACE_MT (ACE_GUARD_RETURN (ACE_Recursive_Thread_Mutex, ace_mon,
			    *ACE_Static_Object_Lock::instance (), 0));
  ACE_Reactor *t = ACE_Reactor::reactor_;
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

// Run the event loop until the <ACE_Reactor::handle_events>
// method returns -1 or the <end_event_loop> method
// is invoked.

int
ACE_Reactor::run_event_loop (void)
{
  ACE_TRACE ("ACE_Reactor::run_event_loop");

  while (ACE_Reactor::end_event_loop_ == 0)
    {
      int result = ACE_Reactor::instance ()->handle_events ();

      if (ACE_Service_Config::reconfig_occurred ())
	ACE_Service_Config::reconfigure ();

      else if (result == -1)
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

      if (ACE_Service_Config::reconfig_occurred ())
	ACE_Service_Config::reconfigure ();
      else if (result <= 0)
	return result;
    }

  /* NOTREACHED */
  return 0;
}

int
ACE_Reactor::end_event_loop (void)
{
  ACE_TRACE ("ACE_Reactor::end_event_loop");
  ACE_Reactor::end_event_loop_ = 1;

  // Send a notification, but don't block if there's no one to receive
  // it.
  return ACE_Reactor::instance ()->notify
    (0, ACE_Event_Handler::NULL_MASK, (ACE_Time_Value *) &ACE_Time_Value::zero);
}

sig_atomic_t
ACE_Reactor::event_loop_done (void)
{
  ACE_TRACE ("ACE_Reactor::event_loop_done");
  return ACE_Reactor::end_event_loop_;
}

