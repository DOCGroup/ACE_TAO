// XtReactor.cpp
// $Id$

#define ACE_BUILD_DLL
#include "ace/Log_Msg.h"
#include "ace/Synch_T.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/SOCK_Connector.h"
#include "ace/XtReactor.h"

#if defined (ACE_HAS_XT)

struct ACE_XtReactorID 
{
    XtInputId id;
    int good_id;
};

ACE_ALLOC_HOOK_DEFINE (ACE_XtReactor)

// Must be called with lock held
ACE_XtReactor::ACE_XtReactor (XtAppContext context,
		      size_t size, 
		      int restart, 
		      ACE_Sig_Handler *h)
  : ACE_Reactor (size, restart, h),
    context_ (context),
    id_len_ (0),
    ids_ (0)
{
  // When the ACE_Reactor is constructed it creates the notify pipe
  // and registers it with the attach() method. The XtReactor
  // overloads this method BUT because the attach occurs when
  // constructing the base class ACE_Reactor, the ACE_Reactor attach()
  // is called not the XtReactor attach().  This means that the notify
  // pipe is registered with the ACE_Reactor event handling code not
  // the XtReactor and so notfications don't work.  To get around this
  // we simply close and re-opened the notification handler in the
  // constructor of the XtReactor.

  this->notification_handler_.close ();
  this->notification_handler_.open (this);
}

ACE_XtReactor::~ACE_XtReactor (void)
{
  delete this->ids_;
}

// This is just the wait_for_multiple_events from ace/Reactor.cpp but
// we use the Xt functions to wait for an event, not select ()
//
// FIXME - someday re-write this to use poll as well.

int 
ACE_XtReactor::wait_for_multiple_events (ACE_Handle_Set &rmask, 
				     ACE_Handle_Set &wmask, 
				     ACE_Handle_Set &emask, 
				     ACE_Time_Value *max_wait_time)
{
    ACE_TRACE ("ACE_Reactor::wait_for_multiple_events");
#if defined (ACE_USE_POLL)
    u_long width = 0;
#endif /* ACE_USE_POLL */
    int nfound;

    do 
      {
	max_wait_time = this->timer_queue_->calculate_timeout (max_wait_time);
      
#if defined (ACE_USE_POLL)
	pollfd *phandles = this->handle_sets_to_poll_fds (width);
	nfound = ACE_OS::poll (phandles, width, max_wait_time);
#else /* USE SELECT */
	size_t width = this->handler_rep_.max_handlep1 ();
	rmask = this->rd_handle_mask_;
	wmask = this->wr_handle_mask_;
	emask = this->ex_handle_mask_;
#if 0
	nfound = ACE_OS::select (int (width), rmask, wmask, emask, max_wait_time);
#else
	nfound = XtWaitForMultipleEvents (width, rmask, wmask, emask, max_wait_time);
#endif
#endif /* ACE_USE_POLL */
      } while (nfound == -1 && this->handle_error () > 0);

#if defined (ACE_USE_POLL)
      this->poll_fds_to_handle_sets (width, rmask, wmask, emask, nfound);
#endif /* ACE_USE_POLL */

    if (nfound > 0)
      {
#if !defined (ACE_WIN32)
	rmask.sync (this->handler_rep_.max_handlep1 ());
	wmask.sync (this->handler_rep_.max_handlep1 ());
	emask.sync (this->handler_rep_.max_handlep1 ());
#endif /* ACE_REACTOR_ALTERANTIVE_IMPL */
      }
    return nfound; // Timed out or input available
}

void 
ACE_XtReactor::TimerCallbackProc (XtPointer closure, XtIntervalId *id)
{
  ACE_XtReactor *self = (ACE_XtReactor*)closure;
  self->timeout_ = 0;

  ACE_DEBUG ((LM_DEBUG, "ACE_XtReactor::Timer on id %d\n", (int) *id));

  ACE_Time_Value zero = ACE_Time_Value::zero; // my copy isn't const

  // Deal with any timer events
  ACE_Handle_Set r, w, e;
  self->dispatch (0, r, w, e);
  self->reset_timeout ();
}

// This could be made shorter if we know which *kind* of event we were
// about to get.  Here we use select () to find out which one might be
// available.

void ACE_XtReactor::InputCallbackProc (XtPointer closure, 
				       int * source, 
				       XtInputId *id)
{
  ACE_XtReactor *self = (ACE_XtReactor *) closure;

  ACE_DEBUG ((LM_DEBUG, "ACE_XtReactor::Input on fd %d\n", *source));

  ACE_Time_Value zero = ACE_Time_Value::zero; // my copy isn't const

  ACE_Handle_Set r, w, e;
  ACE_Handle_Set r2, w2, e2;

  // Deal with one file event
    
  // - read which kind of event
  if (self->rd_handle_mask_.is_set (*source))
    r.set_bit (*source);
  if (self->wr_handle_mask_.is_set (*source))
    w.set_bit (*source);
  if (self->ex_handle_mask_.is_set (*source))
    e.set_bit (*source);

  int result = ACE_OS::select (*source+1, r, w, e, &zero);

  // - Use only that one file event (removes events for other files)
  if (result > 0)
    {
      if (r.is_set (*source))
	r2.set_bit (*source);
      if (w.is_set (*source))
	w2.set_bit (*source);
      if (e.is_set (*source))
	e2.set_bit (*source);

      self->dispatch (1, r2, w2, e2);
    }
}

int ACE_XtReactor::XtWaitForMultipleEvents (int width, 
					    ACE_Handle_Set &rmask, 
					    ACE_Handle_Set &wmask, 
					    ACE_Handle_Set &emask, 
					    ACE_Time_Value *max_wait_time)
{
  // Check to make sure our fd's are all usable

  ACE_Handle_Set r (rmask), w (wmask), e (emask);
  ACE_Time_Value zero = ACE_Time_Value::zero; // my copy isn't const
  int result = ACE_OS::select (width, r, w, e, &zero);

  if (result < 0)		// Bad file arguments...
    return result;

  // Instead of waiting using select, just use the Xt mechanism to wait
  // for a single event.

  // Wait for something to happen.
  XtAppProcessEvent (context_, XtIMAll);

  // Now actually read the result needed by the Reactor using select.
  result = ACE_OS::select (int (width), rmask, wmask, emask,  &zero);
  return result;
}

XtAppContext ACE_XtReactor::context (void) 
{
  return this->context_; 
}

int
ACE_XtReactor::attach (ACE_HANDLE handle, 
		       ACE_Event_Handler *handler,
		       ACE_Reactor_Mask mask)
{
  ACE_TRACE ("ACE_XtReactor::attach");

  ACE_DEBUG ((LM_DEBUG, "+++%d\n", handle));

  int result = ACE_Reactor::attach (handle, handler, mask);

  if (result < 0)
    return -1;

  // Ensure the list of InputId's is big enough
  if (this->ids_ == 0 || this->id_len_ < handle + 1)
    {
      ACE_XtReactorID *more;
      ACE_NEW_RETURN (more, ACE_XtReactorID[handle + 1], -1);

      int i;

      for (i = 0; i < this->id_len_; i++)
	more[i] = ids_[i];

      for (i = this->id_len_; i < handle + 1; i++)
	more[i].good_id = 0;

      id_len_ = handle + 1;
      delete this->ids_;
      ids_ = more;
    }

  int condition = 0;

  if (mask & ACE_Event_Handler::READ_MASK)
    ACE_SET_BITS (condition, XtInputReadMask);
  if (mask & ACE_Event_Handler::WRITE_MASK)
    ACE_SET_BITS (condition, XtInputWriteMask);
  if (mask & ACE_Event_Handler::EXCEPT_MASK)
    ACE_SET_BITS (condition, XtInputExceptMask);

  if (condition != 0)
    {
      if (ids_[handle].good_id)
	XtRemoveInput (ids_[handle].id);

      ids_[handle].id = XtAppAddInput (context_, 
				       handle, 
				       (XtPointer)condition, 
				       InputCallbackProc, 
				       (XtPointer)this);
      ids_[handle].good_id = 1;
    }
  return 0;
}


int
ACE_XtReactor::detach (ACE_HANDLE handle, 
		     ACE_Reactor_Mask mask)
{
  ACE_TRACE ("ACE_XtReactor::detach");
  ACE_DEBUG ((LM_DEBUG, "---%d\n", handle));

  int result = ACE_Reactor::detach (handle, mask);

  if (handle <= id_len_)
    { 
      if (ids_[handle].good_id)
	XtRemoveInput (ids_[handle].id);
      else
	ACE_DEBUG ((LM_DEBUG, "Handle id is not good %d\n", handle));
      ids_[handle].good_id = 0;
    }
  else 
    ACE_DEBUG ((LM_DEBUG, "Handle out of range %d\n", handle));

  if (result < 0)
    return result;
  else
    return 0;
}

// The following functions ensure that there is an Xt timeout for the
// first timeout in the Reactor's Timer_Queue.

void ACE_XtReactor::reset_timeout (void)
{
  if (timeout_)
    XtRemoveTimeOut (timeout_);
  timeout_ = 0;

  ACE_Time_Value *max_wait_time;
  max_wait_time = this->timer_queue_->calculate_timeout (0);

  if (max_wait_time)
    {
      ACE_DEBUG ((LM_DEBUG, "       %ld\n", max_wait_time->msec ()));

      timeout_ = 
	XtAppAddTimeOut (context_, max_wait_time->msec (), 
			 TimerCallbackProc, (XtPointer) this);
    }
}

int
ACE_XtReactor::schedule_timer (ACE_Event_Handler *handler, 
			       const void *arg,
			       const ACE_Time_Value &delta_time, 
			       const ACE_Time_Value &interval)
{
  ACE_TRACE ("ACE_XtReactor::schedule_timer");
  ACE_GUARD_RETURN (ACE_REACTOR_MUTEX, ace_mon, this->token_, -1);

  int result = 
    ACE_Reactor::schedule_timer (handler, arg, delta_time, interval);

  if (result < 0)
    return result;

  this->reset_timeout ();
  return result;
}

int
ACE_XtReactor::cancel_timer (ACE_Event_Handler *handler)
{
  ACE_TRACE ("ACE_XtReactor::cancel_timer");

  int result = ACE_Reactor::cancel_timer (handler);
      
  if (result < 0)
    return -1;

  this->reset_timeout ();
  return 0;
}

int
ACE_XtReactor::cancel_timer (int timer_id, const void **arg)
{
  ACE_TRACE ("ACE_XtReactor::cancel_timer");

  int result = ACE_Reactor::cancel_timer (timer_id, arg);

  if (result < 0)
    return -1;

  this->reset_timeout ();
  return 0;
}

#endif /* ACE_HAS_XT */
