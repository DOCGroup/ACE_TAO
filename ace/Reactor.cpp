// Reactor.cpp
// $Id$

#define ACE_BUILD_DLL
#include "ace/Synch_T.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/SOCK_Connector.h"
#include "ace/Reactor.h"

ACE_ALLOC_HOOK_DEFINE(ACE_Reactor)

// Performs sanity checking on the ACE_HANDLE.

int
ACE_Handler_Repository::invalid_handle (ACE_HANDLE handle)
{
  ACE_TRACE ("ACE_Handler_Repository::invalid_handle");
#if defined (ACE_WIN32)
  return handle == ACE_INVALID_HANDLE;
#else /* !ACE_WIN32 */
  return handle < 0 || handle >= this->max_size_;
#endif /* ACE_WIN32 */
}

size_t
ACE_Handler_Repository::max_handlep1 (void)
{
  ACE_TRACE ("ACE_Handler_Repository::max_handlep1");

#if defined (ACE_WIN32)
  return this->cur_size_;
#else
  return this->max_handlep1_;
#endif /* ACE_WIN32 */
}

int
ACE_Handler_Repository::open (size_t size)
{
  ACE_TRACE ("ACE_Handler_Repository::open");
  this->max_size_ = size;

#if defined (ACE_WIN32)
  this->cur_size_ = 0;

  // Try to allocate the memory. 
  ACE_NEW_RETURN (this->event_handlers_, ACE_NT_EH_Record[size], -1);

  // Initialize the ACE_Event_Handler * to ACE_INVALID_HANDLE;
  for (size_t h = 0; h < size; h++)
    {
      this->event_handlers_[h].handle_ = ACE_INVALID_HANDLE;
      this->event_handlers_[h].event_handler_ = 0;
    }
#else
  this->max_handlep1_ = 0;

  // Try to allocate the memory. 
  ACE_NEW_RETURN (this->event_handlers_, ACE_Event_Handler *[size], -1);

  // Initialize the ACE_Event_Handler * to NULL.
  for (size_t h = 0; h < size; h++)
    this->event_handlers_[h] = 0;

#endif /* ACE_WIN32 */
  return 0;
}

// Initialize a repository of the appropriate <size>.

ACE_Handler_Repository::ACE_Handler_Repository (void)
  : max_size_ (0),    
#if defined (ACE_WIN32)
    cur_size_ (0),
#else
    max_handlep1_ (ACE_INVALID_HANDLE),
#endif /* ACE_WIN32 */
  event_handlers_ (0)
{
  ACE_TRACE ("ACE_Handler_Repository::ACE_Handler_Repository");
}

int
ACE_Handler_Repository::close (ACE_Reactor *reactor)
{
  ACE_TRACE ("ACE_Handler_Repository::close");

  if (this->event_handlers_ != 0)
    {
#if defined(ACE_WIN32)
      for (ssize_t i = 0; 
           i < this->cur_size_; 
           i++)
        reactor->detach (this->event_handlers_[i].handle_,
			 ACE_Event_Handler::RWE_MASK);

      delete [] this->event_handlers_;
      this->event_handlers_ = 0;
#else
      for (ACE_HANDLE h = 0; 
           h < this->max_handlep1_; 
           h++)
        reactor->detach (h, ACE_Event_Handler::RWE_MASK);

      delete [] this->event_handlers_;
      this->event_handlers_ = 0;
#endif /* !ACE_WIN32 */
    }
  return 0;
}

// Return the <ACE_Event_Handler *> associated with <ACE_HANDLE>.

ACE_Event_Handler *
ACE_Handler_Repository::find (ACE_HANDLE handle)
{
  ACE_TRACE ("ACE_Handler_Repository::find");
#if defined (ACE_WIN32)
  for (ssize_t i = 0; i < this->cur_size_; i++)
    if (this->event_handlers_[i].handle_ == handle)
      return this->event_handlers_[i].event_handler_;

  return 0;
#else
  if (handle < 0 || handle >= this->max_handlep1_)
    return 0;
  else
    return this->event_handlers_[handle];
#endif /* ACE_WIN32 */
}

// Bind the <ACE_Event_Handler *> to the <ACE_HANDLE>.

int 
ACE_Handler_Repository::bind (ACE_HANDLE handle, 
                              ACE_Event_Handler *event_handler)
{
  ACE_TRACE ("ACE_Handler_Repository::bind");
#if defined (ACE_WIN32)
  int first_free = -1;

  for (ssize_t i = 0; i < this->cur_size_; i++)
    if (this->event_handlers_[i].handle_ == handle)
      {
	// If the HANDLE is already here just reassign the
	// Event_Handler. 
	this->event_handlers_[i].event_handler_ = event_handler;
	return 0;
      }
    else if (this->event_handlers_[i].handle_ == ACE_INVALID_HANDLE
	     && first_free == -1)
      first_free = i;

  if (first_free > -1)
    // We found a free spot, let's reuse it (and the comments ;-)).
    {
      this->event_handlers_[first_free].handle_ = handle;
      this->event_handlers_[first_free].event_handler_ = event_handler;
      return 0;
    }
  else if (this->cur_size_ < this->max_size_)
    {
      // Insert at the end of the active portion. 
      this->event_handlers_[this->cur_size_].handle_ = handle;
      this->event_handlers_[this->cur_size_].event_handler_ = event_handler;
      this->cur_size_++;
      return 0;
    }
  else
    {
      // No more room! 
      errno = ENOMEM;
      return -1;
    }
#else
  this->event_handlers_[handle] = event_handler;

  if (this->max_handlep1_ < handle + 1)
    this->max_handlep1_ = handle + 1;
  return 0;
#endif /* ACE_WIN32 */
}

// Remove the binding of <ACE_HANDLE>.

int 
ACE_Handler_Repository::unbind (ACE_HANDLE handle,
                                ACE_Handle_Set &rd_mask,
                                ACE_Handle_Set &wr_mask, 
                                ACE_Handle_Set &ex_mask)
{
  ACE_TRACE ("ACE_Handler_Repository::unbind");
#if defined (ACE_WIN32)
  // If all the events were cleared then totally shut down Event
  // Handler.

  if (rd_mask.is_set (handle) == 0
      && wr_mask.is_set (handle) == 0 
      && ex_mask.is_set (handle) == 0)
    {
      for (ssize_t i = 0; i < this->cur_size_; i++)
	if (this->event_handlers_[i].handle_ == handle)
	  {
	    this->event_handlers_[i].handle_ = ACE_INVALID_HANDLE;
	    this->event_handlers_[i].event_handler_ = 0;

	    if (this->cur_size_ == i + 1)
	      {	      
		// We've deleted the last entry (i.e., i + 1 == the
		// current size of the array), so we need to figure
		// out the last valid place in the array that is worth
		// looking at.

		for (int i = this->cur_size_ - 1;
		     i >= 0 && this->event_handlers_[i].handle_ == ACE_INVALID_HANDLE;
		     i--)
		  continue;

		this->cur_size_ = i + 1;
	      }
	    break;
	  }
    }

  return 0;
#else
  // If all the events were cleared then totally shut down Event
  // Handler.

  if (handle < 0 || handle >= this->max_handlep1_)
    return -1;

  if (rd_mask.is_set (handle) == 0
      && wr_mask.is_set (handle) == 0 
      && ex_mask.is_set (handle) == 0)
    {
      this->event_handlers_[handle] = 0;

      if (this->max_handlep1_ == handle + 1)
	{
	  // We've deleted the last entry, so we need to figure out
	  // the last valid place in the array that is worth looking
	  // at. 
	  ACE_HANDLE rd_max = rd_mask.max_set ();
	  ACE_HANDLE wr_max = wr_mask.max_set ();
	  ACE_HANDLE ex_max = ex_mask.max_set ();

	  // Compute the maximum of three values.
	  this->max_handlep1_ = rd_max < wr_max ? wr_max : rd_max;
	  if (this->max_handlep1_ < ex_max)
	    this->max_handlep1_ = ex_max;

	  this->max_handlep1_++;
	}
    }
  return 0;
#endif /* ACE_WIN32 */
}

ACE_Handler_Repository_Iterator::ACE_Handler_Repository_Iterator (const ACE_Handler_Repository &s)
  : rep_ (s),
    current_ (-1)
{
  this->advance ();
}

// Pass back the <next_item> that hasn't been seen in the Set.
// Returns 0 when all items have been seen, else 1.

int 
ACE_Handler_Repository_Iterator::next (ACE_Event_Handler *&next_item)
{
#if defined (ACE_WIN32)
  if (this->current_ >= this->rep_.cur_size_)
    return 0;
  else
    {
      next_item = this->rep_.event_handlers_[this->current_].event_handler_;
      return 1;
    }

#else
  if (this->current_ >= this->rep_.max_handlep1_)
    return 0;
  else
    {
      next_item = this->rep_.event_handlers_[this->current_];
      return 1;
    }
#endif /* ACE_WIN32 */
}

// Move forward by one element in the set.

int 
ACE_Handler_Repository_Iterator::advance (void)
{
#if defined (ACE_WIN32)
  if (this->current_ < this->rep_.cur_size_)
    this->current_++;

  while (this->current_ < this->rep_.cur_size_)
    if (this->rep_.event_handlers_[this->current_].handle_ != ACE_INVALID_HANDLE)
      return 1;
    else
     this->current_++;

  return 0;    
#else
  if (this->current_ < this->rep_.max_handlep1_)
    this->current_++;

  while (this->current_ < this->rep_.max_handlep1_)
    if (this->rep_.event_handlers_[this->current_] != 0)
      return 1;
    else
     this->current_++;

  return 0;
#endif /* ACE_WIN32 */
}

// Dump the state of an object.

void 
ACE_Handler_Repository_Iterator::dump (void) const
{
  ACE_TRACE ("ACE_Handler_Repository_Iterator::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, "current_ = %d", this->current_));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));    
}

void
ACE_Handler_Repository::dump (void) const
{
  ACE_TRACE ("ACE_Handler_Repository::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
#if defined (ACE_WIN32)
  ACE_DEBUG ((LM_DEBUG, 
              "(%t) cur_size_ = %d, max_size_ = %d\n",
              this->cur_size_, this->max_size_)); 
#else
  ACE_DEBUG ((LM_DEBUG, 
              "(%t) max_handlep1_ = %d, max_size_ = %d\n",
              this->max_handlep1_, this->max_size_)); 
#endif /* ACE_WIN32 */

  ACE_DEBUG ((LM_DEBUG, "["));

  ACE_Event_Handler *eh = 0;

  for (ACE_Handler_Repository_Iterator iter (*this);
       iter.next (eh) != 0;
       iter.advance ())
    ACE_DEBUG ((LM_DEBUG, " (eh = %x, eh->handle_ = %d)", eh, eh->get_handle ()));

  ACE_DEBUG ((LM_DEBUG, " ]"));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));    
}

ACE_ALLOC_HOOK_DEFINE(ACE_Handler_Repository_Iterator)

void
ACE_Reactor::dump (void) const
{
  ACE_TRACE ("ACE_Reactor::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));

  this->timer_queue_->dump ();
  this->handler_rep_.dump ();
  this->signal_handler_->dump ();
  ACE_DEBUG ((LM_DEBUG, "delete_signal_handler_ = %d\n", this->delete_signal_handler_));

  ACE_HANDLE h;

  for (ACE_Handle_Set_Iterator handle_iter_wr (this->wr_handle_mask_);
       (h = handle_iter_wr ()) != ACE_INVALID_HANDLE;
       ++handle_iter_wr)
    ACE_DEBUG ((LM_DEBUG, "write_handle = %d\n", h));

  for (ACE_Handle_Set_Iterator handle_iter_rd (this->rd_handle_mask_);
       (h = handle_iter_rd ()) != ACE_INVALID_HANDLE;
       ++handle_iter_rd)
    ACE_DEBUG ((LM_DEBUG, "read_handle = %d\n", h));

  for (ACE_Handle_Set_Iterator handle_iter_ex (this->ex_handle_mask_);
       (h = handle_iter_ex ()) != ACE_INVALID_HANDLE;
       ++handle_iter_ex)
    ACE_DEBUG ((LM_DEBUG, "except_handle = %d\n", h));

  for (ACE_Handle_Set_Iterator handle_iter_wr_ready (this->wr_handle_mask_ready_);
       (h = handle_iter_wr_ready ()) != ACE_INVALID_HANDLE;
       ++handle_iter_wr_ready)
    ACE_DEBUG ((LM_DEBUG, "write_handle_ready = %d\n", h));

  for (ACE_Handle_Set_Iterator handle_iter_rd_ready (this->rd_handle_mask_ready_);
       (h = handle_iter_rd_ready ()) != ACE_INVALID_HANDLE;
       ++handle_iter_rd_ready)
    ACE_DEBUG ((LM_DEBUG, "read_handle_ready = %d\n", h));

  for (ACE_Handle_Set_Iterator handle_iter_ex_ready (this->ex_handle_mask_ready_);
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
  this->requeue_position_ = rp;
}

int 
ACE_Reactor::requeue_position (void)
{
  ACE_TRACE ("ACE_Reactor::requeue_position");
  ACE_MT (ACE_GUARD_RETURN (ACE_REACTOR_MUTEX, ace_mon, this->token_, -1));
  return this->requeue_position_;
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

#if !defined (ACE_WIN32) // There seems to be a Win32 bug with this...
  // Set this into non-blocking mode.
  if (ACE::set_flags (this->notification_pipe_.read_handle (), 
		      ACE_NONBLOCK) == -1)
    return -1;
  else 
#endif /* !ACE_WIN32 */
    if (this->reactor_->register_handler (this->notification_pipe_.read_handle (),
					  this,
					  ACE_Event_Handler::READ_MASK) == -1)
    return -1;
  return 0;
}

int
ACE_Reactor_Notify::close (void)
{
  ACE_TRACE ("ACE_Reactor_Notify::close");
  return this->notification_pipe_.close ();
}

ssize_t
ACE_Reactor_Notify::notify (ACE_Event_Handler *eh, 
				  ACE_Reactor_Mask mask)
{
  ACE_TRACE ("ACE_Reactor_Notify::notify");
  ACE_Notification_Buffer buffer (eh, mask);

  ssize_t n = ACE::send (this->notification_pipe_.write_handle (),
			 (char *) &buffer, 
			 sizeof buffer);
  if (n == -1)
    return -1;
  else
    return 0;
}

// Handles pending threads (if any) that are waiting to unblock the
// Reactor.

int
ACE_Reactor_Notify::handle_notifications (ACE_Handle_Set &rmask)
{
  ACE_TRACE ("ACE_Reactor_Notify::handle_notification");

  ACE_HANDLE read_handle = this->notification_pipe_.read_handle ();

  if (rmask.is_set (read_handle))
    {
      this->reactor_->notify_handle 
	(read_handle, ACE_Event_Handler::READ_MASK,
	 this->reactor_->rd_handle_mask_ready_, 
	 this->reactor_->handler_rep_.find (read_handle),
	 &ACE_Event_Handler::handle_input); 
      return 1;
    }
  else
    return 0;
}

// Special trick to unblock select() or poll() when updates occur in
// somewhere other than the main ACE_Reactor thread.  All we do is
// write data to a pipe that the ACE_Reactor is listening on.  Thanks
// to Paul Stephenson at Ericsson for suggesting this approach.

int
ACE_Reactor_Notify::handle_input (ACE_HANDLE handle)
{
  ACE_TRACE ("ACE_Reactor_Notify::handle_input");
  // Precondition: this->reactor_.token_.current_owner () ==
  // ACE_Thread::self ();

  ACE_ASSERT (this->notification_pipe_.read_handle () == handle);

  ACE_Notification_Buffer buffer;
  ssize_t n;

#if defined (ACE_WIN32)
  if ((n = ACE::recv (handle, (char *) &buffer, sizeof buffer)) == -1)
    return -1;
  // Put ourselves at the head of the queue.
  this->reactor_->requeue_position (0);
#else
  while ((n = ACE::recv (handle, (char *) &buffer, sizeof buffer)) != -1)
#endif /* ACE_WIN32 */
    {
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
              result = buffer.eh_->handle_input (ACE_INVALID_HANDLE);
              break;
            case ACE_Event_Handler::WRITE_MASK:
              result = buffer.eh_->handle_output (ACE_INVALID_HANDLE);
              break;
            case ACE_Event_Handler::EXCEPT_MASK:
              result = buffer.eh_->handle_exception (ACE_INVALID_HANDLE);
              break;
            default:
              ACE_ERROR ((LM_ERROR, "invalid mask = %d\n", buffer.mask_));
              break;
            }
          if (result == -1)
            buffer.eh_->handle_close (ACE_INVALID_HANDLE, 
				      ACE_Event_Handler::EXCEPT_MASK);
        }
    }

  // Enqueue ourselves into the list of waiting threads.  When we
  // reacquire the token we'll be off and running again with ownership
  // of the token.
  this->reactor_->renew ();

  // Postcondition: this->reactor_.token_.current_owner () ==
  // ACE_Thread::self ();
  return n == -1 && errno != EWOULDBLOCK ? -1 : 0;
}
#endif /* ACE_MT_SAFE */

int
ACE_Reactor::notify (ACE_Event_Handler *eh, 
                     ACE_Reactor_Mask mask)
{
  ACE_TRACE ("ACE_Reactor::notify");

  ssize_t n = 0;
#if defined (ACE_MT_SAFE)
  // Pass over both the Event_Handler *and* the mask in order to allow
  // the caller to dictate which Event_Handler method the receiver
  // invokes.

  n = this->notify_handler_.notify (eh, mask);
#else
  eh = eh;
  mask = mask;
#endif /* ACE_MT_SAFE */
  return n == -1 ? -1 : 0;
}

int
ACE_Reactor::resume_handler (ACE_HANDLE handle)
{
  ACE_TRACE ("ACE_Reactor::resume_handler");
  ACE_MT (ACE_GUARD_RETURN (ACE_REACTOR_MUTEX, ace_mon, this->token_, -1));
  return this->resume (handle);
}

int
ACE_Reactor::suspend_handler (ACE_HANDLE handle)
{
  ACE_TRACE ("ACE_Reactor::suspend_handler");
  ACE_MT (ACE_GUARD_RETURN (ACE_REACTOR_MUTEX, ace_mon, this->token_, -1));
  return this->suspend (handle);
}

int
ACE_Reactor::suspend_handlers (void)
{
  ACE_TRACE ("ACE_Reactor::suspend_handlers");
  ACE_MT (ACE_GUARD_RETURN (ACE_REACTOR_MUTEX, ace_mon, this->token_, -1));

  ACE_Event_Handler *eh = 0;

  for (ACE_Handler_Repository_Iterator iter (this->handler_rep_);
       iter.next (eh) != 0;
       iter.advance ())
    this->suspend (eh->get_handle ());

  return 0;
}

int
ACE_Reactor::resume_handlers (void)
{
  ACE_TRACE ("ACE_Reactor::resume_handlers");
  ACE_MT (ACE_GUARD_RETURN (ACE_REACTOR_MUTEX, ace_mon, this->token_, -1));

  ACE_Event_Handler *eh = 0;

  for (ACE_Handler_Repository_Iterator iter (this->handler_rep_);
       iter.next (eh) != 0;
       iter.advance ())
    this->resume (eh->get_handle ());

  return 0;
}

int
ACE_Reactor::register_handler (ACE_Event_Handler *handler, 
                               ACE_Reactor_Mask mask)
{
  ACE_TRACE ("ACE_Reactor::register_handler");
  ACE_MT (ACE_GUARD_RETURN (ACE_REACTOR_MUTEX, ace_mon, this->token_, -1));
  return this->attach (handler->get_handle (), handler, mask);
}

int
ACE_Reactor::register_handler (ACE_HANDLE handle, 
			       ACE_Event_Handler *handler, 
			       ACE_Reactor_Mask mask)
{
  ACE_TRACE ("ACE_Reactor::register_handler");
  ACE_MT (ACE_GUARD_RETURN (ACE_REACTOR_MUTEX, ace_mon, this->token_, -1));
  return this->attach (handle, handler, mask);
}

int
ACE_Reactor::register_handler (const ACE_Handle_Set &handles, 
                           ACE_Event_Handler *handler, 
                           ACE_Reactor_Mask mask)
{
  ACE_TRACE ("ACE_Reactor::register_handler");
  ACE_MT (ACE_GUARD_RETURN (ACE_REACTOR_MUTEX, ace_mon, this->token_, -1));
  return this->register_handlers (handles, handler, mask);
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
  return this->remove_handlers (handles, mask);
}

int
ACE_Reactor::remove_handler (ACE_Event_Handler *handler, 
                             ACE_Reactor_Mask mask)
{
  ACE_TRACE ("ACE_Reactor::remove_handler");
  ACE_MT (ACE_GUARD_RETURN (ACE_REACTOR_MUTEX, ace_mon, this->token_, -1));
  return this->detach (handler->get_handle (), mask);
}

int
ACE_Reactor::remove_handler (ACE_HANDLE handle, 
			     ACE_Reactor_Mask mask)
{
  ACE_TRACE ("ACE_Reactor::remove_handler");
  ACE_MT (ACE_GUARD_RETURN (ACE_REACTOR_MUTEX, ace_mon, this->token_, -1));
  return this->detach (handle, mask);
}

// Performs operations on the "ready" bits.

int
ACE_Reactor::ready_ops (ACE_HANDLE handle, 
			ACE_Reactor_Mask mask, 
			int ops)
{
  ACE_TRACE ("ACE_Reactor::ready_ops");
  ACE_MT (ACE_GUARD_RETURN (ACE_REACTOR_MUTEX, ace_mon, this->token_, -1));
  return this->bit_ops (handle, mask, 
                        this->rd_handle_mask_ready_, 
                        this->wr_handle_mask_ready_, 
                        this->ex_handle_mask_ready_, 
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

  if (this->initialized_ != 0)
    return -1;
  else
    this->initialized_ = 1;

  this->owner_ = ACE_Thread::self ();
  this->restart_ = restart;

  // Allows the signal handler to be overridden.
  if (sh == 0)
    {
      ACE_NEW_RETURN (this->signal_handler_, ACE_Sig_Handler, -1);
      this->delete_signal_handler_ = 1;
    }
  else
    {
      this->signal_handler_ = sh;
      this->delete_signal_handler_ = 0;
    }

  this->timer_queue_ = tq;

  if (this->timer_queue_ == 0)
    {
      // We do this first in case the handler_rep_ call fails (which
      // it sometimes does on Win32 when we restart applications
      // quickly due to the use of sockets as a notification
      // mechanism).  At least this way the timer_queue_ isn't 0, so
      // we can still use the Reactor as a timer...
      ACE_NEW_RETURN (this->timer_queue_, ACE_Timer_Queue, -1);
      this->delete_timer_queue_ = 1;
    }

  if (this->handler_rep_.open (size) == -1)
    return -1;
#if defined (ACE_MT_SAFE)
  else if (this->notify_handler_.open (this) == -1)
    {
      // Make sure to release resources.
      this->handler_rep_.close (this);
      return -1;
    }
#endif /* ACE_MT_SAFE */

#if defined (ACE_USE_POLL)
  ACE_NEW_RETURN (this->poll_h_, pollfd[size], -1);

  for (size_t h = 0; h < size; h++)
    {
      this->poll_h_[h].fd = -1;
      this->poll_h_[h].events = 0;
      this->poll_h_[h].revents = 0;
    }
#endif /* ACE_USE_POLL */

  return 0;
}

ACE_Reactor::ACE_Reactor (ACE_Sig_Handler *sh,
			  ACE_Timer_Queue *tq)
  : timer_queue_ (0),
    delete_timer_queue_ (0),
    requeue_position_ (-1), // Requeue at end of waiters by default.
    initialized_ (0),
#if defined (ACE_MT_SAFE)
    token_ (*this)
#endif /* ACE_MT_SAFE */
{
  ACE_TRACE ("ACE_Reactor::ACE_Reactor");
  if (this->open (ACE_Reactor::DEFAULT_SIZE, 0, sh, tq))
    ACE_ERROR ((LM_ERROR, "%p\n", "open failed"));
}

// Initialize the new ACE_Reactor.

ACE_Reactor::ACE_Reactor (size_t size, 
			  int rs, 
			  ACE_Sig_Handler *sh,
			  ACE_Timer_Queue *tq)
  : timer_queue_ (0),
    delete_timer_queue_ (0),
    requeue_position_ (-1), // Requeue at end of waiters by default.
    initialized_ (0),
#if defined (ACE_MT_SAFE)
    token_ (*this)
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

  if (this->timer_queue_ != 0)
    {
      this->handler_rep_.close (this);

      if (this->delete_signal_handler_)
        delete this->signal_handler_;
      this->signal_handler_ = 0;

      if (this->delete_timer_queue_)
	delete this->timer_queue_;

      this->timer_queue_ = 0;

#if defined (ACE_MT_SAFE)
      this->notify_handler_.close ();
#endif /* ACE_MT_SAFE */

#if defined (ACE_USE_POLL)
      delete [] this->poll_h_;
#endif /* ACE_USE_POLL */
    }
}
  
ACE_Reactor::~ACE_Reactor (void)
{
  ACE_TRACE ("ACE_Reactor::~ACE_Reactor");
  this->close ();
}

int
ACE_Reactor::remove_handlers (const ACE_Handle_Set &handles,
                              ACE_Reactor_Mask mask)
{
  ACE_TRACE ("ACE_Reactor::remove_handlers");
  ACE_HANDLE h;

  for (ACE_Handle_Set_Iterator handle_iter (handles);
       (h = handle_iter ()) != ACE_INVALID_HANDLE;
       ++handle_iter)
    if (this->detach (h, mask) == -1)
      return -1;

  return 0;    
}

int
ACE_Reactor::register_handlers (const ACE_Handle_Set &handles, 
                                ACE_Event_Handler *handler, 
                                ACE_Reactor_Mask mask)
{
  ACE_TRACE ("ACE_Reactor::register_handlers");
  ACE_HANDLE h;

  for (ACE_Handle_Set_Iterator handle_iter (handles);
       (h = handle_iter ()) != ACE_INVALID_HANDLE;
       ++handle_iter)
    if (this->attach (h, handler, mask) == -1)
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

  for (int s = 1; s < NSIG; s++)
    if (sigset.is_member (s) 
        && this->signal_handler_->register_handler (s, new_sh, new_disp) == -1) 
      result = -1;

  return result;
}

int
ACE_Reactor::remove_handler (const ACE_Sig_Set &sigset)
{
  ACE_TRACE ("ACE_Reactor::remove_handler");
  int result = 0;

  for (int s = 1; s < NSIG; s++)
    if (sigset.is_member (s) 
        && this->signal_handler_->remove_handler (s) == -1)
      result = -1;

  return result;    
}

// Note the queue handles its own locking. 

int
ACE_Reactor::schedule_timer (ACE_Event_Handler *handler, 
                             const void *arg,
                             const ACE_Time_Value &delta_time, 
                             const ACE_Time_Value &interval)
{
  ACE_TRACE ("ACE_Reactor::schedule_timer");
  ACE_MT (ACE_GUARD_RETURN (ACE_REACTOR_MUTEX, ace_mon, this->token_, -1));

  return this->timer_queue_->schedule 
    (handler, arg, ACE_OS::gettimeofday () + delta_time, interval);
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
                            ACE_Event_Handler *iop, 
                            ACE_EH_PTMF ptmf)
{
  ACE_TRACE ("ACE_Reactor::notify_handle");
  if (iop == 0)
    return;

  int status = (iop->*ptmf) (handle);

  if (status < 0)
    this->detach (handle, mask);
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
                      ACE_Handle_Set &rd,
                      ACE_Handle_Set &wr, 
                      ACE_Handle_Set &ex, 
                      int ops)
{
  ACE_TRACE ("ACE_Reactor::bit_ops");
  if (this->handler_rep_.invalid_handle (handle))
    return -1;

  ACE_Sig_Guard sb; // Block out all signals until method returns. 

  ACE_FDS_PTMF ptmf  = &ACE_Handle_Set::set_bit;
  u_long       omask = ACE_Event_Handler::NULL_MASK;

  switch (ops)
    {
    case ACE_Reactor::GET_MASK:
      if (rd.is_set (handle))
        ACE_SET_BITS (omask, ACE_Event_Handler::READ_MASK);
      if (wr.is_set (handle))
        ACE_SET_BITS (omask, ACE_Event_Handler::WRITE_MASK);
      if (ex.is_set (handle))
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
      // On the other hand, f we are doing a ACE_Reactor::CLR_MASK or
      // a ACE_Reactor::ADD_MASK we just carry out the operations
      // specified by the mask.

      if (ACE_BIT_ENABLED (mask, ACE_Event_Handler::READ_MASK))
        {
          (rd.*ptmf) (handle);
          ACE_SET_BITS (omask, ACE_Event_Handler::READ_MASK);
        }
      else if (ops == ACE_Reactor::SET_MASK)
        rd.clr_bit (handle);

      if (ACE_BIT_ENABLED (mask, ACE_Event_Handler::WRITE_MASK))
        {
          (wr.*ptmf) (handle);
          ACE_SET_BITS (omask, ACE_Event_Handler::WRITE_MASK);
        }
      else if (ops == ACE_Reactor::SET_MASK)
        wr.clr_bit (handle);

      if (ACE_BIT_ENABLED (mask, ACE_Event_Handler::EXCEPT_MASK))
        {
          (ex.*ptmf) (handle);
          ACE_SET_BITS (omask, ACE_Event_Handler::EXCEPT_MASK);
        }
      else if (ops == ACE_Reactor::SET_MASK)
        ex.clr_bit (handle);
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
                        this->rd_handle_mask_,
                        this->wr_handle_mask_, 
                        this->ex_handle_mask_,
                        ops);
}

// Must be called with locks held

int
ACE_Reactor::handler_i (ACE_HANDLE handle, 
			ACE_Reactor_Mask mask, 
                        ACE_Event_Handler **handler) 
{
  ACE_TRACE ("ACE_Reactor::handler_i");
  if (this->handler_rep_.invalid_handle (handle) 
      || this->handler_rep_.find (handle) == 0)
    return -1;
  else
    {
      if (ACE_BIT_ENABLED (mask, ACE_Event_Handler::READ_MASK)
          && this->rd_handle_mask_.is_set (handle) == 0)
        return -1;
      if (ACE_BIT_ENABLED (mask, ACE_Event_Handler::WRITE_MASK)
          && this->wr_handle_mask_.is_set (handle) == 0)
        return -1;
      if (ACE_BIT_ENABLED (mask, ACE_Event_Handler::EXCEPT_MASK)
          && this->ex_handle_mask_.is_set (handle) == 0)          
        return -1;
    }

  if (handler != 0)
    *handler = this->handler_rep_.find (handle);
  return 0;
}

// Must be called with locks held

int
ACE_Reactor::resume (ACE_HANDLE handle)
{
  ACE_TRACE ("ACE_Reactor::resume");
  if (this->handler_rep_.invalid_handle (handle) 
      || this->handler_rep_.find (handle) == 0)
    return -1;
  
  this->rd_handle_mask_.set_bit (handle);
  this->wr_handle_mask_.set_bit (handle);
  this->ex_handle_mask_.set_bit (handle);
  return 0;
}

// Must be called with locks held

int
ACE_Reactor::suspend (ACE_HANDLE handle)
{
  ACE_TRACE ("ACE_Reactor::suspend");
  if (this->handler_rep_.invalid_handle (handle) 
      || this->handler_rep_.find (handle) == 0)
    return -1;

  this->rd_handle_mask_.clr_bit (handle);
  this->wr_handle_mask_.clr_bit (handle);
  this->ex_handle_mask_.clr_bit (handle);
  return 0;
}

// Must be called with locks held

int
ACE_Reactor::attach (ACE_HANDLE handle, 
		     ACE_Event_Handler *handler,
		     ACE_Reactor_Mask mask)
{
  ACE_TRACE ("ACE_Reactor::attach");

  if (this->handler_rep_.invalid_handle (handle))
    return -1;

  this->handler_rep_.bind (handle, handler);

  this->bit_ops (handle, mask, 
                 this->rd_handle_mask_, 
                 this->wr_handle_mask_, 
                 this->ex_handle_mask_, 
                 ACE_Reactor::ADD_MASK);

  // Assign *this* <Reactor> to the <Event_Handler>.
  handler->reactor (this);
  return 0;
}

int
ACE_Reactor::detach (ACE_HANDLE handle, 
		     ACE_Reactor_Mask mask)
{
  ACE_TRACE ("ACE_Reactor::detach");
  if (this->handler_rep_.invalid_handle (handle) 
      || this->handler_rep_.find (handle) == 0)
    return -1;

  ACE_Event_Handler *eh = this->handler_rep_.find (handle);

  this->bit_ops (handle, mask, 
                 this->rd_handle_mask_,
                 this->wr_handle_mask_,
                 this->ex_handle_mask_, 
                 ACE_Reactor::CLR_MASK);

  if (ACE_BIT_ENABLED (mask, ACE_Event_Handler::DONT_CALL) == 0)
    eh->handle_close (handle, mask);

  // See if we can unbind this handle.
  this->handler_rep_.unbind (handle, 
                             this->rd_handle_mask_,
                             this->wr_handle_mask_, 
                             this->ex_handle_mask_);
  // Reinitialize the Reactor pointer to 0.
  eh->reactor (0);
  return 0;
}

#if defined (ACE_USE_POLL)
// Transforms the select() data structures into a data structure
// suitable for use with poll().

pollfd *
ACE_Reactor::handle_sets_to_poll_fds (ACE_HANDLE &width)
{
  ACE_TRACE ("ACE_Reactor::handle_sets_to_poll_fds");

  for (ACE_HANDLE h = 0; h < this->handler_rep_.max_handlep1 (); h++)
    {
      if (this->wr_handle_mask_.is_set (h))
        {
          this->poll_h_[width].fd = h;
          ACE_SET_BITS (this->poll_h_[width].events, ACE_Event_Handler::WRITE_MASK);
        }
      if (this->rd_handle_mask_.is_set (h))
        {
          this->poll_h_[width].fd = h;
          ACE_SET_BITS (this->poll_h_[width].events, ACE_Event_Handler::READ_MASK);
        }
      if (this->ex_handle_mask_.is_set (h))
        {
          this->poll_h_[width].fd = h;
          ACE_SET_BITS (this->poll_h_[width].events, ACE_Event_Handler::EXCEPT_MASK);
        }
      if (this->poll_h_[width].fd != -1)
        width++;
    }

  return this->poll_h_;
  return 0;
}

// Transforms the poll() data structures into data structures
// suitable for use with select().

void
ACE_Reactor::poll_fds_to_handle_sets (ACE_HANDLE width,
				      ACE_Handle_Set &rmask,
				      ACE_Handle_Set &wmask, 
				      ACE_Handle_Set &emask,
				      int nfound)
{
  ACE_TRACE ("ACE_Reactor::poll_fds_to_handle_sets");

  for (ACE_HANDLE h = 0; nfound > 0 && h < width; h++)
    {
      int found = 0;
      pollfd &p_fd = this->poll_h_[h];

      if (p_fd.revents & POLLOUT)
        {
          wmask.set_bit (p_fd.fd);
          found = 1;
        }

      if (p_fd.revents & POLLPRI)
        {
          emask.set_bit (p_fd.fd);
          found = 1;
        }

      if ((p_fd.revents & POLLIN) 
          || (p_fd.revents & POLLHUP)
          || (p_fd.revents & POLLERR))
        {
          rmask.set_bit (p_fd.fd);
          found = 1;
        }

      p_fd.revents = 0;
      p_fd.events = 0;
      p_fd.fd = -1;

      if (found)
        nfound--;
    }
}
#endif /* ACE_USE_POLL */

// Must be called with lock held

int 
ACE_Reactor::wait_for_multiple_events (ACE_Handle_Set &rmask, 
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
      nfound = ACE_OS::select (int (width), rmask, wmask, emask, max_wait_time);
#endif /* ACE_USE_POLL */
    } 
  while (nfound == -1 && this->handle_error () > 0);

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
ACE_Reactor::dispatch (int nfound, 
                       ACE_Handle_Set &rmask, 
                       ACE_Handle_Set &wmask, 
                       ACE_Handle_Set &emask)
{
  ACE_TRACE ("ACE_Reactor::dispatch");
  // Handle timers first since they may have higher latency
  // constraints... 

  this->timer_queue_->expire ();

#if defined (ACE_MT_SAFE)
  // Check to see if the notify ACE_HANDLE is enabled.  If so, it
  // means that one or more other threads are trying to update the
  // ACE_Reactor's internal tables.  We'll handle all these threads
  // and then break out to continue the event loop.
  
  if (this->notify_handler_.handle_notifications (rmask) == 0)
#endif /* ACE_MT_SAFE */
    {
      ACE_HANDLE h;

      if (nfound > 0)
        {
          // Handle output conditions (this code needs to come first to
          // handle the obscure case of piggy-backed data coming along
          // with the final handshake message of a nonblocking
          // connection).

          for (ACE_Handle_Set_Iterator handle_iter_wr (wmask); 
               (h = handle_iter_wr ()) != ACE_INVALID_HANDLE && --nfound >= 0; 
               ++handle_iter_wr)
            this->notify_handle (h, ACE_Event_Handler::WRITE_MASK,
                                 this->wr_handle_mask_ready_, 
                                 this->handler_rep_.find (h),
                                 &ACE_Event_Handler::handle_output); 
        }

      if (nfound > 0)
        {
          // Handle "exceptional" conditions.
          for (ACE_Handle_Set_Iterator handle_iter_ex (emask); 
               (h = handle_iter_ex ()) != ACE_INVALID_HANDLE && --nfound >= 0; 
               ++handle_iter_ex)
            this->notify_handle (h, ACE_Event_Handler::EXCEPT_MASK,
                                 this->ex_handle_mask_ready_, 
                                 this->handler_rep_.find (h),
                                 &ACE_Event_Handler::handle_exception); 
        }
      if (nfound > 0)
        {
          // Handle input and shutdown conditions.
          for (ACE_Handle_Set_Iterator handle_iter_rd (rmask); 
               (h = handle_iter_rd ()) != ACE_INVALID_HANDLE && --nfound >= 0; 
               ++handle_iter_rd)
            this->notify_handle (h, ACE_Event_Handler::READ_MASK,
                                 this->rd_handle_mask_ready_, 
                                 this->handler_rep_.find (h),
                                 &ACE_Event_Handler::handle_input); 
        }
    }
}

int 
ACE_Reactor::fill_in_ready (ACE_Handle_Set &rmask, 
                            ACE_Handle_Set &wmask, 
                            ACE_Handle_Set &emask)
{
  ACE_TRACE ("ACE_Reactor::fill_in_ready");
  ACE_Sig_Guard sb;
  
  rmask = this->rd_handle_mask_ready_;
  wmask = this->wr_handle_mask_ready_;
  emask = this->ex_handle_mask_ready_;

  this->rd_handle_mask_ready_.reset ();
  this->wr_handle_mask_ready_.reset ();
  this->ex_handle_mask_ready_.reset ();

  return rmask.num_set () + wmask.num_set () + emask.num_set ();
}

int 
ACE_Reactor::handle_events (ACE_Time_Value *max_wait_time)
{
  ACE_TRACE ("ACE_Reactor::handle_events");
#if defined (ACE_MT_SAFE)
  ACE_MT (ACE_GUARD_RETURN (ACE_REACTOR_MUTEX, ace_mon, this->token_, -1));
  if (ACE_OS::thr_equal (ACE_Thread::self (), this->owner_) == 0)
    return -1;
#endif /* ACE_MT_SAFE */

  // Stash the current time -- the destructor of this object will
  // automatically compute how much time elpased since this method was
  // called.
  ACE_Countdown_Time countdown (max_wait_time);

  ACE_Handle_Set rmask;
  ACE_Handle_Set wmask;
  ACE_Handle_Set emask;
  int nfound;

  if (this->any_ready ())
    nfound = this->fill_in_ready (rmask, wmask, emask);
  else
    nfound = this->wait_for_multiple_events (rmask, wmask, emask, max_wait_time); 

  for (;;)
    {
      this->dispatch (nfound, rmask, wmask, emask);

      if (ACE_Sig_Handler::sig_pending () != 0)
        {
          ACE_Sig_Handler::sig_pending (0);
          
          // Dispatch any HANDLES that are activated
          // as a result of signals since they may be
          // time critical...

	  if (nfound == -1)
	    nfound = 0;
          
	  if (this->any_ready ())
            {
              nfound = this->fill_in_ready (rmask, wmask, emask);
              continue;
            }
        }
      break;
    }

  return nfound;
}

int
ACE_Reactor::check_handles (void)
{
  ACE_TRACE ("ACE_Reactor::check_handles");

#if defined (ACE_USE_POLL)
  pollfd p_handle;
  p_handle.events = POLLIN;
#else
  ACE_Time_Value time_poll = ACE_Time_Value::zero;
  ACE_Handle_Set rmask;
#endif /* ACE_USE_POLL */

  ACE_Event_Handler *eh = 0;
  int result = 0;

  for (ACE_Handler_Repository_Iterator iter (this->handler_rep_);
       iter.next (eh) != 0;
       iter.advance ())
    {
      ACE_HANDLE handle = eh->get_handle ();

      // Skip back to the beginning of the loop if the HANDLE is
      // invalid.
      if (handle == ACE_INVALID_HANDLE)
      	continue;

#if defined (ACE_USE_POLL)
      p_handle.fd = handle;
      if (ACE_OS::poll (&p_handle, 1, 0) == -1)
	{
	  result = 1;
	  this->detach (handle, ACE_Event_Handler::RWE_MASK);
	}
#else
      rmask.set_bit (handle);
      if (ACE_OS::select (int (handle) + 1, 
			  rmask, 0, 0, 
			  &time_poll) < 0)
	{
	  result = 1;
	  this->detach (handle, ACE_Event_Handler::RWE_MASK);
	}
      rmask.clr_bit (handle);
#endif /* ACE_USE_POLL */
    }

  return result;
}

#if defined (ACE_TEMPLATES_REQUIRE_SPECIALIZATION)
ACE_MT (template class ACE_Guard<ACE_REACTOR_MUTEX>);
#endif /* ACE_TEMPLATES_REQUIRE_SPECIALIZATION */

