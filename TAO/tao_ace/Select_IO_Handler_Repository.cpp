#include "Select_IO_Handler_Repository.h"
#include "IO_Event_Handler.h"
#include "ace/Signal.h"

#if !defined (__ACE_INLINE__)
#include "Select_IO_Handler_Repository.inl"
#endif /* __ACE_INLINE__ */


ACE_RCSID(tao_ace, Select_IO_Handler_Repository, "$Id$")


TAO_ACE_Select_IO_Handler_Repository::TAO_ACE_Select_IO_Handler_Repository (void)
  : max_size_ (0),
    max_handlep1_ (0),
    wait_set_ (),
# if defined (ACE_WIN32)
  handler_states_ ()
#else
  handler_states_ (0)
#endif /*ACE_WIN32*/
{
  ACE_TRACE ("TAO_ACE_Select_IO_Handler_Repository::Constructor");
}


TAO_ACE_Select_IO_Handler_Repository::TAO_ACE_Select_IO_Handler_Repository (size_t s)
{
  ACE_TRACE ("TAO_ACE_Select_IO_Handler_Repository::Constructor");
  (void) this->open (s);
}


int
TAO_ACE_Select_IO_Handler_Repository::open (size_t s)
{
  ACE_TRACE ("TAO_ACE_Select_IO_Handler_Repository::open");
#if defined (ACE_WIN32)

  // Just open the cache map where the entries are stored on Win32
  if (this->handler_states_.open (s) == -1)
    return -1;
#else

  // On UNIX based platforms allocate an array of Handler_State so
  // that we can index directly into the array.

  // If allocated previously just delete them.
  if (this->handler_states_)
    delete [] this->handler_states_;

  ACE_NEW_RETURN (this->handler_states_,
                  IO_Handler_State [s],
                  -1);
#endif /*ACE_WIN32*/

  return 0;
}


int
TAO_ACE_Select_IO_Handler_Repository::close (void)
{
  ACE_TRACE ("TAO_ACE_Select_IO_Handler_Repository::close");
#if defined (ACE_WIN32)
  // @@ <todo> Where do we do "unbind" of all the elements?. Let this
  // be there for the timebeing...

  // Just close the cache map where the entries are stored.
  if (this->handler_states_.close () == -1)
    return -1;
#else
  // If allocated previously just delete them.
  if (this->handler_states_)
    delete [] this->handler_states_;

  this->handler_states_ = 0;

#endif /*ACE_WIN32*/

  return 0;
}


int
TAO_ACE_Select_IO_Handler_Repository::bind (
    TAO_ACE_IO_Event_Handler *eh,
    TAO_ACE_IO_Event_Masks::Event_Masks mask)
{
  ACE_TRACE ("TAO_ACE_Select_IO_Handler_Repository::bind");

  // Get the handle
  ACE_HANDLE handle = eh->get_handle ();

  // Check for the validity of the handle
  if (this->is_handle_valid (handle) < 0)
    return -1;

  // Increment the reference count of the event handler. This
  // reference count indicates the reference that the repository is
  // holding.
  eh->incr_ref_count ();

#if defined (ACE_WIN32)

  // Copy the pointer to the Event Handler to the object that is
  // being added to the Hash Map.
  TAO_ACE_IO_Handler_Repository::IO_Handle_State handle_state;
  handle_state.event_handler_ = eh;

  // Just use a rebind even if there was a event handle associated
  // with the handle before..
  int retval =
    this->handler_states_.rebind ((ACE_SOCKET) handle,
                                  handle_state);

  if (retval == -1)
    {
      ACE_DEBUG_RETURN  ((LM_DEBUG,
                          "(%P|%t) Could not register handle [%d] "
                          "with the repository \n"),
                         -1);
    }

  // This argument is ignored during the select () call on Win32
  // Winsock 1.1 and Winsock 1.2. We dont do much about this on Win32
  // platforms..
  /*this->max_handlep1_ = 0*/

#else /*if !ACE_WIN32*/

  this->handler_states_[handle].event_handler_ = eh;

  if (this->max_handlep1_ < handle + 1)
    this->max_handlep1_ = handle + 1;

#endif /*ACE_WIN32*/

  // Add the <mask> for this <handle> in to the Select_Reactor's
  // wait_set.
  this->add_masks (handle,
                   mask);

  // <Bala>: Do we need this?
  // this->state_changed_ = 1;

  return 0;
}

int
TAO_ACE_Select_IO_Handler_Repository::unbind (
    TAO_ACE_IO_Event_Handler *eh,
    TAO_ACE_IO_Event_Masks::Event_Masks mask)
{
  ACE_TRACE ("TAO_ACE_Select_IO_Handler_Repository::unbind");

  // Get the handle
  ACE_HANDLE handle = eh->get_handle ();

  // Check the validity of the handle
  if (this->is_handle_valid (handle) < 0)
    return -1;

  // Clear the <mask> for the <handle> from the wait set
  this->clr_masks (handle, mask);

  // Check whether we have any other event masks left behind for this
  // handle.
  if (!this->is_event_mask_available (handle))
    {
      // If no masks are left behind, just get the handle state
# if defined (ACE_WIN32)

      TAO_ACE_IO_Handler_Repository::IO_Handle_State handle_state;

      int retval =
        this->handler_states_.find ((ACE_SOCKET) handle,
                                    handle_state);
      if (retval < 0)
        {
          ACE_DEBUG_RETURN  ((LM_DEBUG,
                              "(%P|%t) Could not find handle [%d] "
                              "in the repository \n", (ACE_SOCKET) handle),
                             -1);
        }

      // We have a few threads making the upcall
      if (handle_state.thread_count_ != 0)
        {
          // Mark the handler for deletion
          handle_state.marked_for_deletion_ = 1;

          // Leave it back in the hash map.
          retval =
            this->handler_states_.rebind ((ACE_SOCKET) handle,
                                          handle_state);

          // If error occurs return -1;
          if (retval == -1)
            return retval;
        }
      else
        {
          // Unbind the entry from the hash map
          retval =
            this->handler_states_.unbind ((ACE_SOCKET) handle);

          // Decrement the ref count as we have removed the entry from
          // the map.
          handle_state.event_handler_.decr_ref_count ();
        }

#else /*!ACE_WIN32*/
      if (this->handler_states_[handle].thread_count_ != 0)
        {
          this->handler_states_[handle].marked_for_deletion_ = 1;
        }
      else
        {
          // Decrement the refcount
          this->handler_states_[handle].event_handler_->decr_ref_count ();

          // Clear up the slot for usage
          this->handler_states_[handle].thread_count_ = 0;
          this->handler_states_[handle].marked_for_deletion_ = 0;
          this->handler_states_[handle].event_handler_ = 0;

          // Re-calculate the max_handlep1_

          if (this->max_handlep1_ == handle + 1)
            {
              // We've deleted the last entry, so we need to figure out
              // the last valid place in the array that is worth looking
              // at.
              ACE_HANDLE wait_rd_max = this->wait_set_.rd_mask_.max_set ();
              ACE_HANDLE wait_wr_max = this->wait_set_.wr_mask_.max_set ();
              ACE_HANDLE wait_ex_max = this->wait_set_.ex_mask_.max_set ();

              // <todo>Need to include the suspend set too????

              // Compute the maximum of six values.
              this->max_handlep1_ = wait_rd_max;
              if (this->max_handlep1_ < wait_wr_max)
                this->max_handlep1_ = wait_wr_max;
              if (this->max_handlep1_ < wait_ex_max)
                this->max_handlep1_ = wait_ex_max;

              this->max_handlep1_++;
            }
        }
#endif /*ACE_WIN32*/
    }

  return 0;
}

int
TAO_ACE_Select_IO_Handler_Repository::add_masks (
    ACE_HANDLE h,
    TAO_ACE_IO_Event_Masks::Event_Masks mask)
{
  ACE_TRACE ("TAO_ACE_Select_IO_Handler_Repository::add_masks");

  if (this->is_handle_valid (h) < 0)
    return -1;

#if !defined (ACE_WIN32)
  ACE_Sig_Guard sb; // Block out all signals until method returns.
#endif /* ACE_WIN32 */

  // READ, ACCEPT, and CONNECT flag will place the handle in the
  // read set.
  if (ACE_BIT_ENABLED (mask, TAO_ACE_IO_Event_Masks::READ_MASK)
      || ACE_BIT_ENABLED (mask, TAO_ACE_IO_Event_Masks::ACCEPT_MASK)
      || ACE_BIT_ENABLED (mask, TAO_ACE_IO_Event_Masks::CONNECT_MASK))
    {
      this->wait_set_.rd_mask_.set_bit (h);
    }

  // WRITE and CONNECT flag will place the handle in the write set
  if (ACE_BIT_ENABLED (mask,
                       TAO_ACE_IO_Event_Masks::WRITE_MASK)
      || ACE_BIT_ENABLED (mask,
                          TAO_ACE_IO_Event_Masks::CONNECT_MASK))
    {
      this->wait_set_.wr_mask_.set_bit (h);
    }

  // EXCEPT (and CONNECT on Win32) flag will place the handle in
  // the except set.
  if (ACE_BIT_ENABLED (mask,
                       TAO_ACE_IO_Event_Masks::EXCEPT_MASK)
#if defined (ACE_WIN32)
      || ACE_BIT_ENABLED (mask,
                          TAO_ACE_IO_Event_Masks::CONNECT_MASK)
#endif /* ACE_WIN32 */
      )
    {
      this->wait_set_.ex_mask_.set_bit (h);
    }

  return 0;
}


int
TAO_ACE_Select_IO_Handler_Repository::clr_masks (
    ACE_HANDLE h,
    TAO_ACE_IO_Event_Masks::Event_Masks mask)
{
  ACE_TRACE ("TAO_ACE_Select_IO_Handler_Repository::clr_masks");

  if (this->is_handle_valid (h) < 0)
    return -1;

#if !defined (ACE_WIN32)
  ACE_Sig_Guard sb; // Block out all signals until method returns.
#endif /* ACE_WIN32 */

  // READ, ACCEPT, and CONNECT flag will place the handle in the
  // read set.
  if (ACE_BIT_ENABLED (mask, TAO_ACE_IO_Event_Masks::READ_MASK)
      || ACE_BIT_ENABLED (mask, TAO_ACE_IO_Event_Masks::ACCEPT_MASK)
      || ACE_BIT_ENABLED (mask, TAO_ACE_IO_Event_Masks::CONNECT_MASK))
    {
      this->wait_set_.rd_mask_.clr_bit (h);
    }

  // WRITE and CONNECT flag will place the handle in the write set
  if (ACE_BIT_ENABLED (mask,
                       TAO_ACE_IO_Event_Masks::WRITE_MASK)
      || ACE_BIT_ENABLED (mask,
                          TAO_ACE_IO_Event_Masks::CONNECT_MASK))
    {
      this->wait_set_.wr_mask_.clr_bit (h);
    }

  // EXCEPT (and CONNECT on Win32) flag will place the handle in
  // the except set.
  if (ACE_BIT_ENABLED (mask,
                       TAO_ACE_IO_Event_Masks::EXCEPT_MASK)
#if defined (ACE_WIN32)
      || ACE_BIT_ENABLED (mask,
                          TAO_ACE_IO_Event_Handler::CONNECT_MASK)
#endif /* ACE_WIN32 */
      )
    {
      this->wait_set_.ex_mask_.clr_bit (h);
    }

  return 0;
}


int
TAO_ACE_Select_IO_Handler_Repository::find_state (
    ACE_HANDLE h,
    TAO_ACE_IO_Handler_Repository::IO_Handler_State &state)
{
#if defined (ACE_WIN32)

  // Look at the Hash Map to get the value
  return this->handle_states_->find ((ACE_SOCKET) h,
                                     state);
#else /*ACE_WIN32*/

  if (h < this->max_handlep1_)
    {
      state = this->handler_states_[h];

      return 0;
    }

  return -1;
#endif /*!ACE_WIN32*/

}


int
TAO_ACE_Select_IO_Handler_Repository::rebind_state (
    TAO_ACE_Select_IO_Handler_Repository::IO_Handler_State &state)
{
  ACE_HANDLE h = state.event_handler_->get_handle ();

#if defined (ACE_WIN32)

  // Look at the Hash Map to get the value
  return this->handler_states_->rebind ((ACE_SOCKET)h,
                                        state);

#else /*ACE_WIN32*/

  if (h < this->max_handlep1_)
    {
      this->handler_states_[h] = state;

      return 0;
    }

  return -1;
#endif /*!ACE_WIN32*/
}

/*************************************************************************/
TAO_ACE_Select_IO_Handler_Repository_Iterator::TAO_ACE_Select_IO_Handler_Repository_Iterator (
    const TAO_ACE_Select_IO_Handler_Repository &repo)
  : repo_ (repo)
#if defined (ACE_WIN32)
  , iter_ (handler_states_)
#else /* !ACE_WIN32*/
  , index_ (-1)
#endif /*ACE_WIN32*/
{
  this->advance ();
}


int
TAO_ACE_Select_IO_Handler_Repository_Iterator::next (
    TAO_ACE_IO_Handler_Repository::IO_Handler_State *&next_item)
{
  int retval = 1;

# if defined (ACE_WIN32)

  // On Win32 platforms just fallback on the Hash Map
  retval =
    this->iter_->next (next_item);
# else

  if (this->index_ >= this->repo_.max_handlep1_)
    retval = 0;
  else
    next_item = &this->repo_.handler_states_[this->index_];
#endif /*ACE_WIN32*/

  return retval;
}

int
TAO_ACE_Select_IO_Handler_Repository_Iterator::done (void) const
{
#if defined (ACE_WIN32)

  // On Win32 platforms just fallback on the Hash Map
  return this->iter_.done ();
#else /*ACE_WIN32*/

  // On non-Win32 platforms just do the check
  return this->index_ >= this->repo_.max_handlep1_;
#endif /*ACE_WIN32*/
}


int
TAO_ACE_Select_IO_Handler_Repository_Iterator::advance (void)
{
  int retval = 0;

#if defined (ACE_WIN32)

  // On Win32 platforms just rely on the iterator
  retval =  this->iter_.advance ();
#else /*ACE_WIN32*/

  // On non-Win32 platforms just do the advance yourself
  if (this->index_ < this->repo_.max_handlep1_)
    this->index_++;

  while (this->index_ < this->repo_.max_handlep1_)
    if (this->repo_.handler_states_[this->index_].event_handler_ != 0)
      return 1;
    else
      this->index_++;
#endif /* !ACE_WIN32*/

  return retval;
}

// Dump the state of an object.

void
TAO_ACE_Select_IO_Handler_Repository_Iterator::dump (void) const
{
  ACE_TRACE ("ACE_Select_Reactor_Handler_Repository_Iterator::dump");

  /**
  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("repo_ = %u"), this->repo_));
  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("current_ = %d"), this->current_));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
  ***/
}
