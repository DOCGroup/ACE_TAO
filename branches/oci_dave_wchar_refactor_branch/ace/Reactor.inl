// -*- C++ -*-
//
// $Id$

// Run the event loop until the <ACE_Reactor::handle_events> method
// returns -1 or the <end_event_loop> method is invoked.

/*
 * Hook to specialize the Reactor with the concrete implementation
 * known at compile time.
 */
//@@ REACTOR_SPL_INCLUDE_FORWARD_DECL_ADD_HOOK

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE int
ACE_Reactor::run_event_loop (void)
{
  ACE_TRACE ("ACE_Reactor::run_event_loop");
  ACE_Reactor *r = ACE_Reactor::instance ();

  if (r == 0)
    return -1;

  return r->run_reactor_event_loop (ACE_Reactor::check_reconfiguration);
}

// Run the event loop until the <ACE_Reactor::handle_events>
// method returns -1, the <end_event_loop> method
// is invoked, or the <ACE_Time_Value> expires.

ACE_INLINE int
ACE_Reactor::run_event_loop (ACE_Time_Value &tv)
{
  ACE_TRACE ("ACE_Reactor::run_event_loop");
  ACE_Reactor *r = ACE_Reactor::instance ();

  if (r == 0)
    return -1;

  return r->run_reactor_event_loop
    (tv, ACE_Reactor::check_reconfiguration);
}

// Run the event loop until the <ACE_Reactor::alertable_handle_events> method
// returns -1 or the <end_event_loop> method is invoked.

ACE_INLINE int
ACE_Reactor::run_alertable_event_loop (void)
{
  ACE_TRACE ("ACE_Reactor::run_alertable_event_loop");
  ACE_Reactor *r = ACE_Reactor::instance ();

  if (r == 0)
    return -1;

  return r->run_alertable_reactor_event_loop (ACE_Reactor::check_reconfiguration);
}

// Run the event loop until the <ACE_Reactor::alertable_handle_events>
// method returns -1, the <end_event_loop> method
// is invoked, or the <ACE_Time_Value> expires.

ACE_INLINE int
ACE_Reactor::run_alertable_event_loop (ACE_Time_Value &tv)
{
  ACE_TRACE ("ACE_Reactor::run_alertable_event_loop");
  ACE_Reactor *r = ACE_Reactor::instance ();

  if (r == 0)
    return -1;

  return r->run_alertable_reactor_event_loop
    (tv, ACE_Reactor::check_reconfiguration);
}

ACE_INLINE void
ACE_Reactor::reset_event_loop (void)
{
  ACE_TRACE ("ACE_Reactor::reset_event_loop");

  ACE_Reactor::instance ()->reset_reactor_event_loop ();
}

ACE_INLINE int
ACE_Reactor::end_event_loop (void)
{
  ACE_TRACE ("ACE_Reactor::end_event_loop");
  ACE_Reactor::instance ()->end_reactor_event_loop ();

  return 0;
}

ACE_INLINE int
ACE_Reactor::event_loop_done (void)
{
  ACE_TRACE ("ACE_Reactor::event_loop_done");
  return ACE_Reactor::instance ()->reactor_event_loop_done ();
}

ACE_END_VERSIONED_NAMESPACE_DECL
