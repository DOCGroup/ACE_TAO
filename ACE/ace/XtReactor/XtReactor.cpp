#include "ace/XtReactor/XtReactor.h"

#include "ace/SOCK_Acceptor.h"
#include "ace/SOCK_Connector.h"

#include <memory>

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_ALLOC_HOOK_DEFINE (ACE_XtReactor)

// Must be called with lock held
ACE_XtReactor::ACE_XtReactor (XtAppContext context,
                              size_t size,
                              bool restart,
                              ACE_Sig_Handler *h)
  : ACE_Select_Reactor (size, restart, h),
    context_ (context)
{
  // When the ACE_Select_Reactor is constructed it creates the notify
  // pipe and registers it with the register_handler_i() method. The
  // XtReactor overloads this method BUT because the
  // register_handler_i occurs when constructing the base class
  // ACE_Select_Reactor, the ACE_Select_Reactor register_handler_i()
  // is called not the XtReactor register_handler_i().  This means
  // that the notify pipe is registered with the ACE_Select_Reactor
  // event handling code not the XtReactor and so notfications don't
  // work.  To get around this we simply close and re-opened the
  // notification handler in the constructor of the XtReactor.

#if defined (ACE_MT_SAFE) && (ACE_MT_SAFE != 0)
  this->notify_handler_->close ();
  this->notify_handler_->open (this, 0);
#endif /* ACE_MT_SAFE */
}

ACE_XtReactor::~ACE_XtReactor ()
{
  if (this->timeout_ != 0)
    ::XtRemoveTimeOut (this->timeout_);

  while (this->ids_)
    {
      ::XtRemoveInput (this->ids_->id_);
      ACE_XtReactorID *const next = this->ids_->next_;
      delete this->ids_;
      this->ids_ = next;
    }
}

// This is just the <wait_for_multiple_events> from ace/Reactor.cpp
// but we use the Xt functions to wait for an event, not <select>

int
ACE_XtReactor::wait_for_multiple_events (ACE_Select_Reactor_Handle_Set &handle_set,
                                         ACE_Time_Value *max_wait_time)
{
  ACE_TRACE ("ACE_XtReactor::wait_for_multiple_events");
  int nfound;

  do
    {
      max_wait_time = this->timer_queue_->calculate_timeout (max_wait_time);

      size_t width = this->handler_rep_.max_handlep1 ();
      handle_set.rd_mask_ = this->wait_set_.rd_mask_;
      handle_set.wr_mask_ = this->wait_set_.wr_mask_;
      handle_set.ex_mask_ = this->wait_set_.ex_mask_;
      nfound = XtWaitForMultipleEvents (width,
                                        handle_set,
                                        max_wait_time);
    } while (nfound == -1 && this->handle_error () > 0);

  if (nfound > 0)
    {
#if !defined (ACE_WIN32)
      handle_set.rd_mask_.sync (this->handler_rep_.max_handlep1 ());
      handle_set.wr_mask_.sync (this->handler_rep_.max_handlep1 ());
      handle_set.ex_mask_.sync (this->handler_rep_.max_handlep1 ());
#endif /* ACE_WIN32 */
    }
  return nfound; // Timed out or input available
}

void
ACE_XtReactor::TimerCallbackProc (XtPointer closure, XtIntervalId * /* id */)
{
  ACE_XtReactor *const self = static_cast<ACE_XtReactor *> (closure);
  self->timeout_ = {};

  // Deal with any timer events
  ACE_Select_Reactor_Handle_Set handle_set;
  self->dispatch (0, handle_set);
  self->reset_timeout ();
}

// This could be made shorter if we know which *kind* of event we were
// about to get.  Here we use <select> to find out which one might be
// available.

void
ACE_XtReactor::InputCallbackProc (XtPointer closure,
                                  int *source,
                                  XtInputId *)
{
  ACE_XtReactor *const self = static_cast<ACE_XtReactor *> (closure);
  ACE_HANDLE const handle = static_cast<ACE_HANDLE> (*source);

  ACE_Time_Value const zero (ACE_Time_Value::zero);

  ACE_Select_Reactor_Handle_Set wait_set;

  // Deal with one file event.

  // - read which kind of event
  if (self->wait_set_.rd_mask_.is_set (handle))
    wait_set.rd_mask_.set_bit (handle);
  if (self->wait_set_.wr_mask_.is_set (handle))
    wait_set.wr_mask_.set_bit (handle);
  if (self->wait_set_.ex_mask_.is_set (handle))
    wait_set.ex_mask_.set_bit (handle);

  int const result = ACE_OS::select (*source + 1,
                                     wait_set.rd_mask_,
                                     wait_set.wr_mask_,
                                     wait_set.ex_mask_,
                                     &zero);

  ACE_Select_Reactor_Handle_Set dispatch_set;

  // - Use only that one file event (removes events for other files).
  if (result > 0)
    {
      if (wait_set.rd_mask_.is_set (handle))
        dispatch_set.rd_mask_.set_bit (handle);
      if (wait_set.wr_mask_.is_set (handle))
        dispatch_set.wr_mask_.set_bit (handle);
      if (wait_set.ex_mask_.is_set (handle))
        dispatch_set.ex_mask_.set_bit (handle);

      self->dispatch (1, dispatch_set);
    }
}

int
ACE_XtReactor::XtWaitForMultipleEvents (int width,
                                        ACE_Select_Reactor_Handle_Set &wait_set,
                                        ACE_Time_Value *)
{
  // Make sure we have a valid context
  ACE_ASSERT (this->context_ != nullptr);

  // Check to make sure our handle's are all usable.
  ACE_Select_Reactor_Handle_Set temp_set = wait_set;

  if (ACE_OS::select (width,
                      temp_set.rd_mask_,
                      temp_set.wr_mask_,
                      temp_set.ex_mask_,
                      &ACE_Time_Value::zero) == -1)
    return -1; // Bad file arguments...

  // Instead of waiting using <select>, just use the Xt mechanism to
  // wait for a single event.

  // Wait for something to happen.
  ::XtAppProcessEvent (this->context_, XtIMAll);

  // Reset the width, in case it changed during the upcalls.
  width = this->handler_rep_.max_handlep1 ();

  // Now actually read the result needed by the <Select_Reactor> using
  // <select>.
  return ACE_OS::select (width,
                         wait_set.rd_mask_,
                         wait_set.wr_mask_,
                         wait_set.ex_mask_,
                         &ACE_Time_Value::zero);
}

XtAppContext
ACE_XtReactor::context () const
{
  return this->context_;
}

void
ACE_XtReactor::context (XtAppContext context)
{
  this->context_ = context;
}

int
ACE_XtReactor::register_handler_i (ACE_HANDLE handle,
                                   ACE_Event_Handler *handler,
                                   ACE_Reactor_Mask mask)
{
  ACE_TRACE ("ACE_XtReactor::register_handler_i");

  // Make sure we have a valid context
  ACE_ASSERT (this->context_ != nullptr);

#if defined ACE_WIN32
  // Let's handle this special case before we do any real work.
  if (ACE_BIT_ENABLED (mask, ACE_Event_Handler::EXCEPT_MASK))
    ACE_NOTSUP_RETURN (-1);
#endif /* ACE_WIN32 */

  int const result = ACE_Select_Reactor::register_handler_i (handle,
                                                             handler,
                                                             mask);
  if (result == -1)
    return -1;

  this->synchronize_XtInput (handle);
  return 0;
}

int
ACE_XtReactor::register_handler_i (ACE_Handle_Set const &handles,
                                   ACE_Event_Handler *handler,
                                   ACE_Reactor_Mask mask)
{
  return ACE_Select_Reactor::register_handler_i (handles,
                                                 handler,
                                                 mask);
}

int
ACE_XtReactor::remove_handler_i (ACE_HANDLE handle,
                                 ACE_Reactor_Mask mask)
{
  ACE_TRACE ("ACE_XtReactor::remove_handler_i");

  int const result = ACE_Select_Reactor::remove_handler_i (handle,
                                                           mask);
  if (result == -1)
    return -1;

  this->synchronize_XtInput (handle);
  return 0;
}

int
ACE_XtReactor::remove_handler_i (ACE_Handle_Set const &handles,
                                 ACE_Reactor_Mask mask)
{
  return ACE_Select_Reactor::remove_handler_i (handles,
                                               mask);
}

int
ACE_XtReactor::suspend_i (ACE_HANDLE handle)
{
  ACE_TRACE ("ACE_XtReactor::suspend_i");

  int const result = ACE_Select_Reactor::suspend_i (handle);

  if (result == -1)
    return -1;

  this->synchronize_XtInput (handle);
  return 0;
}

int
ACE_XtReactor::resume_i (ACE_HANDLE handle)
{
  ACE_TRACE ("ACE_XtReactor::resume_i");

  int const result = ACE_Select_Reactor::resume_i (handle);

  if (result == -1)
    return -1;

  this->synchronize_XtInput (handle);
  return 0;
}

void
ACE_XtReactor::synchronize_XtInput (ACE_HANDLE handle)
{
  ACE_TRACE ("ACE_XtReactor::synchronize_XtInput");

  // The idea here is to call this function after the base class has
  // processed the register/remove/suspend/resume_handler request. The
  // resulting mask is used to find out which XtInput mask we need.

  // Find existing handler in linked list.
  ACE_XtReactorID **xt_id = &this->ids_;

  while (*xt_id && (*xt_id)->handle_ != handle)
    xt_id = &((*xt_id)->next_);

  // Remove existing input handler.
  if (*xt_id)
    ::XtRemoveInput ((*xt_id)->id_);

  int const condition = this->compute_Xt_condition (handle);

  if (condition == 0) // No input handler needed.
    {
      if (*xt_id)
        {
          // Remove linked list entry.
          ACE_XtReactorID *const to_delete = *xt_id;
          *xt_id = (*xt_id)->next_;
          delete to_delete;
        }
      return;
    }

  if (*xt_id == nullptr)
    {
      // Create new node.
      ACE_XtReactorID *new_id {};
      ACE_NEW (new_id, ACE_XtReactorID);
      std::unique_ptr<ACE_XtReactorID> new_id_owner (new_id);
      new_id_owner->next_ = this->ids_;
      new_id_owner->handle_ = handle;
      this->ids_ = new_id_owner.release ();
      xt_id = &this->ids_;
    }

  // Finally, add input handler.
  (*xt_id)->id_ = ::XtAppAddInput (this->context_,
                                   static_cast<int> (handle),
                                   reinterpret_cast<XtPointer> (condition),
                                   InputCallbackProc,
                                   this);
}

int
ACE_XtReactor::compute_Xt_condition (ACE_HANDLE handle)
{
  ACE_TRACE ("ACE_XtReactor::compute_Xt_condition");

  // Retrieve current wait mask from base class.
  // The returned value is either a combination of READ/WRITE/EXCEPT_MASK
  // or -1.
  int const mask = this->bit_ops (handle,
                                  0,
                                  this->wait_set_,
                                  ACE_Reactor::GET_MASK);

  if (mask == -1) // No active mask.
    return 0;

  int condition {};

#if !defined ACE_WIN32
  if (ACE_BIT_ENABLED (mask, ACE_Event_Handler::READ_MASK))
    ACE_SET_BITS (condition, XtInputReadMask);
  if (ACE_BIT_ENABLED (mask, ACE_Event_Handler::WRITE_MASK))
    ACE_SET_BITS (condition, XtInputWriteMask);
  if (ACE_BIT_ENABLED (mask, ACE_Event_Handler::EXCEPT_MASK))
    ACE_SET_BITS (condition, XtInputExceptMask);
#else
  if (ACE_BIT_ENABLED (mask, ACE_Event_Handler::READ_MASK))
    ACE_SET_BITS (condition, XtInputReadWinsock);
  if (ACE_BIT_ENABLED (mask, ACE_Event_Handler::WRITE_MASK))
    ACE_SET_BITS (condition, XtInputWriteWinsock);
  // EXCEPT_MASK is not supported for WIN32. As this was
  // already handled in register_handler_i, no check here.
#endif /* !ACE_WIN32 */

  return condition;
}

// The following functions ensure that there is an Xt timeout for the
// first timeout in the Reactor's Timer_Queue.

void
ACE_XtReactor::reset_timeout ()
{
  // Make sure we have a valid context
  ACE_ASSERT (this->context_ != nullptr);

  if (this->timeout_ != 0)
    ::XtRemoveTimeOut (this->timeout_);
  this->timeout_ = {};

  ACE_Time_Value const *max_wait_time =
    this->timer_queue_->calculate_timeout (nullptr);

  if (max_wait_time)
    this->timeout_ = ::XtAppAddTimeOut (this->context_,
                                        max_wait_time->msec (),
                                        TimerCallbackProc,
                                        this);
}

int
ACE_XtReactor::reset_timer_interval
  (long timer_id,
   ACE_Time_Value const &interval)
{
  ACE_TRACE ("ACE_XtReactor::reset_timer_interval");
  ACE_MT (ACE_GUARD_RETURN (ACE_Select_Reactor_Token, ace_mon, this->token_, -1));

  int const result = ACE_Select_Reactor::timer_queue_->reset_interval
    (timer_id,
     interval);

  if (result == -1)
    return -1;

  this->reset_timeout ();
  return result;
}

long
ACE_XtReactor::schedule_timer (ACE_Event_Handler *event_handler,
                               void const *arg,
                               ACE_Time_Value const &delay,
                               ACE_Time_Value const &interval)
{
  ACE_TRACE ("ACE_XtReactor::schedule_timer");
  ACE_MT (ACE_GUARD_RETURN (ACE_Select_Reactor_Token, ace_mon, this->token_, -1));

  long const result = ACE_Select_Reactor::schedule_timer (event_handler,
                                                          arg,
                                                          delay,
                                                          interval);
  if (result == -1)
    return -1;

  this->reset_timeout ();
  return result;
}

int
ACE_XtReactor::cancel_timer (ACE_Event_Handler *handler,
                             int dont_call_handle_close)
{
  ACE_TRACE ("ACE_XtReactor::cancel_timer");

  if (ACE_Select_Reactor::cancel_timer (handler,
                                        dont_call_handle_close) == -1)
    return -1;

  this->reset_timeout ();
  return 0;
}

int
ACE_XtReactor::cancel_timer (long timer_id,
                             void const **arg,
                             int dont_call_handle_close)
{
  ACE_TRACE ("ACE_XtReactor::cancel_timer");

  if (ACE_Select_Reactor::cancel_timer (timer_id,
                                        arg,
                                        dont_call_handle_close) == -1)
    return -1;

  this->reset_timeout ();
  return 0;
}

ACE_END_VERSIONED_NAMESPACE_DECL
