// $Id$


#include "ace/TP_Reactor.h"
#include "ace/Reactor.h"
#include "ace/Thread.h"

#if !defined (__ACE_INLINE__)
#include "ace/TP_Reactor.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(ace, TP_Reactor, "$Id$")


ACE_ALLOC_HOOK_DEFINE (ACE_TP_Reactor)

int
ACE_TP_Token_Guard::grab_token (ACE_Time_Value *max_wait_time)
{
  ACE_TRACE ("ACE_TP_Token_Guard::grab_token");

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

  // Now that this thread owns the token let us make
  // Check for timeouts and errors.
  if (result == -1)
    {
      if (errno == ETIME)
        return 0;
      else
        return -1;
    }

  // We got the token and so let us mark ourseleves as owner
  this->owner_ = 1;

  return result;
}

/************************************************************************/
// Methods for ACE_TP_Reactor
/************************************************************************/

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



void
ACE_TP_Reactor::max_notify_iterations (int /*iterations*/)
{
  ACE_TRACE ("ACE_TP_Reactor::max_notify_iterations");

}

int
ACE_TP_Reactor::max_notify_iterations (void)
{
  ACE_TRACE ("ACE_TP_Reactor::max_notify_iterations");

  return 0;
}


int
ACE_TP_Reactor::handle_events (ACE_Time_Value *max_wait_time)
{
  ACE_TRACE ("ACE_TP_Reactor::handle_events");

  int result = 0;

  // Stash the current time -- the destructor of this object will
  // automatically compute how much time elpased since this method was
  // called.
  ACE_Countdown_Time countdown (max_wait_time);

  // Instantiate the token guard which will try grabbing the token for
  // this thread.
  ACE_TP_Token_Guard guard (this->token_,
                            max_wait_time,
                            result);

  // If the guard is NOT the owner just return the retval
  if (!guard.is_owner ())
    return result;

  // Update the countdown to reflect time waiting for the token.
  countdown.update ();


  // After acquiring the lock, check if we have been deactivated.  If
  // we are deactivated, simply return without handling further
  // events.
  if (this->deactivated_)
    {
      return -1;
    }

  // We got the lock, lets handle some events. We collect the events
  // that we need to handle. We release the token and then handle
  // those events that needs handling.
  int event_count =
    this->get_event_for_dispatching (max_wait_time);


  // Dispatch signals
  if (event_count == -1)
    {
      // Looks like we dont do any upcalls in dispatch signals. If at
      // a later point of time, we decide to handle signals we have to
      // release the lock before we make any upcalls.. What is here
      // now is not the right thing...
      // @@ We need to do better..
      return this->handle_signals (event_count,
                                   guard);
    }

  if (event_count > 0)
    {
      // If there are no signals and if we had received a proper
      // event_count then first look at dispatching timeouts. We need to
      // handle timers early since they may have higher latency
      // constraints than I/O handlers.  Ideally, the order of
      // dispatching should be a strategy...
      int retval = this->handle_timer_events (event_count,
                                              guard);

      if (retval > 0)
         return retval;

       // Else just fall through for further handling
     }


  if (event_count > 0)
    {
      // Next dispatch the notification handlers (if there are any to
      // dispatch).  These are required to handle multiple-threads that
      // are trying to update the <Reactor>.
      int retval = this->handle_notify_events (event_count,
                                               guard);

       if (retval > 0)
         return retval;

       // Else just fall through for further handling
    }


   if (event_count > 0)
     {
       // Handle socket events
       return this->handle_socket_events (event_count,
                                          guard);
     }

   return 0;
}

int
ACE_TP_Reactor::handle_events (ACE_Time_Value &max_wait_time)
{
  ACE_TRACE ("ACE_TP_Reactor::handle_events");
  return this->handle_events (&max_wait_time);
}

int
ACE_TP_Reactor::resumable_handler (void)
{
  return 1;
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


int
ACE_TP_Reactor::mask_ops (ACE_Event_Handler *eh,
                          ACE_Reactor_Mask mask,
                          int ops)
{
  ACE_TRACE ("ACE_TP_Reactor::mask_ops");
  return this->mask_ops (eh->get_handle (), mask, ops);
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
ACE_TP_Reactor::get_event_for_dispatching (ACE_Time_Value *max_wait_time)
{
  ACE_TRACE ("ACE_TP_Reactor::get_event_for_dispatching");
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
      this->ready_set_.rd_mask_.sync (this->ready_set_.rd_mask_.max_set ());
      this->ready_set_.wr_mask_.sync (this->ready_set_.wr_mask_.max_set ());
      this->ready_set_.ex_mask_.sync (this->ready_set_.ex_mask_.max_set ());
    }

  return this->wait_for_multiple_events (this->ready_set_,
                                         max_wait_time);
}


int
ACE_TP_Reactor::handle_signals (int & /*event_count*/,
                                ACE_TP_Token_Guard & /*guard*/)
{
  ACE_TRACE ("ACE_TP_Reactor::handle_signals");

  /*
   *
   *             THIS METHOD SEEMS BROKEN
   *
   *
   */
  // First check for interrupts.
  // Bail out -- we got here since <select> was interrupted.
  if (ACE_Sig_Handler::sig_pending () != 0)
    {
      ACE_Sig_Handler::sig_pending (0);

      // This piece of code comes from the old TP_Reactor. We did not
      // handle signals at all then. If we happen to handle signals
      // in the TP_Reactor, we should then start worryiung about this
      // - Bala 21-Aug- 01
#if 0
      // Not sure if this should be done in the TP_Reactor
      // case... leave it out for now.   -Steve Huston 22-Aug-00

      // If any HANDLES in the <ready_set_> are activated as a
      // result of signals they should be dispatched since
      // they may be time critical...
      active_handle_count = this->any_ready (dispatch_set);
 #else
      // active_handle_count = 0;
#endif

      // Record the fact that the Reactor has dispatched a
      // handle_signal() method.  We need this to return the
      // appropriate count.
      return 1;
    }

  return -1;
}


int
ACE_TP_Reactor::handle_timer_events (int &event_count,
                                     ACE_TP_Token_Guard &guard)
{
  // Get the current time
  ACE_Time_Value cur_time (this->timer_queue_->gettimeofday () +
                           this->timer_queue_->timer_skew ());

  // Look for a node in the timer queue whose timer <= the present
  // time.
  ACE_Timer_Node_Dispatch_Info info;

  if (this->timer_queue_->dispatch_info (cur_time,
                                         info))
    {
      // Decrement the number of events that needs handling yet.
      event_count--;

      // Release the token before dispatching notifies...
      guard.release_token ();

      // call the functor
      this->timer_queue_->upcall (info.type_,
                                  info.act_,
                                  cur_time);

      // We have dispatched a timer
      return 1;
    }

  return 0;
}


int
ACE_TP_Reactor::handle_notify_events (int &event_count,
                                      ACE_TP_Token_Guard &guard)
{
  // Get the handle on which notify calls could have occured
  ACE_HANDLE notify_handle =
    this->get_notify_handle ();

  if (notify_handle != ACE_INVALID_HANDLE)
    {
      // Clear the handle of the read_mask of our <ready_set_>
      this->ready_set_.rd_mask_.clr_bit (notify_handle);

      // Decrement the number of events that needs handling yet.
      event_count--;

      // Release the token before dispatching notifies...
      guard.release_token ();

      // Dipatch and return
      return
        this->notify_handler_->dispatch_notify (notify_handle);
    }

  return 0;
}

int
ACE_TP_Reactor::handle_socket_events (int &event_count,
                                      ACE_TP_Token_Guard &guard)
{
  ACE_TRACE ("ACE_TP_Reactor::handle_socket_events");

  ACE_EH_Dispatch_Info dispatch_info;

  // Get the socket event dispatch information
  // If there is any event handler that is ready to be dispatched, the
  // dispatch information is recorded in dispatch_info.
  int result =
    this->get_socket_event_info (dispatch_info);

  if (result)
    {
      // Suspend the handler so that other threads don't start
      // dispatching it.
      this->suspend_i (dispatch_info.handle_);

      // Decrement the number of events that needs handling yet.
      event_count--;

      // Release the token before dispatching notifies...
      guard.release_token ();

      result = this->dispatch_socket_events  (dispatch_info);

      int flag = 0;

      if (dispatch_info.event_handler_ != 0)
        {
          flag =
            dispatch_info.event_handler_->resume_handler ();
        }

      if (dispatch_info.handle_ != ACE_INVALID_HANDLE &&
          flag == 0)
        this->resume_handler (dispatch_info.handle_);
    }

  return result;
}


ACE_HANDLE
ACE_TP_Reactor::get_notify_handle (void)
{
  // Call the notify handler to get a handle on which we would have a
  // notify waiting
  ACE_HANDLE read_handle =
    this->notify_handler_->notify_handle ();

  // Check whether the rd_mask has been set on that handle. If so
  // return the handle.
  if (read_handle != ACE_INVALID_HANDLE &&
      this->ready_set_.rd_mask_.is_set (read_handle))
    {
      return read_handle;
    }

  return ACE_INVALID_HANDLE;
}




int
ACE_TP_Reactor::get_socket_event_info (ACE_EH_Dispatch_Info &event)
{

  // Check for dispatch in write, except, read. Only catch one, but if
  // one is caught, be sure to clear the handle from each mask in case
  // there is more than one mask set for it. This would cause problems
  // if the handler is suspended for dispatching, but its set bit in
  // another part of ready_set_ kept it from being dispatched.
  ACE_HANDLE handle;

  // Look at the read masks
  {
    ACE_Handle_Set_Iterator handle_iter (this->ready_set_.rd_mask_);

    while ((handle = handle_iter ()) != ACE_INVALID_HANDLE)
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
        return 1;
      }
  }

  // We havent found any rd_masks for processing yet, so look for
  // write masks
  {
    ACE_Handle_Set_Iterator handle_iter (this->ready_set_.wr_mask_);

    while ((handle = handle_iter ()) != ACE_INVALID_HANDLE)
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
        return 1;
      }
  }

  // We havent found any rd_mask and wr_masks for processing yet, so
  // look for ex_masks
  {
    ACE_Handle_Set_Iterator handle_iter (this->ready_set_.ex_mask_);

    while ((handle = handle_iter ()) != ACE_INVALID_HANDLE)
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
        return 1;
      }
    }

  // We didnt find any..
  return 0;
}

int
ACE_TP_Reactor::dispatch_socket_events (ACE_EH_Dispatch_Info &info)
{
  ACE_TRACE ("ACE_TP_Reactor::dispatch_socket_events");

  ACE_HANDLE handle = info.handle_;
  ACE_Event_Handler *event_handler = info.event_handler_;
  ACE_Reactor_Mask mask = info.mask_;
  ACE_EH_PTMF callback = info.callback_;

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
      info.event_handler_ = 0;
      info.handle_ = ACE_INVALID_HANDLE;

      return retval;
    }

  return 1;
}


void
ACE_TP_Reactor::notify_handle (ACE_HANDLE,
                               ACE_Reactor_Mask,
                               ACE_Handle_Set &,
                               ACE_Event_Handler *,
                               ACE_EH_PTMF)
{
  ACE_ERROR ((LM_ERROR,
              ACE_LIB_TEXT ("ACE_TP_Reactor::notify_handle: Wrong version of notify_handle() gets called")));
}
