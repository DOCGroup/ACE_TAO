// $Id$

#include "ace/TP_Reactor.h"
#include "ace/Reactor.h"
#include "ace/Thread.h"

#if !defined (__ACE_INLINE__)
#include "ace/TP_Reactor.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(ace, TP_Reactor, "$Id$")

ACE_ALLOC_HOOK_DEFINE (ACE_TP_Reactor)

ACE_TP_Reactor::ACE_TP_Reactor (ACE_Sig_Handler *sh,
                                ACE_Timer_Queue *tq,
                                int mask_signals)
  : ACE_Select_Reactor (sh, tq, 0, 0, mask_signals)
{
  ACE_TRACE ("ACE_TP_Reactor::ACE_TP_Reactor");
  this->supress_notify_renew (1);
}

ACE_TP_Reactor::ACE_TP_Reactor (size_t size,
                                int rs,
                                ACE_Sig_Handler *sh,
                                ACE_Timer_Queue *tq,
                                int mask_signals)
  : ACE_Select_Reactor (size, rs, sh, tq, 0, 0, mask_signals)
{
  ACE_TRACE ("ACE_TP_Reactor::ACE_TP_Reactor");
  this->supress_notify_renew (1);
}

int
ACE_TP_Reactor::owner (ACE_thread_t, ACE_thread_t *o_id)
{
  ACE_TRACE ("ACE_TP_Reactor::owner");
  if (o_id)
    *o_id = ACE_Thread::self ();

  return 0;

}

int
ACE_TP_Reactor::owner (ACE_thread_t *t_id)
{
  ACE_TRACE ("ACE_TP_Reactor::owner");
  *t_id = ACE_Thread::self ();

  return 0;

}


int
ACE_TP_Reactor::handle_events (ACE_Time_Value *max_wait_time)
{
  ACE_TRACE ("ACE_TP_Reactor::handle_events");

  // Stash the current time -- the destructor of this object will
  // automatically compute how much time elpased since this method was
  // called.
  ACE_Countdown_Time countdown (max_wait_time);

  // The order of these events is very subtle, modify with care.

  // Try to grab the lock.  If someone if already there, don't wake
  // them up, just queue up in the thread pool.
  int result = 0;

  if (max_wait_time)
    {
      ACE_Time_Value tv = ACE_OS::gettimeofday ();
      tv += *max_wait_time;

      ACE_MT (result = this->token_.acquire_read (&ACE_TP_Reactor::no_op_sleep_hook,
                                                  0,
                                                  &tv));
    }
  else
    {
      ACE_MT (result = this->token_.acquire_read (&ACE_TP_Reactor::no_op_sleep_hook));
    }

  // Update the countdown to reflect time waiting for the token.
  countdown.update ();

  // Check for timeouts and errors.
  if (result == -1)
    {
      if (errno == ETIME)
        return 0;
      else
        return -1;
    }

  // After acquiring the lock, check if we have been deactivated.  If
  // we are deactivated, simply return without handling further
  // events.
  if (this->deactivated_)
    {
      ACE_MT (this->token_.release ());
      return -1;
    }

  // We got the lock, lets handle some events.  Note: this method will
  // *not* dispatch any I/O handlers.  It will dispatch signals,
  // timeouts, and notifications.
  ACE_EH_Dispatch_Info dispatch_info;
  result = this->dispatch_i_protected (max_wait_time, dispatch_info);
  if (result == -1)
    {
      ACE_MT (this->token_.release ());
      return -1;
    }

  // If there is any event handler that is ready to be dispatched, the
  // dispatch information is recorded in dispatch_info.
  if (dispatch_info.dispatch ())
    {
      // Suspend the handler so that other threads don't start
      // dispatching it.
      // Make sure we never suspend the notify_handler_ without holding
      // the lock.
      // @@ Actually, we don't even need to suspend the notify_handler_
      //    here.  But let me get it to work first.
      if (dispatch_info.event_handler_ != this->notify_handler_)
        this->suspend_i (dispatch_info.handle_);
    }

  // Release the lock.  Others threads can start waiting.
  ACE_MT (this->token_.release ());

  // If there was an event handler ready, dispatch it.
  if (dispatch_info.dispatch ())
    {
      if (this->notify_handle (dispatch_info) == 0)
        ++result;                // Dispatched one more event
      int flag = 0;

      if (dispatch_info.event_handler_ != 0)
        {
         flag =
           dispatch_info.event_handler_->resume_handler ();
        }

      if (dispatch_info.handle_ != ACE_INVALID_HANDLE &&
          dispatch_info.event_handler_ != this->notify_handler_ &&
          flag == 0)
        this->resume_handler (dispatch_info.handle_);
    }

  return result;
}

int
ACE_TP_Reactor::mask_ops (ACE_HANDLE handle,
                          ACE_Reactor_Mask mask,
                          int ops)
{
  ACE_TRACE ("ACE_TP_Reactor::mask_ops");
  ACE_MT (ACE_GUARD_RETURN (ACE_Select_Reactor_Token,
          ace_mon, this->token_, -1));

  int result = 0;

  // If it looks like the handle isn't suspended, then
  // set the ops on the wait_set_, otherwise set the suspend_set_.

  if (this->suspend_set_.rd_mask_.is_set (handle) == 0
      && this->suspend_set_.wr_mask_.is_set (handle) == 0
      && this->suspend_set_.ex_mask_.is_set (handle) == 0)

    result = this->bit_ops (handle, mask,
                            this->wait_set_,
                            ops);
  else

    result = this->bit_ops (handle, mask,
                            this->suspend_set_,
                            ops);

  return result;
}

void
ACE_TP_Reactor::no_op_sleep_hook (void *)
{
}


int
ACE_TP_Reactor::dispatch_i (ACE_Time_Value *max_wait_time,
                            ACE_EH_Dispatch_Info &event)
{
  int result = -1;

  event.reset ();           // Nothing to dispatch yet

  // If the reactor handler state has changed, clear any remembered
  // ready bits and re-scan from the master wait_set.
  if (this->state_changed_)
    {
      this->ready_set_.rd_mask_.reset ();
      this->ready_set_.wr_mask_.reset ();
      this->ready_set_.ex_mask_.reset ();
      this->state_changed_ = 0;
    }
  else
    {
      // This is a hack... somewhere, under certain conditions (which
      // I don't understand...) the mask will have all of its bits clear,
      // yet have a size_ > 0. This is an attempt to remedy the affect,
      // without knowing why it happens.

      //# if !(defined (__SUNPRO_CC) && (__SUNPRO_CC > 0x500))
      // SunCC seems to be having problems with this piece of code
      // here. I am  not sure why though. This works fine with other
      // compilers. As we dont seem to understand when this piece of
      // code is needed and as it creates problems for SunCC we will
      // not compile this. Most of the tests in TAO seem to be happy
      // without this in SunCC.
      this->ready_set_.rd_mask_.sync (this->ready_set_.rd_mask_.max_set ());
      this->ready_set_.wr_mask_.sync (this->ready_set_.wr_mask_.max_set ());
      this->ready_set_.ex_mask_.sync (this->ready_set_.ex_mask_.max_set ());
      //# endif /* ! __SUNPRO_CC */

    }

  int active_handle_count = this->wait_for_multiple_events (this->ready_set_,
                                                            max_wait_time);

  int handlers_dispatched = 0;
  int signal_occurred = 0;

  // Note that we keep track of changes to our state.  If any of
  // the dispatching ends up with this->state_changed_ being set,
  // <wait_set_> state has changed as the result of an
  // <ACE_Event_Handler> being dispatched.  This means that we
  // need to bail out and rerun the select() again since our
  // existing notion of handles in <dispatch_set_> may no longer be
  // correct.

  // First check for interrupts.
  if (active_handle_count == -1)
    {
      // Bail out -- we got here since <select> was interrupted.
      if (ACE_Sig_Handler::sig_pending () != 0)
        {
          ACE_Sig_Handler::sig_pending (0);

#if 0
          // Not sure if this should be done in the TP_Reactor
          // case... leave it out for now.   -Steve Huston 22-Aug-00

          // If any HANDLES in the <ready_set_> are activated as a
          // result of signals they should be dispatched since
          // they may be time critical...
          active_handle_count = this->any_ready (dispatch_set);
#else
          active_handle_count = 0;
#endif

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
  this->dispatch_timer_handlers (handlers_dispatched);

  // If either the state has changed as a result of timer
  // expiry, or there are no handles ready for dispatching,
  // all done for now.
  if (this->state_changed_ || active_handle_count == 0)
    return signal_occurred + handlers_dispatched;

  // Next dispatch the notification handlers (if there are any to
  // dispatch).  These are required to handle multi-threads that
  // are trying to update the <Reactor>.

  this->dispatch_notification_handlers (this->ready_set_,
                                        active_handle_count,
                                        handlers_dispatched);

  // If one of those changed the state, return.
  if (this->state_changed_ || active_handle_count == 0)
    return signal_occurred + handlers_dispatched;

  // Check for dispatch in write, except, read. Only catch one, but if
  // one is caught, be sure to clear the handle from each mask in case
  // there is more than one mask set for it. This would cause problems
  // if the handler is suspended for dispatching, but its set bit in
  // another part of ready_set_ kept it from being dispatched.
  int found_io = 0;
  ACE_HANDLE handle;

  {
    ACE_Handle_Set_Iterator handle_iter (this->ready_set_.wr_mask_);

    while (!found_io && (handle = handle_iter ()) != ACE_INVALID_HANDLE)
      {
        if (this->is_suspended_i (handle))
          continue;

        // Remember this info
        event.set (handle,
                   this->handler_rep_.find (handle),
                   ACE_Event_Handler::WRITE_MASK,
                   &ACE_Event_Handler::handle_output);
        this->ready_set_.wr_mask_.clr_bit (handle);
        this->ready_set_.ex_mask_.clr_bit (handle);
        this->ready_set_.rd_mask_.clr_bit (handle);
        found_io = 1;
      }
  }

  if (!found_io)
    {
      ACE_Handle_Set_Iterator handle_iter (this->ready_set_.ex_mask_);

      while (!found_io && (handle = handle_iter ()) != ACE_INVALID_HANDLE)
        {
          if (this->is_suspended_i (handle))
            continue;

          // Remember this info
          event.set (handle,
                     this->handler_rep_.find (handle),
                     ACE_Event_Handler::EXCEPT_MASK,
                     &ACE_Event_Handler::handle_exception);
          this->ready_set_.ex_mask_.clr_bit (handle);
          this->ready_set_.wr_mask_.clr_bit (handle);
          this->ready_set_.rd_mask_.clr_bit (handle);
          found_io = 1;
        }
    }

  if (!found_io)
    {
      ACE_Handle_Set_Iterator handle_iter (this->ready_set_.rd_mask_);

      while (!found_io && (handle = handle_iter ()) != ACE_INVALID_HANDLE)
        {
          if (this->is_suspended_i (handle))
            continue;

          // Remember this info
          event.set (handle,
                     this->handler_rep_.find (handle),
                     ACE_Event_Handler::READ_MASK,
                     &ACE_Event_Handler::handle_input);
          this->ready_set_.rd_mask_.clr_bit (handle);
          this->ready_set_.wr_mask_.clr_bit (handle);
          this->ready_set_.ex_mask_.clr_bit (handle);
          found_io = 1;
        }
    }

  result = signal_occurred + handlers_dispatched;

  return result;
}

int
ACE_TP_Reactor::dispatch_i_protected (ACE_Time_Value *max_wait_time,
                                      ACE_EH_Dispatch_Info &event)
{
  int result;

  ACE_SEH_TRY
    {
      result = this->dispatch_i (max_wait_time, event);
    }
  ACE_SEH_EXCEPT (this->release_token ())
    {
      // As it stands now, we catch and then rethrow all Win32
      // structured exceptions so that we can make sure to release the
      // <token_> lock correctly.
    }

  return result;

}


// Dispatches a single event handler
int
ACE_TP_Reactor::notify_handle (ACE_EH_Dispatch_Info &dispatch_info)
{
  ACE_TRACE ("ACE_TP_Reactor::notify_handle");

  ACE_HANDLE handle = dispatch_info.handle_;
  ACE_Event_Handler *event_handler = dispatch_info.event_handler_;
  ACE_Reactor_Mask mask = dispatch_info.mask_;
  ACE_EH_PTMF callback = dispatch_info.callback_;

  // Check for removed handlers.
  if (event_handler == 0)
    return -1;

  // Upcall. If the handler returns positive value (requesting a
  // reactor callback) don't set the ready-bit because it will be
  // ignored if the reactor state has changed. Just call back
  // as many times as the handler requests it. Other threads are off
  // handling other things.
  int status = 1;
  while (status > 0)
    status = (event_handler->*callback) (handle);

  // If negative, remove from Reactor
  if (status < 0)
    {
      int retval =
        this->remove_handler (handle, mask);

      // As the handler is no longer valid, invalidate the handle
      dispatch_info.event_handler_ = 0;
      dispatch_info.handle_ = ACE_INVALID_HANDLE;

      return retval;
    }

  // assert (status >= 0);
  return 0;
}


ACE_EH_Dispatch_Info::ACE_EH_Dispatch_Info (void)
{
  this->reset ();
}

void
ACE_EH_Dispatch_Info::set (ACE_HANDLE handle,
                           ACE_Event_Handler *event_handler,
                           ACE_Reactor_Mask mask,
                           ACE_EH_PTMF callback)
{
  this->dispatch_ = 1;

  this->handle_ = handle;
  this->event_handler_ = event_handler;
  this->mask_ = mask;
  this->callback_ = callback;
}

void
ACE_EH_Dispatch_Info::reset (void)
{
  this->dispatch_ = 0;

  this->handle_ = ACE_INVALID_HANDLE;
  this->event_handler_ = 0;
  this->mask_ = ACE_Event_Handler::NULL_MASK;
  this->callback_ = 0;
}

int
ACE_EH_Dispatch_Info::dispatch (void) const
{
  return this->dispatch_;
}
