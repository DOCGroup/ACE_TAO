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

  // We got the token and so let us mark ourselves as owner
  this->owner_ = 1;

  return result;
}


int
ACE_TP_Token_Guard::acquire_token (ACE_Time_Value *max_wait_time)
{
  ACE_TRACE ("ACE_TP_Token_Guard::acquire_token");

  // Try to grab the lock.  If someone if already there, don't wake
  // them up, just queue up in the thread pool.
  int result = 0;

  if (max_wait_time)
    {
      ACE_Time_Value tv = ACE_OS::gettimeofday ();
      tv += *max_wait_time;

      ACE_MT (result = this->token_.acquire (0,
                                             0,
                                             &tv));
    }
  else
    {
      ACE_MT (result = this->token_.acquire ());
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



ACE_TP_Reactor::ACE_TP_Reactor (ACE_Sig_Handler *sh,
                                ACE_Timer_Queue *tq,
                                int mask_signals,
                                int s_queue)
  : ACE_Select_Reactor (sh, tq, 0, 0, mask_signals, s_queue)
{
  ACE_TRACE ("ACE_TP_Reactor::ACE_TP_Reactor");
  this->supress_notify_renew (1);
}

ACE_TP_Reactor::ACE_TP_Reactor (size_t size,
                                int rs,
                                ACE_Sig_Handler *sh,
                                ACE_Timer_Queue *tq,
                                int mask_signals,
                                int s_queue)
  : ACE_Select_Reactor (size, rs, sh, tq, 0, 0, mask_signals, s_queue)
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
  // automatically compute how much time elapsed since this method was
  // called.
  ACE_Countdown_Time countdown (max_wait_time);

  // The order of these events is very subtle, modify with care.


  // Instantiate the token guard which will try grabbing the token for
  // this thread.
  ACE_TP_Token_Guard guard (this->token_);


  int result = guard.grab_token (max_wait_time);

  // If the guard is NOT the owner just return the retval
  if (!guard.is_owner ())
    return result;

  // After getting the lock just just for deactivation..
  if (this->deactivated_)
    return -1;

  // Update the countdown to reflect time waiting for the token.
  countdown.update ();


  return this->dispatch_i (max_wait_time,
                           guard);
}


int
ACE_TP_Reactor::remove_handler (ACE_Event_Handler *eh,
                                ACE_Reactor_Mask mask)
{
  int result = 0;
  // Artificial scoping for grabbing and releasing the token
  {
    ACE_TP_Token_Guard guard (this->token_);

    // Acquire the token
    result = guard.acquire_token ();

    if (!guard.is_owner ())
      return result;

    // Call the remove_handler_i () with a DONT_CALL mask. We dont
    // want to call the handle_close with the token held.
    result = this->remove_handler_i (eh->get_handle (),
                                     mask | ACE_Event_Handler::DONT_CALL);

    if (result == -1)
      return -1;
  }

  // Close down the <Event_Handler> unless we've been instructed not
  // to.
  if (result == 0 && (ACE_BIT_ENABLED (mask, ACE_Event_Handler::DONT_CALL) == 0))
    eh->handle_close (ACE_INVALID_HANDLE, mask);

  return 0;
}

int
ACE_TP_Reactor::remove_handler (ACE_HANDLE handle,
                                ACE_Reactor_Mask mask)
{

  ACE_Event_Handler *eh = 0;
  int result = 0;
  // Artificial scoping for grabbing and releasing the token
  {
    ACE_TP_Token_Guard guard (this->token_);

    // Acquire the token
    result = guard.acquire_token ();

    if (!guard.is_owner ())
      return result;

    size_t slot = 0;
    eh =  this->handler_rep_.find (handle, &slot);

    if (eh == 0)
      return -1;

    // Call the remove_handler_i () with a DONT_CALL mask. We dont
    // want to call the handle_close with the token held.
    result = this->remove_handler_i (handle,
                                     mask | ACE_Event_Handler::DONT_CALL);

    if (result == -1)
      return -1;
  }

  // Close down the <Event_Handler> unless we've been instructed not
  // to.
  // @@ Note: The check for result ==0 may be redundant, but shouldnt
  // be a problem.
  if (result ==0 && (ACE_BIT_ENABLED (mask, ACE_Event_Handler::DONT_CALL) == 0))
    eh->handle_close (handle, mask);

  return 0;
}


int
ACE_TP_Reactor::remove_handler (const ACE_Handle_Set &handles,
                                ACE_Reactor_Mask m)
{
  // Array of <Event_Handlers> corresponding to <handles>
  ACE_Event_Handler **aeh = 0;

  // Allocate memory for the size of the handle set
  ACE_NEW_RETURN (aeh,
                  ACE_Event_Handler *[handles.num_set ()],
                  -1);

  size_t index = 0;

  // Artificial scoping for grabbing and releasing the token
  {
    ACE_TP_Token_Guard guard (this->token_);

    // Acquire the token
    int result = guard.acquire_token ();

    if (!guard.is_owner ())
      return result;

    ACE_HANDLE h;

    ACE_Handle_Set_Iterator handle_iter (handles);

    while ((h = handle_iter ()) != ACE_INVALID_HANDLE)
      {
        size_t slot = 0;
        ACE_Event_Handler *eh =
          this->handler_rep_.find (h, &slot);

        if (this->remove_handler_i (h,
                                    m | ACE_Event_Handler::DONT_CALL) == -1)
          {
            delete [] aeh;
            return -1;
          }

        aeh [index] = eh;
        index ++;
      }
  }

  // Close down the <Event_Handler> unless we've been instructed not
  // to.
  if (ACE_BIT_ENABLED (m, ACE_Event_Handler::DONT_CALL) == 0)
    {
      for (size_t i = 0; i < index; i++)
        aeh[i]->handle_close (ACE_INVALID_HANDLE, m);
    }

  delete [] aeh;
  return 0;
}

int
ACE_TP_Reactor::register_handler (int,
                                  ACE_Event_Handler *,
                                  ACE_Sig_Action *,
                                  ACE_Event_Handler **,
                                  ACE_Sig_Action *)
{
  ACE_NOTSUP_RETURN (-1);
}

int
ACE_TP_Reactor::register_handler (const ACE_Sig_Set &,
                                  ACE_Event_Handler *,
                                  ACE_Sig_Action *)
{
  ACE_NOTSUP_RETURN (-1);
}

int
ACE_TP_Reactor::remove_handler (int /*signum*/,
                                ACE_Sig_Action * /*new_disp*/,
                                ACE_Sig_Action * /*old_disp*/,
                                int /*sigkey*/)
{
  ACE_NOTSUP_RETURN (-1);
}

int
ACE_TP_Reactor::remove_handler (const ACE_Sig_Set & /*sigset*/)
{
  ACE_NOTSUP_RETURN (-1);
}


int
ACE_TP_Reactor::dispatch_i (ACE_Time_Value *max_wait_time,
                            ACE_TP_Token_Guard &guard)
{
  int event_count =
    this->get_event_for_dispatching (max_wait_time);

  int result = 0;

  // Note: We are passing the <event_count> around, to have record of
  // how many events still need processing. May be this could be
  // useful in future.

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

  // If there are no signals and if we had received a proper
  // event_count then first look at dispatching timeouts. We need to
  // handle timers early since they may have higher latency
  // constraints than I/O handlers.  Ideally, the order of
  // dispatching should be a strategy...

  // NOTE: The event count does not have the number of timers that
  // needs dispatching. But we are still passing this along. We dont
  // need to do that. In the future we *may* have the timers also
  // returned through the <event_count>. Just passing that along for
  // that day.
  result = this->handle_timer_events (event_count,
                                      guard);

  if (result > 0)
    return result;


  // Else justgo ahead fall through for further handling

  if (event_count > 0)
    {
      // Next dispatch the notification handlers (if there are any to
      // dispatch).  These are required to handle multiple-threads that
      // are trying to update the <Reactor>.
      result = this->handle_notify_events (event_count,
                                           guard);

      if (result > 0)
        return result;

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
ACE_TP_Reactor::handle_timer_events (int & /*event_count*/,
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
ACE_TP_Reactor::handle_notify_events (int & /*event_count*/,
                                      ACE_TP_Token_Guard &guard)
{
  // Get the handle on which notify calls could have occured
  ACE_HANDLE notify_handle =
    this->get_notify_handle ();

  int result = 0;

  // The notify was not in the list returned by
  // wait_for_multiple_events ().
  if (notify_handle == ACE_INVALID_HANDLE)
    return result;

  // Now just do a read on the pipe..
  ACE_Notification_Buffer buffer;

  // Clear the handle of the read_mask of our <ready_set_>
  this->ready_set_.rd_mask_.clr_bit (notify_handle);

  // Keep reading notifies till we empty it or till we have a
  // dispatchable buffer
  while (this->notify_handler_->read_notify_pipe (notify_handle,
                                                  buffer) > 0)
    {
      // Just figure out whether we can read  any buffer that has
      // dispatchable info. If not we have just been unblocked by
      // another thread trying to update the reactor. If we get any
      // buffer that needs dispatching we will dispatch that after
      // releasing the lock
      if (this->notify_handler_->is_dispatchable (buffer) > 0)
        {
          // Release the token before dispatching notifies...
          guard.release_token ();

          // Dispatch the upcall for the notify
          this->notify_handler_->dispatch_notify (buffer);

          // We had a successful dispatch.
          result = 1;

          // break out of the while loop
          break;
        }
    }

  // If we did ssome work, then we just return 1 which will allow us
  // to get out of here. If we return 0, then we will be asked to do
  // some work ie. dispacth socket events
  return result;
}

int
ACE_TP_Reactor::handle_socket_events (int &event_count,
                                      ACE_TP_Token_Guard &guard)
{

  // We got the lock, lets handle some I/O events.
  ACE_EH_Dispatch_Info dispatch_info;

  this->get_socket_event_info (dispatch_info);

  // If there is any event handler that is ready to be dispatched, the
  // dispatch information is recorded in dispatch_info.
  if (!dispatch_info.dispatch ())
    {
      return 0;
    }

  // Suspend the handler so that other threads don't start
  // dispatching it.
  // NOTE: This check was performed in older versions of the
  // TP_Reactor. Looks like it is a waste..
  if (dispatch_info.event_handler_ != this->notify_handler_)
    this->suspend_i (dispatch_info.handle_);

  // Release the lock.  Others threads can start waiting.
  guard.release_token ();

  int result = 0;

  // If there was an event handler ready, dispatch it.
  // Decrement the event left
  --event_count;

  if (this->dispatch_socket_event (dispatch_info) == 0)
    ++result;                // Dispatched an event

  // This is to get around a problem/ which  is well described in
  // 1361. This is just a work around that  would help applications
  // from resuming handles at the most  inopportune moment.
  int flag =
    ACE_Event_Handler::ACE_EVENT_HANDLER_NOT_RESUMED;

  // Acquire the token since we want to access the handler
  // repository. The call to find () does not hold a lock and hence
  // this is required.
  guard.acquire_token ();

  // Get the handler for the handle that we have dispatched to.
  ACE_Event_Handler *eh =
    this->handler_rep_.find (dispatch_info.handle_);

  // This check is required for the following reasons
  // 1. If dispatch operation returned a -1, then there is a
  // possibility that the event handler could be deleted. In such
  // cases the pointer to the event_handler that <dispatch_info>
  // holds is set to 0.
  //
  // 2. If the application did its own memory management, a return
  // value of 0 cannot be believed since the event handler could
  // be deleted by the application based on some conditions. This
  // is *bad*. But we dont have much of a choice with the existing
  // reactor setup. To get around this, we can make a check for
  // the handler registered with the repository for the handle
  // that we have and compare with the handler that we
  // posses. Yeah, I know it is like touching your nose by taking
  // your hand around your head. But that is the way it is. This
  // is a fix for [BUGID 1231]

  if (dispatch_info.event_handler_ != 0 &&
      eh == dispatch_info.event_handler_)
    {
      flag =
           dispatch_info.event_handler_->resume_handler ();
    }

  // Use resume_i () since we hold the token already.
  if (dispatch_info.handle_ != ACE_INVALID_HANDLE &&
      dispatch_info.event_handler_ != this->notify_handler_ &&
      flag == ACE_Event_Handler::ACE_REACTOR_RESUMES_HANDLER)
    this->resume_i (dispatch_info.handle_);

  // Let me release the token here. This is not required since the
  // destruction of the object on the stack will take care of this.
  guard.release_token ();

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



int
ACE_TP_Reactor::get_event_for_dispatching (ACE_Time_Value *max_wait_time)
{

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

  return this->wait_for_multiple_events (this->ready_set_,
                                         max_wait_time);
}

int
ACE_TP_Reactor::get_socket_event_info (ACE_EH_Dispatch_Info &event)
{
  event.reset ();           // Nothing to dispatch yet

  // Check for dispatch in write, except, read. Only catch one, but if
  // one is caught, be sure to clear the handle from each mask in case
  // there is more than one mask set for it. This would cause problems
  // if the handler is suspended for dispatching, but its set bit in
  // another part of ready_set_ kept it from being dispatched.
  int found_io = 0;
  ACE_HANDLE handle;

  // @@todo: We can do quite a bit of code reduction here. Let me get
  // it to work before I do this.
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

        this->clear_handle_read_set (handle);
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

          this->clear_handle_read_set (handle);

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

          this->clear_handle_read_set (handle);
          found_io = 1;
        }
    }

  return found_io;
}



// Dispatches a single event handler
int
ACE_TP_Reactor::dispatch_socket_event (ACE_EH_Dispatch_Info &dispatch_info)
{
  ACE_TRACE ("ACE_TP_Reactor::dispatch_socket_event");

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

int
ACE_TP_Reactor::resumable_handler (void)
{
  return 1;
}

int
ACE_TP_Reactor::handle_events (ACE_Time_Value &max_wait_time)
{
  return this->handle_events (&max_wait_time);
}

int
ACE_TP_Reactor::mask_ops (ACE_Event_Handler *eh,
                          ACE_Reactor_Mask mask,
                          int ops)
{
  return this->mask_ops (eh->get_handle (), mask, ops);
}

void
ACE_TP_Reactor::notify_handle (ACE_HANDLE,
                               ACE_Reactor_Mask,
                               ACE_Handle_Set &,
                               ACE_Event_Handler *eh,
                               ACE_EH_PTMF)
{
  ACE_ERROR ((LM_ERROR,
              ACE_LIB_TEXT ("ACE_TP_Reactor::notify_handle: ")
              ACE_LIB_TEXT ("Wrong version of notify_handle() got called \n")));

  ACE_ASSERT (eh == 0);
  ACE_UNUSED_ARG (eh);
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
    /*if (read_handle != ACE_INVALID_HANDLE)
    {
      ACE_Handle_Set_Iterator handle_iter (this->ready_set_.rd_mask_);
      ACE_HANDLE handle = ACE_INVALID_HANDLE;

      while ((handle = handle_iter ()) == read_handle)
        {
          return read_handle;
        }
      ACE_UNUSED_ARG (handle);
      }*/

  // None found..
  return ACE_INVALID_HANDLE;
}
