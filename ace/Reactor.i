/* -*- C++ -*- */
// $Id$

#include "ace/Reactor_Impl.h"
#include "ace/Handle_Set.h"

ACE_INLINE ACE_Reactor_Impl *
ACE_Reactor::implementation (void) const
{
  return this->implementation_;
}

ACE_INLINE void
ACE_Reactor::implementation (ACE_Reactor_Impl *impl)
{
  this->implementation_ = impl;
}

ACE_INLINE int
ACE_Reactor::current_info (ACE_HANDLE handle,
                           size_t &size)
{
  return this->implementation ()->current_info (handle, size);
}

// Run the event loop until the <ACE_Reactor::handle_events> method
// returns -1 or the <end_event_loop> method is invoked.

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
ACE_Reactor::reactor_event_loop_done (void)
{
  ACE_TRACE ("ACE_Reactor::reactor_event_loop_done");
  return this->implementation_->deactivated ();
}

ACE_INLINE int
ACE_Reactor::event_loop_done (void)
{
  ACE_TRACE ("ACE_Reactor::event_loop_done");
  return ACE_Reactor::instance ()->reactor_event_loop_done ();
}

ACE_INLINE int
ACE_Reactor::open (size_t size,
                   int restart,
                   ACE_Sig_Handler *signal_handler,
                   ACE_Timer_Queue *timer_queue)
{
  return this->implementation ()->open (size,
                                        restart,
                                        signal_handler,
                                        timer_queue);
}
ACE_INLINE int
ACE_Reactor::set_sig_handler (ACE_Sig_Handler *signal_handler)
{
  return this->implementation ()->set_sig_handler (signal_handler);
}

ACE_INLINE int
ACE_Reactor::timer_queue (ACE_Timer_Queue *tq)
{
  return this->implementation ()->timer_queue (tq);
}

ACE_INLINE int
ACE_Reactor::set_timer_queue (ACE_Timer_Queue *tq)
{
  return this->timer_queue (tq);
}

ACE_INLINE ACE_Timer_Queue *
ACE_Reactor::timer_queue (void) const
{
  ACE_Reactor_Impl *impl = this->implementation_;
  return impl->timer_queue ();
}

ACE_INLINE int
ACE_Reactor::close (void)
{
  return this->implementation ()->close ();
}

ACE_INLINE int
ACE_Reactor::work_pending (const ACE_Time_Value &max_wait_time)
{
  return this->implementation ()->work_pending (max_wait_time);
}

ACE_INLINE int
ACE_Reactor::handle_events (ACE_Time_Value *max_wait_time)
{
  return this->implementation ()->handle_events (max_wait_time);
}

ACE_INLINE int
ACE_Reactor::alertable_handle_events (ACE_Time_Value *max_wait_time)
{
  return this->implementation ()->alertable_handle_events (max_wait_time);
}

ACE_INLINE int
ACE_Reactor::handle_events (ACE_Time_Value &max_wait_time)
{
  return this->implementation ()->handle_events (max_wait_time);
}

ACE_INLINE int
ACE_Reactor::alertable_handle_events (ACE_Time_Value &max_wait_time)
{
  return this->implementation ()->alertable_handle_events (max_wait_time);
}


ACE_INLINE int
ACE_Reactor::register_handler (ACE_Event_Handler *event_handler,
                               ACE_Reactor_Mask mask)
{
  // Remember the old reactor.
  ACE_Reactor *old_reactor = event_handler->reactor ();

  // Assign *this* <Reactor> to the <Event_Handler>.
  event_handler->reactor (this);

  int result = this->implementation ()->register_handler (event_handler,
                                                          mask);
  if (result == -1)
    // Reset the old reactor in case of failures.
    event_handler->reactor (old_reactor);

  return result;
}

ACE_INLINE int
ACE_Reactor::register_handler (ACE_HANDLE io_handle,
                               ACE_Event_Handler *event_handler,
                               ACE_Reactor_Mask mask)
{
  // Remember the old reactor.
  ACE_Reactor *old_reactor = event_handler->reactor ();

  // Assign *this* <Reactor> to the <Event_Handler>.
  event_handler->reactor (this);

  int result = this->implementation ()->register_handler (io_handle,
                                                          event_handler,
                                                          mask);
  if (result == -1)
    // Reset the old reactor in case of failures.
    event_handler->reactor (old_reactor);

  return result;
}

#if defined (ACE_WIN32)

ACE_INLINE int
ACE_Reactor::register_handler (ACE_Event_Handler *event_handler,
                               ACE_HANDLE event_handle)
{
  // Remember the old reactor.
  ACE_Reactor *old_reactor = event_handler->reactor ();

  // Assign *this* <Reactor> to the <Event_Handler>.
  event_handler->reactor (this);

  int result = this->implementation ()->register_handler (event_handler,
                                                          event_handle);
  if (result == -1)
    // Reset the old reactor in case of failures.
    event_handler->reactor (old_reactor);

  return result;
}

#endif /* ACE_WIN32 */

ACE_INLINE int
ACE_Reactor::register_handler (ACE_HANDLE event_handle,
                               ACE_HANDLE io_handle,
                               ACE_Event_Handler *event_handler,
                               ACE_Reactor_Mask mask)
{
  // Remember the old reactor.
  ACE_Reactor *old_reactor = event_handler->reactor ();

  // Assign *this* <Reactor> to the <Event_Handler>.
  event_handler->reactor (this);

  int result = this->implementation ()->register_handler (event_handle,
                                                          io_handle,
                                                          event_handler,
                                                          mask);
  if (result == -1)
    // Reset the old reactor in case of failures.
    event_handler->reactor (old_reactor);

  return result;
}

ACE_INLINE int
ACE_Reactor::register_handler (const ACE_Handle_Set &handles,
                               ACE_Event_Handler *event_handler,
                               ACE_Reactor_Mask mask)
{
  // Remember the old reactor.
  ACE_Reactor *old_reactor = event_handler->reactor ();

  // Assign *this* <Reactor> to the <Event_Handler>.
  event_handler->reactor (this);

  int result = this->implementation ()->register_handler (handles,
                                                          event_handler,
                                                          mask);
  if (result == -1)
    // Reset the old reactor in case of failures.
    event_handler->reactor (old_reactor);

  return result;
}

ACE_INLINE int
ACE_Reactor::register_handler (int signum,
                               ACE_Event_Handler *new_sh,
                               ACE_Sig_Action *new_disp,
                               ACE_Event_Handler **old_sh,
                               ACE_Sig_Action *old_disp)
{
  return this->implementation ()->register_handler (signum,
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
  return this->implementation ()->register_handler (sigset,
                                                    new_sh,
                                                    new_disp);
}

ACE_INLINE int
ACE_Reactor::remove_handler (ACE_Event_Handler *event_handler,
                             ACE_Reactor_Mask mask)
{
  return this->implementation ()->remove_handler (event_handler,
                                                  mask);
}

ACE_INLINE int
ACE_Reactor::remove_handler (ACE_HANDLE handle,
                             ACE_Reactor_Mask mask)
{
  return this->implementation ()->remove_handler (handle,
                                                  mask);
}

ACE_INLINE int
ACE_Reactor::remove_handler (const ACE_Handle_Set &handle_set,
                             ACE_Reactor_Mask mask)
{
  return this->implementation ()->remove_handler (handle_set,
                                                  mask);
}

ACE_INLINE int
ACE_Reactor::remove_handler (int signum,
                             ACE_Sig_Action *new_disp,
                             ACE_Sig_Action *old_disp,
                             int sigkey)
{
  return this->implementation ()->remove_handler (signum,
                                                  new_disp,
                                                  old_disp,
                                                  sigkey);
}

ACE_INLINE int
ACE_Reactor::remove_handler (const ACE_Sig_Set &sigset)
{
  return this->implementation ()->remove_handler (sigset);
}


ACE_INLINE int
ACE_Reactor::suspend_handler (ACE_Event_Handler *event_handler)
{
  return this->implementation ()->suspend_handler (event_handler);
}

ACE_INLINE int
ACE_Reactor::suspend_handler (ACE_HANDLE handle)
{
  return this->implementation ()->suspend_handler (handle);
}

ACE_INLINE int
ACE_Reactor::suspend_handler (const ACE_Handle_Set &handles)
{
  return this->implementation ()->suspend_handler (handles);
}

ACE_INLINE int
ACE_Reactor::suspend_handlers (void)
{
  return this->implementation ()->suspend_handlers ();
}

ACE_INLINE int
ACE_Reactor::resume_handler (ACE_Event_Handler *event_handler)
{
  return this->implementation ()->resume_handler (event_handler);
}

ACE_INLINE int
ACE_Reactor::resume_handler (ACE_HANDLE handle)
{
  return this->implementation ()->resume_handler (handle);
}

ACE_INLINE int
ACE_Reactor::resume_handler (const ACE_Handle_Set &handles)
{
  return this->implementation ()->resume_handler (handles);
}

ACE_INLINE int
ACE_Reactor::resume_handlers (void)
{
  return this->implementation ()->resume_handlers ();
}


ACE_INLINE int
ACE_Reactor::reset_timer_interval
  (long timer_id,
   const ACE_Time_Value &interval)
{
  ACE_TRACE ("ACE_Reactor::reset_timer_interval");

  return this->implementation ()->reset_timer_interval
    (timer_id,
     interval);
}

ACE_INLINE long
ACE_Reactor::schedule_timer (ACE_Event_Handler *event_handler,
                             const void *arg,
                             const ACE_Time_Value &delta,
                             const ACE_Time_Value &interval)
{
  // Remember the old reactor.
  ACE_Reactor *old_reactor = event_handler->reactor ();

  // Assign *this* <Reactor> to the <Event_Handler>.
  event_handler->reactor (this);

  int result = this->implementation ()->schedule_timer (event_handler,
                                                        arg,
                                                        delta,
                                                        interval);
  if (result == -1)
    // Reset the old reactor in case of failures.
    event_handler->reactor (old_reactor);

  return result;
}

ACE_INLINE int
ACE_Reactor::cancel_timer (ACE_Event_Handler *event_handler,
                           int dont_call_handle_close)
{
  return this->implementation ()->cancel_timer (event_handler,
                                                dont_call_handle_close);
}

ACE_INLINE int
ACE_Reactor::cancel_timer (long timer_id,
                           const void **arg,
                           int dont_call_handle_close)
{
  return this->implementation ()->cancel_timer (timer_id,
                                                arg,
                                                dont_call_handle_close);
}

ACE_INLINE int
ACE_Reactor::schedule_wakeup (ACE_Event_Handler *event_handler,
                              ACE_Reactor_Mask masks_to_be_added)
{
  // Remember the old reactor.
  ACE_Reactor *old_reactor = event_handler->reactor ();

  // Assign *this* <Reactor> to the <Event_Handler>.
  event_handler->reactor (this);

  int result = this->implementation ()->schedule_wakeup (event_handler,
                                                         masks_to_be_added);
  if (result == -1)
    // Reset the old reactor in case of failures.
    event_handler->reactor (old_reactor);

  return result;
}

ACE_INLINE int
ACE_Reactor::schedule_wakeup (ACE_HANDLE handle,
                              ACE_Reactor_Mask masks_to_be_added)
{
  return implementation ()->schedule_wakeup (handle,
                                             masks_to_be_added);
}

ACE_INLINE int
ACE_Reactor::cancel_wakeup (ACE_Event_Handler *event_handler,
                            ACE_Reactor_Mask masks_to_be_cleared)
{
  return this->implementation ()->cancel_wakeup (event_handler,
                                                 masks_to_be_cleared);
}

ACE_INLINE int
ACE_Reactor::cancel_wakeup (ACE_HANDLE handle,
                            ACE_Reactor_Mask masks_to_be_cleared)
{
  return this->implementation ()->cancel_wakeup (handle,
                                                 masks_to_be_cleared);
}


ACE_INLINE int
ACE_Reactor::notify (ACE_Event_Handler *event_handler,
                     ACE_Reactor_Mask mask,
                     ACE_Time_Value *tv)
{
  // First, try to remember this reactor in the event handler, in case
  // the event handler goes away before the notification is delivered.
  if (event_handler != 0 && event_handler->reactor () == 0)
    event_handler->reactor (this);
  return this->implementation ()->notify (event_handler,
                                          mask,
                                          tv);
}

ACE_INLINE void
ACE_Reactor::max_notify_iterations (int iterations)
{
  this->implementation ()->max_notify_iterations (iterations);
}

ACE_INLINE int
ACE_Reactor::max_notify_iterations (void)
{
  return this->implementation ()->max_notify_iterations ();
}

ACE_INLINE int
ACE_Reactor::purge_pending_notifications (ACE_Event_Handler *eh,
                                          ACE_Reactor_Mask   mask)
{
  return this->implementation ()->purge_pending_notifications (eh, mask);
}

ACE_INLINE ACE_Event_Handler *
ACE_Reactor::handler (ACE_HANDLE handle)
{
  return this->implementation ()->handler (handle);
}

ACE_INLINE int
ACE_Reactor::handler (ACE_HANDLE handle,
                      ACE_Reactor_Mask mask,
                      ACE_Event_Handler **event_handler)
{
  return this->implementation ()->handler (handle,
                                           mask,
                                           event_handler);
}

ACE_INLINE int
ACE_Reactor::handler (int signum,
                      ACE_Event_Handler **event_handler)
{
  return this->implementation ()->handler (signum,
                                           event_handler);
}

ACE_INLINE int
ACE_Reactor::initialized (void)
{
  return this->implementation ()->initialized ();
}

ACE_INLINE ACE_Lock &
ACE_Reactor::lock (void)
{
  return this->implementation ()->lock ();
}

ACE_INLINE void
ACE_Reactor::wakeup_all_threads (void)
{
  this->implementation ()->wakeup_all_threads ();
}

ACE_INLINE int
ACE_Reactor::owner (ACE_thread_t new_owner,
                    ACE_thread_t *old_owner)
{
  return this->implementation ()->owner (new_owner,
                                         old_owner);
}

ACE_INLINE int
ACE_Reactor::owner (ACE_thread_t *owner)
{
  return this->implementation ()->owner (owner);
}

ACE_INLINE int
ACE_Reactor::restart (void)
{
  return this->implementation ()->restart ();
}

ACE_INLINE int
ACE_Reactor::restart (int r)
{
  return this->implementation ()->restart (r);
}

ACE_INLINE void
ACE_Reactor::requeue_position (int position)
{
  this->implementation ()->requeue_position (position);
}

ACE_INLINE int
ACE_Reactor::requeue_position (void)
{
  return this->implementation ()->requeue_position ();
}


ACE_INLINE int
ACE_Reactor::mask_ops (ACE_Event_Handler *event_handler,
                       ACE_Reactor_Mask mask,
                       int ops)
{
  return this->implementation ()->mask_ops (event_handler,
                                            mask,
                                            ops);
}

ACE_INLINE int
ACE_Reactor::mask_ops (ACE_HANDLE handle,
                       ACE_Reactor_Mask mask,
                       int ops)
{
  return this->implementation ()->mask_ops (handle,
                                            mask,
                                            ops);
}

ACE_INLINE int
ACE_Reactor::ready_ops (ACE_Event_Handler *event_handler,
                        ACE_Reactor_Mask mask,
                        int ops)
{
  return this->implementation ()->ready_ops (event_handler,
                                             mask,
                                             ops);
}

ACE_INLINE int
ACE_Reactor::ready_ops (ACE_HANDLE handle,
                        ACE_Reactor_Mask mask,
                        int ops)
{
  return this->implementation ()->ready_ops (handle,
                                             mask,
                                             ops);
}

ACE_INLINE size_t
ACE_Reactor::size (void) const
{
  return this->implementation ()->size ();
}

ACE_INLINE int
ACE_Reactor::uses_event_associations (void)
{
  return this->implementation ()->uses_event_associations ();
}
