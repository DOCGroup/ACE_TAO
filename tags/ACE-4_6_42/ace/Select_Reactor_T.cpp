// $Id$

#ifndef ACE_SELECT_REACTOR_T_C
#define ACE_SELECT_REACTOR_T_C

#define ACE_BUILD_DLL
#include "ace/Select_Reactor_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Thread.h"
#include "ace/Timer_Heap.h"

// @@ The latest version of SunCC can't grok the code if we put inline
// function here.  Therefore, we temporarily disable the code here.
// We shall turn this back on once we know the problem gets fixed.
#if 1 // !defined (__ACE_INLINE__)
#include "ace/Select_Reactor_T.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(ace, Select_Reactor_T, "$Id$")

  ACE_ALLOC_HOOK_DEFINE(ACE_Select_Reactor_T)

#if defined (ACE_WIN32)
#define ACE_SELECT_REACTOR_HANDLE(H) (this->event_handlers_[(H)].handle_)
#define ACE_SELECT_REACTOR_EVENT_HANDLER(THIS,H) ((THIS)->event_handlers_[(H)].event_handler_)
#else
#define ACE_SELECT_REACTOR_HANDLE(H) (H)
#define ACE_SELECT_REACTOR_EVENT_HANDLER(THIS,H) ((THIS)->event_handlers_[(H)])
#endif /* ACE_WIN32 */

  template <class ACE_SELECT_REACTOR_TOKEN> int
ACE_Select_Reactor_T<ACE_SELECT_REACTOR_TOKEN>::any_ready
  (ACE_Select_Reactor_Handle_Set &wait_set)
{
  ACE_TRACE ("ACE_Select_Reactor_T::any_ready");

#if !defined (ACE_WIN32)
  // Make this call signal safe.
  ACE_Sig_Guard sb;
#endif /* ACE_WIN32 */

  int number_ready = this->ready_set_.rd_mask_.num_set ()
    + this->ready_set_.wr_mask_.num_set ()
    + this->ready_set_.ex_mask_.num_set ();

  if (number_ready > 0)
    {
      wait_set.rd_mask_ = this->ready_set_.rd_mask_;
      wait_set.wr_mask_ = this->ready_set_.wr_mask_;
      wait_set.ex_mask_ = this->ready_set_.ex_mask_;

      this->ready_set_.rd_mask_.reset ();
      this->ready_set_.wr_mask_.reset ();
      this->ready_set_.ex_mask_.reset ();
    }

  return number_ready;
}

template <class ACE_SELECT_REACTOR_TOKEN> int
ACE_Select_Reactor_T<ACE_SELECT_REACTOR_TOKEN>::handler_i (int signum,
                                                           ACE_Event_Handler **eh)
{
  ACE_TRACE ("ACE_Select_Reactor_T::handler_i");
  ACE_Event_Handler *handler = this->signal_handler_->handler (signum);

  if (handler == 0)
    return -1;
  else if (eh != 0 && *eh != 0)
    *eh = handler;
  return 0;
}

template <class ACE_SELECT_REACTOR_TOKEN> int
ACE_Select_Reactor_T<ACE_SELECT_REACTOR_TOKEN>::initialized (void)
{
  ACE_TRACE ("ACE_Select_Reactor_T::initialized");
  ACE_MT (ACE_GUARD_RETURN (ACE_SELECT_REACTOR_TOKEN, ace_mon, this->token_, 0));
  return this->initialized_;
}

template <class ACE_SELECT_REACTOR_TOKEN> int
ACE_Select_Reactor_T<ACE_SELECT_REACTOR_TOKEN>::owner (ACE_thread_t tid,
                                                       ACE_thread_t *o_id)
{
  ACE_TRACE ("ACE_Select_Reactor_T::owner");
  ACE_MT (ACE_GUARD_RETURN (ACE_SELECT_REACTOR_TOKEN, ace_mon, this->token_, -1));

  if (o_id)
    *o_id = this->owner_;

  this->owner_ = tid;

  return 0;
}

template <class ACE_SELECT_REACTOR_TOKEN> int
ACE_Select_Reactor_T<ACE_SELECT_REACTOR_TOKEN>::owner (ACE_thread_t *t_id)
{
  ACE_TRACE ("ACE_Select_Reactor_T::owner");
  ACE_MT (ACE_GUARD_RETURN (ACE_SELECT_REACTOR_TOKEN, ace_mon, this->token_, -1));
  *t_id = this->owner_;
  return 0;
}

template <class ACE_SELECT_REACTOR_TOKEN> void
ACE_Select_Reactor_T<ACE_SELECT_REACTOR_TOKEN>::requeue_position (int rp)
{
  ACE_TRACE ("ACE_Select_Reactor_T::requeue_position");
  ACE_MT (ACE_GUARD (ACE_SELECT_REACTOR_TOKEN, ace_mon, this->token_));
#if defined (ACE_WIN32)
  ACE_UNUSED_ARG (rp);
  // Must always requeue ourselves "next" on Win32.
  this->requeue_position_ = 0;
#else
  this->requeue_position_ = rp;
#endif /* ACE_WIN32 */
}

template <class ACE_SELECT_REACTOR_TOKEN> int
ACE_Select_Reactor_T<ACE_SELECT_REACTOR_TOKEN>::requeue_position (void)
{
  ACE_TRACE ("ACE_Select_Reactor_T::requeue_position");
  ACE_MT (ACE_GUARD_RETURN (ACE_SELECT_REACTOR_TOKEN, ace_mon, this->token_, -1));
  return this->requeue_position_;
}

template <class ACE_SELECT_REACTOR_TOKEN> void
ACE_Select_Reactor_T<ACE_SELECT_REACTOR_TOKEN>::max_notify_iterations (int iterations)
{
  ACE_TRACE ("ACE_Select_Reactor_T::max_notify_iterations");
  ACE_MT (ACE_GUARD (ACE_SELECT_REACTOR_TOKEN, ace_mon, this->token_));

  this->notify_handler_->max_notify_iterations (iterations);
}

template <class ACE_SELECT_REACTOR_TOKEN> int
ACE_Select_Reactor_T<ACE_SELECT_REACTOR_TOKEN>::max_notify_iterations (void)
{
  ACE_TRACE ("ACE_Select_Reactor_T::max_notify_iterations");
  ACE_MT (ACE_GUARD_RETURN (ACE_SELECT_REACTOR_TOKEN, ace_mon, this->token_, -1));
  return this->notify_handler_->max_notify_iterations ();
}

// Enqueue ourselves into the list of waiting threads.
template <class ACE_SELECT_REACTOR_TOKEN> void
ACE_Select_Reactor_T<ACE_SELECT_REACTOR_TOKEN>::renew (void)
{
  ACE_TRACE ("ACE_Select_Reactor_T::renew");
#if defined (ACE_MT_SAFE) && (ACE_MT_SAFE != 0)
  if (this->supress_notify_renew () == 0)
    this->token_.renew (this->requeue_position_);
#endif /* defined (ACE_MT_SAFE) && (ACE_MT_SAFE != 0) */
}

template <class ACE_SELECT_REACTOR_MUTEX> void
ACE_Select_Reactor_Token_T<ACE_SELECT_REACTOR_MUTEX>::dump (void) const
{
  ACE_TRACE ("ACE_Select_Reactor_Token::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("\n")));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

template <class ACE_SELECT_REACTOR_MUTEX>
ACE_Select_Reactor_Token_T<ACE_SELECT_REACTOR_MUTEX>::ACE_Select_Reactor_Token_T
  (ACE_Select_Reactor_Impl &r)
    : select_reactor_ (&r)
{
  ACE_TRACE ("ACE_Select_Reactor_Token::ACE_Select_Reactor_Token");
}

template <class ACE_SELECT_REACTOR_MUTEX>
ACE_Select_Reactor_Token_T<ACE_SELECT_REACTOR_MUTEX>::ACE_Select_Reactor_Token_T (void)
  : select_reactor_ (0)
{
  ACE_TRACE ("ACE_Select_Reactor_Token::ACE_Select_Reactor_Token");
}

template <class ACE_SELECT_REACTOR_MUTEX>
ACE_Select_Reactor_Token_T<ACE_SELECT_REACTOR_MUTEX>::~ACE_Select_Reactor_Token_T (void)
{
  ACE_TRACE ("ACE_Select_Reactor_Token::~ACE_Select_Reactor_Token");
}

template <class ACE_SELECT_REACTOR_MUTEX> ACE_Select_Reactor_Impl &
ACE_Select_Reactor_Token_T<ACE_SELECT_REACTOR_MUTEX>::select_reactor (void)
{
  return *this->select_reactor_;
}

template <class ACE_SELECT_REACTOR_MUTEX> void
ACE_Select_Reactor_Token_T<ACE_SELECT_REACTOR_MUTEX>::select_reactor
  (ACE_Select_Reactor_Impl &select_reactor)
{
  this->select_reactor_ = &select_reactor;
}

// Used to wakeup the Select_Reactor.

template <class ACE_SELECT_REACTOR_MUTEX> void
ACE_Select_Reactor_Token_T<ACE_SELECT_REACTOR_MUTEX>::sleep_hook (void)
{
  ACE_TRACE ("ACE_Select_Reactor_Token::sleep_hook");
  if (this->select_reactor_->notify () == -1)
    ACE_ERROR ((LM_ERROR,
                ASYS_TEXT ("%p\n"),
                ASYS_TEXT ("sleep_hook failed")));
}

template <class ACE_SELECT_REACTOR_TOKEN> int
ACE_Select_Reactor_T<ACE_SELECT_REACTOR_TOKEN>::notify (ACE_Event_Handler *eh,
                                                        ACE_Reactor_Mask mask,
                                                        ACE_Time_Value *timeout)
{
  ACE_TRACE ("ACE_Select_Reactor_T::notify");

  ssize_t n = 0;

  // Pass over both the Event_Handler *and* the mask to allow the
  // caller to dictate which Event_Handler method the receiver
  // invokes.  Note that this call can timeout.

  n = this->notify_handler_->notify (eh, mask, timeout);
  return n == -1 ? -1 : 0;
}

template <class ACE_SELECT_REACTOR_TOKEN> int
ACE_Select_Reactor_T<ACE_SELECT_REACTOR_TOKEN>::resume_handler (ACE_HANDLE handle)
{
  ACE_TRACE ("ACE_Select_Reactor_T::resume_handler");
  ACE_MT (ACE_GUARD_RETURN (ACE_SELECT_REACTOR_TOKEN, ace_mon, this->token_, -1));
  return this->resume_i (handle);
}

template <class ACE_SELECT_REACTOR_TOKEN> int
ACE_Select_Reactor_T<ACE_SELECT_REACTOR_TOKEN>::suspend_handler (ACE_HANDLE handle)
{
  ACE_TRACE ("ACE_Select_Reactor_T::suspend_handler");
  ACE_MT (ACE_GUARD_RETURN (ACE_SELECT_REACTOR_TOKEN, ace_mon, this->token_, -1));
  return this->suspend_i (handle);
}

template <class ACE_SELECT_REACTOR_TOKEN> int
ACE_Select_Reactor_T<ACE_SELECT_REACTOR_TOKEN>::suspend_handlers (void)
{
  ACE_TRACE ("ACE_Select_Reactor_T::suspend_handlers");
  ACE_MT (ACE_GUARD_RETURN (ACE_SELECT_REACTOR_TOKEN, ace_mon, this->token_, -1));

  ACE_Event_Handler *eh = 0;

  for (ACE_Select_Reactor_Handler_Repository_Iterator iter (&this->handler_rep_);
       iter.next (eh) != 0;
       iter.advance ())
    this->suspend_i (eh->get_handle ());

  return 0;
}

template <class ACE_SELECT_REACTOR_TOKEN> int
ACE_Select_Reactor_T<ACE_SELECT_REACTOR_TOKEN>::resume_handlers (void)
{
  ACE_TRACE ("ACE_Select_Reactor_T::resume_handlers");
  ACE_MT (ACE_GUARD_RETURN (ACE_SELECT_REACTOR_TOKEN, ace_mon, this->token_, -1));

  ACE_Event_Handler *eh = 0;

  for (ACE_Select_Reactor_Handler_Repository_Iterator iter (&this->handler_rep_);
       iter.next (eh) != 0;
       iter.advance ())
    this->resume_i (eh->get_handle ());

  return 0;
}

template <class ACE_SELECT_REACTOR_TOKEN> int
ACE_Select_Reactor_T<ACE_SELECT_REACTOR_TOKEN>::register_handler
  (ACE_Event_Handler *handler,
   ACE_Reactor_Mask mask)
{
  ACE_TRACE ("ACE_Select_Reactor_T::register_handler");
  ACE_MT (ACE_GUARD_RETURN (ACE_SELECT_REACTOR_TOKEN, ace_mon, this->token_, -1));
  return this->register_handler_i (handler->get_handle (), handler, mask);
}

template <class ACE_SELECT_REACTOR_TOKEN> int
ACE_Select_Reactor_T<ACE_SELECT_REACTOR_TOKEN>::register_handler
  (ACE_HANDLE handle,
   ACE_Event_Handler *handler,
   ACE_Reactor_Mask mask)
{
  ACE_TRACE ("ACE_Select_Reactor_T::register_handler");
  ACE_MT (ACE_GUARD_RETURN (ACE_SELECT_REACTOR_TOKEN, ace_mon, this->token_, -1));
  return this->register_handler_i (handle, handler, mask);
}

template <class ACE_SELECT_REACTOR_TOKEN> int
ACE_Select_Reactor_T<ACE_SELECT_REACTOR_TOKEN>::register_handler
  (const ACE_Handle_Set &handles,
   ACE_Event_Handler *handler,
   ACE_Reactor_Mask mask)
{
  ACE_TRACE ("ACE_Select_Reactor_T::register_handler");
  ACE_MT (ACE_GUARD_RETURN (ACE_SELECT_REACTOR_TOKEN, ace_mon, this->token_, -1));
  return this->register_handler_i (handles, handler, mask);
}

template <class ACE_SELECT_REACTOR_TOKEN> int
ACE_Select_Reactor_T<ACE_SELECT_REACTOR_TOKEN>::handler
  (ACE_HANDLE handle,
   ACE_Reactor_Mask mask,
   ACE_Event_Handler **handler)
{
  ACE_TRACE ("ACE_Select_Reactor_T::handler");
  ACE_MT (ACE_GUARD_RETURN (ACE_SELECT_REACTOR_TOKEN, ace_mon, this->token_, -1));
  return this->handler_i (handle, mask, handler);
}

template <class ACE_SELECT_REACTOR_TOKEN> int
ACE_Select_Reactor_T<ACE_SELECT_REACTOR_TOKEN>::remove_handler
  (const ACE_Handle_Set &handles,
   ACE_Reactor_Mask mask)
{
  ACE_TRACE ("ACE_Select_Reactor_T::remove_handler");
  ACE_MT (ACE_GUARD_RETURN (ACE_SELECT_REACTOR_TOKEN, ace_mon, this->token_, -1));
  return this->remove_handler_i (handles, mask);
}

template <class ACE_SELECT_REACTOR_TOKEN> int
ACE_Select_Reactor_T<ACE_SELECT_REACTOR_TOKEN>::remove_handler
  (ACE_Event_Handler *handler,
   ACE_Reactor_Mask mask)
{
  ACE_TRACE ("ACE_Select_Reactor_T::remove_handler");
  ACE_MT (ACE_GUARD_RETURN (ACE_SELECT_REACTOR_TOKEN, ace_mon, this->token_, -1));
  return this->remove_handler_i (handler->get_handle (), mask);
}

template <class ACE_SELECT_REACTOR_TOKEN> int
ACE_Select_Reactor_T<ACE_SELECT_REACTOR_TOKEN>::remove_handler
  (ACE_HANDLE handle,
   ACE_Reactor_Mask mask)
{
  ACE_TRACE ("ACE_Select_Reactor_T::remove_handler");
  ACE_MT (ACE_GUARD_RETURN (ACE_SELECT_REACTOR_TOKEN, ace_mon, this->token_, -1));
  return this->remove_handler_i (handle, mask);
}

// Performs operations on the "ready" bits.

template <class ACE_SELECT_REACTOR_TOKEN> int
ACE_Select_Reactor_T<ACE_SELECT_REACTOR_TOKEN>::ready_ops
  (ACE_HANDLE handle,
   ACE_Reactor_Mask mask,
   int ops)
{
  ACE_TRACE ("ACE_Select_Reactor_T::ready_ops");
  ACE_MT (ACE_GUARD_RETURN (ACE_SELECT_REACTOR_TOKEN, ace_mon, this->token_, -1));
  return this->bit_ops (handle,
                        mask,
                        this->ready_set_,
                        ops);
}

template <class ACE_SELECT_REACTOR_TOKEN> int
ACE_Select_Reactor_T<ACE_SELECT_REACTOR_TOKEN>::open
  (size_t size,
   int restart,
   ACE_Sig_Handler *sh,
   ACE_Timer_Queue *tq,
   int disable_notify_pipe,
   ACE_Reactor_Notify *notify)
{
  ACE_TRACE ("ACE_Select_Reactor_T::open");
  ACE_MT (ACE_GUARD_RETURN (ACE_SELECT_REACTOR_TOKEN, ace_mon, this->token_, -1));

  // Can't initialize ourselves more than once.
  if (this->initialized_ > 0)
    return -1;

  this->owner_ = ACE_Thread::self ();
  this->restart_ = restart;
  this->signal_handler_ = sh;
  this->timer_queue_ = tq;
  this->notify_handler_ = notify;

  int result = 0;

  // Allows the signal handler to be overridden.
  if (this->signal_handler_ == 0)
    {
      ACE_NEW_RETURN (this->signal_handler_,
                      ACE_Sig_Handler,
                      -1);

      if (this->signal_handler_ == 0)
        result = -1;
      else
        this->delete_signal_handler_ = 1;
    }

  // Allows the timer queue to be overridden.
  if (result != -1 && this->timer_queue_ == 0)
    {
      ACE_NEW_RETURN (this->timer_queue_,
                      ACE_Timer_Heap,
                      -1);

      if (this->timer_queue_ == 0)
        result = -1;
      else
        this->delete_timer_queue_ = 1;
    }

  // Allows the Notify_Handler to be overridden.
  if (result != -1 && this->notify_handler_ == 0)
    {
      ACE_NEW_RETURN (this->notify_handler_,
                      ACE_Select_Reactor_Notify,
                      -1);

      if (this->notify_handler_ == 0)
        result = -1;
      else
        this->delete_notify_handler_ = 1;
    }

  if (result != -1 && this->handler_rep_.open (size) == -1)
    result = -1;
  else if (this->notify_handler_->open (this,
                                        0,
                                        disable_notify_pipe) == -1)
    result = -1;

  if (result != -1)
    // We're all set to go.
    this->initialized_ = 1;
  else
    // This will close down all the allocated resources properly.
    this->close ();

  return result;
}

template <class ACE_SELECT_REACTOR_TOKEN> int
ACE_Select_Reactor_T<ACE_SELECT_REACTOR_TOKEN>::set_sig_handler
  (ACE_Sig_Handler *signal_handler)
{
  if (this->signal_handler_ != 0 && this->delete_signal_handler_ != 0)
    delete this->signal_handler_;
  this->signal_handler_ = signal_handler;
  this->delete_signal_handler_ = 0;
  return 0;
}

template <class ACE_SELECT_REACTOR_TOKEN> int
ACE_Select_Reactor_T<ACE_SELECT_REACTOR_TOKEN>::set_timer_queue
  (ACE_Timer_Queue *timer_queue)
{
  if (this->timer_queue_ != 0 && this->delete_timer_queue_ != 0)
    delete this->timer_queue_;
  this->timer_queue_ = timer_queue;
  this->delete_timer_queue_ = 0;
  return 0;
}

template <class ACE_SELECT_REACTOR_TOKEN>
ACE_Select_Reactor_T<ACE_SELECT_REACTOR_TOKEN>::ACE_Select_Reactor_T
  (ACE_Sig_Handler *sh,
   ACE_Timer_Queue *tq,
   int disable_notify_pipe,
   ACE_Reactor_Notify *notify)
    : token_ (*this),
      lock_adapter_ (token_)
{
  ACE_TRACE ("ACE_Select_Reactor_T::ACE_Select_Reactor_T");

  if (this->open (ACE_Select_Reactor_T<ACE_SELECT_REACTOR_TOKEN>::DEFAULT_SIZE,
                  0,
                  sh,
                  tq,
                  disable_notify_pipe,
                  notify) == -1)
    ACE_ERROR ((LM_ERROR,
                ASYS_TEXT ("%p\n"),
                ASYS_TEXT ("ACE_Select_Reactor_T::open ")
                ASYS_TEXT ("failed inside ACE_Select_Reactor_T::CTOR")));
}

// Initialize ACE_Select_Reactor_T.

template <class ACE_SELECT_REACTOR_TOKEN>
ACE_Select_Reactor_T<ACE_SELECT_REACTOR_TOKEN>::ACE_Select_Reactor_T
  (size_t size,
   int rs,
   ACE_Sig_Handler *sh,
   ACE_Timer_Queue *tq,
   int disable_notify_pipe,
   ACE_Reactor_Notify *notify)
    : token_ (*this),
      lock_adapter_ (token_)
{
  ACE_TRACE ("ACE_Select_Reactor_T::ACE_Select_Reactor_T");

  if (this->open (size,
                  rs,
                  sh,
                  tq,
                  disable_notify_pipe,
                  notify) == -1)
    ACE_ERROR ((LM_ERROR,
                ASYS_TEXT ("%p\n"),
                ASYS_TEXT ("ACE_Select_Reactor_T::open ")
                ASYS_TEXT ("failed inside ACE_Select_Reactor_T::CTOR")));
}

// Close down the ACE_Select_Reactor_T instance, detaching any
// remaining Event_Handers.  This had better be called from the main
// event loop thread...

template <class ACE_SELECT_REACTOR_TOKEN> int
ACE_Select_Reactor_T<ACE_SELECT_REACTOR_TOKEN>::close (void)
{
  ACE_TRACE ("ACE_Select_Reactor_T::close");
  ACE_MT (ACE_GUARD_RETURN (ACE_SELECT_REACTOR_TOKEN, ace_mon, this->token_, -1));

  if (this->delete_signal_handler_)
    {
      delete this->signal_handler_;
      this->signal_handler_ = 0;
      this->delete_signal_handler_ = 0;
    }

  this->handler_rep_.close ();

  if (this->delete_timer_queue_)
    {
      delete this->timer_queue_;
      this->timer_queue_ = 0;
      this->delete_timer_queue_ = 0;
    }

  this->notify_handler_->close ();

  if (this->delete_notify_handler_)
    {
      delete this->notify_handler_;
      this->notify_handler_ = 0;
      this->delete_notify_handler_ = 0;
    }

  this->initialized_ = 0;

  return 0;
}

template <class ACE_SELECT_REACTOR_TOKEN> int
ACE_Select_Reactor_T<ACE_SELECT_REACTOR_TOKEN>::current_info
  (ACE_HANDLE, size_t &)
{
  return -1;
}

template <class ACE_SELECT_REACTOR_TOKEN>
ACE_Select_Reactor_T<ACE_SELECT_REACTOR_TOKEN>::~ACE_Select_Reactor_T (void)
{
  ACE_TRACE ("ACE_Select_Reactor_T::~ACE_Select_Reactor_T");
  this->close ();
}

template <class ACE_SELECT_REACTOR_TOKEN> int
ACE_Select_Reactor_T<ACE_SELECT_REACTOR_TOKEN>::remove_handler_i
  (const ACE_Handle_Set &handles,
   ACE_Reactor_Mask mask)
{
  ACE_TRACE ("ACE_Select_Reactor_T::remove_handler_i");
  ACE_HANDLE h;

  ACE_Handle_Set_Iterator handle_iter (handles);

  while ((h = handle_iter ()) != ACE_INVALID_HANDLE)
    if (this->remove_handler_i (h, mask) == -1)
      return -1;

  return 0;
}

template <class ACE_SELECT_REACTOR_TOKEN> int
ACE_Select_Reactor_T<ACE_SELECT_REACTOR_TOKEN>::register_handler_i
  (const ACE_Handle_Set &handles,
   ACE_Event_Handler *handler,
   ACE_Reactor_Mask mask)
{
  ACE_TRACE ("ACE_Select_Reactor_T::register_handler_i");
  ACE_HANDLE h;

  ACE_Handle_Set_Iterator handle_iter (handles);
  while ((h = handle_iter ()) != ACE_INVALID_HANDLE)
    if (this->register_handler_i (h, handler, mask) == -1)
      return -1;

  return 0;
}

template <class ACE_SELECT_REACTOR_TOKEN> int
ACE_Select_Reactor_T<ACE_SELECT_REACTOR_TOKEN>::register_handler
  (const ACE_Sig_Set &sigset,
   ACE_Event_Handler *new_sh,
   ACE_Sig_Action *new_disp)
{
  ACE_TRACE ("ACE_Select_Reactor_T::register_handler");

  int result = 0;

#if (ACE_NSIG > 0)
  for (int s = 1; s < ACE_NSIG; s++)
    if (sigset.is_member (s)
        && this->signal_handler_->register_handler (s, new_sh,
                                                    new_disp) == -1)
      result = -1;
#else
  ACE_UNUSED_ARG (sigset);
  ACE_UNUSED_ARG (new_sh);
  ACE_UNUSED_ARG (new_disp);
#endif /* ACE_NSIG */
  return result;
}

template <class ACE_SELECT_REACTOR_TOKEN> int
ACE_Select_Reactor_T<ACE_SELECT_REACTOR_TOKEN>::remove_handler
  (const ACE_Sig_Set &sigset)
{
  ACE_TRACE ("ACE_Select_Reactor_T::remove_handler");
  int result = 0;

#if (ACE_NSIG > 0)
  for (int s = 1; s < ACE_NSIG; s++)
    if (sigset.is_member (s)
        && this->signal_handler_->remove_handler (s) == -1)
      result = -1;
#else
  ACE_UNUSED_ARG (sigset);
#endif /* ACE_NSIG */

  return result;
}

// Note the queue handles its own locking.

template <class ACE_SELECT_REACTOR_TOKEN> long
ACE_Select_Reactor_T<ACE_SELECT_REACTOR_TOKEN>::schedule_timer
  (ACE_Event_Handler *handler,
   const void *arg,
   const ACE_Time_Value &delta_time,
   const ACE_Time_Value &interval)
{
  ACE_TRACE ("ACE_Select_Reactor_T::schedule_timer");
  ACE_MT (ACE_GUARD_RETURN (ACE_SELECT_REACTOR_TOKEN, ace_mon, this->token_, -1));

  return this->timer_queue_->schedule
    (handler, arg, timer_queue_->gettimeofday () + delta_time, interval);
}

// Main event loop driver that blocks for <max_wait_time> before
// returning (will return earlier if I/O or signal events occur).

template <class ACE_SELECT_REACTOR_TOKEN> int
ACE_Select_Reactor_T<ACE_SELECT_REACTOR_TOKEN>::handle_events
  (ACE_Time_Value &max_wait_time)
{
  ACE_TRACE ("ACE_Select_Reactor_T::handle_events");

  return this->handle_events (&max_wait_time);
}

template <class ACE_SELECT_REACTOR_TOKEN> int
ACE_Select_Reactor_T<ACE_SELECT_REACTOR_TOKEN>::handle_error (void)
{
  ACE_TRACE ("ACE_Select_Reactor_T::handle_error");
  if (errno == EINTR)
    return this->restart_;
#if defined (__MVS__) || defined (ACE_WIN32)
  // On MVS Open Edition and Win32, there can be a number of failure
  // codes on a bad socket, so check_handles on anything other than
  // EINTR.
  else
    return this->check_handles ();
#else
  else if (errno == EBADF)
    return this->check_handles ();
  else
    return -1;
#endif  /* __MVS__ */
}

template <class ACE_SELECT_REACTOR_TOKEN> void
ACE_Select_Reactor_T<ACE_SELECT_REACTOR_TOKEN>::notify_handle
  (ACE_HANDLE handle,
   ACE_Reactor_Mask mask,
   ACE_Handle_Set &ready_mask,
   ACE_Event_Handler *event_handler,
   ACE_EH_PTMF ptmf)
{
  ACE_TRACE ("ACE_Select_Reactor_T::notify_handle");
  // Check for removed handlers.
  if (event_handler == 0)
    return;

  int status = (event_handler->*ptmf) (handle);

  if (status < 0)
    this->remove_handler_i (handle, mask);
  else if (status > 0)
    ready_mask.set_bit (handle);
}

// Perform GET, CLR, SET, and ADD operations on the select()
// Handle_Sets.
//
// GET = 1, Retrieve current value
// SET = 2, Set value of bits to new mask (changes the entire mask)
// ADD = 3, Bitwise "or" the value into the mask (only changes
//          enabled bits)
// CLR = 4  Bitwise "and" the negation of the value out of the mask
//          (only changes enabled bits)
//
// Returns the original mask.

template <class ACE_SELECT_REACTOR_TOKEN> int
ACE_Select_Reactor_T<ACE_SELECT_REACTOR_TOKEN>::mask_ops
  (ACE_HANDLE handle,
   ACE_Reactor_Mask mask,
   int ops)
{
  ACE_TRACE ("ACE_Select_Reactor_T::mask_ops");
  ACE_MT (ACE_GUARD_RETURN (ACE_SELECT_REACTOR_TOKEN, ace_mon, this->token_, -1));
  return this->bit_ops (handle, mask,
                        this->wait_set_,
                        ops);
}

// Must be called with locks held.

template <class ACE_SELECT_REACTOR_TOKEN> int
ACE_Select_Reactor_T<ACE_SELECT_REACTOR_TOKEN>::handler_i
  (ACE_HANDLE handle,
   ACE_Reactor_Mask mask,
   ACE_Event_Handler **handler)
{
  ACE_TRACE ("ACE_Select_Reactor_T::handler_i");
  ACE_Event_Handler *h = this->handler_rep_.find (handle);

  if (h == 0)
    return -1;
  else
    {
      if ((ACE_BIT_ENABLED (mask, ACE_Event_Handler::READ_MASK)
           || ACE_BIT_ENABLED (mask, ACE_Event_Handler::ACCEPT_MASK))
          && this->wait_set_.rd_mask_.is_set (handle) == 0)
        return -1;
      if (ACE_BIT_ENABLED (mask, ACE_Event_Handler::WRITE_MASK)
          && this->wait_set_.wr_mask_.is_set (handle) == 0)
        return -1;
      if (ACE_BIT_ENABLED (mask, ACE_Event_Handler::EXCEPT_MASK)
          && this->wait_set_.ex_mask_.is_set (handle) == 0)
        return -1;
    }

  if (handler != 0)
    *handler = h;
  return 0;
}

// Must be called with locks held

template <class ACE_SELECT_REACTOR_TOKEN> int
ACE_Select_Reactor_T<ACE_SELECT_REACTOR_TOKEN>::resume_i (ACE_HANDLE handle)
{
  ACE_TRACE ("ACE_Select_Reactor_T::resume");
  if (this->handler_rep_.find (handle) == 0)
    return -1;

  if (this->suspend_set_.rd_mask_.is_set (handle))
    {
      this->wait_set_.rd_mask_.set_bit (handle);
      this->suspend_set_.rd_mask_.clr_bit (handle);
    }
  if (this->suspend_set_.wr_mask_.is_set (handle))
    {
      this->wait_set_.wr_mask_.set_bit (handle);
      this->suspend_set_.wr_mask_.clr_bit (handle);
    }
  if (this->suspend_set_.ex_mask_.is_set (handle))
    {
      this->wait_set_.ex_mask_.set_bit (handle);
      this->suspend_set_.ex_mask_.clr_bit (handle);
    }
  return 0;
}

// Must be called with locks held

template <class ACE_SELECT_REACTOR_TOKEN> int
ACE_Select_Reactor_T<ACE_SELECT_REACTOR_TOKEN>::suspend_i (ACE_HANDLE handle)
{
  ACE_TRACE ("ACE_Select_Reactor_T::suspend");
  if (this->handler_rep_.find (handle) == 0)
    return -1;

  if (this->wait_set_.rd_mask_.is_set (handle))
    {
      this->suspend_set_.rd_mask_.set_bit (handle);
      this->wait_set_.rd_mask_.clr_bit (handle);
    }
  if (this->wait_set_.wr_mask_.is_set (handle))
    {
      this->suspend_set_.wr_mask_.set_bit (handle);
      this->wait_set_.wr_mask_.clr_bit (handle);
    }
  if (this->wait_set_.ex_mask_.is_set (handle))
    {
      this->suspend_set_.ex_mask_.set_bit (handle);
      this->wait_set_.ex_mask_.clr_bit (handle);
    }
  return 0;
}

// Must be called with locks held

template <class ACE_SELECT_REACTOR_TOKEN> int
ACE_Select_Reactor_T<ACE_SELECT_REACTOR_TOKEN>::register_handler_i
  (ACE_HANDLE handle,
   ACE_Event_Handler *event_handler,
   ACE_Reactor_Mask mask)
{
  ACE_TRACE ("ACE_Select_Reactor_T::register_handler_i");

  // Insert the <handle, event_handle> tuple into the Handler
  // Repository.
  return this->handler_rep_.bind (handle, event_handler, mask);
}

template <class ACE_SELECT_REACTOR_TOKEN> int
ACE_Select_Reactor_T<ACE_SELECT_REACTOR_TOKEN>::remove_handler_i
  (ACE_HANDLE handle,
   ACE_Reactor_Mask mask)
{
  ACE_TRACE ("ACE_Select_Reactor_T::remove_handler_i");

  // Unbind this handle.
  return this->handler_rep_.unbind (handle, mask);
}

// Must be called with lock held.

template <class ACE_SELECT_REACTOR_TOKEN> int
ACE_Select_Reactor_T<ACE_SELECT_REACTOR_TOKEN>::wait_for_multiple_events
  (ACE_Select_Reactor_Handle_Set &dispatch_set,
   ACE_Time_Value *max_wait_time)
{
  ACE_TRACE ("ACE_Select_Reactor_T::wait_for_multiple_events");
  u_long width = 0;
  ACE_Time_Value timer_buf (0);
  ACE_Time_Value *this_timeout = &timer_buf;

  int number_of_active_handles = this->any_ready (dispatch_set);

  // If there are any bits enabled in the <ready_set_> then we'll
  // handle those first, otherwise we'll block in select().

  if (number_of_active_handles == 0)
    {
      do
        {
          if (this->timer_queue_->calculate_timeout (max_wait_time,
                                                     this_timeout) == 0)
            this_timeout = 0;

          width = (u_long) this->handler_rep_.max_handlep1 ();

          dispatch_set.rd_mask_ = this->wait_set_.rd_mask_;
          dispatch_set.wr_mask_ = this->wait_set_.wr_mask_;
          dispatch_set.ex_mask_ = this->wait_set_.ex_mask_;

          number_of_active_handles = ACE_OS::select (int (width),
                                                     dispatch_set.rd_mask_,
                                                     dispatch_set.wr_mask_,
                                                     dispatch_set.ex_mask_,
                                                     this_timeout);
        }
      while (number_of_active_handles == -1 && this->handle_error () > 0);

      if (number_of_active_handles > 0)
        {
#if !defined (ACE_WIN32)
          // Resynchronize the fd_sets so their "max" is set properly.
          dispatch_set.rd_mask_.sync (this->handler_rep_.max_handlep1 ());
          dispatch_set.wr_mask_.sync (this->handler_rep_.max_handlep1 ());
          dispatch_set.ex_mask_.sync (this->handler_rep_.max_handlep1 ());
#endif /* ACE_WIN32 */
        }
    }

  // Return the number of events to dispatch.
  return number_of_active_handles;
}

template <class ACE_SELECT_REACTOR_TOKEN> int
ACE_Select_Reactor_T<ACE_SELECT_REACTOR_TOKEN>::dispatch_timer_handlers
  (int &number_of_handlers_dispatched)
{
  number_of_handlers_dispatched += this->timer_queue_->expire ();
  if (this->state_changed_)
    return -1;
  else
    return 0;
}

template <class ACE_SELECT_REACTOR_TOKEN> int
ACE_Select_Reactor_T<ACE_SELECT_REACTOR_TOKEN>::dispatch_notification_handlers
  (ACE_Select_Reactor_Handle_Set &dispatch_set,
   int &number_of_active_handles,
   int &number_of_handlers_dispatched)
{
#if defined (ACE_MT_SAFE) && (ACE_MT_SAFE != 0)
  // Check to see if the ACE_HANDLE associated with the
  // Select_Reactor's notify hook is enabled.  If so, it means that
  // one or more other threads are trying to update the
  // ACE_Select_Reactor_T's internal tables.  We'll handle all these
  // threads and then break out to continue the event loop.

  number_of_handlers_dispatched +=
    this->notify_handler_->dispatch_notifications (number_of_active_handles,
                                                   dispatch_set.rd_mask_);
  return this->state_changed_ ? -1 : 0;
#else
  ACE_UNUSED_ARG (number_of_active_handles);
  ACE_UNUSED_ARG (dispatch_set);
  return 0;
#endif /* ACE_MT_SAFE */
}

template <class ACE_SELECT_REACTOR_TOKEN> int
ACE_Select_Reactor_T<ACE_SELECT_REACTOR_TOKEN>::dispatch_io_set
  (int number_of_active_handles,
   int &number_of_handlers_dispatched,
   int mask,
   ACE_Handle_Set &dispatch_mask,
   ACE_Handle_Set &ready_mask,
   ACE_EH_PTMF callback)
{
  ACE_HANDLE handle;

  ACE_Handle_Set_Iterator handle_iter (dispatch_mask);

  while ((handle = handle_iter ()) != ACE_INVALID_HANDLE
         && number_of_handlers_dispatched < number_of_active_handles
         && this->state_changed_ == 0)
    {
      // ACE_DEBUG ((LM_DEBUG,  ASYS_TEXT ("ACE_Select_Reactor_T::dispatching\n")));
      number_of_handlers_dispatched++;
      this->notify_handle (handle,
                           mask,
                           ready_mask,
                           this->handler_rep_.find (handle),
                           callback);
    }

  if (number_of_handlers_dispatched > 0 && this->state_changed_)
    return -1;

  return 0;
}

template <class ACE_SELECT_REACTOR_TOKEN> int
ACE_Select_Reactor_T<ACE_SELECT_REACTOR_TOKEN>::dispatch_io_handlers
  (ACE_Select_Reactor_Handle_Set &dispatch_set,
   int &number_of_active_handles,
   int &number_of_handlers_dispatched)
{
  // Handle output events (this code needs to come first to handle the
  // obscure case of piggy-backed data coming along with the final
  // handshake message of a nonblocking connection).

  // ACE_DEBUG ((LM_DEBUG,  ASYS_TEXT ("ACE_Select_Reactor_T::dispatch - WRITE\n")));
  if (this->dispatch_io_set (number_of_active_handles,
                             number_of_handlers_dispatched,
                             ACE_Event_Handler::WRITE_MASK,
                             dispatch_set.wr_mask_,
                             this->ready_set_.wr_mask_,
                             &ACE_Event_Handler::handle_output) == -1)
    {
      number_of_active_handles -= number_of_handlers_dispatched;
      return -1;
    }

  // ACE_DEBUG ((LM_DEBUG,  ASYS_TEXT ("ACE_Select_Reactor_T::dispatch - EXCEPT\n")));
  if (this->dispatch_io_set (number_of_active_handles,
                             number_of_handlers_dispatched,
                             ACE_Event_Handler::EXCEPT_MASK,
                             dispatch_set.ex_mask_,
                             this->ready_set_.ex_mask_,
                             &ACE_Event_Handler::handle_exception) == -1)
    {
      number_of_active_handles -= number_of_handlers_dispatched;
      return -1;
    }

  // ACE_DEBUG ((LM_DEBUG,  ASYS_TEXT ("ACE_Select_Reactor_T::dispatch - READ\n")));
  if (this->dispatch_io_set (number_of_active_handles,
                             number_of_handlers_dispatched,
                             ACE_Event_Handler::READ_MASK,
                             dispatch_set.rd_mask_,
                             this->ready_set_.rd_mask_,
                             &ACE_Event_Handler::handle_input) == -1)
    {
      number_of_active_handles -= number_of_handlers_dispatched;
      return -1;
    }

  number_of_active_handles -= number_of_handlers_dispatched;
  return 0;
}

template <class ACE_SELECT_REACTOR_TOKEN> int
ACE_Select_Reactor_T<ACE_SELECT_REACTOR_TOKEN>::dispatch
  (int active_handle_count,
   ACE_Select_Reactor_Handle_Set &dispatch_set)
{
  ACE_TRACE ("ACE_Select_Reactor_T::dispatch");

  int io_handlers_dispatched = 0;
  int other_handlers_dispatched = 0;
  int signal_occurred = 0;
  // The following do/while loop keeps dispatching as long as there
  // are still active handles.  Note that the only way we should ever
  // iterate more than once through this loop is if signals occur
  // while we're dispatching other handlers.

  do
    {
      // Note that we keep track of changes to our state.  If any of
      // the dispatch_*() methods below return -1 it means that the
      // <wait_set_> state has changed as the result of an
      // <ACE_Event_Handler> being dispatched.  This means that we
      // need to bail out and rerun the select() loop since our
      // existing notion of handles in <dispatch_set> may no longer be
      // correct.
      //
      // In the beginning, our state starts out unchanged.  After
      // every iteration (i.e., due to signals), our state starts out
      // unchanged again.

      this->state_changed_ = 0;

      // Perform the Template Method for dispatching all the handlers.

      // First check for interrupts.
      if (active_handle_count == -1)
        {
          // Bail out -- we got here since <select> was interrupted.
          if (ACE_Sig_Handler::sig_pending () != 0)
            {
              ACE_Sig_Handler::sig_pending (0);

              // If any HANDLES in the <ready_set_> are activated as a
              // result of signals they should be dispatched since
              // they may be time critical...
              active_handle_count = this->any_ready (dispatch_set);

              // Record the fact that the Reactor has dispatched a
              // handle_signal() method.  We need this to return the
              // appropriate count below.
              signal_occurred = 1;
            }
          else
            return -1;
        }

      // Handle timers early since they may have higher latency
      // constraints than I/O handlers.  Ideally, the order of
      // dispatching should be a strategy...
      else if (this->dispatch_timer_handlers (other_handlers_dispatched) == -1)
        // State has changed or timer queue has failed, exit loop.
        break;

      // Check to see if there are no more I/O handles left to
      // dispatch AFTER we've handled the timers...
      else if (active_handle_count == 0)
        return io_handlers_dispatched
          + other_handlers_dispatched
          + signal_occurred;

      // Next dispatch the notification handlers (if there are any to
      // dispatch).  These are required to handle multi-threads that
      // are trying to update the <Reactor>.

      else if (this->dispatch_notification_handlers
               (dispatch_set,
                active_handle_count,
                other_handlers_dispatched) == -1)
        break; // State has changed, exit loop.

      // Finally, dispatch the I/O handlers.
      else if (this->dispatch_io_handlers
               (dispatch_set,
                active_handle_count,
                io_handlers_dispatched) == -1)
        // State has changed, so exit loop.
        break;
    }
  while (active_handle_count > 0);

  return io_handlers_dispatched + other_handlers_dispatched+signal_occurred;
}

template <class ACE_SELECT_REACTOR_TOKEN> int
ACE_Select_Reactor_T<ACE_SELECT_REACTOR_TOKEN>::release_token (void)
{
#if defined (ACE_WIN32)
  this->token_.release ();
  return (int) EXCEPTION_CONTINUE_SEARCH;
#else
  return 0;
#endif /* ACE_WIN32 */
}

template <class ACE_SELECT_REACTOR_TOKEN> int
ACE_Select_Reactor_T<ACE_SELECT_REACTOR_TOKEN>::handle_events
  (ACE_Time_Value *max_wait_time)
{
  ACE_TRACE ("ACE_Select_Reactor_T::handle_events");

  // Stash the current time -- the destructor of this object will
  // automatically compute how much time elpased since this method was
  // called.
  ACE_Countdown_Time countdown (max_wait_time);

#if defined (ACE_MT_SAFE) && (ACE_MT_SAFE != 0)
  ACE_GUARD_RETURN (ACE_SELECT_REACTOR_TOKEN, ace_mon, this->token_, -1);

  if (ACE_OS::thr_equal (ACE_Thread::self (), this->owner_) == 0)
    return -1;

  // Update the countdown to reflect time waiting for the mutex.
  countdown.update ();
#endif /* ACE_MT_SAFE */

  return this->handle_events_i (max_wait_time);
}

template <class ACE_SELECT_REACTOR_TOKEN> int
ACE_Select_Reactor_T<ACE_SELECT_REACTOR_TOKEN>::handle_events_i
  (ACE_Time_Value *max_wait_time)
{
  int result = -1;

  ACE_SEH_TRY
    {
      ACE_Select_Reactor_Handle_Set dispatch_set;

      int number_of_active_handles =
        this->wait_for_multiple_events (dispatch_set,
                                        max_wait_time);

      result = this->dispatch (number_of_active_handles,
                               dispatch_set);
    }
  ACE_SEH_EXCEPT (this->release_token ())
    {
      // As it stands now, we catch and then rethrow all Win32
      // structured exceptions so that we can make sure to release the
      // <token_> lock correctly.
    }

  this->state_changed_ = 1;

  return result;
}

template <class ACE_SELECT_REACTOR_TOKEN> int
ACE_Select_Reactor_T<ACE_SELECT_REACTOR_TOKEN>::check_handles (void)
{
  ACE_TRACE ("ACE_Select_Reactor_T::check_handles");

#if defined (ACE_WIN32) || defined (__MVS__)
  ACE_Time_Value time_poll = ACE_Time_Value::zero;
  ACE_Handle_Set rd_mask;
#endif /* ACE_WIN32 || MVS */

  ACE_Event_Handler *eh = 0;
  int result = 0;

  for (ACE_Select_Reactor_Handler_Repository_Iterator iter (&this->handler_rep_);
       iter.next (eh) != 0;
       iter.advance ())
    {
      ACE_HANDLE handle = eh->get_handle ();

      // Skip back to the beginning of the loop if the HANDLE is
      // invalid.
      if (handle == ACE_INVALID_HANDLE)
        continue;

#if defined (ACE_WIN32) || defined (__MVS__)
      // Win32 needs to do the check this way because fstat won't work on
      // a socket handle.  MVS Open Edition needs to do it this way because,
      // even though the docs say to check a handle with either select or
      // fstat, the fstat method always says the handle is ok.
      rd_mask.set_bit (handle);

      if (ACE_OS::select (int (handle) + 1,
                          rd_mask, 0, 0,
                          &time_poll) < 0)
        {
          result = 1;
          this->remove_handler_i (handle,
                                  ACE_Event_Handler::ALL_EVENTS_MASK);
        }
      rd_mask.clr_bit (handle);
#else /* !ACE_WIN32 && !MVS */
      struct stat temp;

      if (ACE_OS::fstat (handle, &temp) == -1)
        {
          result = 1;
          this->remove_handler_i (handle,
                                  ACE_Event_Handler::ALL_EVENTS_MASK);
        }
#endif /* ACE_WIN32 || MVS */
    }

  return result;
}

template <class ACE_SELECT_REACTOR_TOKEN> void
ACE_Select_Reactor_T<ACE_SELECT_REACTOR_TOKEN>::dump (void) const
{
  ACE_TRACE ("ACE_Select_Reactor_T::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));

  this->timer_queue_->dump ();
  this->handler_rep_.dump ();
  this->signal_handler_->dump ();
  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT ("delete_signal_handler_ = %d\n"),
              this->delete_signal_handler_));

  ACE_HANDLE h;

  for (ACE_Handle_Set_Iterator handle_iter_wr (this->wait_set_.wr_mask_);
       (h = handle_iter_wr ()) != ACE_INVALID_HANDLE;
       ++handle_iter_wr)
    ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("write_handle = %d\n"), h));

  for (ACE_Handle_Set_Iterator handle_iter_rd (this->wait_set_.rd_mask_);
       (h = handle_iter_rd ()) != ACE_INVALID_HANDLE;
       ++handle_iter_rd)
    ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("read_handle = %d\n"), h));

  for (ACE_Handle_Set_Iterator handle_iter_ex (this->wait_set_.ex_mask_);
       (h = handle_iter_ex ()) != ACE_INVALID_HANDLE;
       ++handle_iter_ex)
    ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("except_handle = %d\n"), h));

  for (ACE_Handle_Set_Iterator handle_iter_wr_ready (this->ready_set_.wr_mask_);
       (h = handle_iter_wr_ready ()) != ACE_INVALID_HANDLE;
       ++handle_iter_wr_ready)
    ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("write_handle_ready = %d\n"), h));

  for (ACE_Handle_Set_Iterator handle_iter_rd_ready (this->ready_set_.rd_mask_);
       (h = handle_iter_rd_ready ()) != ACE_INVALID_HANDLE;
       ++handle_iter_rd_ready)
    ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("read_handle_ready = %d\n"), h));

  for (ACE_Handle_Set_Iterator handle_iter_ex_ready (this->ready_set_.ex_mask_);
       (h = handle_iter_ex_ready ()) != ACE_INVALID_HANDLE;
       ++handle_iter_ex_ready)
    ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("except_handle_ready = %d\n"), h));

  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("restart_ = %d\n"), this->restart_));
  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("\nrequeue_position_ = %d\n"), this->requeue_position_));
  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("\ninitialized_ = %d\n"), this->initialized_));
  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("\nowner_ = %d\n"), this->owner_));

#if defined (ACE_MT_SAFE) && (ACE_MT_SAFE != 0)
  this->notify_handler_->dump ();
  this->token_.dump ();
#endif /* ACE_MT_SAFE */

  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}
#endif /* ACE_SELECT_REACTOR_T_C */
