#include "Select_Reactor_Handler_Repository.h"
#include "ACE.h"
#include "Select_Reactor_Base.h"
#include "Reactor.h"

#if !defined (__ACE_INLINE__)
#include "ace/Select_Reactor_Handler_Repository.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(ace,
          Select_Reactor_Handler_Repository,
          "$Id$")

#if defined (ACE_WIN32)
#define ACE_SELECT_REACTOR_HANDLE(H) (this->event_handlers_[(H)].handle_)
#define ACE_SELECT_REACTOR_EVENT_HANDLER(THIS,H) ((THIS)->event_handlers_[(H)].event_handler_)
#else
#define ACE_SELECT_REACTOR_HANDLE(H) (H)
#define ACE_SELECT_REACTOR_EVENT_HANDLER(THIS,H) ((THIS)->event_handlers_[(H)])
#endif /* ACE_WIN32 */


ACE_Select_Reactor_Handler_Repository::~ACE_Select_Reactor_Handler_Repository (void)
{
}

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

ACE_Select_Reactor_Handler_Repository::ACE_Select_Reactor_Handler_Repository (
    ACE_Select_Reactor_Impl &select_reactor)
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

/****************************************************************************/

ACE_Select_Reactor_Handler_Repository_Iterator::~ACE_Select_Reactor_Handler_Repository_Iterator (void)
{
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
