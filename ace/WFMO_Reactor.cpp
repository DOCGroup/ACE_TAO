// $Id$

#include "ace/WFMO_Reactor.h"

#include "ace/Handle_Set.h"
#include "ace/Timer_Heap.h"
#include "ace/Thread.h"

#if !defined (__ACE_INLINE__)
#include "ace/WFMO_Reactor.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(ace, WFMO_Reactor, "$Id$")


#if defined (ACE_WIN32) && !defined (ACE_HAS_WINCE)

#include "ace/Auto_Ptr.h"

ACE_WFMO_Reactor_Handler_Repository::ACE_WFMO_Reactor_Handler_Repository (ACE_WFMO_Reactor &wfmo_reactor)
  : wfmo_reactor_ (wfmo_reactor)
{
}

int
ACE_WFMO_Reactor_Handler_Repository::open (size_t size)
{
  if (size > MAXIMUM_WAIT_OBJECTS)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_LIB_TEXT ("%d exceeds MAXIMUM_WAIT_OBJECTS (%d)\n"),
                       size,
                       MAXIMUM_WAIT_OBJECTS),
                      -1);

  // Dynamic allocation
  ACE_NEW_RETURN (this->current_handles_,
                  ACE_HANDLE[size],
                  -1);
  ACE_NEW_RETURN (this->current_info_,
                  Current_Info[size],
                  -1);
  ACE_NEW_RETURN (this->current_suspended_info_,
                  Suspended_Info[size],
                  -1);
  ACE_NEW_RETURN (this->to_be_added_info_,
                  To_Be_Added_Info[size],
                  -1);

  // Initialization
  this->max_size_ = size;
  this->max_handlep1_ = 0;
  this->suspended_handles_ = 0;
  this->handles_to_be_added_ = 0;
  this->handles_to_be_deleted_ = 0;
  this->handles_to_be_suspended_ = 0;
  this->handles_to_be_resumed_ = 0;

  for (size_t i = 0; i < size; i++)
    this->current_handles_[i] = ACE_INVALID_HANDLE;

  return 0;
}

ACE_WFMO_Reactor_Handler_Repository::~ACE_WFMO_Reactor_Handler_Repository (void)
{
  // Free up dynamically allocated space
  delete [] this->current_handles_;
  delete [] this->current_info_;
  delete [] this->current_suspended_info_;
  delete [] this->to_be_added_info_;
}

ACE_Reactor_Mask
ACE_WFMO_Reactor_Handler_Repository::bit_ops (long &existing_masks,
                                              ACE_Reactor_Mask change_masks,
                                              int operation)
{
  // Find the old reactor masks.  This automatically does the work of
  // the GET_MASK operation.

  ACE_Reactor_Mask old_masks = ACE_Event_Handler::NULL_MASK;

  if (ACE_BIT_ENABLED (existing_masks, FD_READ)
      || ACE_BIT_ENABLED (existing_masks, FD_CLOSE))
    ACE_SET_BITS (old_masks, ACE_Event_Handler::READ_MASK);

  if (ACE_BIT_ENABLED (existing_masks, FD_WRITE))
    ACE_SET_BITS (old_masks, ACE_Event_Handler::WRITE_MASK);

  if (ACE_BIT_ENABLED (existing_masks, FD_OOB))
    ACE_SET_BITS (old_masks, ACE_Event_Handler::EXCEPT_MASK);

  if (ACE_BIT_ENABLED (existing_masks, FD_ACCEPT))
    ACE_SET_BITS (old_masks, ACE_Event_Handler::ACCEPT_MASK);

  if (ACE_BIT_ENABLED (existing_masks, FD_CONNECT))
    ACE_SET_BITS (old_masks, ACE_Event_Handler::CONNECT_MASK);

  if (ACE_BIT_ENABLED (existing_masks, FD_QOS))
    ACE_SET_BITS (old_masks, ACE_Event_Handler::QOS_MASK);

  if (ACE_BIT_ENABLED (existing_masks, FD_GROUP_QOS))
    ACE_SET_BITS (old_masks, ACE_Event_Handler::GROUP_QOS_MASK);

  switch (operation)
    {
    case ACE_Reactor::CLR_MASK:
      // For the CLR_MASK operation, clear only the specific masks.

      if (ACE_BIT_ENABLED (change_masks, ACE_Event_Handler::READ_MASK))
        {
          ACE_CLR_BITS (existing_masks, FD_READ);
          ACE_CLR_BITS (existing_masks, FD_CLOSE);
        }

      if (ACE_BIT_ENABLED (change_masks, ACE_Event_Handler::WRITE_MASK))
        ACE_CLR_BITS (existing_masks, FD_WRITE);

      if (ACE_BIT_ENABLED (change_masks, ACE_Event_Handler::EXCEPT_MASK))
        ACE_CLR_BITS (existing_masks, FD_OOB);

      if (ACE_BIT_ENABLED (change_masks, ACE_Event_Handler::ACCEPT_MASK))
        ACE_CLR_BITS (existing_masks, FD_ACCEPT);

      if (ACE_BIT_ENABLED (change_masks, ACE_Event_Handler::CONNECT_MASK))
        ACE_CLR_BITS (existing_masks, FD_CONNECT);

      if (ACE_BIT_ENABLED (change_masks, ACE_Event_Handler::QOS_MASK))
        ACE_CLR_BITS (existing_masks, FD_QOS);

      if (ACE_BIT_ENABLED (change_masks, ACE_Event_Handler::GROUP_QOS_MASK))
        ACE_CLR_BITS (existing_masks, FD_GROUP_QOS);

      break;

    case ACE_Reactor::SET_MASK:
      // If the operation is a set, first reset any existing masks

      existing_masks = 0;
      /* FALLTHRU */

    case ACE_Reactor::ADD_MASK:
      // For the ADD_MASK and the SET_MASK operation, add only the
      // specific masks.

      if (ACE_BIT_ENABLED (change_masks, ACE_Event_Handler::READ_MASK))
        {
          ACE_SET_BITS (existing_masks, FD_READ);
          ACE_SET_BITS (existing_masks, FD_CLOSE);
        }

      if (ACE_BIT_ENABLED (change_masks, ACE_Event_Handler::WRITE_MASK))
        ACE_SET_BITS (existing_masks, FD_WRITE);

      if (ACE_BIT_ENABLED (change_masks, ACE_Event_Handler::EXCEPT_MASK))
        ACE_SET_BITS (existing_masks, FD_OOB);

      if (ACE_BIT_ENABLED (change_masks, ACE_Event_Handler::ACCEPT_MASK))
        ACE_SET_BITS (existing_masks, FD_ACCEPT);

      if (ACE_BIT_ENABLED (change_masks, ACE_Event_Handler::CONNECT_MASK))
        ACE_SET_BITS (existing_masks, FD_CONNECT);

      if (ACE_BIT_ENABLED (change_masks, ACE_Event_Handler::QOS_MASK))
        ACE_SET_BITS (existing_masks, FD_QOS);

      if (ACE_BIT_ENABLED (change_masks, ACE_Event_Handler::GROUP_QOS_MASK))
        ACE_SET_BITS (existing_masks, FD_GROUP_QOS);

      break;

    case ACE_Reactor::GET_MASK:

      // The work for this operation is done in all cases at the
      // begining of the function.

      ACE_UNUSED_ARG (change_masks);

      break;
    }

  return old_masks;
}

int
ACE_WFMO_Reactor_Handler_Repository::unbind_i (ACE_HANDLE handle,
                                               ACE_Reactor_Mask mask,
                                               int &changes_required)
{
  int error = 0;

  // Remember this value; only if it changes do we need to wakeup
  // the other threads
  size_t original_handle_count = this->handles_to_be_deleted_;
  int result = 0;
  size_t i;

  // Go through all the handles looking for <handle>.  Even if we find
  // it, we continue through the rest of the list since <handle> could
  // appear multiple times. All handles are checked.

  // First check the current entries
  for (i = 0; i < this->max_handlep1_ && error == 0; i++)
    // Since the handle can either be the event or the I/O handle,
    // we have to check both
    if ((this->current_handles_[i] == handle
         || this->current_info_[i].io_handle_ == handle)
        && // Make sure that it is not already marked for deleted
        !this->current_info_[i].delete_entry_)
      {
        result = this->remove_handler_i (i,
                                         mask);
        if (result == -1)
          error = 1;
      }

  // Then check the suspended entries
  for (i = 0; i < this->suspended_handles_ && error == 0; i++)
    // Since the handle can either be the event or the I/O handle, we
    // have to check both
    if ((this->current_suspended_info_[i].io_handle_ == handle
         || this->current_suspended_info_[i].event_handle_ == handle)
        &&
        // Make sure that it is not already marked for deleted
        !this->current_suspended_info_[i].delete_entry_)
      {
        result = this->remove_suspended_handler_i (i,
                                                   mask);
        if (result == -1)
          error = 1;
      }

  // Then check the to_be_added entries
  for (i = 0; i < this->handles_to_be_added_ && error == 0; i++)
    // Since the handle can either be the event or the I/O handle,
    // we have to check both
    if ((this->to_be_added_info_[i].io_handle_ == handle
         || this->to_be_added_info_[i].event_handle_ == handle)
        &&
        // Make sure that it is not already marked for deleted
        !this->to_be_added_info_[i].delete_entry_)
      {
        result = this->remove_to_be_added_handler_i (i,
                                                     mask);
        if (result == -1)
          error = 1;
      }

  // Only if the number of handlers to be deleted changes do we need
  // to wakeup the other threads
  if (original_handle_count < this->handles_to_be_deleted_)
    changes_required = 1;

  return error ? -1 : 0;
}

int
ACE_WFMO_Reactor_Handler_Repository::remove_handler_i (size_t slot,
                                                       ACE_Reactor_Mask to_be_removed_masks)
{
  // I/O entries
  if (this->current_info_[slot].io_entry_)
    {
      // See if there are other events that the <Event_Handler> is
      // interested in
      this->bit_ops (this->current_info_[slot].network_events_,
                     to_be_removed_masks,
                     ACE_Reactor::CLR_MASK);

      // Disassociate/Reassociate the event from/with the I/O handle.
      // This will depend on the value of remaining set of network
      // events that the <event_handler> is interested in. I don't
      // think we can do anything about errors here, so I will not
      // check this.
      ::WSAEventSelect ((SOCKET) this->current_info_[slot].io_handle_,
                        this->current_handles_[slot],
                        this->current_info_[slot].network_events_);
    }
  // Normal event entries.
  else if (ACE_BIT_ENABLED (to_be_removed_masks, ACE_Event_Handler::DONT_CALL))
    // Preserve DONT_CALL
    to_be_removed_masks = ACE_Event_Handler::DONT_CALL;
  else
    // Make sure that the <to_be_removed_masks> is the NULL_MASK
    to_be_removed_masks = ACE_Event_Handler::NULL_MASK;

  // If this event was marked for suspension, undo the suspension flag
  // and reduce the to be suspended count.
  if (this->current_info_[slot].suspend_entry_)
    {
      // Undo suspension
      this->current_info_[slot].suspend_entry_ = 0;
      // Decrement the handle count
      this->handles_to_be_suspended_--;
    }

  // If there are no more events that the <Event_Handler> is
  // interested in, or this is a non-I/O entry, schedule the
  // <Event_Handler> for removal
  if (this->current_info_[slot].network_events_ == 0)
    {
      // Mark to be deleted
      this->current_info_[slot].delete_entry_ = 1;
      // Remember the mask
      this->current_info_[slot].close_masks_ = to_be_removed_masks;
      // Increment the handle count
      this->handles_to_be_deleted_++;
    }

  // Since it is not a complete removal, we'll call handle_close
  // for all the masks that were removed.  This does not change
  // the internal state of the reactor.
  //
  // Note: this condition only applies to I/O entries
  else if (ACE_BIT_ENABLED (to_be_removed_masks, ACE_Event_Handler::DONT_CALL) == 0)
    {
      ACE_HANDLE handle = this->current_info_[slot].io_handle_;
      this->current_info_[slot].event_handler_->handle_close (handle,
                                                              to_be_removed_masks);
    }

  return 0;
}

int
ACE_WFMO_Reactor_Handler_Repository::remove_suspended_handler_i (size_t slot,
                                                                 ACE_Reactor_Mask to_be_removed_masks)
{
  // I/O entries
  if (this->current_suspended_info_[slot].io_entry_)
    {
      // See if there are other events that the <Event_Handler> is
      // interested in
      this->bit_ops (this->current_suspended_info_[slot].network_events_,
                     to_be_removed_masks,
                     ACE_Reactor::CLR_MASK);

      // Disassociate/Reassociate the event from/with the I/O handle.
      // This will depend on the value of remaining set of network
      // events that the <event_handler> is interested in. I don't
      // think we can do anything about errors here, so I will not
      // check this.
      ::WSAEventSelect ((SOCKET) this->current_suspended_info_[slot].io_handle_,
                        this->current_suspended_info_[slot].event_handle_,
                        this->current_suspended_info_[slot].network_events_);
    }
  // Normal event entries.
  else if (ACE_BIT_ENABLED (to_be_removed_masks, ACE_Event_Handler::DONT_CALL))
    // Preserve DONT_CALL
    to_be_removed_masks = ACE_Event_Handler::DONT_CALL;
  else
    // Make sure that the <to_be_removed_masks> is the NULL_MASK
    to_be_removed_masks = ACE_Event_Handler::NULL_MASK;

  // If this event was marked for resumption, undo the resumption flag
  // and reduce the to be resumed count.
  if (this->current_suspended_info_[slot].resume_entry_)
    {
      // Undo resumption
      this->current_suspended_info_[slot].resume_entry_ = 0;
      // Decrement the handle count
      this->handles_to_be_resumed_--;
    }

  // If there are no more events that the <Event_Handler> is
  // interested in, or this is a non-I/O entry, schedule the
  // <Event_Handler> for removal
  if (this->current_suspended_info_[slot].network_events_ == 0)
    {
      // Mark to be deleted
      this->current_suspended_info_[slot].delete_entry_ = 1;
      // Remember the mask
      this->current_suspended_info_[slot].close_masks_ = to_be_removed_masks;
      // Increment the handle count
      this->handles_to_be_deleted_++;
    }
  // Since it is not a complete removal, we'll call handle_close for
  // all the masks that were removed.  This does not change the
  // internal state of the reactor.
  //
  // Note: this condition only applies to I/O entries
  else if (ACE_BIT_ENABLED (to_be_removed_masks, ACE_Event_Handler::DONT_CALL) == 0)
    {
      ACE_HANDLE handle = this->current_suspended_info_[slot].io_handle_;
      this->current_suspended_info_[slot].event_handler_->handle_close (handle,
                                                                        to_be_removed_masks);
    }

  return 0;
}

int
ACE_WFMO_Reactor_Handler_Repository::remove_to_be_added_handler_i (size_t slot,
                                                                   ACE_Reactor_Mask to_be_removed_masks)
{
  // I/O entries
  if (this->to_be_added_info_[slot].io_entry_)
    {
      // See if there are other events that the <Event_Handler> is
      // interested in
      this->bit_ops (this->to_be_added_info_[slot].network_events_,
                     to_be_removed_masks,
                     ACE_Reactor::CLR_MASK);

      // Disassociate/Reassociate the event from/with the I/O handle.
      // This will depend on the value of remaining set of network
      // events that the <event_handler> is interested in. I don't
      // think we can do anything about errors here, so I will not
      // check this.
      ::WSAEventSelect ((SOCKET) this->to_be_added_info_[slot].io_handle_,
                        this->to_be_added_info_[slot].event_handle_,
                        this->to_be_added_info_[slot].network_events_);
    }
  // Normal event entries.
  else if (ACE_BIT_ENABLED (to_be_removed_masks, ACE_Event_Handler::DONT_CALL))
    // Preserve DONT_CALL
    to_be_removed_masks = ACE_Event_Handler::DONT_CALL;
  else
    // Make sure that the <to_be_removed_masks> is the NULL_MASK
    to_be_removed_masks = ACE_Event_Handler::NULL_MASK;

  // If this event was marked for suspension, undo the suspension flag
  // and reduce the to be suspended count.
  if (this->to_be_added_info_[slot].suspend_entry_)
    {
      // Undo suspension
      this->to_be_added_info_[slot].suspend_entry_ = 0;
      // Decrement the handle count
      this->handles_to_be_suspended_--;
    }

  // If there are no more events that the <Event_Handler> is
  // interested in, or this is a non-I/O entry, schedule the
  // <Event_Handler> for removal
  if (this->to_be_added_info_[slot].network_events_ == 0)
    {
      // Mark to be deleted
      this->to_be_added_info_[slot].delete_entry_ = 1;
      // Remember the mask
      this->to_be_added_info_[slot].close_masks_ = to_be_removed_masks;
      // Increment the handle count
      this->handles_to_be_deleted_++;
    }
  // Since it is not a complete removal, we'll call handle_close
  // for all the masks that were removed.  This does not change
  // the internal state of the reactor.
  //
  // Note: this condition only applies to I/O entries
  else if (ACE_BIT_ENABLED (to_be_removed_masks, ACE_Event_Handler::DONT_CALL) == 0)
    {
      ACE_HANDLE handle = this->to_be_added_info_[slot].io_handle_;
      this->to_be_added_info_[slot].event_handler_->handle_close (handle,
                                                                  to_be_removed_masks);
    }

  return 0;
}

int
ACE_WFMO_Reactor_Handler_Repository::suspend_handler_i (ACE_HANDLE handle,
                                                        int &changes_required)
{
  size_t i = 0;

  // Go through all the handles looking for <handle>.  Even if we find
  // it, we continue through the rest of the list since <handle> could
  // appear multiple times. All handles are checked.

  // Check the current entries first.
  for (i = 0; i < this->max_handlep1_; i++)
    // Since the handle can either be the event or the I/O handle,
    // we have to check both
    if ((this->current_handles_[i] == handle ||
         this->current_info_[i].io_handle_ == handle) &&
        // Make sure that it is not already marked for suspension
        !this->current_info_[i].suspend_entry_)
      {
        // Mark to be suspended
        this->current_info_[i].suspend_entry_ = 1;
        // Increment the handle count
        this->handles_to_be_suspended_++;
        // Changes will be required
        changes_required = 1;
      }

  // Then check the suspended entries.
  for (i = 0; i < this->suspended_handles_; i++)
    // Since the handle can either be the event or the I/O handle,
    // we have to check both
    if ((this->current_suspended_info_[i].event_handle_ == handle ||
         this->current_suspended_info_[i].io_handle_ == handle) &&
        // Make sure that the resumption is not already undone
        this->current_suspended_info_[i].resume_entry_)
      {
        // Undo resumption
        this->current_suspended_info_[i].resume_entry_ = 0;
        // Decrement the handle count
        this->handles_to_be_resumed_--;
        // Changes will be required
        changes_required = 1;
      }

  // Then check the to_be_added entries.
  for (i = 0; i < this->handles_to_be_added_; i++)
    // Since the handle can either be the event or the I/O handle,
    // we have to check both
    if ((this->to_be_added_info_[i].io_handle_ == handle ||
         this->to_be_added_info_[i].event_handle_ == handle) &&
        // Make sure that it is not already marked for suspension
        !this->to_be_added_info_[i].suspend_entry_)
      {
        // Mark to be suspended
        this->to_be_added_info_[i].suspend_entry_ = 1;
        // Increment the handle count
        this->handles_to_be_suspended_++;
        // Changes will be required
        changes_required = 1;
      }

  return 0;
}

int
ACE_WFMO_Reactor_Handler_Repository::resume_handler_i (ACE_HANDLE handle,
                                                       int &changes_required)
{
  size_t i = 0;

  // Go through all the handles looking for <handle>.  Even if we find
  // it, we continue through the rest of the list since <handle> could
  // appear multiple times. All handles are checked.

  // Check the current entries first.
  for (i = 0; i < this->max_handlep1_; i++)
    // Since the handle can either be the event or the I/O handle,
    // we have to check both
    if ((this->current_handles_[i] == handle ||
         this->current_info_[i].io_handle_ == handle) &&
        // Make sure that the suspension is not already undone
        this->current_info_[i].suspend_entry_)
      {
        // Undo suspension
        this->current_info_[i].suspend_entry_ = 0;
        // Decrement the handle count
        this->handles_to_be_suspended_--;
        // Changes will be required
        changes_required = 1;
      }

  // Then check the suspended entries.
  for (i = 0; i < this->suspended_handles_; i++)
    // Since the handle can either be the event or the I/O handle,
    // we have to check both
    if ((this->current_suspended_info_[i].event_handle_ == handle ||
         this->current_suspended_info_[i].io_handle_ == handle) &&
        // Make sure that it is not already marked for resumption
        !this->current_suspended_info_[i].resume_entry_)
      {
        // Mark to be resumed
        this->current_suspended_info_[i].resume_entry_ = 1;
        // Increment the handle count
        this->handles_to_be_resumed_++;
        // Changes will be required
        changes_required = 1;
      }

  // Then check the to_be_added entries.
  for (i = 0; i < this->handles_to_be_added_; i++)
    // Since the handle can either be the event or the I/O handle,
    // we have to check both
    if ((this->to_be_added_info_[i].io_handle_ == handle ||
         this->to_be_added_info_[i].event_handle_ == handle) &&
        // Make sure that the suspension is not already undone
        this->to_be_added_info_[i].suspend_entry_)
      {
        // Undo suspension
        this->to_be_added_info_[i].suspend_entry_ = 0;
        // Decrement the handle count
        this->handles_to_be_suspended_--;
        // Changes will be required
        changes_required = 1;
      }

  return 0;
}

void
ACE_WFMO_Reactor_Handler_Repository::unbind_all (void)
{
  {
    ACE_GUARD (ACE_Process_Mutex, ace_mon, this->wfmo_reactor_.lock_);

    int dummy;
    size_t i;

    // Remove all the current handlers
    for (i = 0; i < this->max_handlep1_; i++)
      this->unbind_i (this->current_handles_[i],
                      ACE_Event_Handler::ALL_EVENTS_MASK,
                      dummy);

    // Remove all the suspended handlers
    for (i = 0; i < this->suspended_handles_; i++)
      this->unbind_i (this->current_suspended_info_[i].event_handle_,
                      ACE_Event_Handler::ALL_EVENTS_MASK,
                      dummy);

    // Remove all the to_be_added handlers
    for (i = 0; i < this->handles_to_be_added_; i++)
      this->unbind_i (this->to_be_added_info_[i].event_handle_,
                      ACE_Event_Handler::ALL_EVENTS_MASK,
                      dummy);

  }

  // The guard is released here

  // Wake up all threads in WaitForMultipleObjects so that they can
  // reconsult the handle set
  this->wfmo_reactor_.wakeup_all_threads ();
}

int
ACE_WFMO_Reactor_Handler_Repository::bind_i (int io_entry,
                                             ACE_Event_Handler *event_handler,
                                             long network_events,
                                             ACE_HANDLE io_handle,
                                             ACE_HANDLE event_handle,
                                             int delete_event)
{
  // Make sure that the <handle> is valid
  if (event_handle == ACE_INVALID_HANDLE)
    event_handle = event_handler->get_handle ();
  if (this->invalid_handle (event_handle))
    return -1;

  size_t current_size = this->max_handlep1_ +
    this->handles_to_be_added_ -
    this->handles_to_be_deleted_ +
    this->suspended_handles_;

  // Make sure that there's room in the table.
  if (current_size < this->max_size_)
    {
      // Cache this set into the <to_be_added_info_>, till we come
      // around to actually adding this to the <current_info_>
      this->to_be_added_info_[this->handles_to_be_added_].set (event_handle,
                                                               io_entry,
                                                               event_handler,
                                                               io_handle,
                                                               network_events,
                                                               delete_event);

      this->handles_to_be_added_++;

      // Wake up all threads in WaitForMultipleObjects so that they can
      // reconsult the handle set
      this->wfmo_reactor_.wakeup_all_threads ();
    }
  else
    {
      errno = EMFILE;   // File descriptor table is full (better than nothing)
      return -1;
    }

  return 0;
}

int
ACE_WFMO_Reactor_Handler_Repository::make_changes_in_current_infos (void)
{
  // Go through the entire valid array and check for all handles that
  // have been schedule for deletion
  if (this->handles_to_be_deleted_ > 0 || this->handles_to_be_suspended_ > 0)
    {
      // This will help us in keeping track of the last valid slot in the
      // handle arrays
      int last_valid_slot = this->max_handlep1_ - 1;

      for (int i = last_valid_slot; i >= 0; i--)
        {
          // This stuff is necessary here, since we should not make
          // the upcall until all the internal data structures have
          // been updated.  This is to protect against upcalls that
          // try to deregister again.
          ACE_HANDLE handle = ACE_INVALID_HANDLE;
          ACE_Reactor_Mask masks = ACE_Event_Handler::NULL_MASK;
          ACE_Event_Handler *event_handler = 0;

          // See if this entry is scheduled for deletion
          if (this->current_info_[i].delete_entry_)
            {
              // Calling the <handle_close> method here will ensure that we
              // will only call it once per deregistering <Event_Handler>.
              // This is essential in the case when the <Event_Handler> will
              // do something like delete itself and we have multiple
              // threads in WFMO_Reactor.
              //
              // Make sure that the DONT_CALL mask is not set
              masks = this->current_info_[i].close_masks_;
              if (ACE_BIT_ENABLED (masks, ACE_Event_Handler::DONT_CALL) == 0)
                {
                  // Grab the correct handle depending on the type entry
                  if (this->current_info_[i].io_entry_)
                    handle = this->current_info_[i].io_handle_;
                  else
                    handle = this->current_handles_[i];

                  // Event handler
                  event_handler = this->current_info_[i].event_handler_;
                }

              // If <WFMO_Reactor> created the event, we need to clean it up
              if (this->current_info_[i].delete_event_)
                ACE_OS::event_destroy (&this->current_handles_[i]);

              // Reduce count by one
              this->handles_to_be_deleted_--;
            }

          // See if this entry is scheduled for suspension
          else if (this->current_info_[i].suspend_entry_)
            {
              this->current_suspended_info_ [this->suspended_handles_].set (this->current_handles_[i],
                                                                            this->current_info_[i]);
              // Increase number of suspended handles
              this->suspended_handles_++;

              // Reduce count by one
              this->handles_to_be_suspended_--;
            }

          // See if this entry is scheduled for deletion or suspension
          // If so we need to clean up
          if (this->current_info_[i].delete_entry_ || this->current_info_[i].suspend_entry_)
            {
              if (i == last_valid_slot)
                // If this is the last handle in the set, no need to swap
                // places. Simply remove it.
                {
                  // Reset the info in this slot
                  this->current_info_[i].reset ();
                  this->current_handles_[i] = ACE_INVALID_HANDLE;
                }
              else
                // Swap this handle with the last valid handle
                {
                  // Struct copy
                  this->current_info_[i] = this->current_info_[last_valid_slot];
                  this->current_handles_[i] = this->current_handles_[last_valid_slot];
                  // Reset the info in the last slot
                  this->current_info_[last_valid_slot].reset ();
                  this->current_handles_[last_valid_slot] = ACE_INVALID_HANDLE;
                }
              // Reset the last valid slot and clean up the entry in the
              // <to_be_deleted_set_>
              last_valid_slot--;
            }

          // Now that all internal structures have been updated, make
          // the upcall.
          if (event_handler != 0)
            event_handler->handle_close (handle, masks);
        }
      // Reset <this->max_handlep1_>
      this->max_handlep1_ = last_valid_slot + 1;
    }

  return 0;
}

int
ACE_WFMO_Reactor_Handler_Repository::make_changes_in_suspension_infos (void)
{
  int i;

  // Go through the <suspended_handle> array
  if (this->handles_to_be_deleted_ > 0 || this->handles_to_be_resumed_ > 0)
    {
      int last_valid_slot = this->suspended_handles_ - 1;
      for (i = last_valid_slot; i >= 0; i--)
        {
          // This stuff is necessary here, since we should not make
          // the upcall until all the internal data structures have
          // been updated.  This is to protect against upcalls that
          // try to deregister again.
          ACE_HANDLE handle = ACE_INVALID_HANDLE;
          ACE_Reactor_Mask masks = ACE_Event_Handler::NULL_MASK;
          ACE_Event_Handler *event_handler = 0;

          // See if this entry is scheduled for deletion
          if (this->current_suspended_info_[i].delete_entry_)
            {
              // Calling the <handle_close> method here will ensure that we
              // will only call it once per deregistering <Event_Handler>.
              // This is essential in the case when the <Event_Handler> will
              // do something like delete itself and we have multiple
              // threads in WFMO_Reactor.
              //
              // Make sure that the DONT_CALL mask is not set
              masks = this->current_suspended_info_[i].close_masks_;
              if (ACE_BIT_ENABLED (masks, ACE_Event_Handler::DONT_CALL) == 0)
                {
                  // Grab the correct handle depending on the type entry
                  if (this->current_suspended_info_[i].io_entry_)
                    handle = this->current_suspended_info_[i].io_handle_;
                  else
                    handle = this->current_suspended_info_[i].event_handle_;

                  // Upcall
                  event_handler = this->current_suspended_info_[i].event_handler_;
                }

              // If <WFMO_Reactor> created the event, we need to clean it up
              if (this->current_suspended_info_[i].delete_event_)
                ACE_OS::event_destroy (&this->current_suspended_info_[i].event_handle_);

              // Reduce count by one
              this->handles_to_be_deleted_--;
            }

          else if (this->current_suspended_info_[i].resume_entry_)
            {
              // Add to the end of the current handles set
              this->current_handles_[this->max_handlep1_] = this->current_suspended_info_[i].event_handle_;
              // Struct copy
              this->current_info_[this->max_handlep1_].set (this->current_suspended_info_[i]);
              this->max_handlep1_++;

              // Reduce count by one
              this->handles_to_be_resumed_--;
            }

          if (this->current_suspended_info_[i].resume_entry_ ||
              this->current_suspended_info_[i].delete_entry_)
            {
              // Is this the last entry
              if (i == last_valid_slot)
                // Reset the <suspended> arrays entries
                this->current_suspended_info_[i].reset ();
              else
                {
                  // Struct copy
                  this->current_suspended_info_[i] = this->current_suspended_info_[last_valid_slot];
                  this->current_suspended_info_[last_valid_slot].reset ();
                }
              // Reduce the number of suspended handles
              last_valid_slot--;
            }

          // Now that all internal structures have been updated, make
          // the upcall.
          if (event_handler != 0)
            event_handler->handle_close (handle, masks);
        }

      // Reset <this->suspended_handles_>
      this->suspended_handles_ = last_valid_slot + 1;
    }

  return 0;
}

int
ACE_WFMO_Reactor_Handler_Repository::make_changes_in_to_be_added_infos (void)
{
  int i;

  // Go through the <to_be_added_*> arrays
  for (i = 0; i < (int) this->handles_to_be_added_; i++)
    {
      // This stuff is necessary here, since we should not make
      // the upcall until all the internal data structures have
      // been updated.  This is to protect against upcalls that
      // try to deregister again.
      ACE_HANDLE handle = ACE_INVALID_HANDLE;
      ACE_Reactor_Mask masks = ACE_Event_Handler::NULL_MASK;
      ACE_Event_Handler *event_handler = 0;

      // See if this entry is scheduled for deletion
      if (this->to_be_added_info_[i].delete_entry_)
        {
          // Calling the <handle_close> method here will ensure that we
          // will only call it once per deregistering <Event_Handler>.
          // This is essential in the case when the <Event_Handler> will
          // do something like delete itself and we have multiple
          // threads in WFMO_Reactor.
          //
          // Make sure that the DONT_CALL mask is not set
          masks = this->to_be_added_info_[i].close_masks_;
          if (ACE_BIT_ENABLED (masks, ACE_Event_Handler::DONT_CALL) == 0)
            {
              // Grab the correct handle depending on the type entry
              if (this->to_be_added_info_[i].io_entry_)
                handle = this->to_be_added_info_[i].io_handle_;
              else
                handle = this->to_be_added_info_[i].event_handle_;

              // Upcall
              event_handler = this->to_be_added_info_[i].event_handler_;
            }

          // If <WFMO_Reactor> created the event, we need to clean it up
          if (this->to_be_added_info_[i].delete_event_)
            ACE_OS::event_destroy (&this->to_be_added_info_[i].event_handle_);

          // Reduce count by one
          this->handles_to_be_deleted_--;
        }

      // See if this entry is scheduled for suspension
      else if (this->to_be_added_info_[i].suspend_entry_)
        {
          this->current_suspended_info_ [this->suspended_handles_].set (this->to_be_added_info_[i].event_handle_,
                                                                        this->to_be_added_info_[i]);
          // Increase number of suspended handles
          this->suspended_handles_++;

          // Reduce count by one
          this->handles_to_be_suspended_--;
        }

      // If neither of the two flags are on, add to current
      else
        {
          // Add to the end of the current handles set
          this->current_handles_[this->max_handlep1_] = this->to_be_added_info_[i].event_handle_;
          // Struct copy
          this->current_info_[this->max_handlep1_].set (this->to_be_added_info_[i]);
          this->max_handlep1_++;
        }

      // Reset the <to_be_added_info_>
      this->to_be_added_info_[i].reset ();

      // Now that all internal structures have been updated, make the
      // upcall.
      if (event_handler != 0)
        event_handler->handle_close (handle, masks);
    }

  // Since all to be added handles have been taken care of, reset the
  // counter
  this->handles_to_be_added_ = 0;

  return 0;
}

void
ACE_WFMO_Reactor_Handler_Repository::dump (void) const
{
  size_t i = 0;

  ACE_TRACE ("ACE_WFMO_Reactor_Handler_Repository::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));

  ACE_DEBUG ((LM_DEBUG,
              ACE_LIB_TEXT ("Max size = %d\n"),
              this->max_size_));

  ACE_DEBUG ((LM_DEBUG,
              ACE_LIB_TEXT ("Current info table\n\n")));
  ACE_DEBUG ((LM_DEBUG,
              ACE_LIB_TEXT ("\tSize = %d\n"),
              this->max_handlep1_));
  ACE_DEBUG ((LM_DEBUG,
              ACE_LIB_TEXT ("\tHandles to be suspended = %d\n"),
              this->handles_to_be_suspended_));

  for (i = 0; i < this->max_handlep1_; i++)
    this->current_info_[i].dump (this->current_handles_[i]);

  ACE_DEBUG ((LM_DEBUG,
              ACE_LIB_TEXT ("\n")));

  ACE_DEBUG ((LM_DEBUG,
              ACE_LIB_TEXT ("To-be-added info table\n\n")));
  ACE_DEBUG ((LM_DEBUG,
              ACE_LIB_TEXT ("\tSize = %d\n"),
              this->handles_to_be_added_));

  for (i = 0; i < this->handles_to_be_added_; i++)
    this->to_be_added_info_[i].dump ();

  ACE_DEBUG ((LM_DEBUG,
              ACE_LIB_TEXT ("\n")));

  ACE_DEBUG ((LM_DEBUG,
              ACE_LIB_TEXT ("Suspended info table\n\n")));
  ACE_DEBUG ((LM_DEBUG,
              ACE_LIB_TEXT ("\tSize = %d\n"),
              this->suspended_handles_));
  ACE_DEBUG ((LM_DEBUG,
              ACE_LIB_TEXT ("\tHandles to be resumed = %d\n"),
              this->handles_to_be_resumed_));

  for (i = 0; i < this->suspended_handles_; i++)
    this->current_suspended_info_[i].dump ();

  ACE_DEBUG ((LM_DEBUG,
              ACE_LIB_TEXT ("\n")));

  ACE_DEBUG ((LM_DEBUG,
              ACE_LIB_TEXT ("Total handles to be deleted = %d\n"),
              this->handles_to_be_deleted_));

  ACE_DEBUG ((LM_DEBUG,
              ACE_END_DUMP));
}

/************************************************************/

int
ACE_WFMO_Reactor::work_pending (const ACE_Time_Value &)
{
  ACE_NOTSUP_RETURN (-1);
}

ACE_WFMO_Reactor::ACE_WFMO_Reactor (ACE_Sig_Handler *sh,
                                    ACE_Timer_Queue *tq)
  : signal_handler_ (0),
    delete_signal_handler_ (0),
    timer_queue_ (0),
    delete_timer_queue_ (0),
    delete_handler_rep_ (0),
    delete_notify_handler_ (0),
    lock_adapter_ (lock_),
    handler_rep_ (*this),
    // this event is initially signaled
    ok_to_wait_ (1),
    // this event is initially unsignaled
    wakeup_all_threads_ (0),
    // this event is initially unsignaled
    waiting_to_change_state_ (0),
    active_threads_ (0),
    owner_ (ACE_Thread::self ()),
    new_owner_ (0),
    change_state_thread_ (0),
    open_for_business_ (0),
    deactivated_ (0)
{
  if (this->open (ACE_WFMO_Reactor::DEFAULT_SIZE, 0, sh, tq) == -1)
    ACE_ERROR ((LM_ERROR,
                ACE_LIB_TEXT ("%p\n"),
                ACE_LIB_TEXT ("WFMO_Reactor")));
}

ACE_WFMO_Reactor::ACE_WFMO_Reactor (size_t size,
                                    int unused,
                                    ACE_Sig_Handler *sh,
                                    ACE_Timer_Queue *tq)
  : signal_handler_ (0),
    delete_signal_handler_ (0),
    timer_queue_ (0),
    delete_timer_queue_ (0),
    delete_handler_rep_ (0),
    delete_notify_handler_ (0),
    lock_adapter_ (lock_),
    handler_rep_ (*this),
    // this event is initially signaled
    ok_to_wait_ (1),
    // this event is initially unsignaled
    wakeup_all_threads_ (0),
    // this event is initially unsignaled
    waiting_to_change_state_ (0),
    active_threads_ (0),
    owner_ (ACE_Thread::self ()),
    new_owner_ (0),
    change_state_thread_ (0),
    open_for_business_ (0),
    deactivated_ (0)
{
  ACE_UNUSED_ARG (unused);

  if (this->open (size, 0, sh, tq) == -1)
    ACE_ERROR ((LM_ERROR,
                ACE_LIB_TEXT ("%p\n"),
                ACE_LIB_TEXT ("WFMO_Reactor")));
}

int
ACE_WFMO_Reactor::current_info (ACE_HANDLE, size_t &)
{
  return -1;
}

int
ACE_WFMO_Reactor::open (size_t size,
                        int unused,
                        ACE_Sig_Handler *sh,
                        ACE_Timer_Queue *tq,
                        int disable_notify_pipe,
                        ACE_Reactor_Notify *notify)
{
  ACE_UNUSED_ARG (unused);
  ACE_UNUSED_ARG (disable_notify_pipe);

  // This GUARD is necessary since we are updating shared state.
  ACE_GUARD_RETURN (ACE_Process_Mutex, ace_mon, this->lock_, -1);

  // If we are already open, return -1
  if (this->open_for_business_)
    return -1;

  // Timer Queue
  if (this->delete_timer_queue_)
    delete this->timer_queue_;

  if (tq == 0)
    {
      ACE_NEW_RETURN (this->timer_queue_,
                      ACE_Timer_Heap,
                      -1);
      this->delete_timer_queue_ = 1;
    }
  else
    {
      this->timer_queue_ = tq;
      this->delete_timer_queue_ = 0;
    }

  // Signal Handler
  if (this->delete_signal_handler_)
    delete this->signal_handler_;

  if (sh == 0)
    {
      ACE_NEW_RETURN (this->signal_handler_,
                      ACE_Sig_Handler,
                      -1);
      this->delete_signal_handler_ = 1;
    }
  else
    {
      this->signal_handler_ = sh;
      this->delete_signal_handler_ = 0;
    }

  // Setup the atomic wait array (used later in <handle_events>)
  this->atomic_wait_array_[0] = this->lock_.lock ().proc_mutex_;
  this->atomic_wait_array_[1] = this->ok_to_wait_.handle ();

  // This is to guard against reopens of WFMO_Reactor
  if (this->delete_handler_rep_)
    this->handler_rep_.~ACE_WFMO_Reactor_Handler_Repository ();

  // Open the handle repository.  Two additional handles for internal
  // purposes
  if (this->handler_rep_.open (size + 2) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_LIB_TEXT ("%p\n"),
                       ACE_LIB_TEXT ("opening handler repository")),
                      -1);
  else
    this->delete_handler_rep_ = 1;

  this->notify_handler_ = notify;

  if (this->notify_handler_ == 0)
    {
      ACE_NEW_RETURN (this->notify_handler_,
                      ACE_WFMO_Reactor_Notify,
                      -1);

      if (this->notify_handler_ == 0)
        return -1;
      else
        this->delete_notify_handler_ = 1;
    }

  /* NOTE */
  // The order of the following two registrations is very important

  // Open the notification handler
  if (this->notify_handler_->open (this, this->timer_queue_) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_LIB_TEXT ("%p\n"),
                       ACE_LIB_TEXT ("opening notify handler ")),
                      -1);

  // Register for <wakeup_all_threads> event
  if (this->register_handler (&this->wakeup_all_threads_handler_,
                              this->wakeup_all_threads_.handle ()) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_LIB_TEXT ("%p\n"),
                       ACE_LIB_TEXT ("registering thread wakeup handler")),
                      -1);

  // Since we have added two handles into the handler repository,
  // update the <handler_repository_>
  if (this->handler_rep_.changes_required ())
    {
      // Make necessary changes to the handler repository
      this->handler_rep_.make_changes ();
      // Turn off <wakeup_all_threads_> since all necessary changes
      // have completed
      this->wakeup_all_threads_.reset ();
    }

  // We are open for business
  this->open_for_business_ = 1;

  return 0;
}

int
ACE_WFMO_Reactor::set_sig_handler (ACE_Sig_Handler *signal_handler)
{
  if (this->signal_handler_ != 0 && this->delete_signal_handler_ != 0)
    delete this->signal_handler_;
  this->signal_handler_ = signal_handler;
  this->delete_signal_handler_ = 0;
  return 0;
}

ACE_Timer_Queue *
ACE_WFMO_Reactor::timer_queue (void) const
{
  return this->timer_queue_;
}

int
ACE_WFMO_Reactor::timer_queue (ACE_Timer_Queue *tq)
{
  if (this->timer_queue_ != 0 && this->delete_timer_queue_ != 0)
    delete this->timer_queue_;
  this->timer_queue_ = tq;
  this->delete_timer_queue_ = 0;
  return 0;
}

int
ACE_WFMO_Reactor::set_timer_queue (ACE_Timer_Queue *tq)
{
  return this->timer_queue (tq);
}

int
ACE_WFMO_Reactor::close (void)
{
  // This GUARD is necessary since we are updating shared state.
  ACE_GUARD_RETURN (ACE_Process_Mutex, ace_mon, this->lock_, -1);

  // If we are already closed, return error
  if (!this->open_for_business_)
    return -1;

  // We are now closed
  this->open_for_business_ = 0;
  // This will unregister all handles
  this->handler_rep_.close ();

  return 0;
}

ACE_WFMO_Reactor::~ACE_WFMO_Reactor (void)
{
  // Assumption: No threads are left in the Reactor when this method
  // is called (i.e., active_threads_ == 0)

  // Close down
  this->close ();

  // Make necessary changes to the handler repository that we caused
  // by <close>.
  this->handler_rep_.make_changes ();

  if (this->delete_timer_queue_)
    {
      delete this->timer_queue_;
      this->timer_queue_ = 0;
      this->delete_timer_queue_ = 0;
    }

  if (this->delete_signal_handler_)
    {
      delete this->signal_handler_;
      this->signal_handler_ = 0;
      this->delete_signal_handler_ = 0;
    }

  if (this->delete_notify_handler_)
    {
      delete this->notify_handler_;
      this->notify_handler_ = 0;
      this->delete_notify_handler_ = 0;
    }
}

int
ACE_WFMO_Reactor::register_handler_i (ACE_HANDLE event_handle,
                                      ACE_HANDLE io_handle,
                                      ACE_Event_Handler *event_handler,
                                      ACE_Reactor_Mask new_masks)
{
  // If this is a Winsock 1 system, the underlying event assignment will
  // not work, so don't try. Winsock 1 must use ACE_Select_Reactor for
  // reacting to socket activity.
#if !defined (ACE_HAS_WINSOCK2) || (ACE_HAS_WINSOCK2 == 0)
  ACE_UNUSED_ARG (event_handle);
  ACE_UNUSED_ARG (io_handle);
  ACE_UNUSED_ARG (event_handler);
  ACE_UNUSED_ARG (new_masks);
  ACE_NOTSUP_RETURN (-1);
#else

  // Make sure that the <handle> is valid
  if (io_handle == ACE_INVALID_HANDLE)
    io_handle = event_handler->get_handle ();

  if (this->handler_rep_.invalid_handle (io_handle))
    {
      errno = ERROR_INVALID_HANDLE;
      return -1;
    }

  long new_network_events = 0;
  int delete_event = 0;
  auto_ptr <ACE_Auto_Event> event;

  // Look up the repository to see if the <event_handler> is already
  // there.
  ACE_Reactor_Mask old_masks;
  int found = this->handler_rep_.modify_network_events_i (io_handle,
                                                          new_masks,
                                                          old_masks,
                                                          new_network_events,
                                                          event_handle,
                                                          delete_event,
                                                          ACE_Reactor::ADD_MASK);

  // Check to see if the user passed us a valid event; If not then we
  // need to create one
  if (event_handle == ACE_INVALID_HANDLE)
    {
      // Note: don't change this since some C++ compilers have
      // <auto_ptr>s that don't work properly...
      auto_ptr<ACE_Auto_Event> tmp (new ACE_Auto_Event);
      event = tmp;
      event_handle = event->handle ();
      delete_event = 1;
    }

  int result = ::WSAEventSelect ((SOCKET) io_handle,
                                 event_handle,
                                 new_network_events);
  // If we had found the <Event_Handler> there is nothing more to do
  if (found)
    return result;
  else if (result != SOCKET_ERROR &&
           this->handler_rep_.bind_i (1,
                                      event_handler,
                                      new_network_events,
                                      io_handle,
                                      event_handle,
                                      delete_event) != -1)
    {
      // The <event_handler> was not found in the repository, add to
      // the repository.
      if (delete_event)
        {
          // Clear out the handle in the ACE_Auto_Event so that when
          // it is destroyed, the handle isn't closed out from under
          // the reactor. After setting it, running down the event
          // (via auto_ptr<> event, above) at function return will
          // cause an error because it'll try to close an invalid handle.
          // To avoid that smashing the errno value, save the errno
          // here, explicitly remove the event so the dtor won't do it
          // again, then restore errno.
          ACE_Errno_Guard guard (errno);
          event->handle (ACE_INVALID_HANDLE);
          event->remove ();
        }
      return 0;
    }
  else
    return -1;
#endif /* ACE_HAS_PHARLAP */
}

int
ACE_WFMO_Reactor::mask_ops_i (ACE_HANDLE io_handle,
                              ACE_Reactor_Mask new_masks,
                              int operation)
{
  // Make sure that the <handle> is valid
  if (this->handler_rep_.invalid_handle (io_handle))
    return -1;

  long new_network_events = 0;
  int delete_event = 0;
  ACE_HANDLE event_handle = ACE_INVALID_HANDLE;

  // Look up the repository to see if the <Event_Handler> is already
  // there.
  ACE_Reactor_Mask old_masks;
  int found = this->handler_rep_.modify_network_events_i (io_handle,
                                                          new_masks,
                                                          old_masks,
                                                          new_network_events,
                                                          event_handle,
                                                          delete_event,
                                                          operation);
  if (found)
    {
      int result = ::WSAEventSelect ((SOCKET) io_handle,
                                     event_handle,
                                     new_network_events);
      if (result == 0)
        return old_masks;
      else
        return result;
    }
  else
    return -1;
}



int
ACE_WFMO_Reactor_Handler_Repository::modify_network_events_i (ACE_HANDLE io_handle,
                                                              ACE_Reactor_Mask new_masks,
                                                              ACE_Reactor_Mask &old_masks,
                                                              long &new_network_events,
                                                              ACE_HANDLE &event_handle,
                                                              int &delete_event,
                                                              int operation)
{
  long *modified_network_events = &new_network_events;
  int found = 0;
  size_t i;

  // First go through the current entries
  //
  // Look for all entries in the current handles for matching handle
  // (except those that have been scheduled for deletion)
  for (i = 0; i < this->max_handlep1_ && !found; i++)
    if (io_handle == this->current_info_[i].io_handle_ &&
        !this->current_info_[i].delete_entry_)
      {
        found = 1;
        modified_network_events = &this->current_info_[i].network_events_;
        delete_event = this->current_info_[i].delete_event_;
        event_handle = this->current_handles_[i];
      }

  // Then pass through the suspended handles
  //
  // Look for all entries in the suspended handles for matching handle
  // (except those that have been scheduled for deletion)
  for (i = 0; i < this->suspended_handles_ && !found; i++)
    if (io_handle == this->current_suspended_info_[i].io_handle_ &&
        !this->current_suspended_info_[i].delete_entry_)
      {
        found = 1;
        modified_network_events = &this->current_suspended_info_[i].network_events_;
        delete_event = this->current_suspended_info_[i].delete_event_;
        event_handle = this->current_suspended_info_[i].event_handle_;
      }

  // Then check the to_be_added handles
  //
  // Look for all entries in the to_be_added handles for matching
  // handle (except those that have been scheduled for deletion)
  for (i = 0; i < this->handles_to_be_added_ && !found; i++)
    if (io_handle == this->to_be_added_info_[i].io_handle_ &&
        !this->to_be_added_info_[i].delete_entry_)
      {
        found = 1;
        modified_network_events = &this->to_be_added_info_[i].network_events_;
        delete_event = this->to_be_added_info_[i].delete_event_;
        event_handle = this->to_be_added_info_[i].event_handle_;
      }

  old_masks = this->bit_ops (*modified_network_events,
                             new_masks,
                             operation);

  new_network_events = *modified_network_events;

  return found;
}

int
ACE_WFMO_Reactor_Handler_Repository::handler (ACE_HANDLE handle,
                                              ACE_Reactor_Mask user_masks,
                                              ACE_Event_Handler **user_event_handler)
{
  int found = 0;
  size_t i = 0;
  ACE_Event_Handler *event_handler = 0;
  long existing_masks = 0;

  // Look for the handle first

  // First go through the current entries
  //
  // Look for all entries in the current handles for matching handle
  // (except those that have been scheduled for deletion)
  for (i = 0; i < this->max_handlep1_ && !found; i++)
    if ((handle == this->current_info_[i].io_handle_ ||
         handle == this->current_handles_[i]) &&
        !this->current_info_[i].delete_entry_)
      {
        found = 1;
        event_handler = this->current_info_[i].event_handler_;
        existing_masks = this->current_info_[i].network_events_;
      }

  // Then pass through the suspended handles
  //
  // Look for all entries in the suspended handles for matching handle
  // (except those that have been scheduled for deletion)
  for (i = 0; i < this->suspended_handles_ && !found; i++)
    if ((handle == this->current_suspended_info_[i].io_handle_ ||
         handle == this->current_suspended_info_[i].event_handle_) &&
        !this->current_suspended_info_[i].delete_entry_)
      {
        found = 1;
        event_handler = this->current_suspended_info_[i].event_handler_;
        existing_masks = this->current_suspended_info_[i].network_events_;
      }

  // Then check the to_be_added handles
  //
  // Look for all entries in the to_be_added handles for matching
  // handle (except those that have been scheduled for deletion)
  for (i = 0; i < this->handles_to_be_added_ && !found; i++)
    if ((handle == this->to_be_added_info_[i].io_handle_ ||
         handle == this->to_be_added_info_[i].event_handle_) &&
        !this->to_be_added_info_[i].delete_entry_)
      {
        found = 1;
        event_handler = this->to_be_added_info_[i].event_handler_;
        existing_masks = this->to_be_added_info_[i].network_events_;
      }

  // If the handle is not found, return failure.
  if (!found)
    return -1;

  // Otherwise, make sure that the masks that the user is looking for
  // are on.
  if (found &&
      ACE_BIT_ENABLED (user_masks, ACE_Event_Handler::READ_MASK))
    if (!ACE_BIT_ENABLED (existing_masks, FD_READ)
        && !ACE_BIT_ENABLED (existing_masks, FD_CLOSE))
      found = 0;

  if (found &&
      ACE_BIT_ENABLED (user_masks, ACE_Event_Handler::WRITE_MASK))
    if (!ACE_BIT_ENABLED (existing_masks, FD_WRITE))
      found = 0;

  if (found &&
      ACE_BIT_ENABLED (user_masks, ACE_Event_Handler::EXCEPT_MASK))
    if (!ACE_BIT_ENABLED (existing_masks, FD_OOB))
      found = 0;

  if (found &&
      ACE_BIT_ENABLED (user_masks, ACE_Event_Handler::ACCEPT_MASK))
    if (!ACE_BIT_ENABLED (existing_masks, FD_ACCEPT))
      found = 0;

  if (found &&
      ACE_BIT_ENABLED (user_masks, ACE_Event_Handler::CONNECT_MASK))
    if (!ACE_BIT_ENABLED (existing_masks, FD_CONNECT))
      found = 0;

  if (found &&
      ACE_BIT_ENABLED (user_masks, ACE_Event_Handler::QOS_MASK))
    if (!ACE_BIT_ENABLED (existing_masks, FD_QOS))
      found = 0;

  if (found &&
      ACE_BIT_ENABLED (user_masks, ACE_Event_Handler::GROUP_QOS_MASK))
    if (!ACE_BIT_ENABLED (existing_masks, FD_GROUP_QOS))
      found = 0;

  if (found &&
      user_event_handler)
    *user_event_handler = event_handler;

  if (found)
    return 0;
  else
    return -1;
}

// Waits for and dispatches all events.  Returns -1 on error, 0 if
// max_wait_time expired, or the number of events that were dispatched.
int
ACE_WFMO_Reactor::event_handling (ACE_Time_Value *max_wait_time,
                                  int alertable)
{
  ACE_TRACE ("ACE_WFMO_Reactor::event_handling");

  // Make sure we are not closed
  if (!this->open_for_business_ || this->deactivated_)
    return -1;

  // Stash the current time -- the destructor of this object will
  // automatically compute how much time elapsed since this method was
  // called.
  ACE_Countdown_Time countdown (max_wait_time);

  // Check to see if it is ok to enter ::WaitForMultipleObjects
  // This will acquire <this->lock_> on success
  // On failure, the lock will not be acquired
  int result = this->ok_to_wait (max_wait_time, alertable);
  if (result != 1)
    return result;

  // Increment the number of active threads
  this->active_threads_++;

  // Release the <lock_>
  this->lock_.release ();

  // Update the countdown to reflect time waiting to play with the
  // mut and event.
  countdown.update ();

  do
    {
      // Calculate timeout
      int timeout = this->calculate_timeout (max_wait_time);

      // Wait for event to happen
      int wait_status = this->wait_for_multiple_events (timeout,
                                                        alertable);

      // Upcall
      result = this->safe_dispatch (wait_status);
      if (0 == result)
        {
          // wait_for_multiple_events timed out without dispatching
          // anything.  Because of rounding and conversion errors and
          // such, it could be that the wait loop timed out, but
          // the timer queue said it wasn't quite ready to expire a
          // timer. In this case, max_wait_time won't have quite been
          // reduced to 0, and we need to go around again. If max_wait_time
          // is all the way to 0, just return, as the entire time the
          // caller wanted to wait has been used up.
          countdown.update ();     // Reflect time waiting for events
          if (0 == max_wait_time || max_wait_time->usec () == 0)
            break;
        }
    }
  while (result == 0);

  return result;
}

int
ACE_WFMO_Reactor::ok_to_wait (ACE_Time_Value *max_wait_time,
                              int alertable)
{
  // Calculate the max time we should spend here
  //
  // Note: There is really no need to involve the <timer_queue_> here
  // because even if a timeout in the <timer_queue_> does expire we
  // will not be able to dispatch it
  int timeout = max_wait_time == 0 ? INFINITE : max_wait_time->msec ();

  // Atomically wait for both the <lock_> and <ok_to_wait_> event
  DWORD result = 0;
  while (1)
    {
#if defined (ACE_HAS_PHARLAP)
      // PharLap doesn't implement WaitForMultipleObjectsEx, and doesn't
      // do async I/O, so it's not needed in this case anyway.
      result = ::WaitForMultipleObjects (sizeof this->atomic_wait_array_ / sizeof (ACE_HANDLE),
                                         this->atomic_wait_array_,
                                         TRUE,
                                         timeout);
#else
      result = ::WaitForMultipleObjectsEx (sizeof this->atomic_wait_array_ / sizeof (ACE_HANDLE),
                                           this->atomic_wait_array_,
                                           TRUE,
                                           timeout,
                                           alertable);
#endif /* ACE_HAS_PHARLAP */

      if (result != WAIT_IO_COMPLETION)
        break;
    }

  switch (result)
    {
    case WAIT_TIMEOUT:
      errno = ETIME;
      return 0;
    case WAIT_FAILED:
    case WAIT_ABANDONED_0:
      ACE_OS::set_errno_to_last_error ();
      return -1;
    default:
      break;
    }

  // It is ok to enter ::WaitForMultipleObjects
  return 1;
}

int
ACE_WFMO_Reactor::wait_for_multiple_events (int timeout,
                                            int alertable)
{
  // Wait for any of handles_ to be active, or until timeout expires.
  // If <alertable> is enabled allow asynchronous completion of
  // ReadFile and WriteFile operations.
#if defined (ACE_HAS_PHARLAP)
  // PharLap doesn't do async I/O and doesn't implement
  // WaitForMultipleObjectsEx, so use WaitForMultipleObjects.
  ACE_UNUSED_ARG (alertable);
  return ::WaitForMultipleObjects (this->handler_rep_.max_handlep1 (),
                                   this->handler_rep_.handles (),
                                   FALSE,
                                   timeout);
#else
  return ::WaitForMultipleObjectsEx (this->handler_rep_.max_handlep1 (),
                                     this->handler_rep_.handles (),
                                     FALSE,
                                     timeout,
                                     alertable);
#endif /* ACE_HAS_PHARLAP */
}

DWORD
ACE_WFMO_Reactor::poll_remaining_handles (size_t slot)
{
  return ::WaitForMultipleObjects (this->handler_rep_.max_handlep1 () - slot,
                                   this->handler_rep_.handles () + slot,
                                   FALSE,
                                   0);
}

int
ACE_WFMO_Reactor::calculate_timeout (ACE_Time_Value *max_wait_time)
{
  ACE_Time_Value *time = 0;
  if (this->owner_ == ACE_Thread::self ())
    time = this->timer_queue_->calculate_timeout (max_wait_time);
  else
    time = max_wait_time;

  if (time == 0)
    return INFINITE;
  else
    return time->msec ();
}


int
ACE_WFMO_Reactor::expire_timers (void)
{
  // If "owner" thread
  if (ACE_Thread::self () == this->owner_)
    // expire all pending timers.
    return this->timer_queue_->expire ();

  else
    // Nothing to expire
    return 0;
}

int
ACE_WFMO_Reactor::dispatch (int wait_status)
{
  int handlers_dispatched = 0;

  // Expire timers
  handlers_dispatched += this->expire_timers ();

  switch ((DWORD)wait_status)
    {
    case WAIT_FAILED: // Failure.
      ACE_OS::set_errno_to_last_error ();
      return -1;

    case WAIT_TIMEOUT: // Timeout.
      errno = ETIME;
      return handlers_dispatched;

    case WAIT_IO_COMPLETION: // APC.
      return handlers_dispatched;

    default:  // Dispatch.
      // We'll let dispatch worry about abandoned mutes.
      handlers_dispatched += this->dispatch_handles (wait_status);
      return handlers_dispatched;
    }
}

// Dispatches any active handles from <handles_[slot]> to
// <handles_[max_handlep1_]>, polling through our handle set looking
// for active handles.
int
ACE_WFMO_Reactor::dispatch_handles (size_t wait_status)
{
  // dispatch_slot is the absolute slot.  Only += is used to
  // increment it.
  size_t dispatch_slot = 0;

  // Cache this value, this is the absolute value.
  size_t max_handlep1 = this->handler_rep_.max_handlep1 ();

  // nCount starts off at <max_handlep1>, this is a transient count of
  // handles last waited on.
  size_t nCount = max_handlep1;

  for (int number_of_handlers_dispatched = 1;
       ;
       number_of_handlers_dispatched++)
    {
      bool ok = (
#if ! (defined(__BORLANDC__) && (__BORLANDC__ >= 0x0530)) \
    && !defined (ghs) \
    && !defined (__MINGW32__)
                 // wait_status is unsigned in Borland, Green Hills
                 // and mingw32;
                 // This >= is always true, with a warning.
                 wait_status >= WAIT_OBJECT_0 &&
#endif
                 wait_status <= (WAIT_OBJECT_0 + nCount));
      if (ok)
        dispatch_slot += wait_status - WAIT_OBJECT_0;
      else
        // Otherwise, a handle was abandoned.
        dispatch_slot += wait_status - WAIT_ABANDONED_0;

      // Dispatch handler
      if (this->dispatch_handler (dispatch_slot, max_handlep1) == -1)
        return -1;

      // Increment slot
      dispatch_slot++;

      // We're done.
      if (dispatch_slot >= max_handlep1)
        return number_of_handlers_dispatched;

      // Readjust nCount
      nCount = max_handlep1 - dispatch_slot;

      // Check the remaining handles
      wait_status = this->poll_remaining_handles (dispatch_slot);
      switch (wait_status)
        {
        case WAIT_FAILED: // Failure.
          ACE_OS::set_errno_to_last_error ();
          /* FALLTHRU */
        case WAIT_TIMEOUT:
          // There are no more handles ready, we can return.
          return number_of_handlers_dispatched;
        }
    }
}

int
ACE_WFMO_Reactor::dispatch_handler (size_t slot,
                                    size_t max_handlep1)
{
  // Check if there are window messages that need to be dispatched
  if (slot == max_handlep1)
    return this->dispatch_window_messages ();

  // Dispatch the handler if it has not been scheduled for deletion.
  // Note that this is a very week test if there are multiple threads
  // dispatching this slot as no locks are held here. Generally, you
  // do not want to do something like deleting the this pointer in
  // handle_close() if you have registered multiple times and there is
  // more than one thread in WFMO_Reactor->handle_events().
  else if (!this->handler_rep_.scheduled_for_deletion (slot))
    {
      ACE_HANDLE event_handle = *(this->handler_rep_.handles () + slot);

      if (this->handler_rep_.current_info ()[slot].io_entry_)
        return this->complex_dispatch_handler (slot,
                                               event_handle);
      else
        return this->simple_dispatch_handler (slot,
                                              event_handle);
    }
  else
    // The handle was scheduled for deletion, so we will skip it.
    return 0;
}

int
ACE_WFMO_Reactor::simple_dispatch_handler (int slot,
                                           ACE_HANDLE event_handle)
{
  // This dispatch is used for non-I/O entires

  // Assign the ``signaled'' HANDLE so that callers can get it.
#if defined (ACE_HAS_PACE)
  // siginfo_t is typedef'd to a pace_siginfo_t where there
  // exists no constructor.
  siginfo_t sig;
  sig.si_handle_ = event_handle;
#else
  // siginfo_t is an ACE - specific fabrication. Constructor exists.
  siginfo_t sig (event_handle);
#endif // ACE_HAS_PACE

  ACE_Event_Handler *eh =
    this->handler_rep_.current_info ()[slot].event_handler_;

  // Upcall
  if (eh->handle_signal (0, &sig) == -1)
    this->handler_rep_.unbind (event_handle,
                               ACE_Event_Handler::NULL_MASK);

  return 0;
}

int
ACE_WFMO_Reactor::complex_dispatch_handler (int slot,
                                            ACE_HANDLE event_handle)
{
  // This dispatch is used for I/O entires.

  ACE_WFMO_Reactor_Handler_Repository::Current_Info &current_info =
    this->handler_rep_.current_info ()[slot];

  WSANETWORKEVENTS events;
  ACE_Reactor_Mask problems = ACE_Event_Handler::NULL_MASK;
  if (::WSAEnumNetworkEvents ((SOCKET) current_info.io_handle_,
                              event_handle,
                              &events) == SOCKET_ERROR)
    problems = ACE_Event_Handler::ALL_EVENTS_MASK;
  else
    {
      // Prepare for upcalls. Clear the bits from <events> representing
      // events the handler is not interested in. If there are any left,
      // do the upcall(s). upcall will replace events.lNetworkEvents
      // with bits representing any functions that requested a repeat
      // callback before checking handles again. In this case, continue
      // to call back unless the handler is unregistered as a result of
      // one of the upcalls. The way this is written, the upcalls will
      // keep being done even if one or more upcalls reported problems.
      // In practice this may turn out not so good, but let's see. If any
      // problems, please notify Steve Huston <shuston@riverace.com>
      // before or after you change this code.
      events.lNetworkEvents &= current_info.network_events_;
      while (events.lNetworkEvents != 0)
        {
          // Upcall
          problems |= this->upcall (current_info.event_handler_,
                                    current_info.io_handle_,
                                    events);
          if (this->handler_rep_.scheduled_for_deletion (slot))
            break;
        }
    }

  if (problems != ACE_Event_Handler::NULL_MASK
      && !this->handler_rep_.scheduled_for_deletion (slot)  )
    this->handler_rep_.unbind (event_handle, problems);

  return 0;
}

ACE_Reactor_Mask
ACE_WFMO_Reactor::upcall (ACE_Event_Handler *event_handler,
                          ACE_HANDLE io_handle,
                          WSANETWORKEVENTS &events)
{
  // This method figures out what exactly has happened to the socket
  // and then calls appropriate methods.
  ACE_Reactor_Mask problems = ACE_Event_Handler::NULL_MASK;

  // Go through the events and do the indicated upcalls. If the handler
  // doesn't want to be called back, clear the bit for that event.
  // At the end, set the bits back to <events> to request a repeat call.

  long actual_events = events.lNetworkEvents;
  int action;

  if (ACE_BIT_ENABLED (actual_events, FD_READ))
    {
      action = event_handler->handle_input (io_handle);
      if (action <= 0)
        {
          ACE_CLR_BITS (actual_events, FD_READ);
          if (action == -1)
            ACE_SET_BITS (problems, ACE_Event_Handler::READ_MASK);
        }
    }

  if (ACE_BIT_ENABLED (actual_events, FD_CLOSE))
      && ACE_BIT_DISABLED (problems, ACE_Event_Handler::READ_MASK))
    {
      action = event_handler->handle_input (io_handle);
      if (action <= 0)
        {
          ACE_CLR_BITS (actual_events, FD_CLOSE);
          if (action == -1)
            ACE_SET_BITS (problems, ACE_Event_Handler::READ_MASK);
        }
    }

  if (ACE_BIT_ENABLED (actual_events, FD_ACCEPT))
    {
      action = event_handler->handle_input (io_handle);
      if (action <= 0)
        {
          ACE_CLR_BITS (actual_events, FD_ACCEPT);
          if (action == -1)
            ACE_SET_BITS (problems, ACE_Event_Handler::ACCEPT_MASK);
        }
    }

  if (ACE_BIT_ENABLED (actual_events, FD_WRITE))
    {
      action = event_handler->handle_output (io_handle);
      if (action <= 0)
        {
          ACE_CLR_BITS (actual_events, FD_WRITE);
          if (action == -1)
            ACE_SET_BITS (problems, ACE_Event_Handler::WRITE_MASK);
        }
    }

  if (ACE_BIT_ENABLED (actual_events, FD_CONNECT))
    {
      if (events.iErrorCode[FD_CONNECT_BIT] == 0)
        {
          // Successful connect
          action = event_handler->handle_output (io_handle);
          if (action <= 0)
            {
              ACE_CLR_BITS (actual_events, FD_CONNECT);
              if (action == -1)
                ACE_SET_BITS (problems,
                              ACE_Event_Handler::CONNECT_MASK);
            }
        }
      // Unsuccessful connect
      else
        {
          action = event_handler->handle_input (io_handle);
          if (action <= 0)
            {
              ACE_CLR_BITS (actual_events, FD_CONNECT);
              if (action == -1)
                ACE_SET_BITS (problems,
                              ACE_Event_Handler::CONNECT_MASK);
            }
        }
    }

  if (ACE_BIT_ENABLED (actual_events, FD_OOB))
    {
      action = event_handler->handle_exception (io_handle);
      if (action <= 0)
        {
          ACE_CLR_BITS (actual_events, FD_OOB);
          if (action == -1)
            ACE_SET_BITS (problems, ACE_Event_Handler::EXCEPT_MASK);
        }
    }

  if (ACE_BIT_ENABLED (actual_events, FD_QOS))
    {
      action = event_handler->handle_qos (io_handle);
      if (action <= 0)
        {
          ACE_CLR_BITS (actual_events, FD_QOS);
          if (action == -1)
            ACE_SET_BITS (problems, ACE_Event_Handler::QOS_MASK);
        }
    }

  if (ACE_BIT_ENABLED (actual_events, FD_GROUP_QOS))
    {
      action = event_handler->handle_group_qos (io_handle);
      if (action <= 0)
        {
          ACE_CLR_BITS (actual_events, FD_GROUP_QOS);
          if (action == -1)
            ACE_SET_BITS (problems, ACE_Event_Handler::GROUP_QOS_MASK);
        }
    }

  events.lNetworkEvents = actual_events;
  return problems;
}


int
ACE_WFMO_Reactor::update_state (void)
{
  // This GUARD is necessary since we are updating shared state.
  ACE_GUARD_RETURN (ACE_Process_Mutex, monitor, this->lock_, -1);

  // Decrement active threads
  this->active_threads_--;

  // Check if the state of the handler repository has changed or new
  // owner has to be set
  if (this->handler_rep_.changes_required () || this->new_owner ())
    {
      if (this->change_state_thread_ == 0)
        // Try to become the thread which will be responsible for the
        // changes
        {
          this->change_state_thread_ = ACE_Thread::self ();
          // Make sure no new threads are allowed to enter
          this->ok_to_wait_.reset ();

          if (this->active_threads_ > 0)
            // Check for other active threads
            {
              // Wake up all other threads
              this->wakeup_all_threads_.signal ();
              // Release <lock_>
              monitor.release ();
              // Go to sleep waiting for all other threads to get done
              this->waiting_to_change_state_.wait ();
              // Re-acquire <lock_> again
              monitor.acquire ();
            }

          // Note that make_changes() calls into user code which can
          // request other changes.  So keep looping until all
          // requested changes are completed.
          while (this->handler_rep_.changes_required ())
            // Make necessary changes to the handler repository
            this->handler_rep_.make_changes ();
          if (this->new_owner ())
            // Update the owner
            this->change_owner ();
          // Turn off <wakeup_all_threads_>
          this->wakeup_all_threads_.reset ();
          // Let everyone know that it is ok to go ahead
          this->ok_to_wait_.signal ();
          // Reset this flag
          this->change_state_thread_ = 0;
        }
      else if (this->active_threads_ == 0)
        // This thread did not get a chance to become the change
        // thread. If it is the last one out, it will wakeup the
        // change thread
        this->waiting_to_change_state_.signal ();
    }
  // This is if we were woken up explicitily by the user and there are
  // no state changes required.
  else if (this->active_threads_ == 0)
    // Turn off <wakeup_all_threads_>
    this->wakeup_all_threads_.reset ();

  return 0;
}

void
ACE_WFMO_Reactor::dump (void) const
{
  ACE_TRACE ("ACE_WFMO_Reactor::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));

  ACE_DEBUG ((LM_DEBUG,
              ACE_LIB_TEXT ("Count of currently active threads = %d\n"),
              this->active_threads_));

  ACE_DEBUG ((LM_DEBUG,
              ACE_LIB_TEXT ("ID of owner thread = %d\n"),
              this->owner_));

  this->handler_rep_.dump ();
  this->signal_handler_->dump ();
  this->timer_queue_->dump ();

  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

int
ACE_WFMO_Reactor_Notify::dispatch_notifications (int & /*number_of_active_handles*/,
                                                 ACE_Handle_Set & /*rd_mask*/)
{
  return -1;
}

int
ACE_WFMO_Reactor_Notify::is_dispatchable (ACE_Notification_Buffer & /*buffer*/)
{
  return 0;
}

ACE_HANDLE
ACE_WFMO_Reactor_Notify::notify_handle (void)
{
  return ACE_INVALID_HANDLE;
}

int
ACE_WFMO_Reactor_Notify::read_notify_pipe (ACE_HANDLE ,
                                           ACE_Notification_Buffer &)
{
  return 0;
}

int
ACE_WFMO_Reactor_Notify::dispatch_notify (ACE_Notification_Buffer &)
{
  return 0;
}

int
ACE_WFMO_Reactor_Notify::close (void)
{
  return -1;
}

ACE_WFMO_Reactor_Notify::ACE_WFMO_Reactor_Notify (void)
  : timer_queue_ (0),
    max_notify_iterations_ (-1)
{
}

int
ACE_WFMO_Reactor_Notify::open (ACE_Reactor_Impl *wfmo_reactor,
                               ACE_Timer_Queue *timer_queue,
                               int ignore_notify)
{
  ACE_UNUSED_ARG (ignore_notify);
  timer_queue_ = timer_queue;
  return wfmo_reactor->register_handler (this);
}

ACE_HANDLE
ACE_WFMO_Reactor_Notify::get_handle (void) const
{
  return this->wakeup_one_thread_.handle ();
}

// Handle all pending notifications.

int
ACE_WFMO_Reactor_Notify::handle_signal (int signum,
                                        siginfo_t *siginfo,
                                        ucontext_t *)
{
  ACE_UNUSED_ARG (signum);

  // Just check for sanity...
  if (siginfo->si_handle_ != this->wakeup_one_thread_.handle ())
    return -1;

  // This will get called when <WFMO_Reactor->wakeup_one_thread_> event
  // is signaled.
  //  ACE_DEBUG ((LM_DEBUG,
  //             ACE_LIB_TEXT ("(%t) waking up to handle internal notifications\n")));

  for (int i = 1; ; i++)
    {
      ACE_Message_Block *mb = 0;
      // Copy ACE_Time_Value::zero since dequeue_head will modify it.
      ACE_Time_Value zero_timeout (ACE_Time_Value::zero);
      if (this->message_queue_.dequeue_head (mb, &zero_timeout) == -1)
        {
          if (errno == EWOULDBLOCK)
            // We've reached the end of the processing, return
            // normally.
            return 0;
          else
            return -1; // Something weird happened...
        }
      else
        {
          ACE_Notification_Buffer *buffer =
            (ACE_Notification_Buffer *) mb->base ();

          // If eh == 0 then we've got major problems!  Otherwise, we
          // need to dispatch the appropriate handle_* method on the
          // ACE_Event_Handler pointer we've been passed.

          if (buffer->eh_ != 0)
            {
              int result = 0;

              switch (buffer->mask_)
                {
                case ACE_Event_Handler::READ_MASK:
                case ACE_Event_Handler::ACCEPT_MASK:
                  result = buffer->eh_->handle_input (ACE_INVALID_HANDLE);
                  break;
                case ACE_Event_Handler::WRITE_MASK:
                  result = buffer->eh_->handle_output (ACE_INVALID_HANDLE);
                  break;
                case ACE_Event_Handler::EXCEPT_MASK:
                  result = buffer->eh_->handle_exception (ACE_INVALID_HANDLE);
                  break;
                case ACE_Event_Handler::QOS_MASK:
                  result = buffer->eh_->handle_qos (ACE_INVALID_HANDLE);
                  break;
                case ACE_Event_Handler::GROUP_QOS_MASK:
                  result = buffer->eh_->handle_group_qos (ACE_INVALID_HANDLE);
                  break;
                default:
                  ACE_ERROR ((LM_ERROR,
                              ACE_LIB_TEXT ("invalid mask = %d\n"),
                              buffer->mask_));
                  break;
                }
              if (result == -1)
                buffer->eh_->handle_close (ACE_INVALID_HANDLE,
                                           ACE_Event_Handler::EXCEPT_MASK);
            }

          // Make sure to delete the memory regardless of success or
          // failure!
          mb->release ();

          // Bail out if we've reached the <max_notify_iterations_>.
          // Note that by default <max_notify_iterations_> is -1, so
          // we'll loop until we're done.
          if (i == this->max_notify_iterations_)
            {
              // If there are still notification in the queue, we need
              // to wake up again
              if (!this->message_queue_.is_empty ())
                this->wakeup_one_thread_.signal ();

              // Break the loop as we have reached max_notify_iterations_
              return 0;
            }
        }
    }
}

// Notify the WFMO_Reactor, potentially enqueueing the
// <ACE_Event_Handler> for subsequent processing in the WFMO_Reactor
// thread of control.

int
ACE_WFMO_Reactor_Notify::notify (ACE_Event_Handler *eh,
                                 ACE_Reactor_Mask mask,
                                 ACE_Time_Value *timeout)
{
  if (eh != 0)
    {
      ACE_Message_Block *mb = 0;
      ACE_NEW_RETURN (mb,
                      ACE_Message_Block (sizeof (ACE_Notification_Buffer)),
                      -1);

      ACE_Notification_Buffer *buffer =
        (ACE_Notification_Buffer *) mb->base ();
      buffer->eh_ = eh;
      buffer->mask_ = mask;

      // Convert from relative time to absolute time by adding the
      // current time of day.  This is what <ACE_Message_Queue>
      // expects.
      if (timeout != 0)
        *timeout += timer_queue_->gettimeofday ();

      if (this->message_queue_.enqueue_tail
          (mb, timeout) == -1)
        {
          mb->release ();
          return -1;
        }
    }

  return this->wakeup_one_thread_.signal ();
}

void
ACE_WFMO_Reactor_Notify::max_notify_iterations (int iterations)
{
  ACE_TRACE ("ACE_WFMO_Reactor_Notify::max_notify_iterations");
  // Must always be > 0 or < 0 to optimize the loop exit condition.
  if (iterations == 0)
    iterations = 1;

  this->max_notify_iterations_ = iterations;
}

int
ACE_WFMO_Reactor_Notify::max_notify_iterations (void)
{
  ACE_TRACE ("ACE_WFMO_Reactor_Notify::max_notify_iterations");
  return this->max_notify_iterations_;
}

int
ACE_WFMO_Reactor_Notify::purge_pending_notifications (ACE_Event_Handler *eh,
                                                      ACE_Reactor_Mask mask)
{
  ACE_TRACE ("ACE_WFMO_Reactor_Notify::purge_pending_notifications");

  // Go over message queue and take out all the matching event
  // handlers.  If eh == 0, purge all. Note that reactor notifies (no
  // handler specified) are never purged, as this may lose a needed
  // notify the reactor queued for itself.

  if (this->message_queue_.is_empty ())
    return 0;

  // Guard against new and/or delivered notifications while purging.
  // WARNING!!! The use of the notification queue's lock object for
  // this guard makes use of the knowledge that on Win32, the mutex
  // protecting the queue is really a CriticalSection, which is
  // recursive. This is how we can get away with locking it down here
  // and still calling member functions on the queue object.
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, monitor, this->message_queue_.lock(), -1);

  // first, copy all to our own local queue. Since we've locked everyone out
  // of here, there's no need to use any synchronization on this queue.
  ACE_Message_Queue<ACE_NULL_SYNCH> local_queue;

  size_t queue_size  = this->message_queue_.message_count ();
  int number_purged = 0;

  size_t index;

  for (index = 0; index < queue_size; ++index)
    {
      ACE_Message_Block  *mb;
      if (-1 == this->message_queue_.dequeue_head (mb))
        return -1;        // This shouldn't happen...

      ACE_Notification_Buffer *buffer =
        ACE_reinterpret_cast (ACE_Notification_Buffer *, mb->base ());

      // If this is not a Reactor notify (it is for a particular handler),
      // and it matches the specified handler (or purging all),
      // and applying the mask would totally eliminate the notification, then
      // release it and count the number purged.
      if ((0 != buffer->eh_) &&
          (0 == eh || eh == buffer->eh_) &&
          ACE_BIT_DISABLED (buffer->mask_, ~mask)) // the existing notification mask
                                                   // is left with nothing when
                                                   // applying the mask
      {
        mb->release ();
        ++number_purged;
      }
      else
      {
        // To preserve it, move it to the local_queue.  But first, if
        // this is not a Reactor notify (it is for a
        // particularhandler), and it matches the specified handler
        // (or purging all), then apply the mask
        if ((0 != buffer->eh_) &&
            (0 == eh || eh == buffer->eh_))
          ACE_CLR_BITS(buffer->mask_, mask);
        if (-1 == local_queue.enqueue_head (mb))
          return -1;
      }
    }

  if (this->message_queue_.message_count ())
    { // Should be empty!
      ACE_ASSERT (0);
      return -1;
    }

  // Now copy back from the local queue to the class queue, taking
  // care to preserve the original order...
  queue_size  = local_queue.message_count ();
  for (index = 0; index < queue_size; ++index)
    {
      ACE_Message_Block  *mb;
      if (-1 == local_queue.dequeue_head (mb))
        {
          ACE_ASSERT (0);
          return -1;
        }

      if (-1 == this->message_queue_.enqueue_head (mb))
        {
          ACE_ASSERT (0);
          return -1;
        }
    }

  return number_purged;
}

void
ACE_WFMO_Reactor_Notify::dump (void) const
{
  ACE_TRACE ("ACE_WFMO_Reactor_Notify::dump");
  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  this->timer_queue_->dump ();
  ACE_DEBUG ((LM_DEBUG,
              ACE_LIB_TEXT ("Max. iteration: %d\n"),
              this->max_notify_iterations_));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

void
ACE_WFMO_Reactor::max_notify_iterations (int iterations)
{
  ACE_TRACE ("ACE_WFMO_Reactor::max_notify_iterations");
  ACE_GUARD (ACE_Process_Mutex, monitor, this->lock_);

  // Must always be > 0 or < 0 to optimize the loop exit condition.
  this->notify_handler_->max_notify_iterations (iterations);
}

int
ACE_WFMO_Reactor::max_notify_iterations (void)
{
  ACE_TRACE ("ACE_WFMO_Reactor::max_notify_iterations");
  ACE_GUARD_RETURN (ACE_Process_Mutex, monitor, this->lock_, -1);

  return this->notify_handler_->max_notify_iterations ();
}

int
ACE_WFMO_Reactor::purge_pending_notifications (ACE_Event_Handler *eh,
                                               ACE_Reactor_Mask mask)
{
  ACE_TRACE ("ACE_WFMO_Reactor::purge_pending_notifications");
  if (this->notify_handler_ == 0)
    return 0;
  else
    return this->notify_handler_->purge_pending_notifications (eh, mask);
}

int
ACE_WFMO_Reactor::resumable_handler (void)
{
  ACE_TRACE ("ACE_WFMO_Reactor::resumable_handler");
  return 0;
}


// No-op WinSOCK2 methods to help WFMO_Reactor compile
#if !defined (ACE_HAS_WINSOCK2) || (ACE_HAS_WINSOCK2 == 0)
int
WSAEventSelect (SOCKET s,
                WSAEVENT hEventObject,
                long lNetworkEvents)
{
  ACE_UNUSED_ARG (s);
  ACE_UNUSED_ARG (hEventObject);
  ACE_UNUSED_ARG (lNetworkEvents);

  return -1;
}

int
WSAEnumNetworkEvents (SOCKET s,
                      WSAEVENT hEventObject,
                      LPWSANETWORKEVENTS lpNetworkEvents)
{
  ACE_UNUSED_ARG (s);
  ACE_UNUSED_ARG (hEventObject);
  ACE_UNUSED_ARG (lpNetworkEvents);

  return -1;
}
#endif /* !defined ACE_HAS_WINSOCK2 */

#endif /* ACE_WIN32 */
