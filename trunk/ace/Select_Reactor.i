/* -*- C++ -*- */
// $Id$

#include "ace/Reactor.h"

ACE_INLINE int
ACE_Select_Reactor::resume_handler (ACE_Event_Handler *h)
{
  ACE_TRACE ("ACE_Select_Reactor::resume_handler");
  return this->resume_handler (h->get_handle ());
}

ACE_INLINE int
ACE_Select_Reactor::resume_handler (const ACE_Handle_Set &handles)
{
  ACE_TRACE ("ACE_Select_Reactor::resume_handler");
  ACE_Handle_Set_Iterator handle_iter (handles);
  ACE_HANDLE h;

  ACE_MT (ACE_GUARD_RETURN (ACE_SELECT_REACTOR_MUTEX, ace_mon, this->token_, -1));
  
  while ((h = handle_iter ()) != ACE_INVALID_HANDLE)
    if (this->resume_i (h) == -1)
      return -1;
  
  return 0;
}

ACE_INLINE int
ACE_Select_Reactor::suspend_handler (ACE_Event_Handler *h)
{
  ACE_TRACE ("ACE_Select_Reactor::suspend_handler");
  return this->suspend_handler (h->get_handle ());
}

ACE_INLINE int
ACE_Select_Reactor::suspend_handler (const ACE_Handle_Set &handles)
{
  ACE_TRACE ("ACE_Select_Reactor::suspend_handler");
  ACE_Handle_Set_Iterator handle_iter (handles);
  ACE_HANDLE h;

  ACE_MT (ACE_GUARD_RETURN (ACE_SELECT_REACTOR_MUTEX, ace_mon, this->token_, -1));
  
  while ((h = handle_iter ()) != ACE_INVALID_HANDLE)
    if (this->suspend_i (h) == -1)
      return -1;
  
  return 0;
}

ACE_INLINE int
ACE_Select_Reactor::register_handler (int signum, 
				      ACE_Event_Handler *new_sh, 
				      ACE_Sig_Action *new_disp, 
				      ACE_Event_Handler **old_sh,
				      ACE_Sig_Action *old_disp)
{
  ACE_TRACE ("ACE_Select_Reactor::register_handler");
  return this->signal_handler_->register_handler (signum, 
						  new_sh, new_disp,
						  old_sh, old_disp); 
}

#if defined (ACE_WIN32)

ACE_INLINE int 
ACE_Select_Reactor::register_handler (ACE_Event_Handler *event_handler, 
				      ACE_HANDLE event_handle)
{
  // Don't have an implementation for this yet...
  ACE_UNUSED_ARG (event_handler);
  ACE_UNUSED_ARG (event_handle);
  ACE_NOTSUP_RETURN (-1);
}

#endif /* ACE_WIN32 */

ACE_INLINE int 
ACE_Select_Reactor::register_handler (ACE_HANDLE event_handle,
				      ACE_HANDLE io_handle,
				      ACE_Event_Handler *event_handler, 
				      ACE_Reactor_Mask mask)
{
  // Don't have an implementation for this yet...
  ACE_UNUSED_ARG (event_handle);
  ACE_UNUSED_ARG (io_handle);
  ACE_UNUSED_ARG (event_handler);
  ACE_UNUSED_ARG (mask);
  ACE_NOTSUP_RETURN (-1);
}

ACE_INLINE int
ACE_Select_Reactor::handler (int signum, ACE_Event_Handler **handler)
{
  ACE_TRACE ("ACE_Select_Reactor::handler");
  return this->handler_i (signum, handler);
}

ACE_INLINE int
ACE_Select_Reactor::remove_handler (int signum, 
				    ACE_Sig_Action *new_disp, 
				    ACE_Sig_Action *old_disp,
				    int sigkey)
{
  ACE_TRACE ("ACE_Select_Reactor::remove_handler");
  return this->signal_handler_->remove_handler (signum, new_disp, old_disp, sigkey);
}

// = The remaining methods in this file must be called with locks
// held.  Note the queue handles its own locking.

ACE_INLINE int
ACE_Select_Reactor::cancel_timer (ACE_Event_Handler *handler,
				  int dont_call_handle_close)
{
  ACE_TRACE ("ACE_Select_Reactor::cancel_timer");
  return this->timer_queue_->cancel (handler, 
				     dont_call_handle_close);
}

ACE_INLINE int
ACE_Select_Reactor::cancel_timer (long timer_id, 
				  const void **arg,
				  int dont_call_handle_close)
{
  ACE_TRACE ("ACE_Select_Reactor::cancel_timer");
  return this->timer_queue_->cancel (timer_id, 
				     arg,
				     dont_call_handle_close);
}

// Performs operations on the "ready" bits.

ACE_INLINE int
ACE_Select_Reactor::ready_ops (ACE_Event_Handler *handler, 
			       ACE_Reactor_Mask mask, 
			       int ops)
{
  ACE_TRACE ("ACE_Select_Reactor::ready_ops");
  return this->ready_ops (handler->get_handle (), mask, ops);
}

// Performs operations on the "dispatch" masks.

ACE_INLINE int
ACE_Select_Reactor::mask_ops (ACE_Event_Handler *handler, 
			      ACE_Reactor_Mask mask, 
			      int ops)
{
  ACE_TRACE ("ACE_Select_Reactor::mask_ops");
  return this->mask_ops (handler->get_handle (), mask, ops);
}

ACE_INLINE int 
ACE_Select_Reactor::schedule_wakeup (ACE_Event_Handler *eh,
				     ACE_Reactor_Mask mask)
{
  ACE_TRACE ("ACE_Select_Reactor::schedule_wakeup");
  return this->mask_ops (eh->get_handle (), mask, ACE_Reactor::ADD_MASK);
}

ACE_INLINE int 
ACE_Select_Reactor::cancel_wakeup (ACE_Event_Handler *eh,
				   ACE_Reactor_Mask mask)
{
  ACE_TRACE ("ACE_Select_Reactor::cancel_wakeup");
  return this->mask_ops (eh->get_handle (), mask, ACE_Reactor::CLR_MASK);
}

ACE_INLINE int 
ACE_Select_Reactor::schedule_wakeup (ACE_HANDLE handle,
				     ACE_Reactor_Mask mask)
{
  ACE_TRACE ("ACE_Select_Reactor::schedule_wakeup");
  return this->mask_ops (handle, mask, ACE_Reactor::ADD_MASK);
}

ACE_INLINE int 
ACE_Select_Reactor::cancel_wakeup (ACE_HANDLE handle,
				   ACE_Reactor_Mask mask)
{
  ACE_TRACE ("ACE_Select_Reactor::cancel_wakeup");
  return this->mask_ops (handle, mask, ACE_Reactor::CLR_MASK);
}

ACE_INLINE ACE_Lock &
ACE_Select_Reactor::lock (void)
{
  ACE_TRACE ("ACE_Select_Reactor::lock");
  return this->lock_adapter_;
}

ACE_INLINE void
ACE_Select_Reactor::wakeup_all_threads (void)
{
  // Send a notification, but don't block if there's no one to receive
  // it.
  this->notify (0, ACE_Event_Handler::NULL_MASK, (ACE_Time_Value *) &ACE_Time_Value::zero);
}

ACE_INLINE int 
ACE_Select_Reactor::alertable_handle_events (ACE_Time_Value *max_wait_time)
{
  return this->handle_events (max_wait_time);
}

ACE_INLINE int 
ACE_Select_Reactor::alertable_handle_events (ACE_Time_Value &max_wait_time)
{
  return this->handle_events (max_wait_time);
}

ACE_INLINE size_t 
ACE_Select_Reactor_Handler_Repository::size (void)
{
  return this->max_size_;
}

ACE_INLINE size_t 
ACE_Select_Reactor::size (void)
{
  return this->handler_rep_.size ();
}
