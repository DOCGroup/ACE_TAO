#include "Select_Reactor_Notify.h"
#include "ACE.h"
#include "Select_Reactor_Base.h"

ACE_RCSID(ace,
          Select_Reactor_Notify,
          "$Id$")


ACE_Select_Reactor_Notify::ACE_Select_Reactor_Notify (void)
  : max_notify_iterations_ (-1)
{
}

ACE_Select_Reactor_Notify::~ACE_Select_Reactor_Notify (void)
{
}

void
ACE_Select_Reactor_Notify::max_notify_iterations (int iterations)
{
  // Must always be > 0 or < 0 to optimize the loop exit condition.
  if (iterations == 0)
    iterations = 1;

  this->max_notify_iterations_ = iterations;
}

int
ACE_Select_Reactor_Notify::max_notify_iterations (void)
{
  return this->max_notify_iterations_;
}

int
ACE_Select_Reactor_Notify::purge_pending_notifications (ACE_Event_Handler *eh,
                                                        ACE_Reactor_Mask  mask )
{
  ACE_TRACE ("ACE_Select_Reactor_Notify::purge_pending_notifications");

#if defined (ACE_HAS_REACTOR_NOTIFICATION_QUEUE)

  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, mon, this->notify_queue_lock_, -1);

  if (this->notify_queue_.is_empty ())
    return 0;

  ACE_Notification_Buffer *temp;
  ACE_Unbounded_Queue <ACE_Notification_Buffer *> local_queue;

  size_t queue_size = this->notify_queue_.size ();
  int number_purged = 0;
  size_t i;
  for (i = 0; i < queue_size; ++i)
    {
      if (-1 == this->notify_queue_.dequeue_head (temp))
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_LIB_TEXT ("%p\n"),
                           ACE_LIB_TEXT ("dequeue_head")),
                          -1);

      // If this is not a Reactor notify (it is for a particular handler),
      // and it matches the specified handler (or purging all),
      // and applying the mask would totally eliminate the notification, then
      // release it and count the number purged.
      if ((0 != temp->eh_) &&
          (0 == eh || eh == temp->eh_) &&
          ACE_BIT_DISABLED (temp->mask_, ~mask)) // the existing notificationmask
                                                 // is left with nothing when
                                                 // applying the mask
      {
        if (-1 == this->free_queue_.enqueue_head (temp))
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_LIB_TEXT ("%p\n"),
                             ACE_LIB_TEXT ("enqueue_head")),
                            -1);
        ++number_purged;
      }
      else
      {
        // To preserve it, move it to the local_queue.
        // But first, if this is not a Reactor notify (it is for a particularhandler),
        // and it matches the specified handler (or purging all), then
        // apply the mask
        if ((0 != temp->eh_) &&
            (0 == eh || eh == temp->eh_))
          ACE_CLR_BITS(temp->mask_, mask);
        if (-1 == local_queue.enqueue_head (temp))
          return -1;
      }
    }

  if (this->notify_queue_.size ())
    { // should be empty!
      ACE_ASSERT (0);
      return -1;
    }

  // now put it back in the notify queue
  queue_size = local_queue.size ();
  for (i = 0; i < queue_size; ++i)
    {
      if (-1 == local_queue.dequeue_head (temp))
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_LIB_TEXT ("%p\n"),
                           ACE_LIB_TEXT ("dequeue_head")),
                          -1);

      if (-1 == this->notify_queue_.enqueue_head (temp))
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_LIB_TEXT ("%p\n"),
                           ACE_LIB_TEXT ("enqueue_head")),
                          -1);
    }

  return number_purged;

#else /* defined (ACE_HAS_REACTOR_NOTIFICATION_QUEUE) */
  ACE_UNUSED_ARG (eh);
  ACE_UNUSED_ARG (mask);
  ACE_NOTSUP_RETURN (-1);
#endif  /* defined (ACE_HAS_REACTOR_NOTIFICATION_QUEUE) */
}

void
ACE_Select_Reactor_Notify::dump (void) const
{
  ACE_TRACE ("ACE_Select_Reactor_Notify::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("select_reactor_ = %x"), this->select_reactor_));
  this->notification_pipe_.dump ();
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

int
ACE_Select_Reactor_Notify::open (ACE_Reactor_Impl *r,
                                 ACE_Timer_Queue *,
                                 int disable_notify_pipe)
{
  ACE_TRACE ("ACE_Select_Reactor_Notify::open");

  if (disable_notify_pipe == 0)
    {
      this->select_reactor_ =
        ACE_dynamic_cast (ACE_Select_Reactor_Impl *, r);

      if (this->select_reactor_ == 0)
        {
          errno = EINVAL;
          return -1;
        }

      if (this->notification_pipe_.open () == -1)
        return -1;
#if defined (F_SETFD)
      ACE_OS::fcntl (this->notification_pipe_.read_handle (), F_SETFD, 1);
      ACE_OS::fcntl (this->notification_pipe_.write_handle (), F_SETFD, 1);
#endif /* F_SETFD */

#if defined (ACE_HAS_REACTOR_NOTIFICATION_QUEUE)
      ACE_Notification_Buffer *temp;

      ACE_NEW_RETURN (temp,
                      ACE_Notification_Buffer[ACE_REACTOR_NOTIFICATION_ARRAY_SIZE],
                      -1);

      if (this->alloc_queue_.enqueue_head (temp) == -1)
        {
          delete [] temp;
          return -1;
        }

      for (size_t i = 0; i < ACE_REACTOR_NOTIFICATION_ARRAY_SIZE; i++)
        if (free_queue_.enqueue_head (temp + i) == -1)
          return -1;

#endif /* ACE_HAS_REACTOR_NOTIFICATION_QUEUE */

      // There seems to be a Win32 bug with this...  Set this into
      // non-blocking mode.
      if (ACE::set_flags (this->notification_pipe_.read_handle (),
                          ACE_NONBLOCK) == -1)
        return -1;
      else
        return this->select_reactor_->register_handler
          (this->notification_pipe_.read_handle (),
           this,
           ACE_Event_Handler::READ_MASK);
    }
  else
    {
      this->select_reactor_ = 0;
      return 0;
    }
}

int
ACE_Select_Reactor_Notify::close (void)
{
  ACE_TRACE ("ACE_Select_Reactor_Notify::close");

#if defined (ACE_HAS_REACTOR_NOTIFICATION_QUEUE)
  // Free up the dynamically allocated resources.
  ACE_Notification_Buffer **b;

  for (ACE_Unbounded_Queue_Iterator<ACE_Notification_Buffer *> alloc_iter (this->alloc_queue_);
       alloc_iter.next (b) != 0;
       alloc_iter.advance ())
    {
      delete [] *b;
      *b = 0;
    }

  this->alloc_queue_.reset ();
  this->notify_queue_.reset ();
  this->free_queue_.reset ();
#endif /* ACE_HAS_REACTOR_NOTIFICATION_QUEUE */

  return this->notification_pipe_.close ();
}

int
ACE_Select_Reactor_Notify::notify (ACE_Event_Handler *eh,
                                   ACE_Reactor_Mask mask,
                                   ACE_Time_Value *timeout)
{
  ACE_TRACE ("ACE_Select_Reactor_Notify::notify");

  // Just consider this method a "no-op" if there's no
  // <ACE_Select_Reactor> configured.
  if (this->select_reactor_ == 0)
    return 0;

  ACE_Notification_Buffer buffer (eh, mask);

  int notification_required = 1;

#if defined (ACE_HAS_REACTOR_NOTIFICATION_QUEUE)
  // Artificial scope to limit the duration of the mutex.
  {
    ACE_GUARD_RETURN (ACE_SYNCH_MUTEX,
                      mon,
                      this->notify_queue_lock_, -1);

    if (this->notify_message_to_queue (buffer,
                                       notification_required) == -1)
      return -1;
  }
#endif /* ACE_HAS_REACTOR_NOTIFICATION_QUEUE */

  ssize_t n = 0;

  // Send a notification message on the pipe if required.
  if (notification_required)
    {
      n = ACE::send (this->notification_pipe_.write_handle (),
                     (char *) &buffer,
                     sizeof buffer,
                     timeout);
    }

  if (n == -1)
    return -1;

  return 0;
}


#if defined (ACE_HAS_REACTOR_NOTIFICATION_QUEUE)
int
ACE_Select_Reactor_Notify::notify_message_to_queue (ACE_Notification_Buffer &buffer,
                                                    int &notification_required)
{
  // If the notification queue is not empty, set the
  // notification_required flag to zero.
  if (!this->notify_queue_.is_empty ())
    notification_required = 0;

  ACE_Notification_Buffer *temp = 0;

  // Take a node from the free list.
  if (this->free_queue_.dequeue_head (temp) == -1)
    {
      // Grow the queue of available buffers.
      ACE_Notification_Buffer *temp1;

      ACE_NEW_RETURN (temp1,
                      ACE_Notification_Buffer[ACE_REACTOR_NOTIFICATION_ARRAY_SIZE],
                      -1);

      if (this->alloc_queue_.enqueue_head (temp1) == -1)
        {
          delete [] temp1;
          return -1;
        }

      // Start at 1 and enqueue only
      // (ACE_REACTOR_NOTIFICATION_ARRAY_SIZE - 1) elements since
      // the first one will be used right now.
      for (size_t i = 1;
           i < ACE_REACTOR_NOTIFICATION_ARRAY_SIZE;
           i++)
        this->free_queue_.enqueue_head (temp1 + i);

      temp = temp1;
    }


  ACE_ASSERT (temp != 0);
  *temp = buffer;

  // Enqueue the node in the <notify_queue>
  if (this->notify_queue_.enqueue_tail (temp) == -1)
    return -1;

  return 0;
}

#endif /*ACE_HAS_REACTOR_NOTIFICATION_QUEUE*/

int
ACE_Select_Reactor_Notify::dispatch_notifications (int &number_of_active_handles,
                                                   ACE_Handle_Set &rd_mask)
{
  ACE_TRACE ("ACE_Select_Reactor_Notify::dispatch_notifications");

  ACE_HANDLE read_handle =
    this->notification_pipe_.read_handle ();

  if (read_handle != ACE_INVALID_HANDLE
      && rd_mask.is_set (read_handle))
    {
      number_of_active_handles--;
      rd_mask.clr_bit (read_handle);
      return this->handle_input (read_handle);
    }
  else
    return 0;
}


ACE_HANDLE
ACE_Select_Reactor_Notify::notify_handle (void)
{
  ACE_TRACE ("ACE_Select_Reactor_Notify::notify_handle");

  return this->notification_pipe_.read_handle ();
}


// Special trick to unblock <select> when updates occur in somewhere
// other than the main <ACE_Select_Reactor> thread.  All we do is
// write data to a pipe that the <ACE_Select_Reactor> is listening on.
// Thanks to Paul Stephenson for suggesting this approach.
int
ACE_Select_Reactor_Notify::is_dispatchable (ACE_Notification_Buffer &buffer)
{
   // There is tonnes of code that can be abstracted...
#if defined (ACE_HAS_REACTOR_NOTIFICATION_QUEUE)
  {
    ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, mon, this->notify_queue_lock_, -1);

    ACE_Notification_Buffer *temp;

    ACE_UNUSED_ARG (buffer);

    // If the queue is empty just return 0
    if (notify_queue_.is_empty ())
      return 0;

    if (this->notify_queue_.dequeue_head (temp) == -1)
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_LIB_TEXT ("%p\n"),
                         ACE_LIB_TEXT ("dequeue_head")),
                        -1);
    if (temp->eh_ != 0)
      {
        // If the queue had a buffer that has an event handler, put
        // the element  back in the queue and return a 1
        if (this->notify_queue_.enqueue_head (temp) == -1)
          {
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_LIB_TEXT ("%p\n"),
                               ACE_LIB_TEXT ("enque_head")),
                              -1);
          }

        return 1;
      }
    // Else put the element in the free queue
    if (free_queue_.enqueue_head (temp) == -1)
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_LIB_TEXT ("%p\n"),
                         ACE_LIB_TEXT ("enqueue_head")),
                        -1);
  }
#else
  // If eh == 0 then another thread is unblocking the
  // <ACE_Select_Reactor> to update the <ACE_Select_Reactor>'s
  // internal structures.  Otherwise, we need to dispatch the
  // appropriate handle_* method on the <ACE_Event_Handler>
  // pointer we've been passed.
  if (buffer.eh_ != 0)
    return 1;

#endif /*ACE_HAS_REACTOR_NOTIFICATION_QUEUE */

  // has no dispatchable buffer
  return 0;
}

int
ACE_Select_Reactor_Notify::dispatch_notify (ACE_Notification_Buffer &buffer)
{
  int result = 0;
#if defined (ACE_HAS_REACTOR_NOTIFICATION_QUEUE)
  // Dispatch all messages that are in the <notify_queue_>.
  for (;;)
    {
      ////////@@@@@@@@@@ Bala Need to work from here...
      {
        // We acquire the lock in a block to make sure we're not
        // holding the lock while delivering callbacks...
        ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, mon, this->notify_queue_lock_, -1);

        if (this->notify_queue_.is_empty ())
          return 0;

        ACE_Notification_Buffer *temp;

        if (notify_queue_.dequeue_head (temp) == -1)
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_LIB_TEXT ("%p\n"),
                             ACE_LIB_TEXT ("dequeue_head")),
                            -1);
        buffer = *temp;
        if (free_queue_.enqueue_head (temp) == -1)
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_LIB_TEXT ("%p\n"),
                             ACE_LIB_TEXT ("enqueue_head")),
                            -1);
      }

  // If eh == 0 then another thread is unblocking the
  // <ACE_Select_Reactor> to update the <ACE_Select_Reactor>'s
  // internal structures.  Otherwise, we need to dispatch the
  // appropriate handle_* method on the <ACE_Event_Handler>
  // pointer we've been passed.
  if (buffer.eh_ != 0)
    {

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
          ACE_ERROR ((LM_ERROR, ACE_LIB_TEXT ("invalid mask = %d\n"), buffer.mask_));
        }
      if (result == -1)
        buffer.eh_->handle_close (ACE_INVALID_HANDLE,
                                  ACE_Event_Handler::EXCEPT_MASK);
    }
#else
  // If eh == 0 then another thread is unblocking the
  // <ACE_Select_Reactor> to update the <ACE_Select_Reactor>'s
  // internal structures.  Otherwise, we need to dispatch the
  // appropriate handle_* method on the <ACE_Event_Handler>
  // pointer we've been passed.
  if (buffer.eh_ != 0)
    {
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
        case ACE_Event_Handler::QOS_MASK:
          result = buffer.eh_->handle_qos (ACE_INVALID_HANDLE);
          break;
        case ACE_Event_Handler::GROUP_QOS_MASK:
          result = buffer.eh_->handle_group_qos (ACE_INVALID_HANDLE);
          break;
        default:
          // Should we bail out if we get an invalid mask?
          ACE_ERROR ((LM_ERROR,
                      ACE_LIB_TEXT ("invalid mask = %d\n"),
                      buffer.mask_));
        }
      if (result == -1)
        buffer.eh_->handle_close (ACE_INVALID_HANDLE,
                                  ACE_Event_Handler::EXCEPT_MASK);
    }

#endif /* ACE_HAS_REACTOR_NOTIFICATION_QUEUE */

  return 1;
}

int
ACE_Select_Reactor_Notify::read_notify_pipe (ACE_HANDLE handle,
                                             ACE_Notification_Buffer &buffer)
{
  ACE_TRACE ("ACE_Select_Reactor_Notify::read_notify_pipe");

  ssize_t n = ACE::recv (handle, (char *) &buffer, sizeof buffer);

  if (n > 0)
    {
      // Check to see if we've got a short read.
      if (n != sizeof buffer)
        {
          ssize_t remainder = sizeof buffer - n;

          // If so, try to recover by reading the remainder.  If this
          // doesn't work we're in big trouble since the input stream
          // won't be aligned correctly.  I'm not sure quite what to
          // do at this point.  It's probably best just to return -1.
          if (ACE::recv (handle,
                         ((char *) &buffer) + n,
                         remainder) != remainder)
            return -1;
        }


      return 1;
    }

  // Return -1 if things have gone seriously  wrong.
  if (n <= 0 && (errno != EWOULDBLOCK && errno != EAGAIN))
    return -1;

  return 0;
}


int
ACE_Select_Reactor_Notify::handle_input (ACE_HANDLE handle)
{
  ACE_TRACE ("ACE_Select_Reactor_Notify::handle_input");
  // Precondition: this->select_reactor_.token_.current_owner () ==
  // ACE_Thread::self ();

  int number_dispatched = 0;
  int result = 0;
  ACE_Notification_Buffer buffer;

  while ((result = this->read_notify_pipe (handle, buffer)) > 0)
    {
      // Dispatch the buffer
      // NOTE: We count only if we made any dispatches ie. upcalls.
      if (this->dispatch_notify (buffer) > 0)
        number_dispatched++;

      // Bail out if we've reached the <notify_threshold_>.  Note that
      // by default <notify_threshold_> is -1, so we'll loop until all
      // the notifications in the pipe have been dispatched.
      if (number_dispatched == this->max_notify_iterations_)
        break;
    }

  // Reassign number_dispatched to -1 if things have gone seriously
  // wrong.
  if (result < 0)
    number_dispatched = -1;

  // Enqueue ourselves into the list of waiting threads.  When we
  // reacquire the token we'll be off and running again with ownership
  // of the token.  The postcondition of this call is that
  // <select_reactor_.token_.current_owner> == <ACE_Thread::self>.
  this->select_reactor_->renew ();
  return number_dispatched;
}


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
#if defined (ACE_HAS_REACTOR_NOTIFICATION_QUEUE)
template class ACE_Unbounded_Queue <ACE_Notification_Buffer *>;
template class ACE_Unbounded_Queue_Iterator <ACE_Notification_Buffer *>;
template class ACE_Node <ACE_Notification_Buffer *>;
#endif /* ACE_HAS_REACTOR_NOTIFICATION_QUEUE */
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#if defined (ACE_HAS_REACTOR_NOTIFICATION_QUEUE)
#pragma instantiate ACE_Unbounded_Queue <ACE_Notification_Buffer *>
#pragma instantiate ACE_Unbounded_Queue_Iterator <ACE_Notification_Buffer *>
#pragma instantiate ACE_Node <ACE_Notification_Buffer *>
#endif /* ACE_HAS_REACTOR_NOTIFICATION_QUEUE */
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
