// $Id$

#if !defined (ACE_SELECT_REACTOR_T_C)
#define ACE_SELECT_REACTOR_T_C

#define ACE_BUILD_DLL
#include "ace/Select_Reactor_T.h"
#include "ace/Timer_Heap.h"

#if !defined (__ACE_INLINE__)
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
ACE_Select_Reactor_T<ACE_SELECT_REACTOR_TOKEN>::initialized (void)
{
  ACE_TRACE ("ACE_Select_Reactor_T::initialized");
  ACE_MT (ACE_GUARD_RETURN (ACE_SELECT_REACTOR_TOKEN, ace_mon, this->token_, 0));
  return this->initialized_;
}

template <class ACE_SELECT_REACTOR_TOKEN> int
ACE_Select_Reactor_T<ACE_SELECT_REACTOR_TOKEN>::owner (ACE_thread_t tid, ACE_thread_t *o_id)
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
ACE_Select_Reactor_Token_T<ACE_SELECT_REACTOR_MUTEX>::ACE_Select_Reactor_Token_T (ACE_Select_Reactor_Impl &r)
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
ACE_Select_Reactor_Token_T<ACE_SELECT_REACTOR_MUTEX>::select_reactor (ACE_Select_Reactor_Impl &select_reactor)
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
ACE_Select_Reactor_T<ACE_SELECT_REACTOR_TOKEN>::register_handler (ACE_Event_Handler *handler,
                                                                  ACE_Reactor_Mask mask)
{
  ACE_TRACE ("ACE_Select_Reactor_T::register_handler");
  ACE_MT (ACE_GUARD_RETURN (ACE_SELECT_REACTOR_TOKEN, ace_mon, this->token_, -1));
  return this->register_handler_i (handler->get_handle (), handler, mask);
}

template <class ACE_SELECT_REACTOR_TOKEN> int
ACE_Select_Reactor_T<ACE_SELECT_REACTOR_TOKEN>::register_handler (ACE_HANDLE handle,
                                                                  ACE_Event_Handler *handler,
                                                                  ACE_Reactor_Mask mask)
{
  ACE_TRACE ("ACE_Select_Reactor_T::register_handler");
  ACE_MT (ACE_GUARD_RETURN (ACE_SELECT_REACTOR_TOKEN, ace_mon, this->token_, -1));
  return this->register_handler_i (handle, handler, mask);
}

template <class ACE_SELECT_REACTOR_TOKEN> int
ACE_Select_Reactor_T<ACE_SELECT_REACTOR_TOKEN>::register_handler (const ACE_Handle_Set &handles,
                                                                  ACE_Event_Handler *handler,
                                                                  ACE_Reactor_Mask mask)
{
  ACE_TRACE ("ACE_Select_Reactor_T::register_handler");
  ACE_MT (ACE_GUARD_RETURN (ACE_SELECT_REACTOR_TOKEN, ace_mon, this->token_, -1));
  return this->register_handler_i (handles, handler, mask);
}

template <class ACE_SELECT_REACTOR_TOKEN> int
ACE_Select_Reactor_T<ACE_SELECT_REACTOR_TOKEN>::handler (ACE_HANDLE handle,
                                                         ACE_Reactor_Mask mask,
                                                         ACE_Event_Handler **handler)
{
  ACE_TRACE ("ACE_Select_Reactor_T::handler");
  ACE_MT (ACE_GUARD_RETURN (ACE_SELECT_REACTOR_TOKEN, ace_mon, this->token_, -1));
  return this->handler_i (handle, mask, handler);
}

template <class ACE_SELECT_REACTOR_TOKEN> int
ACE_Select_Reactor_T<ACE_SELECT_REACTOR_TOKEN>::remove_handler (const ACE_Handle_Set &handles,
                                                                ACE_Reactor_Mask mask)
{
  ACE_TRACE ("ACE_Select_Reactor_T::remove_handler");
  ACE_MT (ACE_GUARD_RETURN (ACE_SELECT_REACTOR_TOKEN, ace_mon, this->token_, -1));
  return this->remove_handler_i (handles, mask);
}

template <class ACE_SELECT_REACTOR_TOKEN> int
ACE_Select_Reactor_T<ACE_SELECT_REACTOR_TOKEN>::remove_handler (ACE_Event_Handler *handler,
                                                                ACE_Reactor_Mask mask)
{
  ACE_TRACE ("ACE_Select_Reactor_T::remove_handler");
  ACE_MT (ACE_GUARD_RETURN (ACE_SELECT_REACTOR_TOKEN, ace_mon, this->token_, -1));
  return this->remove_handler_i (handler->get_handle (), mask);
}

template <class ACE_SELECT_REACTOR_TOKEN> int
ACE_Select_Reactor_T<ACE_SELECT_REACTOR_TOKEN>::remove_handler (ACE_HANDLE handle,
                                                                ACE_Reactor_Mask mask)
{
  ACE_TRACE ("ACE_Select_Reactor_T::remove_handler");
  ACE_MT (ACE_GUARD_RETURN (ACE_SELECT_REACTOR_TOKEN, ace_mon, this->token_, -1));
  return this->remove_handler_i (handle, mask);
}

// Performs operations on the "ready" bits.

template <class ACE_SELECT_REACTOR_TOKEN> int
ACE_Select_Reactor_T<ACE_SELECT_REACTOR_TOKEN>::ready_ops (ACE_HANDLE handle,
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
ACE_Select_Reactor_T<ACE_SELECT_REACTOR_TOKEN>::open (size_t size,
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

template <class ACE_SELECT_REACTOR_TOKEN>
ACE_Select_Reactor_T<ACE_SELECT_REACTOR_TOKEN>::ACE_Select_Reactor_T (ACE_Sig_Handler *sh,
                                                                      ACE_Timer_Queue *tq,
                                                                      int disable_notify_pipe,
                                                                      ACE_Reactor_Notify *notify)
  : token_ (*this),
    lock_adapter_ (token_)

{
  ACE_TRACE ("ACE_Select_Reactor_T::ACE_Select_Reactor_T");

  if (this->open (ACE_Select_Reactor_T::DEFAULT_SIZE,
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
ACE_Select_Reactor_T<ACE_SELECT_REACTOR_TOKEN>::ACE_Select_Reactor_T (size_t size,
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

// Close down the ACE_Select_Reactor_T instance, detaching any remaining
// Event_Handers.  This had better be called from the main event loop
// thread...

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

template <class ACE_SELECT_REACTOR_TOKEN>
ACE_Select_Reactor_T<ACE_SELECT_REACTOR_TOKEN>::~ACE_Select_Reactor_T (void)
{
  ACE_TRACE ("ACE_Select_Reactor_T::~ACE_Select_Reactor_T");
  this->close ();
}

// Note the queue handles its own locking.

template <class ACE_SELECT_REACTOR_TOKEN> long
ACE_Select_Reactor_T<ACE_SELECT_REACTOR_TOKEN>::schedule_timer (ACE_Event_Handler *handler,
                                                                const void *arg,
                                                                const ACE_Time_Value &delta_time,
                                                                const ACE_Time_Value &interval)
{
  ACE_TRACE ("ACE_Select_Reactor_T::schedule_timer");
  ACE_MT (ACE_GUARD_RETURN (ACE_SELECT_REACTOR_TOKEN, ace_mon, this->token_, -1));

  return this->timer_queue_->schedule
    (handler, arg, timer_queue_->gettimeofday () + delta_time, interval);
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
ACE_Select_Reactor_T<ACE_SELECT_REACTOR_TOKEN>::mask_ops (ACE_HANDLE handle,
                                                          ACE_Reactor_Mask mask,
                                                          int ops)
{
  ACE_TRACE ("ACE_Select_Reactor_T::mask_ops");
  ACE_MT (ACE_GUARD_RETURN (ACE_SELECT_REACTOR_TOKEN, ace_mon, this->token_, -1));
  return this->bit_ops (handle, mask,
                        this->wait_set_,
                        ops);
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
ACE_Select_Reactor_T<ACE_SELECT_REACTOR_TOKEN>::handle_events (ACE_Time_Value *max_wait_time)
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
