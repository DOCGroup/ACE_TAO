// Reactor.cpp
// $Id$

#define ACE_BUILD_DLL
#include "ace/Synch_T.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/SOCK_Connector.h"
#include "ace/Timer_List.h"
#include "ace/Reactor.h"

#if !defined (__ACE_INLINE__)
#include "ace/Reactor.i"
#endif /* __ACE_INLINE__ */

ACE_ALLOC_HOOK_DEFINE(ACE_Reactor)

#if defined (ACE_WIN32)
#define ACE_REACTOR_HANDLE(H) (this->event_handlers_[(H)].handle_)
#define ACE_REACTOR_EVENT_HANDLER(THIS,H) ((THIS)->event_handlers_[(H)].event_handler_)
#else
#define ACE_REACTOR_HANDLE(H) (H)
#define ACE_REACTOR_EVENT_HANDLER(THIS,H) ((THIS)->event_handlers_[(H)])
#endif /* ACE_WIN32 */

// Performs sanity checking on the ACE_HANDLE.

int
ACE_Reactor_Handler_Repository::invalid_handle (ACE_HANDLE handle)
{
  ACE_TRACE ("ACE_Reactor_Handler_Repository::invalid_handle");
#if defined (ACE_WIN32)
  // It's too expensive to perform more exhaustive validity checks on
  // Win32 due to the way that they implement SOCKET HANDLEs.
  if (handle == ACE_INVALID_HANDLE)
#else /* !ACE_WIN32 */
  if (handle < 0 || handle >= this->max_size_)
#endif /* ACE_WIN32 */
    {
      errno = EINVAL;
      return 1;
    }
  else
    return 0;
}

// Performs sanity checking on the ACE_HANDLE.

int
ACE_Reactor_Handler_Repository::handle_in_range (ACE_HANDLE handle)
{
  ACE_TRACE ("ACE_Reactor_Handler_Repository::handle_in_range");
#if defined (ACE_WIN32)
  // It's too expensive to perform more exhaustive validity checks on
  // Win32 due to the way that they implement SOCKET HANDLEs.
  if (handle != ACE_INVALID_HANDLE)
#else /* !ACE_WIN32 */
  if (handle >= 0 && handle < this->max_handlep1_)
#endif /* ACE_WIN32 */
    return 1;
  else
    {
      errno = EINVAL;
      return 0;
    }
}

size_t
ACE_Reactor_Handler_Repository::max_handlep1 (void)
{
  ACE_TRACE ("ACE_Reactor_Handler_Repository::max_handlep1");

  return this->max_handlep1_;
}

int
ACE_Reactor_Handler_Repository::open (size_t size)
{
  ACE_TRACE ("ACE_Reactor_Handler_Repository::open");
  this->max_size_ = size;
  this->max_handlep1_ = 0;

#if defined (ACE_WIN32)
  // Try to allocate the memory. 
  ACE_NEW_RETURN (this->event_handlers_, 
		  ACE_NT_EH_Record[size], 
		  -1);

  // Initialize the ACE_Event_Handler * to { ACE_INVALID_HANDLE, 0 }.
  for (size_t h = 0; h < size; h++)
    {
      ACE_REACTOR_HANDLE (h) = ACE_INVALID_HANDLE;
      ACE_REACTOR_EVENT_HANDLER (this, h) = 0;
    }
#else
  // Try to allocate the memory. 
  ACE_NEW_RETURN (this->event_handlers_, 
		  ACE_Event_Handler *[size], 
		  -1);

  // Initialize the ACE_Event_Handler * to NULL.
  for (size_t h = 0; h < size; h++)
    ACE_REACTOR_EVENT_HANDLER (this, h) = 0;
#endif /* ACE_WIN32 */
  return 0;
}

// Initialize a repository of the appropriate <size>.

ACE_Reactor_Handler_Repository::ACE_Reactor_Handler_Repository (ACE_Reactor &reactor)
  : reactor_ (reactor),
    max_size_ (0),    
    max_handlep1_ (0),
    event_handlers_ (0)
{
  ACE_TRACE ("ACE_Reactor_Handler_Repository::ACE_Reactor_Handler_Repository");
}

int
ACE_Reactor_Handler_Repository::unbind_all (void)
{
  // Unbind all of the <handle, ACE_Event_Handler>s.
  for (int handle = 0; 
       handle < this->max_handlep1_; 
       handle++)
    this->unbind (ACE_REACTOR_HANDLE (handle),
		  ACE_Event_Handler::ALL_EVENTS_MASK);

  return 0;
}

int
ACE_Reactor_Handler_Repository::close (void)
{
  ACE_TRACE ("ACE_Reactor_Handler_Repository::close");

  if (this->event_handlers_ != 0)
    {
      this->unbind_all ();

      delete [] this->event_handlers_;
      this->event_handlers_ = 0;
    }
  return 0;
}

// Return the <ACE_Event_Handler *> associated with the <handle>.

ACE_Event_Handler *
ACE_Reactor_Handler_Repository::find (ACE_HANDLE handle,
				      size_t *index_p)
{
  ACE_TRACE ("ACE_Reactor_Handler_Repository::find");

  ACE_Event_Handler *eh = 0;
  ssize_t i;

  // Only bother to search for the <handle> if it's in range.
  if (this->handle_in_range (handle))
    {
#if defined (ACE_WIN32)
      i = 0;

      for (; i < this->max_handlep1_; i++)
	if (ACE_REACTOR_HANDLE (i) == handle)
	  {
	    eh = ACE_REACTOR_EVENT_HANDLER (this, i);
	    break;
	  }
#else
      i = handle;

      eh = ACE_REACTOR_EVENT_HANDLER (this, handle);
#endif /* ACE_WIN32 */
    }
  else
    // g++ can't figure out that i won't be used below if the handle
    // is out of range, so keep it happy by defining i here . . .
    i = 0;
  
  if (eh != 0 && index_p != 0)
    *index_p = i;
  else
    errno = ENOENT;

  return eh;
}

// Bind the <ACE_Event_Handler *> to the <ACE_HANDLE>.

int 
ACE_Reactor_Handler_Repository::bind (ACE_HANDLE handle, 
				      ACE_Event_Handler *event_handler,
				      ACE_Reactor_Mask mask)
{
  ACE_TRACE ("ACE_Reactor_Handler_Repository::bind");

  if (handle == ACE_INVALID_HANDLE)
    handle = event_handler->get_handle ();

  if (this->invalid_handle (handle))
    return -1;

#if defined (ACE_WIN32)
  int assigned_slot = -1;

  for (ssize_t i = 0; i < this->max_handlep1_; i++)
    {
      // Found it, so let's just reuse this location.
      if (ACE_REACTOR_HANDLE (i) == handle)
	{
	  assigned_slot = i;
	  break;
	}
      // Here's the first free slot, so let's take it.
      else if (ACE_REACTOR_HANDLE (i) == ACE_INVALID_HANDLE
	       && assigned_slot == -1)
	assigned_slot = i;
    }

  if (assigned_slot > -1)
    // We found a free spot, let's reuse it.
    {
      ACE_REACTOR_HANDLE (assigned_slot) = handle;
      ACE_REACTOR_EVENT_HANDLER (this, assigned_slot) = event_handler;
    }
  else if (this->max_handlep1_ < this->max_size_)
    {
      // Insert at the end of the active portion. 
      ACE_REACTOR_HANDLE (this->max_handlep1_) = handle;
      ACE_REACTOR_EVENT_HANDLER (this, this->max_handlep1_) = event_handler;
      this->max_handlep1_++;
    }
  else
    {
      // No more room at the inn!
      errno = ENOMEM;
      return -1;
    }
#else
  ACE_REACTOR_EVENT_HANDLER (this, handle) = event_handler;

  if (this->max_handlep1_ < handle + 1)
    this->max_handlep1_ = handle + 1;
#endif /* ACE_WIN32 */

  // Add the <mask> for this <handle> in the Reactor's wait_set.
  this->reactor_.bit_ops (handle, 
			  mask, 
			  this->reactor_.wait_set_,
			  ACE_Reactor::ADD_MASK);

  // Note the fact that we've changed the state of the <wait_set_>,
  // which is used by the dispatching loop to determine whether it can
  // keep going or if it needs to reconsult select().
  this->reactor_.state_changed_ = 1;

  // Assign *this* <Reactor> to the <Event_Handler>.
  event_handler->reactor (&this->reactor_);
  return 0;
}

// Remove the binding of <ACE_HANDLE>.

int 
ACE_Reactor_Handler_Repository::unbind (ACE_HANDLE handle,
					ACE_Reactor_Mask mask)
{
  ACE_TRACE ("ACE_Reactor_Handler_Repository::unbind");

  size_t index;
  ACE_Event_Handler *eh = this->find (handle, &index);

  if (eh == 0)
    return -1;

  // Clear out the <mask> bits in the Reactor's wait_set.
  this->reactor_.bit_ops (handle, 
			  mask, 
			  this->reactor_.wait_set_,
			  ACE_Reactor::CLR_MASK);

  // Note the fact that we've changed the state of the <wait_set_>,
  // which is used by the dispatching loop to determine whether it can
  // keep going or if it needs to reconsult select().
  this->reactor_.state_changed_ = 1;

  // Close down the <Event_Handler> unless we've been instructed not
  // to.
  if (ACE_BIT_ENABLED (mask, ACE_Event_Handler::DONT_CALL) == 0)
    eh->handle_close (handle, mask);

  // If there are no longer any outstanding events on this <handle>
  // then we can totally shut down the Event_Handler.
  if (this->reactor_.wait_set_.rd_mask_.is_set (handle) == 0
      && this->reactor_.wait_set_.wr_mask_.is_set (handle) == 0 
      && this->reactor_.wait_set_.ex_mask_.is_set (handle) == 0)
#if defined (ACE_WIN32)
    {
      ACE_REACTOR_HANDLE (index) = ACE_INVALID_HANDLE;
      ACE_REACTOR_EVENT_HANDLER (this, index) = 0;

      if (this->max_handlep1_ == (int) index + 1)
	{	      
	  // We've deleted the last entry (i.e., i + 1 == the current
	  // size of the array), so we need to figure out the last
	  // valid place in the array that we should consider in
	  // subsequent searches.

	  for (int i = this->max_handlep1_ - 1;
	       i >= 0 && ACE_REACTOR_HANDLE (i) == ACE_INVALID_HANDLE;
	       i--)
	    continue;
	  
	  this->max_handlep1_ = i + 1;
	}
    }
#else
    {     
      ACE_REACTOR_EVENT_HANDLER (this, handle) = 0;
      
      if (this->max_handlep1_ == handle + 1)
	{
	  // We've deleted the last entry, so we need to figure out
	  // the last valid place in the array that is worth looking
	  // at.
	  ACE_HANDLE rd_max = this->reactor_.wait_set_.rd_mask_.max_set ();
	  ACE_HANDLE wr_max = this->reactor_.wait_set_.wr_mask_.max_set ();
	  ACE_HANDLE ex_max = this->reactor_.wait_set_.ex_mask_.max_set ();

	  // Compute the maximum of three values.
	  this->max_handlep1_ = rd_max < wr_max ? wr_max : rd_max;

	  if (this->max_handlep1_ < ex_max)
	    this->max_handlep1_ = ex_max;

	  this->max_handlep1_++;
	}
  }
#endif /* ACE_WIN32 */

  return 0;
}

ACE_Reactor_Handler_Repository_Iterator::ACE_Reactor_Handler_Repository_Iterator 
  (const ACE_Reactor_Handler_Repository *s)
  : rep_ (s),
    current_ (-1)
{
  this->advance ();
}

// Pass back the <next_item> that hasn't been seen in the Set.
// Returns 0 when all items have been seen, else 1.

int 
ACE_Reactor_Handler_Repository_Iterator::next (ACE_Event_Handler *&next_item)
{
  int result = 1;

  if (this->current_ >= this->rep_->max_handlep1_)
    result = 0;
  else
    next_item = ACE_REACTOR_EVENT_HANDLER (this->rep_, 
					   this->current_);
  return result;
}

int 
ACE_Reactor_Handler_Repository_Iterator::done (void) const
{
  return this->current_ >= this->rep_->max_handlep1_;
}

// Move forward by one element in the set.

int 
ACE_Reactor_Handler_Repository_Iterator::advance (void)
{
  if (this->current_ < this->rep_->max_handlep1_)
    this->current_++;

  while (this->current_ < this->rep_->max_handlep1_)
    if (ACE_REACTOR_EVENT_HANDLER (this->rep_, this->current_) != 0)
      return 1;
    else
     this->current_++;

  return this->current_ < this->rep_->max_handlep1_;    
}

// Dump the state of an object.

void 
ACE_Reactor_Handler_Repository_Iterator::dump (void) const
{
  ACE_TRACE ("ACE_Reactor_Handler_Repository_Iterator::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, "rep_ = %u", this->rep_));
  ACE_DEBUG ((LM_DEBUG, "current_ = %d", this->current_));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));    
}

void
ACE_Reactor_Handler_Repository::dump (void) const
{
  ACE_TRACE ("ACE_Reactor_Handler_Repository::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, 
              "(%t) max_handlep1_ = %d, max_size_ = %d\n",
              this->max_handlep1_, this->max_size_)); 
  ACE_DEBUG ((LM_DEBUG, "["));

  ACE_Event_Handler *eh = 0;

  for (ACE_Reactor_Handler_Repository_Iterator iter (this);
       iter.next (eh) != 0;
       iter.advance ())
    ACE_DEBUG ((LM_DEBUG, " (eh = %x, eh->handle_ = %d)", 
		eh, eh->get_handle ()));

  ACE_DEBUG ((LM_DEBUG, " ]"));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));    
}

ACE_ALLOC_HOOK_DEFINE(ACE_Reactor_Handler_Repository_Iterator)

int 
ACE_Reactor::any_ready (ACE_Reactor_Handle_Set &wait_set)
{
  ACE_TRACE ("ACE_Reactor::fill_in_ready");

#if !defined (ACE_WIN32)
  // Make this call signal safe.
  ACE_Sig_Guard sb;
#endif /* ACE_WIN32 */

  int number_ready = this->ready_set_.rd_mask_.num_set () 
    + this->ready_set_.wr_mask_.num_set ()
    + this->ready_set_.ex_mask_.num_set ();

  if (number_ready > 0)
    {
      wait_set.rd_mask_ = this->wait_set_.rd_mask_;
      wait_set.wr_mask_ = this->wait_set_.wr_mask_;
      wait_set.ex_mask_ = this->wait_set_.ex_mask_;

      this->ready_set_.rd_mask_.reset ();
      this->ready_set_.wr_mask_.reset ();
      this->ready_set_.ex_mask_.reset ();
    }

  return number_ready;
}

int 
ACE_Reactor::handler_i (int signum, ACE_Event_Handler **eh)
{
  ACE_TRACE ("ACE_Reactor::handler_i");
  ACE_Event_Handler *handler = this->signal_handler_->handler (signum);

  if (handler == 0)
    return -1;
  else if (*eh != 0)
    *eh = handler;
  return 0;
}

int
ACE_Reactor::initialized (void)
{
  ACE_TRACE ("ACE_Reactor::initialized");
  ACE_MT (ACE_GUARD_RETURN (ACE_REACTOR_MUTEX, ace_mon, this->token_, 0));
  return this->initialized_;
}

void
ACE_Reactor::owner (ACE_thread_t tid, ACE_thread_t *o_id)
{
  ACE_TRACE ("ACE_Reactor::owner");
  ACE_MT (ACE_GUARD (ACE_REACTOR_MUTEX, ace_mon, this->token_));

  if (o_id)
    *o_id = this->owner_;

  this->owner_ = tid;
}

int
ACE_Reactor::owner (ACE_thread_t *t_id)
{
  ACE_TRACE ("ACE_Reactor::owner");
  ACE_MT (ACE_GUARD_RETURN (ACE_REACTOR_MUTEX, ace_mon, this->token_, -1));
  *t_id = this->owner_;
  return 0;
}

void 
ACE_Reactor::requeue_position (int rp)
{
  ACE_TRACE ("ACE_Reactor::requeue_position");
  ACE_MT (ACE_GUARD (ACE_REACTOR_MUTEX, ace_mon, this->token_));
#if defined (ACE_WIN32)
  // Must always requeue ourselves "next" on Win32.
  this->requeue_position_ = 0;
#else
  this->requeue_position_ = rp;
#endif /* ACE_WIN32 */
}

int 
ACE_Reactor::requeue_position (void)
{
  ACE_TRACE ("ACE_Reactor::requeue_position");
  ACE_MT (ACE_GUARD_RETURN (ACE_REACTOR_MUTEX, ace_mon, this->token_, -1));
  return this->requeue_position_;
}

void 
ACE_Reactor::max_notify_iterations (int iterations)
{
  ACE_TRACE ("ACE_Reactor::max_notify_iterations");
  ACE_MT (ACE_GUARD (ACE_REACTOR_MUTEX, ace_mon, this->token_));

  // Must always be > 0 or < 0 to optimize the loop exit condition.
  if (iterations == 0)
    iterations = 1;

  this->max_notify_iterations_ = iterations;
}

int 
ACE_Reactor::max_notify_iterations (void)
{
  ACE_TRACE ("ACE_Reactor::max_notify_iterations");
  ACE_MT (ACE_GUARD_RETURN (ACE_REACTOR_MUTEX, ace_mon, this->token_, -1));
  return this->max_notify_iterations_;
}

#if defined (ACE_MT_SAFE)
// Enqueue ourselves into the list of waiting threads.
void 
ACE_Reactor::renew (void)
{
  ACE_TRACE ("ACE_Reactor::renew");
  this->token_.renew (this->requeue_position_);
}

void
ACE_Reactor_Token::dump (void) const
{
  ACE_TRACE ("ACE_Reactor_Token::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, "\n"));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));    
}

ACE_Reactor_Token::ACE_Reactor_Token (ACE_Reactor &r)
  : reactor_ (r)
#if defined (ACE_REACTOR_HAS_DEADLOCK_DETECTION)
    , ACE_Local_Mutex (0) // Triggers unique name by stringifying "this"...
#endif /* ACE_REACTOR_HAS_DEADLOCK_DETECTION */
{
  ACE_TRACE ("ACE_Reactor_Token::ACE_Reactor_Token");
}

// Used to wakeup the Reactor.

void
ACE_Reactor_Token::sleep_hook (void)
{
  ACE_TRACE ("ACE_Reactor_Token::sleep_hook");
  if (this->reactor_.notify () == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "sleep_hook failed"));
}

void
ACE_Reactor_Notify::dump (void) const
{
  ACE_TRACE ("ACE_Reactor_Notify::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, "reactor_ = %x", this->reactor_));
  this->notification_pipe_.dump ();
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));    
}

int
ACE_Reactor_Notify::open (ACE_Reactor *r)
{
  ACE_TRACE ("ACE_Reactor_Notify::open");

  this->reactor_ = r;

  if (this->notification_pipe_.open () == -1)
    return -1;

  // There seems to be a Win32 bug with this...  Set this into
  // non-blocking mode.
  if (ACE::set_flags (this->notification_pipe_.read_handle (), 
		      ACE_NONBLOCK) == -1)
    return -1;
  else 
    return this->reactor_->register_handler 
      (this->notification_pipe_.read_handle (),
       this,
       ACE_Event_Handler::READ_MASK);
}

int
ACE_Reactor_Notify::close (void)
{
  ACE_TRACE ("ACE_Reactor_Notify::close");
  return this->notification_pipe_.close ();
}

ssize_t
ACE_Reactor_Notify::notify (ACE_Event_Handler *eh, 
			    ACE_Reactor_Mask mask, 
			    ACE_Time_Value *timeout)
{
  ACE_TRACE ("ACE_Reactor_Notify::notify");

  ACE_Notification_Buffer buffer (eh, mask);

  ssize_t n = ACE::send (this->notification_pipe_.write_handle (),
			 (char *) &buffer, 
			 sizeof buffer,
			 timeout);
  return n == -1 ? -1 : 0;
}

// Handles pending threads (if any) that are waiting to unblock the
// Reactor.

int
ACE_Reactor_Notify::dispatch_notifications (int &number_of_active_handles,
					    const ACE_Handle_Set &rd_mask)
{
  ACE_TRACE ("ACE_Reactor_Notify::handle_notification");

  ACE_HANDLE read_handle = 
    this->notification_pipe_.read_handle ();

  if (rd_mask.is_set (read_handle))
    {
      number_of_active_handles--;
      return this->handle_input (read_handle);
    }
  else
    return 0;
}

// Special trick to unblock select() when updates occur in somewhere
// other than the main ACE_Reactor thread.  All we do is write data to
// a pipe that the ACE_Reactor is listening on.  Thanks to Paul
// Stephenson for suggesting this approach.

int
ACE_Reactor_Notify::handle_input (ACE_HANDLE handle)
{
  ACE_TRACE ("ACE_Reactor_Notify::handle_input");
  // Precondition: this->reactor_.token_.current_owner () ==
  // ACE_Thread::self ();

  ACE_Notification_Buffer buffer;
  ssize_t n;
  int number_dispatched = 0;

  while ((n = ACE::recv (handle, (char *) &buffer, sizeof buffer)) > 0)
    {
      // Check to see if we've got a short read.
      if (n != sizeof buffer)      
	{
	  ssize_t remainder = sizeof buffer - n;

	  // If so, try to recover by reading the remainder.  If this doesn't
	  // work we're in big trouble since the input stream won't be aligned
	  // correctly.  I'm not sure quite what to do at this point.  It's
	  // probably best just to return -1.
	  if (ACE::recv (handle, ((char *) &buffer) + n, remainder) != remainder)
	    return -1;
	}

      // If eh == 0 then another thread is unblocking the ACE_Reactor
      // to update the ACE_Reactor's internal structures.  Otherwise,
      // we need to dispatch the appropriate handle_* method on the
      // ACE_Event_Handler pointer we've been passed.
      if (buffer.eh_ != 0)
        {
          int result = 0;

          switch (buffer.mask_)
            {
            case ACE_Event_Handler::READ_MASK:
            case ACE_Event_Handler::ACCEPT_MASK:
              result = buffer.eh_->handle_input (ACE_INVALID_HANDLE);
              break;
            case ACE_Event_Handler::WRITE_MASK:
              result = buffer.eh_->handle_output (ACE_INVALID_HANDLE);
              break;
            case ACE_Event_Handler::EXCEPT_MASK:
              result = buffer.eh_->handle_exception (ACE_INVALID_HANDLE);
              break;
            default:
	      // Should we bail out if we get an invalid mask?
              ACE_ERROR ((LM_ERROR, "invalid mask = %d\n", buffer.mask_));
            }
          if (result == -1)
            buffer.eh_->handle_close (ACE_INVALID_HANDLE, 
				      ACE_Event_Handler::EXCEPT_MASK);
        }

      number_dispatched++;

      // Bail out if we've reached the <notify_threshold_>.  Note that
      // by default <notify_threshold_> is -1, so we'll loop until all
      // the notifications in the pipe have been dispatched.
      if (number_dispatched == this->reactor_->max_notify_iterations_)
	break;
    }

  // Reassign number_dispatched to -1 if things have gone seriously
  // wrong.
  if (n <= 0 && errno != EWOULDBLOCK)
    number_dispatched = -1;

  // Enqueue ourselves into the list of waiting threads.  When we
  // reacquire the token we'll be off and running again with ownership
  // of the token.  The postcondition of this call is that
  // this->reactor_.token_.current_owner () == ACE_Thread::self ();
  this->reactor_->renew ();
  return number_dispatched;
}
#endif /* ACE_MT_SAFE */

int
ACE_Reactor::notify (ACE_Event_Handler *eh, 
                     ACE_Reactor_Mask mask,
		     ACE_Time_Value *timeout)
{
  ACE_TRACE ("ACE_Reactor::notify");

  ssize_t n = 0;

#if defined (ACE_MT_SAFE)
  // Pass over both the Event_Handler *and* the mask to allow the
  // caller to dictate which Event_Handler method the receiver
  // invokes.  Note that this call can timeout.

  n = this->notify_handler_.notify (eh, mask, timeout);
#else
  ACE_UNUSED_ARG (eh);
  ACE_UNUSED_ARG (mask);
  ACE_UNUSED_ARG (timeout);
#endif /* ACE_MT_SAFE */
  return n == -1 ? -1 : 0;
}

int
ACE_Reactor::resume_handler (ACE_HANDLE handle)
{
  ACE_TRACE ("ACE_Reactor::resume_handler");
  ACE_MT (ACE_GUARD_RETURN (ACE_REACTOR_MUTEX, ace_mon, this->token_, -1));
  return this->resume_i (handle);
}

int
ACE_Reactor::suspend_handler (ACE_HANDLE handle)
{
  ACE_TRACE ("ACE_Reactor::suspend_handler");
  ACE_MT (ACE_GUARD_RETURN (ACE_REACTOR_MUTEX, ace_mon, this->token_, -1));
  return this->suspend_i (handle);
}

int
ACE_Reactor::suspend_handlers (void)
{
  ACE_TRACE ("ACE_Reactor::suspend_handlers");
  ACE_MT (ACE_GUARD_RETURN (ACE_REACTOR_MUTEX, ace_mon, this->token_, -1));

  ACE_Event_Handler *eh = 0;

  for (ACE_Reactor_Handler_Repository_Iterator iter (&this->handler_rep_);
       iter.next (eh) != 0;
       iter.advance ())
    this->suspend_i (eh->get_handle ());

  return 0;
}

int
ACE_Reactor::resume_handlers (void)
{
  ACE_TRACE ("ACE_Reactor::resume_handlers");
  ACE_MT (ACE_GUARD_RETURN (ACE_REACTOR_MUTEX, ace_mon, this->token_, -1));

  ACE_Event_Handler *eh = 0;

  for (ACE_Reactor_Handler_Repository_Iterator iter (&this->handler_rep_);
       iter.next (eh) != 0;
       iter.advance ())
    this->resume_i (eh->get_handle ());

  return 0;
}

int
ACE_Reactor::register_handler (ACE_Event_Handler *handler, 
                               ACE_Reactor_Mask mask)
{
  ACE_TRACE ("ACE_Reactor::register_handler");
  ACE_MT (ACE_GUARD_RETURN (ACE_REACTOR_MUTEX, ace_mon, this->token_, -1));
  return this->register_handler_i (handler->get_handle (), handler, mask);
}

int
ACE_Reactor::register_handler (ACE_HANDLE handle, 
			       ACE_Event_Handler *handler, 
			       ACE_Reactor_Mask mask)
{
  ACE_TRACE ("ACE_Reactor::register_handler");
  ACE_MT (ACE_GUARD_RETURN (ACE_REACTOR_MUTEX, ace_mon, this->token_, -1));
  return this->register_handler_i (handle, handler, mask);
}

int
ACE_Reactor::register_handler (const ACE_Handle_Set &handles, 
			       ACE_Event_Handler *handler, 
			       ACE_Reactor_Mask mask)
{
  ACE_TRACE ("ACE_Reactor::register_handler");
  ACE_MT (ACE_GUARD_RETURN (ACE_REACTOR_MUTEX, ace_mon, this->token_, -1));
  return this->register_handler_i (handles, handler, mask);
}

int
ACE_Reactor::handler (ACE_HANDLE handle, 
		      ACE_Reactor_Mask mask, 
		      ACE_Event_Handler **handler)
{
  ACE_TRACE ("ACE_Reactor::handler");
  ACE_MT (ACE_GUARD_RETURN (ACE_REACTOR_MUTEX, ace_mon, this->token_, -1));
  return this->handler_i (handle, mask, handler);
}

int
ACE_Reactor::remove_handler (const ACE_Handle_Set &handles, 
                             ACE_Reactor_Mask mask)
{
  ACE_TRACE ("ACE_Reactor::remove_handler");
  ACE_MT (ACE_GUARD_RETURN (ACE_REACTOR_MUTEX, ace_mon, this->token_, -1));
  return this->remove_handler_i (handles, mask);
}

int
ACE_Reactor::remove_handler (ACE_Event_Handler *handler, 
                             ACE_Reactor_Mask mask)
{
  ACE_TRACE ("ACE_Reactor::remove_handler");
  ACE_MT (ACE_GUARD_RETURN (ACE_REACTOR_MUTEX, ace_mon, this->token_, -1));
  return this->remove_handler_i (handler->get_handle (), mask);
}

int
ACE_Reactor::remove_handler (ACE_HANDLE handle, 
			     ACE_Reactor_Mask mask)
{
  ACE_TRACE ("ACE_Reactor::remove_handler");
  ACE_MT (ACE_GUARD_RETURN (ACE_REACTOR_MUTEX, ace_mon, this->token_, -1));
  return this->remove_handler_i (handle, mask);
}

// Performs operations on the "ready" bits.

int
ACE_Reactor::ready_ops (ACE_HANDLE handle, 
			ACE_Reactor_Mask mask, 
			int ops)
{
  ACE_TRACE ("ACE_Reactor::ready_ops");
  ACE_MT (ACE_GUARD_RETURN (ACE_REACTOR_MUTEX, ace_mon, this->token_, -1));
  return this->bit_ops (handle, 
			mask,
			this->ready_set_,
                        ops);
}

// Initialize the ACE_Reactor

int
ACE_Reactor::open (size_t size, 
		   int restart, 
		   ACE_Sig_Handler *sh,
		   ACE_Timer_Queue *tq)
{
  ACE_TRACE ("ACE_Reactor::open");
  ACE_MT (ACE_GUARD_RETURN (ACE_REACTOR_MUTEX, ace_mon, this->token_, -1));

  // Can't initialize ourselves more than once.
  if (this->initialized_ > 0)
    return -1;

  this->owner_ = ACE_Thread::self ();
  this->restart_ = restart;
  this->signal_handler_ = sh;
  this->timer_queue_ = tq;

  int result = 0;

  // Allows the signal handler to be overridden.
  if (this->signal_handler_ == 0)
    {
      this->signal_handler_ = new ACE_Sig_Handler;
      
      if (this->signal_handler_ == 0)
	result = -1;
      else
	this->delete_signal_handler_ = 1;
    }

  if (result != -1 && this->timer_queue_ == 0)
    {
      this->timer_queue_ = new ACE_Timer_List;

      if (this->timer_queue_ == 0)
	result = -1;
      else
	this->delete_timer_queue_ = 1;
    }

  if (result != -1 && this->handler_rep_.open (size) == -1)
    result = -1;
#if defined (ACE_MT_SAFE)
  else if (this->notify_handler_.open (this) == -1)
    result = -1;
#endif /* ACE_MT_SAFE */

  if (result != -1)
    // We're all set to go.
    this->initialized_ = 1;
  else
    // This will close down all the allocated resources properly.
    this->close ();

  return result;
}

ACE_Reactor::ACE_Reactor (ACE_Sig_Handler *sh,
			  ACE_Timer_Queue *tq)
  : handler_rep_ (*this),
    timer_queue_ (0),
    delete_timer_queue_ (0),
    delete_signal_handler_ (0),
    requeue_position_ (-1), // Requeue at end of waiters by default.
    max_notify_iterations_ (-1),
    initialized_ (0),
    state_changed_ (0)
#if defined (ACE_MT_SAFE)
    , token_ (*this)
#endif /* ACE_MT_SAFE */
{
  ACE_TRACE ("ACE_Reactor::ACE_Reactor");

  if (this->open (ACE_Reactor::DEFAULT_SIZE, 0, sh, tq))
    ACE_ERROR ((LM_ERROR, "%p\n", "open failed"));
}

// Initialize ACE_Reactor.

ACE_Reactor::ACE_Reactor (size_t size, 
			  int rs, 
			  ACE_Sig_Handler *sh,
			  ACE_Timer_Queue *tq)
  : handler_rep_ (*this),
    timer_queue_ (0),
    delete_timer_queue_ (0),
    delete_signal_handler_ (0),
    requeue_position_ (-1), // Requeue at end of waiters by default.
    max_notify_iterations_ (-1),
    initialized_ (0),
    state_changed_ (0)
#if defined (ACE_MT_SAFE)
    , token_ (*this)
#endif /* ACE_MT_SAFE */
{
  ACE_TRACE ("ACE_Reactor::ACE_Reactor");

  if (this->open (size, rs, sh, tq) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "open failed"));
}

// Close down the ACE_Reactor instance, detaching any remaining
// Event_Handers.  This had better be called from the main event loop
// thread...

void
ACE_Reactor::close (void)
{
  ACE_TRACE ("ACE_Reactor::close");
  ACE_MT (ACE_GUARD (ACE_REACTOR_MUTEX, ace_mon, this->token_));

  if (this->delete_signal_handler_)
    delete this->signal_handler_;
  this->signal_handler_ = 0;

  this->handler_rep_.close ();

  if (this->delete_timer_queue_)
    delete this->timer_queue_;
  this->timer_queue_ = 0;

#if defined (ACE_MT_SAFE)
  this->notify_handler_.close ();
#endif /* ACE_MT_SAFE */
  this->initialized_ = 0;
}
  
ACE_Reactor::~ACE_Reactor (void)
{
  ACE_TRACE ("ACE_Reactor::~ACE_Reactor");
  this->close ();
}

int
ACE_Reactor::remove_handler_i (const ACE_Handle_Set &handles,
			       ACE_Reactor_Mask mask)
{
  ACE_TRACE ("ACE_Reactor::remove_handler_i");
  ACE_HANDLE h;

  ACE_Handle_Set_Iterator handle_iter (handles);

  while ((h = handle_iter ()) != ACE_INVALID_HANDLE)
    if (this->remove_handler_i (h, mask) == -1)
      return -1;

  return 0;    
}

int
ACE_Reactor::register_handler_i (const ACE_Handle_Set &handles, 
				 ACE_Event_Handler *handler, 
				 ACE_Reactor_Mask mask)
{
  ACE_TRACE ("ACE_Reactor::register_handler_i");
  ACE_HANDLE h;

  ACE_Handle_Set_Iterator handle_iter (handles);
  while ((h = handle_iter ()) != ACE_INVALID_HANDLE)
    if (this->register_handler_i (h, handler, mask) == -1)
      return -1;

  return 0;    
}

int
ACE_Reactor::register_handler (const ACE_Sig_Set &sigset, 
                               ACE_Event_Handler *new_sh,
                               ACE_Sig_Action *new_disp)
{
  ACE_TRACE ("ACE_Reactor::register_handler");

  int result = 0;

#if (NSIG > 0)

  for (int s = 1; s < NSIG; s++)
    if (sigset.is_member (s) 
        && this->signal_handler_->register_handler (s, new_sh, 
						    new_disp) == -1) 
      result = -1;
#endif /* NSIG */
  return result;
}

int
ACE_Reactor::remove_handler (const ACE_Sig_Set &sigset)
{
  ACE_TRACE ("ACE_Reactor::remove_handler");
  int result = 0;

#if (NSIG == 0)
  for (int s = 1; s < NSIG; s++)
    if (sigset.is_member (s) 
        && this->signal_handler_->remove_handler (s) == -1)
      result = -1;
#else
  ACE_UNUSED_ARG (sigset);
#endif /* NSIG */

  return result;    
}

// Note the queue handles its own locking. 

long
ACE_Reactor::schedule_timer (ACE_Event_Handler *handler, 
                             const void *arg,
                             const ACE_Time_Value &delta_time, 
                             const ACE_Time_Value &interval)
{
  ACE_TRACE ("ACE_Reactor::schedule_timer");
  ACE_MT (ACE_GUARD_RETURN (ACE_REACTOR_MUTEX, ace_mon, this->token_, -1));

  return this->timer_queue_->schedule 
    (handler, arg, timer_queue_->gettimeofday () + delta_time, interval);
}

// Main event loop driver that blocks for <max_wait_time> before
// returning (will return earlier if I/O or signal events occur).

int 
ACE_Reactor::handle_events (ACE_Time_Value &max_wait_time)
{
  ACE_TRACE ("ACE_Reactor::handle_events");

  return this->handle_events (&max_wait_time);
}

int
ACE_Reactor::handle_error (void)
{
  ACE_TRACE ("ACE_Reactor::handle_error");
  if (errno == EINTR)
    return this->restart_;
  else if (errno == EBADF)
    return this->check_handles ();
  else
    return -1;
}

void
ACE_Reactor::notify_handle (ACE_HANDLE handle, 
                            ACE_Reactor_Mask mask, 
                            ACE_Handle_Set &ready_mask,
                            ACE_Event_Handler *event_handler,
                            ACE_EH_PTMF ptmf)
{
  ACE_TRACE ("ACE_Reactor::notify_handle");
  // Check for removed handlers.
  if (event_handler == 0)
    return;

  int status = (event_handler->*ptmf) (handle);

  if (status < 0)
    this->remove_handler_i (handle, mask);
  else if (status > 0)
    ready_mask.set_bit (handle);
}

// Perform GET, CLR, SET, and ADD operations on the Handle_Sets.
//
// GET = 1, Retrieve current value
// SET = 2, Set value of bits to new mask (changes the entire mask)
// ADD = 3, Bitwise "or" the value into the mask (only changes
//          enabled bits) 
// CLR = 4  Bitwise "and" the negation of the value out of the mask
//          (only changes enabled bits) 
// 
// Returns the original mask.  Must be called with locks held.

int
ACE_Reactor::bit_ops (ACE_HANDLE handle,
                      ACE_Reactor_Mask mask,
		      ACE_Reactor_Handle_Set &handle_set,
                      int ops)
{
  ACE_TRACE ("ACE_Reactor::bit_ops");
  if (this->handler_rep_.handle_in_range (handle) == 0)
    return -1;

#if !defined (ACE_WIN32)
  ACE_Sig_Guard sb; // Block out all signals until method returns. 
#endif /* ACE_WIN32 */

  ACE_FDS_PTMF ptmf  = &ACE_Handle_Set::set_bit;
  u_long omask = ACE_Event_Handler::NULL_MASK;

  switch (ops)
    {
    case ACE_Reactor::GET_MASK:
      if (handle_set.rd_mask_.is_set (handle))
        ACE_SET_BITS (omask, ACE_Event_Handler::READ_MASK);
      if (handle_set.wr_mask_.is_set (handle))
        ACE_SET_BITS (omask, ACE_Event_Handler::WRITE_MASK);
      if (handle_set.ex_mask_.is_set (handle))
        ACE_SET_BITS (omask, ACE_Event_Handler::EXCEPT_MASK);
      break;

    case ACE_Reactor::CLR_MASK:
      ptmf = &ACE_Handle_Set::clr_bit;
      /* FALLTHRU */
    case ACE_Reactor::SET_MASK:
      /* FALLTHRU */
    case ACE_Reactor::ADD_MASK:

      // The following code is rather subtle...  Note that if we are
      // doing a ACE_Reactor::SET_MASK then if the bit is not enabled
      // in the mask we need to clear the bit from the ACE_Handle_Set.
      // On the other hand, if we are doing a ACE_Reactor::CLR_MASK or
      // a ACE_Reactor::ADD_MASK we just carry out the operations
      // specified by the mask.

      if (ACE_BIT_ENABLED (mask, ACE_Event_Handler::READ_MASK)
	   || ACE_BIT_ENABLED (mask, ACE_Event_Handler::ACCEPT_MASK))
        {
          (handle_set.rd_mask_.*ptmf) (handle);
          ACE_SET_BITS (omask, ACE_Event_Handler::READ_MASK);
        }
      else if (ops == ACE_Reactor::SET_MASK)
        handle_set.rd_mask_.clr_bit (handle);

      if (ACE_BIT_ENABLED (mask, ACE_Event_Handler::WRITE_MASK))
        {
          (handle_set.wr_mask_.*ptmf) (handle);
          ACE_SET_BITS (omask, ACE_Event_Handler::WRITE_MASK);
        }
      else if (ops == ACE_Reactor::SET_MASK)
        handle_set.wr_mask_.clr_bit (handle);

      if (ACE_BIT_ENABLED (mask, ACE_Event_Handler::EXCEPT_MASK))
        {
          (handle_set.ex_mask_.*ptmf) (handle);
          ACE_SET_BITS (omask, ACE_Event_Handler::EXCEPT_MASK);
        }
      else if (ops == ACE_Reactor::SET_MASK)
        handle_set.ex_mask_.clr_bit (handle);
      break;
    default: 
      return -1;
    }
  return omask;
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

int
ACE_Reactor::mask_ops (ACE_HANDLE handle, 
		       ACE_Reactor_Mask mask, 
		       int ops)
{
  ACE_TRACE ("ACE_Reactor::mask_ops");
  ACE_MT (ACE_GUARD_RETURN (ACE_REACTOR_MUTEX, ace_mon, this->token_, -1));
  return this->bit_ops (handle, mask, 
                        this->wait_set_,
                        ops);
}

// Must be called with locks held.

int
ACE_Reactor::handler_i (ACE_HANDLE handle, 
			ACE_Reactor_Mask mask, 
                        ACE_Event_Handler **handler) 
{
  ACE_TRACE ("ACE_Reactor::handler_i");
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

int
ACE_Reactor::resume_i (ACE_HANDLE handle)
{
  ACE_TRACE ("ACE_Reactor::resume");
  if (this->handler_rep_.find (handle) == 0)
    return -1;
  
  this->wait_set_.rd_mask_.set_bit (handle);
  this->wait_set_.wr_mask_.set_bit (handle);
  this->wait_set_.ex_mask_.set_bit (handle);
  return 0;
}

// Must be called with locks held

int
ACE_Reactor::suspend_i (ACE_HANDLE handle)
{
  ACE_TRACE ("ACE_Reactor::suspend");
  if (this->handler_rep_.find (handle) == 0)
    return -1;

  this->wait_set_.rd_mask_.clr_bit (handle);
  this->wait_set_.wr_mask_.clr_bit (handle);
  this->wait_set_.ex_mask_.clr_bit (handle);
  return 0;
}

// Must be called with locks held

int
ACE_Reactor::register_handler_i (ACE_HANDLE handle, 
				 ACE_Event_Handler *event_handler,
				 ACE_Reactor_Mask mask)
{
  ACE_TRACE ("ACE_Reactor::register_handler_i");

  // Insert the <handle, event_handle> tuple into the Handler
  // Repository.
  return this->handler_rep_.bind (handle, event_handler, mask);
}

int
ACE_Reactor::remove_handler_i (ACE_HANDLE handle, 
			       ACE_Reactor_Mask mask)
{
  ACE_TRACE ("ACE_Reactor::remove_handler_i");

  // Unbind this handle.
  return this->handler_rep_.unbind (handle, mask);
}

// Must be called with lock held.

int 
ACE_Reactor::wait_for_multiple_events (ACE_Reactor_Handle_Set &dispatch_set,
                                       ACE_Time_Value *max_wait_time)
{
  ACE_TRACE ("ACE_Reactor::wait_for_multiple_events");
  u_long width = 0;
  ACE_Time_Value timer_buf (0) ;
  ACE_Time_Value *this_timeout = &timer_buf ;

  int number_of_active_handles = this->any_ready (dispatch_set);

  // If there are any bits enabled in the <ready_set_> then we'll
  // handle those first, otherwise we'll block in select().

  if (number_of_active_handles == 0)
    {
      do
	{
	  if (this->timer_queue_->calculate_timeout (max_wait_time,
						     this_timeout) == 0) {
	    this_timeout = 0 ;
	  }
      
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
	  dispatch_set.rd_mask_.sync (this->handler_rep_.max_handlep1 ());
	  dispatch_set.wr_mask_.sync (this->handler_rep_.max_handlep1 ());
	  dispatch_set.ex_mask_.sync (this->handler_rep_.max_handlep1 ());
#endif /* ACE_WIN32 */
	}

    }

  // Return the number of events to dispatch.
  return number_of_active_handles; 
}

int
ACE_Reactor::dispatch_timer_handlers (void)
{
  int number_dispatched = this->timer_queue_->expire ();
  return this->state_changed_ ? -1 : number_dispatched;
}

int
ACE_Reactor::dispatch_notification_handlers (int &number_of_active_handles,
					     ACE_Reactor_Handle_Set &dispatch_set)
{
#if defined (ACE_MT_SAFE)
      // Check to see if the ACE_HANDLE associated with the Reactor's
      // notify hook is enabled.  If so, it means that one or more
      // other threads are trying to update the ACE_Reactor's internal
      // tables.  We'll handle all these threads and then break out to
      // continue the event loop.
  
  int number_dispatched = 
    this->notify_handler_.dispatch_notifications (number_of_active_handles, 
						  dispatch_set.rd_mask_);
  return this->state_changed_ ? -1 : number_dispatched;
#else
  ACE_UNUSED_ARG (number_of_active_handles);
  ACE_UNUSED_ARG (dispatch_set);
  return 0;
#endif /* ACE_MT_SAFE */
}

int
ACE_Reactor::dispatch_io_handlers (int &number_of_active_handles,
				   ACE_Reactor_Handle_Set &dispatch_set)
{
  int number_dispatched = 0;

  if (number_of_active_handles > 0)
    {
      ACE_HANDLE handle;

      // Handle output events (this code needs to come first to handle
      // the obscure case of piggy-backed data coming along with the
      // final handshake message of a nonblocking connection).

      ACE_Handle_Set_Iterator handle_iter_wr (dispatch_set.wr_mask_);

      while ((handle = handle_iter_wr ()) != ACE_INVALID_HANDLE
	     && number_dispatched < number_of_active_handles
	     && this->state_changed_ == 0)
	{
	  number_dispatched++;
	  this->notify_handle (handle,
			       ACE_Event_Handler::WRITE_MASK,
			       this->ready_set_.wr_mask_, 
			       this->handler_rep_.find (handle),
			       &ACE_Event_Handler::handle_output); 
	}
    }

  if (number_dispatched > 0)
    {
      number_of_active_handles -= number_dispatched;
      if (this->state_changed_)
	return -1;
    }

  number_dispatched = 0;

  if (number_of_active_handles > 0)
    {
      ACE_HANDLE handle;

      // Handle "exceptional" events.

      ACE_Handle_Set_Iterator handle_iter_ex (dispatch_set.ex_mask_);

      while ((handle = handle_iter_ex ()) != ACE_INVALID_HANDLE 
	     && number_dispatched < number_of_active_handles
	     && this->state_changed_ == 0)
	{
	  this->notify_handle (handle,
			       ACE_Event_Handler::EXCEPT_MASK,
			       this->ready_set_.ex_mask_, 
			       this->handler_rep_.find (handle),
			       &ACE_Event_Handler::handle_exception); 
	  number_dispatched++;
	}
    }

  if (number_dispatched > 0)
    {
      number_of_active_handles -= number_dispatched;
      if (this->state_changed_)
	return -1;
    }

  number_dispatched = 0;

  if (number_of_active_handles > 0)
    {
      ACE_HANDLE handle;

      // Handle input, passive connection, and shutdown events.

      ACE_Handle_Set_Iterator handle_iter_rd (dispatch_set.rd_mask_);

      while ((handle = handle_iter_rd ()) != ACE_INVALID_HANDLE 
	     && number_dispatched < number_of_active_handles
	     && this->state_changed_ == 0)
	{
	  this->notify_handle (handle,
			       ACE_Event_Handler::READ_MASK,
			       this->ready_set_.rd_mask_, 
			       this->handler_rep_.find (handle),
			       &ACE_Event_Handler::handle_input); 
	  number_dispatched++;
	}
    }

  if (number_dispatched > 0)
    {
      number_of_active_handles -= number_dispatched;

      if (this->state_changed_)
	return -1;
    }

  return number_dispatched;
}

int
ACE_Reactor::dispatch (int number_of_active_handles, 
                       ACE_Reactor_Handle_Set &dispatch_set)
{
  ACE_TRACE ("ACE_Reactor::dispatch");

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

      // Handle timers first since they may have higher latency
      // constraints.

      if (this->dispatch_timer_handlers () == -1)
	// State has changed or timer queue has failed, exit inner
	// loop.
	break; 
      else if (number_of_active_handles <= 0)
	// Bail out since we got here since select() was interrupted.
	{
	  if (ACE_Sig_Handler::sig_pending () != 0)
	    {
	      ACE_Sig_Handler::sig_pending (0);
	      
	      // If any HANDLES in the <ready_set_> are activated as a
	      // result of signals they should be dispatched since
	      // they may be time critical...
	      number_of_active_handles = this->any_ready (dispatch_set);
	    }
	  else
	    return number_of_active_handles;
	}
      else if (this->dispatch_notification_handlers 
	       (number_of_active_handles, dispatch_set) == -1)
	break; // State has changed, exit inner loop.
      else if (this->dispatch_io_handlers 
	       (number_of_active_handles, dispatch_set) == -1)
	// State has changed, so exit the inner loop.
	break; 

    }
  while (number_of_active_handles > 0);

  return 1;
}

int
ACE_Reactor::release_token (void)
{
#if defined (ACE_WIN32)
  this->token_.release ();
  return (int) EXCEPTION_CONTINUE_SEARCH;
#else
  return 0;
#endif /* ACE_WIN32 */
}

int 
ACE_Reactor::handle_events (ACE_Time_Value *max_wait_time)
{
  ACE_TRACE ("ACE_Reactor::handle_events");

  // Stash the current time -- the destructor of this object will
  // automatically compute how much time elpased since this method was
  // called.
  ACE_Countdown_Time countdown (max_wait_time);

#if defined (ACE_MT_SAFE)
  ACE_GUARD_RETURN (ACE_REACTOR_MUTEX, ace_mon, this->token_, -1);

  if (ACE_OS::thr_equal (ACE_Thread::self (), this->owner_) == 0)
    return -1;

  // Update the countdown to reflect time waiting for the mutex.
  countdown.update ();
#endif /* ACE_MT_SAFE */

  return this->handle_events_i (max_wait_time);
}

int 
ACE_Reactor::handle_events_i (ACE_Time_Value *max_wait_time)
{
  int result;

  ACE_SEH_TRY {
    ACE_Reactor_Handle_Set dispatch_set;

    int number_of_active_handles = 
      this->wait_for_multiple_events (dispatch_set, 
				      max_wait_time); 

    result = this->dispatch (number_of_active_handles, dispatch_set);
  }
  ACE_SEH_EXCEPT (this->release_token ()) {
    // As it stands now, we catch and then rethrow all Win32
    // structured exceptions so that we can make sure to release the
    // <token_> lock correctly.
  }

  return result;
}

int
ACE_Reactor::check_handles (void)
{
  ACE_TRACE ("ACE_Reactor::check_handles");

  ACE_Time_Value time_poll = ACE_Time_Value::zero;
  ACE_Handle_Set rd_mask;

  ACE_Event_Handler *eh = 0;
  int result = 0;

  for (ACE_Reactor_Handler_Repository_Iterator iter (&this->handler_rep_);
       iter.next (eh) != 0;
       iter.advance ())
    {
      ACE_HANDLE handle = eh->get_handle ();

      // Skip back to the beginning of the loop if the HANDLE is
      // invalid.
      if (handle == ACE_INVALID_HANDLE)
      	continue;

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
    }

  return result;
}

void
ACE_Reactor::dump (void) const
{
  ACE_TRACE ("ACE_Reactor::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));

  this->timer_queue_->dump ();
  this->handler_rep_.dump ();
  this->signal_handler_->dump ();
  ACE_DEBUG ((LM_DEBUG, 
	      "delete_signal_handler_ = %d\n", 
	      this->delete_signal_handler_));

  ACE_HANDLE h;

  for (ACE_Handle_Set_Iterator handle_iter_wr (this->wait_set_.wr_mask_);
       (h = handle_iter_wr ()) != ACE_INVALID_HANDLE;
       ++handle_iter_wr)
    ACE_DEBUG ((LM_DEBUG, "write_handle = %d\n", h));

  for (ACE_Handle_Set_Iterator handle_iter_rd (this->wait_set_.rd_mask_);
       (h = handle_iter_rd ()) != ACE_INVALID_HANDLE;
       ++handle_iter_rd)
    ACE_DEBUG ((LM_DEBUG, "read_handle = %d\n", h));

  for (ACE_Handle_Set_Iterator handle_iter_ex (this->wait_set_.ex_mask_);
       (h = handle_iter_ex ()) != ACE_INVALID_HANDLE;
       ++handle_iter_ex)
    ACE_DEBUG ((LM_DEBUG, "except_handle = %d\n", h));

  for (ACE_Handle_Set_Iterator handle_iter_wr_ready (this->ready_set_.wr_mask_);
       (h = handle_iter_wr_ready ()) != ACE_INVALID_HANDLE;
       ++handle_iter_wr_ready)
    ACE_DEBUG ((LM_DEBUG, "write_handle_ready = %d\n", h));

  for (ACE_Handle_Set_Iterator handle_iter_rd_ready (this->ready_set_.rd_mask_);
       (h = handle_iter_rd_ready ()) != ACE_INVALID_HANDLE;
       ++handle_iter_rd_ready)
    ACE_DEBUG ((LM_DEBUG, "read_handle_ready = %d\n", h));

  for (ACE_Handle_Set_Iterator handle_iter_ex_ready (this->ready_set_.ex_mask_);
       (h = handle_iter_ex_ready ()) != ACE_INVALID_HANDLE;
       ++handle_iter_ex_ready)
    ACE_DEBUG ((LM_DEBUG, "except_handle_ready = %d\n", h));

  ACE_DEBUG ((LM_DEBUG, "restart_ = %d\n", this->restart_));
  ACE_DEBUG ((LM_DEBUG, "\nrequeue_position_ = %d\n", this->requeue_position_));
  ACE_DEBUG ((LM_DEBUG, "\ninitialized_ = %d\n", this->initialized_));
  ACE_DEBUG ((LM_DEBUG, "\nowner_ = %d\n", this->owner_));

#if defined (ACE_MT_SAFE)
  this->notify_handler_.dump ();
  this->token_.dump ();
#endif /* ACE_MT_SAFE */

  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));    
}

#if defined (ACE_TEMPLATES_REQUIRE_SPECIALIZATION)
ACE_MT (template class ACE_Guard<ACE_REACTOR_MUTEX>);
template class ACE_Event_Handler_Handle_Timeout_Upcall<ACE_SYNCH_RECURSIVE_MUTEX>;
#endif /* ACE_TEMPLATES_REQUIRE_SPECIALIZATION */
