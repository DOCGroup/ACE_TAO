// $Id$

#include "ace/Reactor.h"

//#if !defined (ACE_HAS_WINCE)
#  if !defined (ACE_LACKS_ACE_SVCCONF)
#    include "ace/Service_Config.h"
#  endif /* !ACE_LACKS_ACE_SVCCONF */
//#endif /* ! ACE_HAS_WINCE */

/*
 * Hook to specialize the includes directly with the concrete
 * Reactor type, e.g., select, thread pool reactor
 * known at compile time. This hook results in all the
 * #defines being commented
 * out and the concrete header file directly included.
 */
//@@ REACTOR_SPL_COMMENT_INCLUDE_START_HOOK
// Only include the headers needed to compile.
#if !defined (ACE_WIN32) \
      || !defined (ACE_HAS_WINSOCK2) || (ACE_HAS_WINSOCK2 == 0) \
      || defined (ACE_USE_SELECT_REACTOR_FOR_REACTOR_IMPL) \
      || defined (ACE_USE_TP_REACTOR_FOR_REACTOR_IMPL) \
      || defined (ACE_USE_DEV_POLL_REACTOR_FOR_REACTOR_IMPL)
#  if defined (ACE_USE_TP_REACTOR_FOR_REACTOR_IMPL)
#    include "ace/TP_Reactor.h"
#  else
#    if defined (ACE_USE_DEV_POLL_REACTOR_FOR_REACTOR_IMPL)
#      include "ace/Dev_Poll_Reactor.h"
#    else
#      include "ace/Select_Reactor.h"
#    endif /* ACE_USE_DEV_POLL_REACTOR_FOR_REACTOR_IMPL */
#  endif /* ACE_USE_TP_REACTOR_FOR_REACTOR_IMPL */
#else /* We are on Win32 and we have winsock and ACE_USE_SELECT_REACTOR_FOR_REACTOR_IMPL is not defined */
#  if defined (ACE_USE_MSG_WFMO_REACTOR_FOR_REACTOR_IMPL)
#    include "ace/Msg_WFMO_Reactor.h"
#  else
#    include "ace/WFMO_Reactor.h"
#  endif /* ACE_USE_MSG_WFMO_REACTOR_FOR_REACTOR_IMPL */
#endif /* !defined (ACE_WIN32) || !defined (ACE_HAS_WINSOCK2) || (ACE_HAS_WINSOCK2 == 0) || defined (ACE_USE_SELECT_REACTOR_FOR_REACTOR_IMPL) */

/*
 * End comment hook.
 */
//@@ REACTOR_SPL_COMMENT_INCLUDE_END_HOOK

#include "ace/Static_Object_Lock.h"
#include "ace/Framework_Component.h"
#include "ace/Guard_T.h"
#include "ace/Recursive_Thread_Mutex.h"

#if !defined (__ACE_INLINE__)
#include "ace/Reactor.inl"
#endif /* __ACE_INLINE__ */


ACE_RCSID (ace,
           Reactor,
           "$Id$")


ACE_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_ALLOC_HOOK_DEFINE(ACE_Reactor)

ACE_Reactor::ACE_Reactor (ACE_Reactor_Impl *impl,
                          int delete_implementation)
  : implementation_ (0),
    delete_implementation_ (delete_implementation)
{
  this->implementation (impl);

  if (this->implementation () == 0)
    {
/*
 * Hook to specialize the reactor implementation with the concrete
 * Reactor implementation known at compile time. This hook will
 * cause the conditionally defined code to be commented out and
 * the concrete Reactor directly created.
 */
//@@ REACTOR_SPL_CONSTRUCTOR_COMMENT_HOOK_START
#if !defined (ACE_WIN32) \
      || !defined (ACE_HAS_WINSOCK2) || (ACE_HAS_WINSOCK2 == 0) \
      || defined (ACE_USE_SELECT_REACTOR_FOR_REACTOR_IMPL) \
      || defined (ACE_USE_TP_REACTOR_FOR_REACTOR_IMPL) \
      || defined (ACE_USE_DEV_POLL_REACTOR_FOR_REACTOR_IMPL)
#  if defined (ACE_USE_TP_REACTOR_FOR_REACTOR_IMPL)
      ACE_NEW (impl,
               ACE_TP_Reactor);
#  else
#    if defined (ACE_USE_DEV_POLL_REACTOR_FOR_REACTOR_IMPL)
      ACE_NEW (impl,
               ACE_Dev_Poll_Reactor);
#    else
      ACE_NEW (impl,
               ACE_Select_Reactor);
#    endif /* ACE_USE_DEV_POLL_REACTOR_FOR_REACTOR_IMPL */
#  endif /* ACE_USE_TP_REACTOR_FOR_REACTOR_IMPL */
#else /* We are on Win32 and we have winsock and ACE_USE_SELECT_REACTOR_FOR_REACTOR_IMPL is not defined */
  #if defined (ACE_USE_MSG_WFMO_REACTOR_FOR_REACTOR_IMPL)
      ACE_NEW (impl,
               ACE_Msg_WFMO_Reactor);
  #else
      ACE_NEW (impl,
               ACE_WFMO_Reactor);
  #endif /* ACE_USE_MSG_WFMO_REACTOR_FOR_REACTOR_IMPL */
#endif /* !defined (ACE_WIN32) || !defined (ACE_HAS_WINSOCK2) || (ACE_HAS_WINSOCK2 == 0) || defined (ACE_USE_SELECT_REACTOR_FOR_REACTOR_IMPL) */

/*
 * End hook.
 */
//@@ REACTOR_SPL_CONSTRUCTOR_COMMENT_HOOK_END

      this->implementation (impl);
      this->delete_implementation_ = 1;
    }
}

ACE_Reactor::~ACE_Reactor (void)
{
  this->implementation ()->close ();
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

  // We can't register the Reactor singleton as a framework component twice.
  // Therefore we test to see if we had an existing reactor instance, which
  // if so means it must have already been registered.
  if (t == 0)
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

const ACE_TCHAR *
ACE_Reactor::dll_name (void)
{
  return ACE_LIB_TEXT ("ACE");
}

const ACE_TCHAR *
ACE_Reactor::name (void)
{
  return ACE_LIB_TEXT ("ACE_Reactor");
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

ACE_Reactor_Impl *
ACE_Reactor::implementation (void) const
{
  return this->implementation_;
}

void
ACE_Reactor::implementation (ACE_Reactor_Impl *impl)
{
  this->implementation_ = impl;
}

int
ACE_Reactor::current_info (ACE_HANDLE handle,
                           size_t &size)
{
  return this->implementation ()->current_info (handle, size);
}

int
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
int
ACE_Reactor::set_sig_handler (ACE_Sig_Handler *signal_handler)
{
  return this->implementation ()->set_sig_handler (signal_handler);
}

int
ACE_Reactor::timer_queue (ACE_Timer_Queue *tq)
{
  return this->implementation ()->timer_queue (tq);
}

ACE_Timer_Queue *
ACE_Reactor::timer_queue (void) const
{
  return this->implementation ()->timer_queue ();
}

int
ACE_Reactor::close (void)
{
  return this->implementation ()->close ();
}

int
ACE_Reactor::work_pending (const ACE_Time_Value &max_wait_time)
{
  return this->implementation ()->work_pending (max_wait_time);
}

int
ACE_Reactor::handle_events (ACE_Time_Value *max_wait_time)
{
  return this->implementation ()->handle_events (max_wait_time);
}

int
ACE_Reactor::alertable_handle_events (ACE_Time_Value *max_wait_time)
{
  return this->implementation ()->alertable_handle_events (max_wait_time);
}

int
ACE_Reactor::handle_events (ACE_Time_Value &max_wait_time)
{
  return this->implementation ()->handle_events (max_wait_time);
}

int
ACE_Reactor::alertable_handle_events (ACE_Time_Value &max_wait_time)
{
  return this->implementation ()->alertable_handle_events (max_wait_time);
}


int
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

int
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

int
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

int
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

int
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

int
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

int
ACE_Reactor::register_handler (const ACE_Sig_Set &sigset,
                               ACE_Event_Handler *new_sh,
                               ACE_Sig_Action *new_disp)
{
  return this->implementation ()->register_handler (sigset,
                                                    new_sh,
                                                    new_disp);
}

int
ACE_Reactor::remove_handler (ACE_Event_Handler *event_handler,
                             ACE_Reactor_Mask mask)
{
  return this->implementation ()->remove_handler (event_handler,
                                                  mask);
}

int
ACE_Reactor::remove_handler (ACE_HANDLE handle,
                             ACE_Reactor_Mask mask)
{
  return this->implementation ()->remove_handler (handle,
                                                  mask);
}

int
ACE_Reactor::remove_handler (const ACE_Handle_Set &handle_set,
                             ACE_Reactor_Mask mask)
{
  return this->implementation ()->remove_handler (handle_set,
                                                  mask);
}

int
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

int
ACE_Reactor::remove_handler (const ACE_Sig_Set &sigset)
{
  return this->implementation ()->remove_handler (sigset);
}


int
ACE_Reactor::suspend_handler (ACE_Event_Handler *event_handler)
{
  return this->implementation ()->suspend_handler (event_handler);
}

int
ACE_Reactor::suspend_handler (ACE_HANDLE handle)
{
  return this->implementation ()->suspend_handler (handle);
}

int
ACE_Reactor::suspend_handler (const ACE_Handle_Set &handles)
{
  return this->implementation ()->suspend_handler (handles);
}

int
ACE_Reactor::suspend_handlers (void)
{
  return this->implementation ()->suspend_handlers ();
}

int
ACE_Reactor::resume_handler (ACE_Event_Handler *event_handler)
{
  return this->implementation ()->resume_handler (event_handler);
}

int
ACE_Reactor::resume_handler (ACE_HANDLE handle)
{
  return this->implementation ()->resume_handler (handle);
}

int
ACE_Reactor::resume_handler (const ACE_Handle_Set &handles)
{
  return this->implementation ()->resume_handler (handles);
}

int
ACE_Reactor::resume_handlers (void)
{
  return this->implementation ()->resume_handlers ();
}


int
ACE_Reactor::reset_timer_interval
  (long timer_id,
   const ACE_Time_Value &interval)
{
  ACE_TRACE ("ACE_Reactor::reset_timer_interval");

  return this->implementation ()->reset_timer_interval
    (timer_id,
     interval);
}

long
ACE_Reactor::schedule_timer (ACE_Event_Handler *event_handler,
                             const void *arg,
                             const ACE_Time_Value &delta,
                             const ACE_Time_Value &interval)
{
  // Remember the old reactor.
  ACE_Reactor *old_reactor = event_handler->reactor ();

  // Assign *this* <Reactor> to the <Event_Handler>.
  event_handler->reactor (this);

  long result = this->implementation ()->schedule_timer (event_handler,
                                                         arg,
                                                         delta,
                                                         interval);
  if (result == -1)
    // Reset the old reactor in case of failures.
    event_handler->reactor (old_reactor);

  return result;
}

int
ACE_Reactor::cancel_timer (ACE_Event_Handler *event_handler,
                           int dont_call_handle_close)
{
  return this->implementation ()->cancel_timer (event_handler,
                                                dont_call_handle_close);
}

int
ACE_Reactor::cancel_timer (long timer_id,
                           const void **arg,
                           int dont_call_handle_close)
{
  return this->implementation ()->cancel_timer (timer_id,
                                                arg,
                                                dont_call_handle_close);
}

int
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

int
ACE_Reactor::schedule_wakeup (ACE_HANDLE handle,
                              ACE_Reactor_Mask masks_to_be_added)
{
  return implementation ()->schedule_wakeup (handle,
                                             masks_to_be_added);
}

int
ACE_Reactor::cancel_wakeup (ACE_Event_Handler *event_handler,
                            ACE_Reactor_Mask masks_to_be_cleared)
{
  return this->implementation ()->cancel_wakeup (event_handler,
                                                 masks_to_be_cleared);
}

int
ACE_Reactor::cancel_wakeup (ACE_HANDLE handle,
                            ACE_Reactor_Mask masks_to_be_cleared)
{
  return this->implementation ()->cancel_wakeup (handle,
                                                 masks_to_be_cleared);
}


int
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

void
ACE_Reactor::max_notify_iterations (int iterations)
{
  this->implementation ()->max_notify_iterations (iterations);
}

int
ACE_Reactor::max_notify_iterations (void)
{
  return this->implementation ()->max_notify_iterations ();
}

int
ACE_Reactor::purge_pending_notifications (ACE_Event_Handler *eh,
                                          ACE_Reactor_Mask   mask)
{
  return this->implementation ()->purge_pending_notifications (eh, mask);
}

ACE_Event_Handler *
ACE_Reactor::find_handler (ACE_HANDLE handle)
{
  return this->implementation ()->find_handler (handle);
}

int
ACE_Reactor::handler (ACE_HANDLE handle,
                      ACE_Reactor_Mask mask,
                      ACE_Event_Handler **event_handler)
{
  return this->implementation ()->handler (handle,
                                           mask,
                                           event_handler);
}

int
ACE_Reactor::handler (int signum,
                      ACE_Event_Handler **event_handler)
{
  return this->implementation ()->handler (signum,
                                           event_handler);
}

int
ACE_Reactor::initialized (void)
{
  return this->implementation ()->initialized ();
}

ACE_Lock &
ACE_Reactor::lock (void)
{
  return this->implementation ()->lock ();
}

void
ACE_Reactor::wakeup_all_threads (void)
{
  this->implementation ()->wakeup_all_threads ();
}

int
ACE_Reactor::owner (ACE_thread_t new_owner,
                    ACE_thread_t *old_owner)
{
  return this->implementation ()->owner (new_owner,
                                         old_owner);
}

int
ACE_Reactor::owner (ACE_thread_t *owner)
{
  return this->implementation ()->owner (owner);
}

int
ACE_Reactor::restart (void)
{
  return this->implementation ()->restart ();
}

int
ACE_Reactor::restart (int r)
{
  return this->implementation ()->restart (r);
}

void
ACE_Reactor::requeue_position (int position)
{
  this->implementation ()->requeue_position (position);
}

int
ACE_Reactor::requeue_position (void)
{
  return this->implementation ()->requeue_position ();
}


int
ACE_Reactor::mask_ops (ACE_Event_Handler *event_handler,
                       ACE_Reactor_Mask mask,
                       int ops)
{
  return this->implementation ()->mask_ops (event_handler,
                                            mask,
                                            ops);
}

int
ACE_Reactor::mask_ops (ACE_HANDLE handle,
                       ACE_Reactor_Mask mask,
                       int ops)
{
  return this->implementation ()->mask_ops (handle,
                                            mask,
                                            ops);
}

int
ACE_Reactor::ready_ops (ACE_Event_Handler *event_handler,
                        ACE_Reactor_Mask mask,
                        int ops)
{
  return this->implementation ()->ready_ops (event_handler,
                                             mask,
                                             ops);
}

int
ACE_Reactor::ready_ops (ACE_HANDLE handle,
                        ACE_Reactor_Mask mask,
                        int ops)
{
  return this->implementation ()->ready_ops (handle,
                                             mask,
                                             ops);
}

int
ACE_Reactor::reactor_event_loop_done (void)
{
  ACE_TRACE ("ACE_Reactor::reactor_event_loop_done");
  return this->implementation_->deactivated ();
}

size_t
ACE_Reactor::size (void) const
{
  return this->implementation ()->size ();
}

int
ACE_Reactor::uses_event_associations (void)
{
  return this->implementation ()->uses_event_associations ();
}

void
ACE_Reactor::dump (void) const
{
#if defined (ACE_HAS_DUMP)
  ACE_TRACE ("ACE_Reactor::dump");

  implementation_->dump ();
#endif /* ACE_HAS_DUMP */
}

ACE_END_VERSIONED_NAMESPACE_DECL
