// $Id$

#include "ace/Demux/Select_Reactor_Base.h"
#include "ace/Demux/Reactor.h"
#include "ace/Threads/Thread.h"
#include "ace/Threads/Synch_T.h"
#include "ace/Sockets/SOCK_Acceptor.h"
#include "ace/Sockets/SOCK_Connector.h"
#include "ace/Timer/Timer_Heap.h"

#ifdef ACE_SUBSET_0
#include "ace/Log_Msg.h"
#endif

#if !defined (__ACE_INLINE__)
#include "ace/Demux/Select_Reactor_Base.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(ace, Select_Reactor_Base, "$Id$")


#if defined (ACE_WIN32)
#define ACE_SELECT_REACTOR_HANDLE(H) (this->event_handlers_[(H)].handle_)
#define ACE_SELECT_REACTOR_EVENT_HANDLER(THIS,H) ((THIS)->event_handlers_[(H)].event_handler_)
#else
#define ACE_SELECT_REACTOR_HANDLE(H) (H)
#define ACE_SELECT_REACTOR_EVENT_HANDLER(THIS,H) ((THIS)->event_handlers_[(H)])
#endif /* ACE_WIN32 */

// Performs sanity checking on the ACE_HANDLE.

int
ACE_Select_Reactor_Handler_Repository::invalid_handle (ACE_HANDLE handle)
{
  ACE_TRACE ("ACE_Select_Reactor_Handler_Repository::invalid_handle");
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
ACE_Select_Reactor_Handler_Repository::handle_in_range (ACE_HANDLE handle)
{
  ACE_TRACE ("ACE_Select_Reactor_Handler_Repository::handle_in_range");
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
ACE_Select_Reactor_Handler_Repository::max_handlep1 (void)
{
  ACE_TRACE ("ACE_Select_Reactor_Handler_Repository::max_handlep1");

  return this->max_handlep1_;
}

int
ACE_Select_Reactor_Handler_Repository::open (size_t size)
{
  ACE_TRACE ("ACE_Select_Reactor_Handler_Repository::open");
  this->max_size_ = size;
  this->max_handlep1_ = 0;

#if defined (ACE_WIN32)
  // Try to allocate the memory.
  ACE_NEW_RETURN (this->event_handlers_,
                  ACE_Event_Tuple[size],
                  -1);

  // Initialize the ACE_Event_Handler * to { ACE_INVALID_HANDLE, 0 }.
  for (size_t h = 0; h < size; h++)
    {
      ACE_SELECT_REACTOR_HANDLE (h) = ACE_INVALID_HANDLE;
      ACE_SELECT_REACTOR_EVENT_HANDLER (this, h) = 0;
    }
#else
  // Try to allocate the memory.
  ACE_NEW_RETURN (this->event_handlers_,
                  ACE_Event_Handler *[size],
                  -1);

  // Initialize the ACE_Event_Handler * to NULL.
  for (size_t h = 0; h < size; h++)
    ACE_SELECT_REACTOR_EVENT_HANDLER (this, h) = 0;
#endif /* ACE_WIN32 */

  // Try to increase the number of handles if <size> is greater than
  // the current limit.
  return ACE::set_handle_limit (size);
}

// Initialize a repository of the appropriate <size>.

ACE_Select_Reactor_Handler_Repository::ACE_Select_Reactor_Handler_Repository (ACE_Select_Reactor_Impl &select_reactor)
  : select_reactor_ (select_reactor),
    max_size_ (0),
    max_handlep1_ (0),
    event_handlers_ (0)
{
  ACE_TRACE ("ACE_Select_Reactor_Handler_Repository::ACE_Select_Reactor_Handler_Repository");
}

int
ACE_Select_Reactor_Handler_Repository::unbind_all (void)
{
  // Unbind all of the <handle, ACE_Event_Handler>s.
  for (int handle = 0;
       handle < this->max_handlep1_;
       handle++)
    this->unbind (ACE_SELECT_REACTOR_HANDLE (handle),
                  ACE_Event_Handler::ALL_EVENTS_MASK);

  return 0;
}

int
ACE_Select_Reactor_Handler_Repository::close (void)
{
  ACE_TRACE ("ACE_Select_Reactor_Handler_Repository::close");

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
ACE_Select_Reactor_Handler_Repository::find (ACE_HANDLE handle,
                                             size_t *index_p)
{
  ACE_TRACE ("ACE_Select_Reactor_Handler_Repository::find");

  ACE_Event_Handler *eh = 0;
  ssize_t i;

  // Only bother to search for the <handle> if it's in range.
  if (this->handle_in_range (handle))
    {
#if defined (ACE_WIN32)
      i = 0;

      for (; i < this->max_handlep1_; i++)
        if (ACE_SELECT_REACTOR_HANDLE (i) == handle)
          {
            eh = ACE_SELECT_REACTOR_EVENT_HANDLER (this, i);
            break;
          }
#else
      i = handle;

      eh = ACE_SELECT_REACTOR_EVENT_HANDLER (this, handle);
#endif /* ACE_WIN32 */
    }
  else
    // g++ can't figure out that <i> won't be used below if the handle
    // is out of range, so keep it happy by defining <i> here . . .
    i = 0;

  if (eh != 0)
    {
      if (index_p != 0)
        *index_p = i;
    }
  else
    errno = ENOENT;

  return eh;
}

// Bind the <ACE_Event_Handler *> to the <ACE_HANDLE>.

int
ACE_Select_Reactor_Handler_Repository::bind (ACE_HANDLE handle,
                                             ACE_Event_Handler *event_handler,
                                             ACE_Reactor_Mask mask)
{
  ACE_TRACE ("ACE_Select_Reactor_Handler_Repository::bind");

  if (handle == ACE_INVALID_HANDLE)
    handle = event_handler->get_handle ();

  if (this->invalid_handle (handle))
    return -1;

#if defined (ACE_WIN32)
  int assigned_slot = -1;

  for (ssize_t i = 0; i < this->max_handlep1_; i++)
    {
      // Found it, so let's just reuse this location.
      if (ACE_SELECT_REACTOR_HANDLE (i) == handle)
        {
          assigned_slot = i;
          break;
        }
      // Here's the first free slot, so let's take it.
      else if (ACE_SELECT_REACTOR_HANDLE (i) == ACE_INVALID_HANDLE
               && assigned_slot == -1)
        assigned_slot = i;
    }

  if (assigned_slot > -1)
    // We found a free spot, let's reuse it.
    {
      ACE_SELECT_REACTOR_HANDLE (assigned_slot) = handle;
      ACE_SELECT_REACTOR_EVENT_HANDLER (this, assigned_slot) = event_handler;
    }
  else if (this->max_handlep1_ < this->max_size_)
    {
      // Insert at the end of the active portion.
      ACE_SELECT_REACTOR_HANDLE (this->max_handlep1_) = handle;
      ACE_SELECT_REACTOR_EVENT_HANDLER (this, this->max_handlep1_) = event_handler;
      this->max_handlep1_++;
    }
  else
    {
      // No more room at the inn!
      errno = ENOMEM;
      return -1;
    }
#else
  ACE_SELECT_REACTOR_EVENT_HANDLER (this, handle) = event_handler;

  if (this->max_handlep1_ < handle + 1)
    this->max_handlep1_ = handle + 1;
#endif /* ACE_WIN32 */

  // Add the <mask> for this <handle> in the Select_Reactor's wait_set.
  this->select_reactor_.bit_ops (handle,
                                 mask,
                                 this->select_reactor_.wait_set_,
                                 ACE_Reactor::ADD_MASK);

  // Note the fact that we've changed the state of the <wait_set_>,
  // which is used by the dispatching loop to determine whether it can
  // keep going or if it needs to reconsult select().
  this->select_reactor_.state_changed_ = 1;

  return 0;
}

// Remove the binding of <ACE_HANDLE>.

int
ACE_Select_Reactor_Handler_Repository::unbind (ACE_HANDLE handle,
                                               ACE_Reactor_Mask mask)
{
  ACE_TRACE ("ACE_Select_Reactor_Handler_Repository::unbind");

  size_t slot;
  ACE_Event_Handler *eh = this->find (handle, &slot);

  if (eh == 0)
    return -1;

  // Clear out the <mask> bits in the Select_Reactor's wait_set.
  this->select_reactor_.bit_ops (handle,
                                 mask,
                                 this->select_reactor_.wait_set_,
                                 ACE_Reactor::CLR_MASK);

  // And suspend_set.
  this->select_reactor_.bit_ops (handle,
                                 mask,
                                 this->select_reactor_.suspend_set_,
                                 ACE_Reactor::CLR_MASK);

  // Note the fact that we've changed the state of the <wait_set_>,
  // which is used by the dispatching loop to determine whether it can
  // keep going or if it needs to reconsult select().
  this->select_reactor_.state_changed_ = 1;

  // Close down the <Event_Handler> unless we've been instructed not
  // to.
  if (ACE_BIT_ENABLED (mask, ACE_Event_Handler::DONT_CALL) == 0)
    eh->handle_close (handle, mask);

  // If there are no longer any outstanding events on this <handle>
  // then we can totally shut down the Event_Handler.
  if (this->select_reactor_.wait_set_.rd_mask_.is_set (handle) == 0
      && this->select_reactor_.wait_set_.wr_mask_.is_set (handle) == 0
      && this->select_reactor_.wait_set_.ex_mask_.is_set (handle) == 0)
#if defined (ACE_WIN32)
    {
      ACE_SELECT_REACTOR_HANDLE (slot) = ACE_INVALID_HANDLE;
      ACE_SELECT_REACTOR_EVENT_HANDLER (this, slot) = 0;

      if (this->max_handlep1_ == (int) slot + 1)
        {
          // We've deleted the last entry (i.e., i + 1 == the current
          // size of the array), so we need to figure out the last
          // valid place in the array that we should consider in
          // subsequent searches.

          int i;

          for (i = this->max_handlep1_ - 1;
               i >= 0 && ACE_SELECT_REACTOR_HANDLE (i) == ACE_INVALID_HANDLE;
               i--)
            continue;

          this->max_handlep1_ = i + 1;
        }
    }
#else
  {
    ACE_SELECT_REACTOR_EVENT_HANDLER (this, handle) = 0;

    if (this->max_handlep1_ == handle + 1)
      {
        // We've deleted the last entry, so we need to figure out
        // the last valid place in the array that is worth looking
        // at.
        ACE_HANDLE wait_rd_max = this->select_reactor_.wait_set_.rd_mask_.max_set ();
        ACE_HANDLE wait_wr_max = this->select_reactor_.wait_set_.wr_mask_.max_set ();
        ACE_HANDLE wait_ex_max = this->select_reactor_.wait_set_.ex_mask_.max_set ();

        ACE_HANDLE suspend_rd_max = this->select_reactor_.suspend_set_.rd_mask_.max_set ();
        ACE_HANDLE suspend_wr_max = this->select_reactor_.suspend_set_.wr_mask_.max_set ();
        ACE_HANDLE suspend_ex_max = this->select_reactor_.suspend_set_.ex_mask_.max_set ();

        // Compute the maximum of six values.
        this->max_handlep1_ = wait_rd_max;
        if (this->max_handlep1_ < wait_wr_max)
          this->max_handlep1_ = wait_wr_max;
        if (this->max_handlep1_ < wait_ex_max)
          this->max_handlep1_ = wait_ex_max;

        if (this->max_handlep1_ < suspend_rd_max)
          this->max_handlep1_ = suspend_rd_max;
        if (this->max_handlep1_ < suspend_wr_max)
          this->max_handlep1_ = suspend_wr_max;
        if (this->max_handlep1_ < suspend_ex_max)
          this->max_handlep1_ = suspend_ex_max;

        this->max_handlep1_++;
      }
  }
#endif /* ACE_WIN32 */

  return 0;
}

ACE_Select_Reactor_Handler_Repository_Iterator::ACE_Select_Reactor_Handler_Repository_Iterator
  (const ACE_Select_Reactor_Handler_Repository *s)
    : rep_ (s),
      current_ (-1)
{
  this->advance ();
}

// Pass back the <next_item> that hasn't been seen in the Set.
// Returns 0 when all items have been seen, else 1.

int
ACE_Select_Reactor_Handler_Repository_Iterator::next (ACE_Event_Handler *&next_item)
{
  int result = 1;

  if (this->current_ >= this->rep_->max_handlep1_)
    result = 0;
  else
    next_item = ACE_SELECT_REACTOR_EVENT_HANDLER (this->rep_,
                                                  this->current_);
  return result;
}

int
ACE_Select_Reactor_Handler_Repository_Iterator::done (void) const
{
  return this->current_ >= this->rep_->max_handlep1_;
}

// Move forward by one element in the set.

int
ACE_Select_Reactor_Handler_Repository_Iterator::advance (void)
{
  if (this->current_ < this->rep_->max_handlep1_)
    this->current_++;

  while (this->current_ < this->rep_->max_handlep1_)
    if (ACE_SELECT_REACTOR_EVENT_HANDLER (this->rep_, this->current_) != 0)
      return 1;
    else
      this->current_++;

  return this->current_ < this->rep_->max_handlep1_;
}

// Dump the state of an object.

void
ACE_Select_Reactor_Handler_Repository_Iterator::dump (void) const
{
  ACE_TRACE ("ACE_Select_Reactor_Handler_Repository_Iterator::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("rep_ = %u"), this->rep_));
  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("current_ = %d"), this->current_));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

void
ACE_Select_Reactor_Handler_Repository::dump (void) const
{
  ACE_TRACE ("ACE_Select_Reactor_Handler_Repository::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG,
              ACE_LIB_TEXT ("(%t) max_handlep1_ = %d, max_size_ = %d\n"),
              this->max_handlep1_, this->max_size_));
  ACE_DEBUG ((LM_DEBUG,  ACE_LIB_TEXT ("[")));

  ACE_Event_Handler *eh = 0;

  for (ACE_Select_Reactor_Handler_Repository_Iterator iter (this);
       iter.next (eh) != 0;
       iter.advance ())
    ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT (" (eh = %x, eh->handle_ = %d)"),
                eh, eh->get_handle ()));

  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT (" ]")));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

ACE_ALLOC_HOOK_DEFINE(ACE_Select_Reactor_Handler_Repository_Iterator)

ACE_Select_Reactor_Notify::ACE_Select_Reactor_Notify (void)
  : max_notify_iterations_ (-1)
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

// purge_pending_notifications
// Removes all entries from the notify_queue_ and each one that
// matches <eh> is put on the free_queue_. The rest are saved on a
// local queue and copied back to the notify_queue_ at the end.
// Returns the number of entries removed. Returns -1 on error.
// ACE_NOTSUP_RETURN if ACE_HAS_REACTOR_NOTIFICATION_QUEUE is not defined.
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

      if (select_reactor_ == 0)
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
        return -1;

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

ssize_t
ACE_Select_Reactor_Notify::notify (ACE_Event_Handler *eh,
                                   ACE_Reactor_Mask mask,
                                   ACE_Time_Value *timeout)
{
  ACE_TRACE ("ACE_Select_Reactor_Notify::notify");

  // Just consider this method a "no-op" if there's no
  // <ACE_Select_Reactor> configured.
  if (this->select_reactor_ == 0)
    return 0;
  else
    {
#if defined (ACE_HAS_REACTOR_NOTIFICATION_QUEUE)
      ACE_Notification_Buffer buffer (eh, mask);
      // int notification_required = 0;

      ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, mon, this->notify_queue_lock_, -1);

      // No pending notifications.

      // We will send notify for every message..
      // if (this->notify_queue_.is_empty ())
      // notification_required = 1;

      ACE_Notification_Buffer *temp = 0;

      if (free_queue_.dequeue_head (temp) == -1)
        {
          // Grow the queue of available buffers.
          ACE_Notification_Buffer *temp1;

          ACE_NEW_RETURN (temp1,
                          ACE_Notification_Buffer[ACE_REACTOR_NOTIFICATION_ARRAY_SIZE],
                          -1);

          if (this->alloc_queue_.enqueue_head (temp1) == -1)
            return -1;

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

      if (notify_queue_.enqueue_tail (temp) == -1)
        return -1;

      // Let us send a notify for every message
      // if (notification_required)
        {
          ssize_t n = ACE::send (this->notification_pipe_.write_handle (),
                                 (char *) &buffer,
                                 sizeof buffer,
                                 timeout);
          if (n == -1)
            return -1;
        }
      return 0;
#else
      ACE_Notification_Buffer buffer (eh, mask);

      ssize_t n = ACE::send (this->notification_pipe_.write_handle (),
                             (char *) &buffer,
                             sizeof buffer,
                             timeout);
      if (n == -1)
        return -1;
      else
        return 0;
#endif /* ACE_HAS_REACTOR_NOTIFICATION_QUEUE */
    }
}

// Handles pending threads (if any) that are waiting to unblock the
// Select_Reactor.

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
  {
    // We acquire the lock in a block to make sure we're not
    // holding the lock while delivering callbacks...
    ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, mon, this->notify_queue_lock_, -1);

    ACE_Notification_Buffer *temp;

    if (notify_queue_.is_empty ())
      return 0;
    else if (notify_queue_.dequeue_head (temp) == -1)
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
ACE_Select_Reactor_Impl::bit_ops (ACE_HANDLE handle,
                                  ACE_Reactor_Mask mask,
                                  ACE_Select_Reactor_Handle_Set &handle_set,
                                  int ops)
{
  ACE_TRACE ("ACE_Select_Reactor_Impl::bit_ops");
  if (this->handler_rep_.handle_in_range (handle) == 0)
    return -1;

#if !defined (ACE_WIN32)
  ACE_Sig_Guard sb; // Block out all signals until method returns.
#endif /* ACE_WIN32 */

  ACE_FDS_PTMF ptmf  = &ACE_Handle_Set::set_bit;
  u_long omask = ACE_Event_Handler::NULL_MASK;

  // Find the old reactor masks.  This automatically does the work of
  // the GET_MASK operation.
  if (handle_set.rd_mask_.is_set (handle))
    ACE_SET_BITS (omask, ACE_Event_Handler::READ_MASK);
  if (handle_set.wr_mask_.is_set (handle))
    ACE_SET_BITS (omask, ACE_Event_Handler::WRITE_MASK);
  if (handle_set.ex_mask_.is_set (handle))
    ACE_SET_BITS (omask, ACE_Event_Handler::EXCEPT_MASK);

  switch (ops)
    {
    case ACE_Reactor::GET_MASK:
      // The work for this operation is done in all cases at the
      // begining of the function.
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

      // READ, ACCEPT, and CONNECT flag will place the handle in the
      // read set.
      if (ACE_BIT_ENABLED (mask, ACE_Event_Handler::READ_MASK)
          || ACE_BIT_ENABLED (mask, ACE_Event_Handler::ACCEPT_MASK)
          || ACE_BIT_ENABLED (mask, ACE_Event_Handler::CONNECT_MASK))
        {
          (handle_set.rd_mask_.*ptmf) (handle);
        }
      else if (ops == ACE_Reactor::SET_MASK)
        handle_set.rd_mask_.clr_bit (handle);

      // WRITE and CONNECT flag will place the handle in the write set
      if (ACE_BIT_ENABLED (mask,
                           ACE_Event_Handler::WRITE_MASK)
          || ACE_BIT_ENABLED (mask,
                              ACE_Event_Handler::CONNECT_MASK))
        {
          (handle_set.wr_mask_.*ptmf) (handle);
        }
      else if (ops == ACE_Reactor::SET_MASK)
        handle_set.wr_mask_.clr_bit (handle);

      // EXCEPT (and CONNECT on Win32) flag will place the handle in
      // the except set.
      if (ACE_BIT_ENABLED (mask, ACE_Event_Handler::EXCEPT_MASK)
#if defined (ACE_WIN32)
          || ACE_BIT_ENABLED (mask, ACE_Event_Handler::CONNECT_MASK)
#endif /* ACE_WIN32 */
          )
        {
          (handle_set.ex_mask_.*ptmf) (handle);
        }
      else if (ops == ACE_Reactor::SET_MASK)
        handle_set.ex_mask_.clr_bit (handle);
      break;
    default:
      return -1;
    }
  return omask;
}

int
ACE_Select_Reactor_Impl::resumable_handler (void)
{
  // The select reactor has no handlers that can be resumed by the
  // application. So return 0;

  return 0;
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
