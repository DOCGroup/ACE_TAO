/* -*- C++ -*- */
// $Id$

ACE_INLINE int
ACE_TP_Reactor::handle_events (ACE_Time_Value &max_wait_time)
{
  return ACE_Select_Reactor::handle_events (max_wait_time);
}

ACE_INLINE void
ACE_TP_Reactor::wakeup_all_threads (void)
{
  ACE_MT (this->token_.signal_all_threads (););
  // Send a notification, but don't block if there's no one to receive
  // it.
  this->notify (0, ACE_Event_Handler::NULL_MASK, (ACE_Time_Value *) &ACE_Time_Value::zero);
}

ACE_INLINE void
ACE_TP_Reactor::notify_handle (ACE_HANDLE,
                               ACE_Reactor_Mask,
                               ACE_Handle_Set &,
                               ACE_Event_Handler *,
                               ACE_EH_PTMF)
{
  ACE_ERROR ((LM_ERROR,
              ASYS_TEXT ("ACE_TP_Reactor::notify_handle: Wrong version of notify_handle() gets called")));
}
