// ReactorEx.cpp
// $Id$

#define ACE_BUILD_DLL
#include "ace/ReactorEx.h"
#include "ace/Timer_List.h"
#include "ace/Thread.h"
#include "ace/Service_Config.h"

#if defined (ACE_WIN32)

#if !defined (__ACE_INLINE__)
#include "ace/Handle_Set.h"
#include "ace/ReactorEx.i"
#endif /* __ACE_INLINE__ */

#include "ace/Auto_Ptr.h"

// Process-wide ACE_ReactorEx.
ACE_ReactorEx *ACE_ReactorEx::reactorEx_ = 0;

// Controls whether the ReactorEx is deleted when we shut down (we can
// only delete it safely if we created it!)
int ACE_ReactorEx::delete_reactorEx_ = 0;

// Terminate the eventloop.
sig_atomic_t ACE_ReactorEx::end_event_loop_ = 0;

/************************************************************/

ACE_ReactorEx_Handler_Repository::ACE_ReactorEx_Handler_Repository (ACE_ReactorEx &reactorEx)
  : reactorEx_ (reactorEx)
{
}

int
ACE_ReactorEx_Handler_Repository::open (size_t size)
{
  // Dynamic allocation
  ACE_NEW_RETURN (this->current_handles_, ACE_HANDLE[size], -1);
  ACE_NEW_RETURN (this->current_info_, Current_Info[size], -1);
  ACE_NEW_RETURN (this->current_suspended_info_, Suspended_Info[size], -1);
  ACE_NEW_RETURN (this->to_be_added_info_, To_Be_Added_Info[size], -1);

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

ACE_ReactorEx_Handler_Repository::~ACE_ReactorEx_Handler_Repository (void)
{
  // Free up dynamically allocated space
  delete[] this->current_handles_;
  delete[] this->current_info_;
  delete[] this->current_suspended_info_;
  delete[] this->to_be_added_info_;
}

long
ACE_ReactorEx_Handler_Repository::remove_network_events_i (long existing_masks,
							   ACE_Reactor_Mask to_be_removed_masks)
{
  long result = existing_masks;
  
  if (ACE_BIT_STRICTLY_ENABLED (to_be_removed_masks, ACE_Event_Handler::READ_MASK))
    ACE_CLR_BITS (result, FD_READ);
  
  if (ACE_BIT_STRICTLY_ENABLED (to_be_removed_masks, ACE_Event_Handler::WRITE_MASK))
    ACE_CLR_BITS (result, FD_WRITE);
  
  if (ACE_BIT_STRICTLY_ENABLED (to_be_removed_masks, ACE_Event_Handler::EXCEPT_MASK))
    ACE_CLR_BITS (result, FD_OOB);
  
  if (ACE_BIT_STRICTLY_ENABLED (to_be_removed_masks, ACE_Event_Handler::ACCEPT_MASK))
    ACE_CLR_BITS (result, FD_ACCEPT);
  
  if (ACE_BIT_STRICTLY_ENABLED (to_be_removed_masks, ACE_Event_Handler::CONNECT_MASK))
    ACE_CLR_BITS (result, FD_CONNECT);
  
  if (ACE_BIT_STRICTLY_ENABLED (to_be_removed_masks, ACE_Event_Handler::QOS_MASK))
    ACE_CLR_BITS (result, FD_QOS);
  
  if (ACE_BIT_STRICTLY_ENABLED (to_be_removed_masks, ACE_Event_Handler::GROUP_QOS_MASK))
    ACE_CLR_BITS (result, FD_GROUP_QOS);
  
  if (ACE_BIT_STRICTLY_ENABLED (to_be_removed_masks, ACE_Event_Handler::CLOSE_MASK))
    ACE_CLR_BITS (result, FD_CLOSE);
  
  return result;
}

int
ACE_ReactorEx_Handler_Repository::unbind_i (ACE_HANDLE handle,
					    ACE_Reactor_Mask mask,
					    int &changes_required)
{
  int error = 0;

  // Remember this value; only if it changes do we need to wakeup
  // the other threads
  size_t original_handle_count = this->handles_to_be_deleted_;
  
  // Go through all the handles looking for <handle>.  Even if we find
  // it, we continue through the rest of the list since <handle> could
  // appear multiple times. All handles are checked.

  // First check the current entries
  int result = 0;
  for (size_t i = 0; i < this->max_handlep1_ && error == 0; i++)
    {
      // Since the handle can either be the event or the I/O handle,
      // we have to check both
      if (this->current_handles_[i] == handle || 
	  this->current_info_[i].io_handle_ == handle)
	{
	  result = this->remove_handler_i (i, mask);
	  if (result == -1)
	    error = 1;
	}
    }
  
  // Then check the suspended entries
  for (i = 0; i < this->suspended_handles_ && error == 0; i++)
    {
      // Since the handle can either be the event or the I/O handle,
      // we have to check both
      if (this->current_suspended_info_[i].io_handle_ == handle || 
	  this->current_suspended_info_[i].event_handle_ == handle)
	{
	  result = this->remove_suspended_handler_i (i, mask);
	  if (result == -1)
	    error = 1;
	}
    }
  
  // Only if the number of handlers to be deleted changes do we need
  // to wakeup the other threads
  if (original_handle_count < this->handles_to_be_deleted_)
    changes_required = 1;

  return error ? -1 : 0;
}

int 
ACE_ReactorEx_Handler_Repository::remove_handler_i (size_t index,
						    ACE_Reactor_Mask to_be_removed_masks)
{
  long result = 0;

  // I/O entries
  if (this->current_info_[index].io_entry_)
    {
      // See if there are other events that the <Event_Handler> is
      // interested in
      result = this->remove_network_events_i (this->current_info_[index].network_events_,
					      to_be_removed_masks);
      
      // Disassociate/Reassociate the event from the I/O handle. This
      // will depend on the value of <result>. I don't think we can do
      // anything about errors here, so I will not check this.
      ::WSAEventSelect ((SOCKET) this->current_info_[index].io_handle_,
			this->current_handles_[index],
			result);
    }
  
  // If there are no more events that the <Event_Handler> is
  // interested in, or this is a non-I/O entry, schedule the
  // <Event_Handler> for removal
  if (result == 0)
    {
      // Mark to be deleted
      this->current_info_[index].delete_entry_ = 1;
      // Remember the mask
      this->current_info_[index].close_masks_ = to_be_removed_masks;
      // Increment the handle count
      this->handles_to_be_deleted_++;
    }
      
  return 0;
}

int 
ACE_ReactorEx_Handler_Repository::remove_suspended_handler_i (size_t index,
							      ACE_Reactor_Mask to_be_removed_masks)
{
  long result = 0;

  // I/O entries
  if (this->current_suspended_info_[index].io_entry_)
    {
      // See if there are other events that the <Event_Handler> is
      // interested in
      result = this->remove_network_events_i (this->current_suspended_info_[index].network_events_,
					      to_be_removed_masks);
      
      // Disassociate/Reassociate the event from the I/O handle. This
      // will depend on the value of <result>. I don't think we can do
      // anything about errors here, so I will not check this.
      ::WSAEventSelect ((SOCKET) this->current_suspended_info_[index].io_handle_,
			this->current_suspended_info_[index].event_handle_,
			result);
    }

  // If there are no more events that the <Event_Handler> is
  // interested in, or this is a non-I/O entry, schedule the
  // <Event_Handler> for removal
  if (result == 0)
    {
      // Mark to be deleted
      this->current_suspended_info_[index].delete_entry_ = 1;
      // Remember the mask
      this->current_suspended_info_[index].close_masks_ = to_be_removed_masks;
      // Increment the handle count
      this->handles_to_be_deleted_++;
    }
      
  return 0;
}

int
ACE_ReactorEx_Handler_Repository::suspend_handler_i (ACE_HANDLE handle,
						     int &changes_required)
{
  // Remember this value; only if it changes do we need to wakeup
  // the other threads
  size_t original_handle_count = this->handles_to_be_suspended_;
  
  // Go through all the handles looking for <handle>.  Even if we find
  // it, we continue through the rest of the list since <handle> could
  // appear multiple times. All handles are checked.
  int result = 0;
  for (size_t i = 0; i < this->max_handlep1_; i++)
    {
      // Since the handle can either be the event or the I/O handle,
      // we have to check both
      if (this->current_handles_[i] == handle || 
	  this->current_info_[i].io_handle_ == handle)
	{
	  // Mark to be suspended
	  this->current_info_[i].suspend_entry_ = 1;
	  // Increment the handle count
	  this->handles_to_be_suspended_++;
	}
    }
  
  // Only if the number of handlers to be deleted changes do we need
  // to wakeup the other threads
  if (original_handle_count < this->handles_to_be_suspended_)
    changes_required = 1;
  
  return 0;
}

int
ACE_ReactorEx_Handler_Repository::resume_handler_i (ACE_HANDLE handle,
						    int &changes_required)
{
  // Remember this value; only if it changes do we need to wakeup
  // the other threads
  size_t original_handle_count = this->handles_to_be_resumed_;
  
  // Go through all the handles looking for <handle>.  Even if we find
  // it, we continue through the rest of the list since <handle> could
  // appear multiple times. All handles are checked.
  int result = 0;
  for (size_t i = 0; i < this->suspended_handles_; i++)
    {
      // Since the handle can either be the event or the I/O handle,
      // we have to check both
      if (this->current_suspended_info_[i].event_handle_ == handle || 
	  this->current_suspended_info_[i].io_handle_ == handle)
	{
	  this->current_suspended_info_[i].resume_entry_ = 1;
	  this->handles_to_be_resumed_++;
	}
    }
  
  // Only if the number of handlers to be deleted changes do we need
  // to wakeup the other threads
  if (original_handle_count < this->handles_to_be_resumed_)
    changes_required = 1;
  
  return 0;
}

void
ACE_ReactorEx_Handler_Repository::unbind_all (void)
{
  {
    ACE_GUARD (ACE_Process_Mutex, ace_mon, this->reactorEx_.lock_);
    
    // Remove all the handlers 
    for (size_t i = 0; i < this->max_handlep1_; i++)
      this->remove_handler_i (i, ACE_Event_Handler::NULL_MASK);

    for (i = 0; i < this->suspended_handles_; i++)
      this->remove_suspended_handler_i (i, ACE_Event_Handler::NULL_MASK);    
  }
  // The guard is released here
  
  // Wake up all threads in WaitForMultipleObjects so that they can
  // reconsult the handle set
  this->reactorEx_.wakeup_all_threads ();
}

int
ACE_ReactorEx_Handler_Repository::bind_i (int io_entry,
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

      // Assign *this* <ReactorEx> to the <Event_Handler>.
      event_handler->reactorEx (&this->reactorEx_);      

      // Wake up all threads in WaitForMultipleObjects so that they can
      // reconsult the handle set
      this->reactorEx_.wakeup_all_threads ();
    }
  else
    return -1;
  
  return 0;
}

int
ACE_ReactorEx_Handler_Repository::handle_deletions (void)
{
  // Go through the entire valid array and check for all handles that
  // have been schedule for deletion
  if (this->handles_to_be_deleted_ > 0 || this->handles_to_be_suspended_ > 0)
    {
      // This will help us in keeping track of the last valid index in the
      // handle arrays
      int last_valid_index = this->max_handlep1_ - 1;

      for (int i = last_valid_index; i >= 0; i--)
	{
	  // See if this entry is scheduled for deletion
	  if (this->current_info_[i].delete_entry_)
	    {
	      // Calling the <handle_close> method here will ensure that we
	      // will only call it once per deregistering <Event_Handler>.
	      // This is essential in the case when the <Event_Handler> will
	      // do something like delete itself and we have multiple
	      // threads in ReactorEx.
	      //
	      // Make sure that the DONT_CALL mask is not set
	      ACE_Reactor_Mask masks = this->current_info_[i].close_masks_;
	      if (ACE_BIT_ENABLED (masks, ACE_Event_Handler::DONT_CALL) == 0)
		{
		  // Grab the correct handle depending on the type entry
		  ACE_HANDLE handle;
		  if (this->current_info_[i].io_entry_)
		    handle = this->current_info_[i].io_handle_;
		  else
		    handle = this->current_handles_[i];

		  // Upcall
		  this->current_info_[i].event_handler_->handle_close (handle, masks);
		}

	      // If <ReactorEx> created the event, we need to clean it up
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
	      if (i == last_valid_index)
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
		  this->current_info_[i] = this->current_info_[last_valid_index];
		  this->current_handles_[i] = this->current_handles_[last_valid_index];
		  // Reset the info in the last slot
		  this->current_info_[last_valid_index].reset ();
		  this->current_handles_[last_valid_index] = ACE_INVALID_HANDLE;
		}
	      // Reset the last valid index and clean up the entry in the
	      // <to_be_deleted_set_>
	      last_valid_index--;
	    }
	}
      // Reset <this->max_handlep1_>
      this->max_handlep1_ = last_valid_index + 1;
    }

  return 0;
}

int
ACE_ReactorEx_Handler_Repository::handle_additions (void)
{
  // Go through the <to_be_added_*> arrays
  for (int i = 0; i < (int) this->handles_to_be_added_; i++)
    {
      // Add to the end of the current handles set
      this->current_handles_[this->max_handlep1_] = this->to_be_added_info_[i].event_handle_;
      // Struct copy
      this->current_info_[this->max_handlep1_].set (this->to_be_added_info_[i]);
      this->max_handlep1_++;

      // Reset the <to_be_added_info_>
      this->to_be_added_info_[i].reset ();      
    }
  // Since all to be added handles have been taken care of, reset the
  // counter
  this->handles_to_be_added_ = 0;

  // Go through the <suspended_handle> array
  if (this->handles_to_be_deleted_ > 0 || this->handles_to_be_resumed_ > 0)
    {
      int last_valid_index = this->suspended_handles_ - 1;
      for (i = last_valid_index; i >= 0; i--)
	{
	  // See if this entry is scheduled for deletion
	  if (this->current_suspended_info_[i].delete_entry_)
	    {
	      // Calling the <handle_close> method here will ensure that we
	      // will only call it once per deregistering <Event_Handler>.
	      // This is essential in the case when the <Event_Handler> will
	      // do something like delete itself and we have multiple
	      // threads in ReactorEx.
	      //
	      // Make sure that the DONT_CALL mask is not set
	      ACE_Reactor_Mask masks = this->current_suspended_info_[i].close_masks_;
	      if (ACE_BIT_ENABLED (masks, ACE_Event_Handler::DONT_CALL) == 0)
		{
		  // Grab the correct handle depending on the type entry
		  ACE_HANDLE handle;
		  if (this->current_suspended_info_[i].io_entry_)
		    handle = this->current_suspended_info_[i].io_handle_;
		  else
		    handle = this->current_suspended_info_[i].event_handle_;

		  // Upcall
		  this->current_suspended_info_[i].event_handler_->handle_close (handle, masks);
		}
	  
	      // If <ReactorEx> created the event, we need to clean it up
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
	      if (i == last_valid_index)
		// Reset the <suspended> arrays entries
		this->current_suspended_info_[i].reset ();      
	      else
		{
		  // Struct copy
		  this->current_suspended_info_[i] = this->current_suspended_info_[last_valid_index];
		  this->current_suspended_info_[last_valid_index].reset ();      
		}
	      // Reduce the number of suspended handles
	      last_valid_index--;
	    }
	}
      
      // Reset <this->suspended_handles_>
      this->suspended_handles_ = last_valid_index + 1;
    }

  return 0;
}

/************************************************************/

ACE_ReactorEx::ACE_ReactorEx (ACE_Sig_Handler *sh,
			      ACE_Timer_Queue *tq)
  : timer_queue_ (0),
    delete_timer_queue_ (0),
    handler_rep_ (*this),
    delete_handler_rep_ (0),
    // this event is initially signaled
    ok_to_wait_ (1), 
    // this event is initially unsignaled
    wakeup_all_threads_ (0),
    // this event is initially unsignaled
    waiting_to_change_state_ (0),
    new_owner_ (0),
    active_threads_ (0),
    owner_ (ACE_Thread::self ()),
    change_state_thread_ (0),
    open_for_business_ (0)
{
  if (this->open (ACE_ReactorEx::DEFAULT_SIZE, 0, sh, tq) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "ReactorEx"));
}

ACE_ReactorEx::ACE_ReactorEx (size_t size,
			      int unused,
			      ACE_Sig_Handler *sh,
			      ACE_Timer_Queue *tq)
  : timer_queue_ (0),
    delete_timer_queue_ (0),
    handler_rep_ (*this),
    delete_handler_rep_ (0),
    // this event is initially signaled
    ok_to_wait_ (1), 
    // this event is initially unsignaled
    wakeup_all_threads_ (0),
    // this event is initially unsignaled
    waiting_to_change_state_ (0),
    new_owner_ (0),
    active_threads_ (0),
    owner_ (ACE_Thread::self ()),
    change_state_thread_ (0),
    open_for_business_ (0)
{
  ACE_UNUSED_ARG (unused);

  if (this->open (size, 0, sh, tq) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "ReactorEx"));
}

ACE_ReactorEx *
ACE_ReactorEx::instance (void)
{
  ACE_TRACE ("ACE_ReactorEx::instance");

  if (ACE_ReactorEx::reactorEx_ == 0)
    {
      // Perform Double-Checked Locking Optimization.
      ACE_MT (ACE_GUARD_RETURN (ACE_Recursive_Thread_Mutex, ace_mon,
				*ACE_Static_Object_Lock::instance (), 0));

      if (ACE_ReactorEx::reactorEx_ == 0)
	{
	  ACE_NEW_RETURN (ACE_ReactorEx::reactorEx_, ACE_ReactorEx, 0);
	  ACE_ReactorEx::delete_reactorEx_ = 1;
	}
    }

  return ACE_ReactorEx::reactorEx_;
}

ACE_ReactorEx *
ACE_ReactorEx::instance (ACE_ReactorEx *r)
{
  ACE_TRACE ("ACE_ReactorEx::instance");

  ACE_MT (ACE_GUARD_RETURN (ACE_Recursive_Thread_Mutex, ace_mon,
			    *ACE_Static_Object_Lock::instance (), 0));
  ACE_ReactorEx *t = ACE_ReactorEx::reactorEx_;
  // We can't safely delete it since we don't know who created it!
  ACE_ReactorEx::delete_reactorEx_ = 0;

  ACE_ReactorEx::reactorEx_ = r;
  return t;
}

void
ACE_ReactorEx::close_singleton (void)
{
  ACE_TRACE ("ACE_ReactorEx::close_singleton");

  ACE_MT (ACE_GUARD (ACE_Recursive_Thread_Mutex, ace_mon,
		     *ACE_Static_Object_Lock::instance ()));

  if (ACE_ReactorEx::delete_reactorEx_)
    {
      delete ACE_ReactorEx::reactorEx_;
      ACE_ReactorEx::reactorEx_ = 0;
      ACE_ReactorEx::delete_reactorEx_ = 0;
    }
}

int
ACE_ReactorEx::run_event_loop (void)
{
  ACE_TRACE ("ACE_ReactorEx::run_event_loop");

  while (ACE_ReactorEx::end_event_loop_ == 0)
    {
      int result = ACE_ReactorEx::instance ()->handle_events ();

      if (ACE_Service_Config::reconfig_occurred ())
	ACE_Service_Config::reconfigure ();
      
      else if (result == -1)
	return -1;
    }
  /* NOTREACHED */
  return 0;
}


int
ACE_ReactorEx::run_event_loop (ACE_Time_Value &tv)
{
  ACE_TRACE ("ACE_ReactorEx::run_event_loop");

  while (ACE_ReactorEx::end_event_loop_ == 0 && tv != ACE_Time_Value::zero)
    {
      int result = ACE_ReactorEx::instance ()->handle_events (tv);
      if (ACE_Service_Config::reconfig_occurred ())
	ACE_Service_Config::reconfigure ();
      else if (result == -1)
	return result;
    }
  
  /* NOTREACHED */
  return 0;
}

int
ACE_ReactorEx::end_event_loop (void)
{
  ACE_TRACE ("ACE_ReactorEx::end_event_loop");
  ACE_ReactorEx::end_event_loop_ = 1;
  return ACE_ReactorEx::instance ()->notify ();
}

/* static */
sig_atomic_t
ACE_ReactorEx::event_loop_done (void)
{
  ACE_TRACE ("ACE_ReactorEx::end_event_loop");
  return ACE_ReactorEx::end_event_loop_;
}

int
ACE_ReactorEx::open (size_t size,
		     int unused,
		     ACE_Sig_Handler *sh,
		     ACE_Timer_Queue *tq)
{
  ACE_UNUSED_ARG (unused);
  ACE_UNUSED_ARG (sh);

  // This GUARD is necessary since we are updating shared state.
  ACE_GUARD_RETURN (ACE_Process_Mutex, ace_mon, this->lock_, -1);

  // If we are already open, return -1
  if (this->open_for_business_)
    return -1;

  // Setup the atomic wait array (used later in <handle_events>)
  this->atomic_wait_array_[0] = this->lock_.lock ().proc_mutex_;
  this->atomic_wait_array_[1] = this->ok_to_wait_.handle ();
  
  // This is to guard against reopens of ReactorEx
  if (this->delete_handler_rep_)
    this->handler_rep_.~ACE_ReactorEx_Handler_Repository ();

  // Open the handle repository
  // Two additional handles for internal purposes
  if (this->handler_rep_.open (size + 2) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", 
		       "opening handler repository"), 
		      -1);
  else
    this->delete_handler_rep_ = 1;

  // Open the notification handler
  if (this->notify_handler_.open (*this, timer_queue_) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", 
		       "opening notify handler "), 
		      -1);

  // Register for <wakeup_all_threads> event
  if (this->register_handler (&this->wakeup_all_threads_handler_, 
			      this->wakeup_all_threads_.handle ()) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", 
		       "registering thread wakeup handler"), 
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

  // Timer Queue 
  if (this->delete_timer_queue_)
    delete this->timer_queue_;

  if (tq == 0)
    {
      ACE_NEW_RETURN (this->timer_queue_, ACE_Timer_Heap, -1);
      this->delete_timer_queue_ = 1;
    }
  else 
    {
      this->timer_queue_ = tq;
      this->delete_timer_queue_ = 0;
    }

  // We are open for business
  this->open_for_business_ = 1;
  
  return 0;
}

int
ACE_ReactorEx::close (void)
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

ACE_ReactorEx::~ACE_ReactorEx (void)
{
  this->close ();

  if (this->delete_timer_queue_)
    {
      delete this->timer_queue_;
      this->timer_queue_ = 0;
      this->delete_timer_queue_ = 0;      
    }
}

int 
ACE_ReactorEx::register_handler_i (ACE_HANDLE event_handle,
				   ACE_HANDLE io_handle,
				   ACE_Event_Handler *event_handler,
				   ACE_Reactor_Mask mask)
{
  // Make sure that the <handle> is valid
  if (io_handle == ACE_INVALID_HANDLE)
    io_handle = event_handler->get_handle ();
  if (this->handler_rep_.invalid_handle (io_handle))
    return -1;
  
  long new_network_events = 0;
  int delete_event = 0;

  auto_ptr <ACE_Auto_Event> event;

  // Look up the repository to see if the <Event_Handler> is already
  // there.
  int found = this->handler_rep_.add_network_events_i (mask, 
						       io_handle, 
						       new_network_events,
						       event_handle,
						       delete_event);
  
  // Check to see if the user passed us a valid event; If not then we
  // need to create one
  if (event_handle == ACE_INVALID_HANDLE)
    {
      event = auto_ptr <ACE_Auto_Event> (new ACE_Auto_Event);
      event_handle = event->handle ();
      delete_event = 1;
    }  

  int result = ::WSAEventSelect ((SOCKET) io_handle, 
				 event_handle, 
				 new_network_events);

  // If we had found the <Event_Handler> there is nothing more to do
  if (found)
    return result;

  else
    {
      // The <Event_Handler was not found in the repository
      // Add to the repository
      if (result != SOCKET_ERROR &&
	  this->handler_rep_.bind_i (1, 
				     event_handler,
				     new_network_events,
				     io_handle,
				     event_handle, 
				     delete_event) != -1)
	{
	  if (delete_event)
	    event->handle (ACE_INVALID_HANDLE);
	  return 0;
	}
      else
	return -1;
    }
}

int
ACE_ReactorEx_Handler_Repository::add_network_events_i (ACE_Reactor_Mask mask,
							ACE_HANDLE io_handle,
							long &new_mask,
							ACE_HANDLE &event_handle,
							int &delete_event)
{
  int found = 0;
  size_t i;

  // First go through the current entries
  size_t total_entries = this->max_handlep1_;

  for (i = 0; i < total_entries && !found; i++)
    if (io_handle == this->current_info_[i].io_handle_)
      {
	found = 1;
	new_mask = this->current_info_[i].network_events_;
	delete_event = this->current_info_[i].delete_event_;
	event_handle = this->current_handles_[i];
      }

  // Then pass through the suspended handles
  total_entries = this->suspended_handles_;

  for (i = 0; i < total_entries && !found; i++)
    if (io_handle == this->current_suspended_info_[i].io_handle_)
      {
	found = 1;
	new_mask = this->current_suspended_info_[i].network_events_;
	delete_event = this->current_suspended_info_[i].delete_event_;
	event_handle = this->current_suspended_info_[i].event_handle_;
      }

  if (ACE_BIT_STRICTLY_ENABLED (mask, ACE_Event_Handler::READ_MASK))
    ACE_SET_BITS (new_mask, FD_READ);
  
  if (ACE_BIT_STRICTLY_ENABLED (mask, ACE_Event_Handler::WRITE_MASK))
    ACE_SET_BITS (new_mask, FD_WRITE);
  
  if (ACE_BIT_STRICTLY_ENABLED (mask, ACE_Event_Handler::EXCEPT_MASK))
    ACE_SET_BITS (new_mask, FD_OOB);

  if (ACE_BIT_STRICTLY_ENABLED (mask, ACE_Event_Handler::ACCEPT_MASK))
    ACE_SET_BITS (new_mask, FD_ACCEPT);

  if (ACE_BIT_STRICTLY_ENABLED (mask, ACE_Event_Handler::CONNECT_MASK))
    ACE_SET_BITS (new_mask, FD_CONNECT);

  if (ACE_BIT_STRICTLY_ENABLED (mask, ACE_Event_Handler::QOS_MASK))
    ACE_SET_BITS (new_mask, FD_QOS);

  if (ACE_BIT_STRICTLY_ENABLED (mask, ACE_Event_Handler::GROUP_QOS_MASK))
    ACE_SET_BITS (new_mask, FD_GROUP_QOS);

  if (ACE_BIT_STRICTLY_ENABLED (mask, ACE_Event_Handler::CLOSE_MASK))
    ACE_SET_BITS (new_mask, FD_CLOSE);

  return found;
}

// Waits for and dispatches all events.  Returns -1 on error, 0 if
// max_wait_time expired, or the number of events that were dispatched.
int 
ACE_ReactorEx::event_handling (ACE_Time_Value *max_wait_time,
			       int alertable)
{
  ACE_TRACE ("ACE_ReactorEx::event_handling");

  // Make sure we are not closed 
  if (!this->open_for_business_)
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
  // mutex and event.
  countdown.update ();

  // Wait for event to happen
  int wait_status = this->wait_for_multiple_events (max_wait_time,
						    alertable);

  // Upcall
  result = this->safe_dispatch (wait_status);

  return result;
}

int
ACE_ReactorEx::ok_to_wait (ACE_Time_Value *max_wait_time,
			   int alertable)
{
  // Calculate the max time we should spend here 
  //
  // Note: There is really no need to involve the <timer_queue_> here
  // because even if a timeout in the <timer_queue_> does expire we
  // will not be able to dispatch it
  int timeout = max_wait_time == 0 ? INFINITE : max_wait_time->msec ();

  // Atomically wait for both the <lock_> and <ok_to_wait_> event
  int result = ::WaitForMultipleObjectsEx (sizeof this->atomic_wait_array_ / sizeof (ACE_HANDLE),
					   this->atomic_wait_array_,
					   TRUE,
					   timeout,
					   alertable);
  
  switch (result)
    {
    case WAIT_TIMEOUT: 
      errno = ETIME;
      return 0;
    case WAIT_FAILED: 
    case WAIT_ABANDONED_0:
      errno = ::GetLastError ();
      return -1;      
    default:
      break;
    }  
  
  // It is ok to enter ::WaitForMultipleObjects
  return 1;
}

int
ACE_ReactorEx::wait_for_multiple_events (ACE_Time_Value *max_wait_time,
					 int alertable)
{
  int timeout = this->calculate_timeout (max_wait_time);

  // Wait for any of handles_ to be active, or until timeout expires.
  // If <alertable> is enabled allow asynchronous completion of
  // ReadFileEx and WriteFileEx operations.
  return ::WaitForMultipleObjectsEx (this->handler_rep_.max_handlep1 (),
				     this->handler_rep_.handles (),
				     FALSE,
				     timeout,
				     alertable);
}

int
ACE_ReactorEx::calculate_timeout (ACE_Time_Value *max_wait_time)
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
ACE_ReactorEx::dispatch (int wait_status)
{
  // If "owner" thread
  if (ACE_Thread::self () == this->owner_)
    // Expire all pending timers.
    this->timer_queue_->expire ();

  switch (wait_status)
    {
    case WAIT_FAILED: // Failure.
      errno = ::GetLastError ();
      return -1;
    case WAIT_TIMEOUT: // Timeout.
      errno = ETIME;
      return 0;
    case WAIT_ABANDONED_0:
      // We'll let dispatch worry about abandoned mutexes.
    default:  // Dispatch.
      return this->dispatch_handles (wait_status - WAIT_OBJECT_0);
    }
}

// Dispatches any active handles from <handles_[index]> to
// <handles_[max_handlep1_]> using <WaitForMultipleObjects> to poll
// through our handle set looking for active handles.

int
ACE_ReactorEx::dispatch_handles (size_t index)
{
  for (int number_of_handlers_dispatched = 1;
       ; 
       number_of_handlers_dispatched++)
    {
      this->dispatch_handler (index++);

      // We're done.
      if (index >= this->handler_rep_.max_handlep1 ())
	return number_of_handlers_dispatched;
      
      DWORD wait_status = 
	::WaitForMultipleObjects (this->handler_rep_.max_handlep1 () - index,
				  this->handler_rep_.handles () + index,
				  FALSE, 0); // We're polling.
      switch (wait_status)
	{
	case WAIT_FAILED: // Failure.
	  errno = ::GetLastError ();
	  /* FALLTHRU */
	case WAIT_TIMEOUT:
	  // There are no more handles ready, we can return.
	  return number_of_handlers_dispatched;
	default: // Dispatch.
	  // Check if a handle successfully became signaled.
	  if (wait_status >= WAIT_OBJECT_0 && 
	      wait_status < WAIT_OBJECT_0 + this->handler_rep_.max_handlep1 ())
	    index += wait_status - WAIT_OBJECT_0;
	  else
	    // Otherwise, a handle was abandoned.
	    index += wait_status - WAIT_ABANDONED_0;
	}
    }
}

// Dispatches a single handler.  Returns 0 on success, -1 if the
// handler was removed.

int
ACE_ReactorEx::dispatch_handler (int index)
{
  // Dispatch the handler if it has not been scheduled for deletion.
  // Note that this is a very week test if there are multiple threads
  // dispatching this index as no locks are held here. Generally, you
  // do not want to do something like deleting the this pointer in
  // handle_close() if you have registered multiple times and there is
  // more than one thread in ReactorEx->handle_events().
  if (!this->handler_rep_.scheduled_for_deletion (index))
    {
      ACE_HANDLE event_handle = *(this->handler_rep_.handles () + index);

      if (this->handler_rep_.current_info ()[index].io_entry_)
	return this->complex_dispatch_handler (index, event_handle);
      else
	return this->simple_dispatch_handler (index, event_handle);
    }
  return 0;
}

int
ACE_ReactorEx::simple_dispatch_handler (int index, 
					ACE_HANDLE event_handle)
{
  // This dispatch is used for non-I/O entires

  // Assign the ``signaled'' HANDLE so that callers can get it.
  siginfo_t sig (event_handle);

  ACE_Event_Handler *eh = this->handler_rep_.current_info ()[index].event_handler_;
  
  // Upcall
  if (eh->handle_signal (0, &sig) == -1)
    this->handler_rep_.unbind (event_handle, ACE_Event_Handler::NULL_MASK);
  return 0;
}

int
ACE_ReactorEx::complex_dispatch_handler (int index, 
					 ACE_HANDLE event_handle)
{
  // This dispatch is used for I/O entires

  ACE_ReactorEx_Handler_Repository::Current_Info &current_info = 
    this->handler_rep_.current_info ()[index];
  
  // Upcall
  if (this->upcall (current_info.event_handler_,
		    current_info.io_handle_, 
		    event_handle,
		    current_info.network_events_) == -1)
    this->handler_rep_.unbind (event_handle, ACE_Event_Handler::NULL_MASK);
  
  return 0;
}

int
ACE_ReactorEx::upcall (ACE_Event_Handler *event_handler,
		       ACE_HANDLE io_handle, 
		       ACE_HANDLE event_handle, 
		       long interested_events)
{
  // This method figures out what exactly has happened to the socket and then calls appropriate methods
  int result = 0;
  WSANETWORKEVENTS events;
  
  if (::WSAEnumNetworkEvents ((SOCKET) io_handle,
			      event_handle,
			      &events) == SOCKET_ERROR)
    return -1;
  else
    {
      long actual_events = events.lNetworkEvents;

      if (result != -1 && interested_events & actual_events & FD_READ)
	result = event_handler->handle_input (io_handle);

      if (result != -1 && interested_events & actual_events & FD_WRITE)
	result = event_handler->handle_output (io_handle);

      if (result != -1 && interested_events & actual_events & FD_OOB)
	result = event_handler->handle_exception (io_handle);

      if (result != -1 && interested_events & actual_events & FD_ACCEPT)
	result = event_handler->handle_input (io_handle);
      
      if (result != -1 && interested_events & actual_events & FD_CONNECT)
	{
	  if (events.iErrorCode[FD_CONNECT_BIT] == 0)
	    // Successful connect
	    result = event_handler->handle_output (io_handle);
	  else
	    // Unsuccessful connect
	    result = event_handler->handle_input (io_handle);
	}
      
      if (result != -1 && interested_events & actual_events & FD_CLOSE)
	result = event_handler->handle_close (io_handle, ACE_Event_Handler::CLOSE_MASK);
      
      if (result != -1 && interested_events & actual_events & FD_QOS)
	result = event_handler->handle_qos (io_handle);
      
      if (result != -1 && interested_events & actual_events & FD_GROUP_QOS)
	result = event_handler->handle_group_qos (io_handle);
    }

  return result;
}

 
ACE_ReactorEx::update_state (void)
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
	  
	  if (this->handler_rep_.changes_required ())
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

  return 0;
}

// ************************************************************

ACE_ReactorEx_Notify::ACE_ReactorEx_Notify (void)
  : max_notify_iterations_ (-1),
    timer_queue_ (0)
{
}

int 
ACE_ReactorEx_Notify::open (ACE_ReactorEx &reactorEx,
			    ACE_Timer_Queue *timer_queue)
{
  timer_queue_ = timer_queue;
  return reactorEx.register_handler (this);
}

ACE_HANDLE
ACE_ReactorEx_Notify::get_handle (void) const
{
  return this->wakeup_one_thread_.handle ();
}

// Handle all pending notifications.

int
ACE_ReactorEx_Notify::handle_signal (int signum, 
				     siginfo_t *siginfo, 
				     ucontext_t *)
{
  ACE_UNUSED_ARG (signum);

  // Just check for sanity...
  if (siginfo->si_handle_ != this->wakeup_one_thread_.handle ())
    return -1;

  // This will get called when <ReactorEx->wakeup_one_thread_> event
  // is signaled.
  //  ACE_DEBUG ((LM_DEBUG, "(%t) waking up to handle internal notifications\n"));

  for (int i = 1; ; i++)
    {
      ACE_Message_Block *mb = 0;
  
      if (this->message_queue_.dequeue_head 
	  (mb, (ACE_Time_Value *) &ACE_Time_Value::zero) == -1)
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
		  result = buffer->eh_->handle_input (ACE_INVALID_HANDLE);
		  break;
		case ACE_Event_Handler::WRITE_MASK:
		  result = buffer->eh_->handle_output (ACE_INVALID_HANDLE);
		  break;
		case ACE_Event_Handler::EXCEPT_MASK:
		  result = buffer->eh_->handle_exception (ACE_INVALID_HANDLE);
		  break;
		default:
		  ACE_ERROR ((LM_ERROR, "invalid mask = %d\n", buffer->mask_));
		  break;
		}
	      if (result == -1)
		buffer->eh_->handle_close (ACE_INVALID_HANDLE, 
					   ACE_Event_Handler::EXCEPT_MASK);
	    }

	  // Make sure to delete the memory regardless of success or
	  // failure!
	  mb->release ();

	  // Bail out if we've reached the <notify_threshold_>.  Note
	  // that by default <notify_threshold_> is -1, so we'll loop
	  // until we're done.
	  if (i == this->max_notify_iterations_)
	    break;
	}
    }
}

// Notify the ReactorEx, potentially enqueueing the
// <ACE_Event_Handler> for subsequent processing in the ReactorEx
// thread of control.

int 
ACE_ReactorEx_Notify::notify (ACE_Event_Handler *eh, 
			      ACE_Reactor_Mask mask,
			      ACE_Time_Value *timeout)
{
  if (eh != 0)
    {
      ACE_Message_Block *mb = 0;
      ACE_NEW_RETURN (mb, ACE_Message_Block (sizeof ACE_Notification_Buffer), -1);

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
ACE_ReactorEx_Notify::max_notify_iterations (int iterations)
{
  ACE_TRACE ("ACE_ReactorEx_Notify::max_notify_iterations");
  // Must always be > 0 or < 0 to optimize the loop exit condition.
  if (iterations == 0)
    iterations = 1;

  this->max_notify_iterations_ = iterations;
}

int 
ACE_ReactorEx_Notify::max_notify_iterations (void)
{
  ACE_TRACE ("ACE_ReactorEx_Notify::max_notify_iterations");
  return this->max_notify_iterations_;
}

void 
ACE_ReactorEx::max_notify_iterations (int iterations)
{
  ACE_TRACE ("ACE_ReactorEx::max_notify_iterations");
  ACE_GUARD (ACE_Process_Mutex, monitor, this->lock_);

  // Must always be > 0 or < 0 to optimize the loop exit condition.
  this->notify_handler_.max_notify_iterations (iterations);
}

int 
ACE_ReactorEx::max_notify_iterations (void)
{
  ACE_TRACE ("ACE_ReactorEx::max_notify_iterations");
  ACE_GUARD_RETURN (ACE_Process_Mutex, monitor, this->lock_, -1);

  return this->notify_handler_.max_notify_iterations ();
}

// No-op WinSOCK2 methods to help ReactorEx compile
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

#else /* ACE_WIN32 */

ACE_ReactorEx *
ACE_ReactorEx::instance (void)
{
  return NULL;
}

ACE_ReactorEx *
ACE_ReactorEx::instance (ACE_ReactorEx *r)
{
  ACE_UNUSED_ARG (r);
  return NULL;
}

void
ACE_ReactorEx::close_singleton (void)
{
}

int 
ACE_ReactorEx::run_event_loop (void)
{
  // not implemented
  return -1;
}

int 
ACE_ReactorEx::run_event_loop (ACE_Time_Value &tv)
{
  // not implemented
  ACE_UNUSED_ARG (tv);
  return -1;
}

int 
ACE_ReactorEx::end_event_loop (void)
{
  // not implemented
  return -1;
}

sig_atomic_t 
ACE_ReactorEx::event_loop_done (void)
{
  return sig_atomic_t(1);
}

#endif /* ACE_WIN32 */

