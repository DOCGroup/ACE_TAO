// $Id$

#define ACE_BUILD_DLL

#include "ace/TP_Reactor.h"
#include "ace/Reactor.h"

#if !defined (__ACE_INLINE__)
#include "ace/TP_Reactor.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(ace, TP_Reactor, "$Id$")

ACE_ALLOC_HOOK_DEFINE (ACE_TP_Reactor)

ACE_TP_Reactor::ACE_TP_Reactor (ACE_Sig_Handler *sh,
                                ACE_Timer_Queue *tq)
  : ACE_Select_Reactor (sh, tq)
{
  ACE_TRACE ("ACE_TP_Reactor::ACE_TP_Reactor");
  this->supress_notify_renew (1);
}

ACE_TP_Reactor::ACE_TP_Reactor (size_t size,
                                int rs,
                                ACE_Sig_Handler *sh,
                                ACE_Timer_Queue *tq)
  : ACE_Select_Reactor (size, rs, sh, tq)
{
  ACE_TRACE ("ACE_TP_Reactor::ACE_TP_Reactor");
  this->supress_notify_renew (1);
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

  // Upcall
  int status = (event_handler->*callback) (handle);

  // If negative, remove from Reactor
  if (status < 0)
    return this->remove_handler (handle, mask);

  // If positive, reschedule in Reactor
  else if (status > 0)
    this->ready_ops (handle, mask, ACE_Reactor::SET_MASK);

  // assert (status >= 0);
  // resume in Reactor
  return (event_handler != this->notify_handler_ ?
          this->resume_handler (handle) : 0);
}

// Overwrites ACE_Select_Reactor::dispatch_io_set() to *not* dispatch
// any event handlers.  The information of one activated event handler
// is stored away, so that the event handler can be dispatch later.
int
ACE_TP_Reactor::dispatch_io_set (int number_of_active_handles,
                                 int& number_dispatched,
                                 int mask,
                                 ACE_Handle_Set& dispatch_mask,
                                 ACE_Handle_Set& ready_mask,
                                 ACE_EH_PTMF callback)
{
  ACE_UNUSED_ARG (ready_mask);

  ACE_HANDLE handle;

  ACE_Handle_Set_Iterator handle_iter (dispatch_mask);

  while ((handle = handle_iter ()) != ACE_INVALID_HANDLE
         && number_dispatched < number_of_active_handles
         && this->state_changed_ == 0)
    {
      // ACE_DEBUG ((LM_DEBUG,  ASYS_TEXT ("ACE_TP_Reactor::dispatching\n")));
      number_dispatched++;

      // Remember this info
      this->dispatch_info_.set (handle,
                                this->handler_rep_.find (handle),
                                mask,
                                callback);

      // One is enough
      break;
    }

  if (number_dispatched > 0 && this->state_changed_)
    {
      return -1;
    }

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
  ACE_MT (result = this->token_.acquire_read (&ACE_TP_Reactor::no_op_sleep_hook));
  switch (result)
    {
    case 2:
      ACE_MT (this->token_.release ());
      return 0;
    case -1:
      return -1;
    }

  // We got the lock, lets handle some events.  Note: this method will
  // *not* dispatch any handlers.  It will dispatch timeouts and
  // signals.
  result = this->handle_events_i (max_wait_time);
  if (result == -1)
    {
      ACE_MT (this->token_.release ());
      return -1;
    }

  // If there is any event handler that is ready to be dispatched, the
  // dispatch information is recorded in this->dispatch_info_.
  ACE_EH_Dispatch_Info dispatch_info;
  if (this->dispatch_info_.dispatch ())
    {
      // Copy dispatch_info_ to the stack, as other threads can change
      // dispatch_info_ after we release the lock.
      dispatch_info = this->dispatch_info_;

      // Reset dispatch_info_ so that we don't trip over it again.
      this->dispatch_info_.reset ();

      if (dispatch_info.event_handler_ == this->notify_handler_)
        {
          // Make sure we never suspend the notify_handler_ without holding the
          // lock.
          // @@ Actually, we don't even need to suspend the notify_handler_
          //    here.  But let me get it to work first.
          int retv = this->notify_handle (dispatch_info);
          ACE_MT (this->token_.release ());
          return retv;
        }
      else
        // Suspend the handler so that other thread don't start
        // dispatching it.
        result = this->suspend_i (dispatch_info.handle_);
    }

  // Release the lock.  Others threads can start waiting.
  ACE_MT (this->token_.release ());

  // If there was an event handler ready, dispatch it.
  if (dispatch_info.dispatch ())
    return this->notify_handle (dispatch_info);
  else
    return result;
}

void
ACE_TP_Reactor::no_op_sleep_hook (void *)
{
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
