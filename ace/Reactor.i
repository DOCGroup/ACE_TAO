/* -*- C++ -*- */
// $Id$

#include "ace/Reactor_Impl.h"
#include "ace/Handle_Set.h"

ACE_INLINE int 
ACE_Reactor::open (size_t size,
		   int restart,
		   ACE_Sig_Handler *signal_handler,
		   ACE_Timer_Queue *timer_queue)
{
  return this->implementation_->open (size,
				      restart,
				      signal_handler,
				      timer_queue);
}

ACE_INLINE int 
ACE_Reactor::close (void)
{
  return this->implementation_->close ();
}

ACE_INLINE int 
ACE_Reactor::handle_events (ACE_Time_Value *max_wait_time)
{
  return this->implementation_->handle_events (max_wait_time);
}

ACE_INLINE int 
ACE_Reactor::alertable_handle_events (ACE_Time_Value *max_wait_time)
{
  return this->implementation_->alertable_handle_events (max_wait_time);
}

ACE_INLINE int 
ACE_Reactor::handle_events (ACE_Time_Value &max_wait_time)
{
  return this->implementation_->handle_events (max_wait_time);
}

ACE_INLINE int 
ACE_Reactor::alertable_handle_events (ACE_Time_Value &max_wait_time)
{
  return this->implementation_->alertable_handle_events (max_wait_time);
}


ACE_INLINE int 
ACE_Reactor::register_handler (ACE_Event_Handler *event_handler,
			       ACE_Reactor_Mask mask)
{
  int result = this->implementation_->register_handler (event_handler,
							mask);
  if (result != -1)
    // Assign *this* <Reactor> to the <Event_Handler>.
    event_handler->reactor (this);

  return result;
}

ACE_INLINE int 
ACE_Reactor::register_handler (ACE_HANDLE io_handle,
			       ACE_Event_Handler *event_handler,
			       ACE_Reactor_Mask mask)
{
  int result = this->implementation_->register_handler (io_handle,
							event_handler,
							mask);
  if (result != -1)
    // Assign *this* <Reactor> to the <Event_Handler>.
    event_handler->reactor (this);

  return result;
}
 
#if defined (ACE_WIN32)

ACE_INLINE int 
ACE_Reactor::register_handler (ACE_Event_Handler *event_handler,
			       ACE_HANDLE event_handle)
{
  int result = this->implementation_->register_handler (event_handler,
							event_handle);
  if (result != -1)
    // Assign *this* <Reactor> to the <Event_Handler>.
    event_handler->reactor (this);

  return result;
}
 
#endif /* ACE_WIN32 */

ACE_INLINE int 
ACE_Reactor::register_handler (ACE_HANDLE event_handle,
			       ACE_HANDLE io_handle,
			       ACE_Event_Handler *event_handler,
			       ACE_Reactor_Mask mask)
{
  int result = this->implementation_->register_handler (event_handle,
							io_handle,
							event_handler,
							mask);
  if (result != -1)
    // Assign *this* <Reactor> to the <Event_Handler>.
    event_handler->reactor (this);

  return result;
}
 
ACE_INLINE int 
ACE_Reactor::register_handler (const ACE_Handle_Set &handles,
			       ACE_Event_Handler *event_handler,
			       ACE_Reactor_Mask mask)
{
  int result = this->implementation_->register_handler (handles,
							event_handler,
							mask);
  if (result != -1)
    // Assign *this* <Reactor> to the <Event_Handler>.
    event_handler->reactor (this);
  
  return result;
}

ACE_INLINE int 
ACE_Reactor::register_handler (int signum,
			       ACE_Event_Handler *new_sh,
			       ACE_Sig_Action *new_disp,
			       ACE_Event_Handler **old_sh,
			       ACE_Sig_Action *old_disp)
{
  return this->implementation_->register_handler (signum,
						  new_sh,
						  new_disp,
						  old_sh,
						  old_disp);
}

ACE_INLINE int 
ACE_Reactor::register_handler (const ACE_Sig_Set &sigset,
			       ACE_Event_Handler *new_sh,
			       ACE_Sig_Action *new_disp)
{
  return this->implementation_->register_handler (sigset,
						  new_sh,
						  new_disp);
}

ACE_INLINE int 
ACE_Reactor::remove_handler (ACE_Event_Handler *event_handler,
			     ACE_Reactor_Mask mask)
{
  return this->implementation_->remove_handler (event_handler,
						mask);
}

ACE_INLINE int 
ACE_Reactor::remove_handler (ACE_HANDLE handle,
			     ACE_Reactor_Mask mask)
{
  return this->implementation_->remove_handler (handle,
						mask);
}

ACE_INLINE int 
ACE_Reactor::remove_handler (const ACE_Handle_Set &handle_set,
			     ACE_Reactor_Mask mask)
{
  return this->implementation_->remove_handler (handle_set,
						mask);
}

ACE_INLINE int 
ACE_Reactor::remove_handler (int signum,
			     ACE_Sig_Action *new_disp,
			     ACE_Sig_Action *old_disp,
			     int sigkey)
{
  return this->implementation_->remove_handler (signum,
						new_disp,
						old_disp,
						sigkey);
}

ACE_INLINE int 
ACE_Reactor::remove_handler (const ACE_Sig_Set &sigset)
{
  return this->implementation_->remove_handler (sigset);
}


ACE_INLINE int 
ACE_Reactor::suspend_handler (ACE_Event_Handler *event_handler)
{
  return this->implementation_->suspend_handler (event_handler);
}

ACE_INLINE int 
ACE_Reactor::suspend_handler (ACE_HANDLE handle)
{
  return this->implementation_->suspend_handler (handle);
}

ACE_INLINE int 
ACE_Reactor::suspend_handler (const ACE_Handle_Set &handles)
{
  return this->implementation_->suspend_handler (handles);
}

ACE_INLINE int 
ACE_Reactor::suspend_handlers (void)
{
  return this->implementation_->suspend_handlers ();
}

ACE_INLINE int 
ACE_Reactor::resume_handler (ACE_Event_Handler *event_handler)
{
  return this->implementation_->resume_handler (event_handler);
}

ACE_INLINE int 
ACE_Reactor::resume_handler (ACE_HANDLE handle)
{
  return this->implementation_->resume_handler (handle);
}

ACE_INLINE int 
ACE_Reactor::resume_handler (const ACE_Handle_Set &handles)
{
  return this->implementation_->resume_handler (handles);
}

ACE_INLINE int 
ACE_Reactor::resume_handlers (void)
{
  return this->implementation_->resume_handlers ();
}


ACE_INLINE long 
ACE_Reactor::schedule_timer (ACE_Event_Handler *event_handler,
			     const void *arg,
			     const ACE_Time_Value &delta,
			     const ACE_Time_Value &interval)
{
  return this->implementation_->schedule_timer (event_handler,
						arg,
						delta,
						interval);
}

ACE_INLINE int 
ACE_Reactor::cancel_timer (ACE_Event_Handler *event_handler,
			   int dont_call_handle_close)
{
  return this->implementation_->cancel_timer (event_handler,
					      dont_call_handle_close);
}

ACE_INLINE int 
ACE_Reactor::cancel_timer (long timer_id,
			   const void **arg,
			   int dont_call_handle_close)
{
  return this->implementation_->cancel_timer (timer_id,
					      arg,
					      dont_call_handle_close);
}

  
ACE_INLINE int 
ACE_Reactor::schedule_wakeup (ACE_Event_Handler *event_handler,
			      ACE_Reactor_Mask masks_to_be_added)
{
  return this->implementation_->schedule_wakeup (event_handler,
						 masks_to_be_added);
}
  
ACE_INLINE int 
ACE_Reactor::schedule_wakeup (ACE_HANDLE handle,
			      ACE_Reactor_Mask masks_to_be_added)
{
  return this->implementation_->schedule_wakeup (handle,
						 masks_to_be_added);
}

ACE_INLINE int 
ACE_Reactor::cancel_wakeup (ACE_Event_Handler *event_handler,
			    ACE_Reactor_Mask masks_to_be_cleared)
{
  return this->implementation_->cancel_wakeup (event_handler,
					       masks_to_be_cleared);
}

ACE_INLINE int 
ACE_Reactor::cancel_wakeup (ACE_HANDLE handle,
			    ACE_Reactor_Mask masks_to_be_cleared)
{
  return this->implementation_->cancel_wakeup (handle,
					       masks_to_be_cleared);
}


ACE_INLINE int 
ACE_Reactor::notify (ACE_Event_Handler *event_handler,
		     ACE_Reactor_Mask mask,
		     ACE_Time_Value *tv)
{
  return this->implementation_->notify (event_handler,
					mask,
					tv);
}

ACE_INLINE void 
ACE_Reactor::max_notify_iterations (int iterations)
{
  this->implementation_->max_notify_iterations (iterations);
}

ACE_INLINE int 
ACE_Reactor::max_notify_iterations (void)
{
  return this->implementation_->max_notify_iterations ();
}

ACE_INLINE int 
ACE_Reactor::handler (ACE_HANDLE handle,
		      ACE_Reactor_Mask mask,
		      ACE_Event_Handler **event_handler)
{
  return this->implementation_->handler (handle,
					 mask,
					 event_handler);
}

ACE_INLINE int 
ACE_Reactor::handler (int signum,
		      ACE_Event_Handler **event_handler)
{
  return this->implementation_->handler (signum,
					 event_handler);
}

ACE_INLINE int 
ACE_Reactor::initialized (void)
{
  return this->implementation_->initialized ();
}

ACE_INLINE ACE_Lock &
ACE_Reactor::lock (void)
{
  return this->implementation_->lock ();
}

ACE_INLINE void 
ACE_Reactor::wakeup_all_threads (void)
{
  this->implementation_->wakeup_all_threads ();
}

ACE_INLINE int 
ACE_Reactor::owner (ACE_thread_t new_owner, 
		    ACE_thread_t *old_owner)
{
  return this->implementation_->owner (new_owner, 
				       old_owner);
}

ACE_INLINE int 
ACE_Reactor::owner (ACE_thread_t *owner)
{
  return this->implementation_->owner (owner);
}

ACE_INLINE void 
ACE_Reactor::requeue_position (int position)
{
  this->implementation_->requeue_position (position);
}

ACE_INLINE int 
ACE_Reactor::requeue_position (void)
{
  return this->implementation_->requeue_position ();
}


ACE_INLINE int 
ACE_Reactor::mask_ops (ACE_Event_Handler *event_handler,
		       ACE_Reactor_Mask mask,
		       int ops)
{
  return this->implementation_->mask_ops (event_handler,
					  mask,
					  ops);
}

ACE_INLINE int 
ACE_Reactor::mask_ops (ACE_HANDLE handle,
		       ACE_Reactor_Mask mask,
		       int ops)
{
  return this->implementation_->mask_ops (handle,
					  mask,
					  ops);
}

ACE_INLINE int 
ACE_Reactor::ready_ops (ACE_Event_Handler *event_handler,
			ACE_Reactor_Mask mask,
			int ops)
{
  return this->implementation_->ready_ops (event_handler,
					   mask,
					   ops);
}

ACE_INLINE int 
ACE_Reactor::ready_ops (ACE_HANDLE handle,
			ACE_Reactor_Mask mask,
			int ops)
{
  return this->implementation_->ready_ops (handle,
					   mask,
					   ops);
}

ACE_INLINE size_t
ACE_Reactor::size (void)
{
  return this->implementation_->size ();
}
