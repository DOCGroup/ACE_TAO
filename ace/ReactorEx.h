/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    ReactorEx.h
//
// = AUTHOR
//    Tim Harrison and Doug Schmidt
// 
// ============================================================================

#if !defined (ACE_REACTOREX_H)
#define ACE_REACTOREX_H

#include "ace/Time_Value.h"
#include "ace/Timer_Queue.h"
#include "ace/Event_Handler.h"
#include "ace/Message_Queue.h"
#include "ace/Synch.h"

// Forward decl.
class ACE_ReactorEx;
 
class ACE_Export ACE_Wakeup_All_Threads_Handler : public ACE_Event_Handler
  // = TITLE
  //     This is a helper class whose sole purpose is to handle events
  //     on <ACE_ReactorEx->wakeup_all_threads_>  
{
public:

  virtual int handle_signal (int signum, siginfo_t * = 0, ucontext_t * = 0);
  // Called when the <ACE_ReactorEx->wakeup_all_threads_>
  
private:
};

class ACE_Export ACE_ReactorEx_Handler_Repository
  // = TITLE
  //     Used to map <ACE_HANDLE>s onto the appropriate
  //     <ACE_Event_Handler> *. 
  // 
{
public:
  ACE_ReactorEx_Handler_Repository (ACE_ReactorEx &reactorEx);
  // Constructor.

  virtual ~ACE_ReactorEx_Handler_Repository (void);
  // Destructor.

  int open (size_t size);
  // Initialize the repository of the approriate <size>.
  
  int close (void);
  // Close down the handler repository.

  // = Search structure operations.

  int bind (ACE_HANDLE, ACE_Event_Handler *);
  // Bind the <ACE_Event_Handler *> to the <ACE_HANDLE>.

  int unbind (ACE_HANDLE, ACE_Reactor_Mask mask);
  // Remove the binding of <ACE_HANDLE> in accordance with the <mask>.

  void unbind_all (void);
  // Remove all bindings of <ACE_HANDLE, ACE_Event_Handler> tuples.

  // = Sanity checking.

  // Check the <handle> to make sure it's a valid ACE_HANDLE 
  int invalid_handle (ACE_HANDLE handle) const;

  // = Accessors.
  size_t max_handlep1 (void) const;
  // Maximum ACE_HANDLE value, plus 1.

  ACE_HANDLE *handles (void) const;
  // Pointer to the beginning of the current array of <ACE_HANDLE>
  // *'s.

  ACE_Event_Handler **event_handlers (void) const;
  // Pointer to the beginning of the current array of
  // <ACE_Event_Handler> *'s.

  virtual int changes_required (void);
  // Check if changes to the handle set are required.

  virtual int make_changes (void);
  // Make changes to the handle set

  int scheduled_for_deletion (int index) const;
  // Check to see if <index> has been scheduled for deletion

  void dump (void) const;
  // Dump the state of an object.

private:
  ACE_ReactorEx &reactorEx_;
  // Reference to our <ReactorEx>.
  
  int handle_deletions (void);
  // Add handles to the handle set

  int handle_additions (void);
  // Remove handles from the handle set

  int remove_handler_i (size_t index,
			ACE_Reactor_Mask mask = 0);
  // Removes the <ACE_Event_Handler> at <index> from the table.

  size_t max_size_;
  // Maximum number of handles.

  size_t max_handlep1_;
  // The highest currently active handle, plus 1.  Since this ranges
  // between 0 and <max_size_> it also is a count of the number of
  // active handles.

  ACE_HANDLE *current_handles_;
  // Array of <ACE_HANDLEs> passed to <WaitForMultipleObjects>.

  ACE_Event_Handler **current_event_handlers_;
  // Array of <ACE_Event_Handler> pointers that store the event
  // handlers to dispatch when the corresponding <handles_[i]> entry
  // becomes signaled.

  ACE_HANDLE *to_be_added_handles_;
  // These handles are going to be added to the <current_set_>

  ACE_Event_Handler **to_be_added_event_handlers_;
  // Corresponding Event Handler to the above

  int handles_to_be_added_;
  // A count of the number of records to be added
  
  int *to_be_deleted_set_;
  // These are going to be deleted from the <current_handles_>
  
  int handles_to_be_deleted_;
  // A count of the number of records to be deleted
};

class ACE_Export ACE_ReactorEx_Notify : public ACE_Event_Handler
  // = TITLE
  //     Unblock the <ACE_ReactorEx> from its event loop, passing it an
  //     optional <ACE_Event_Handler> to dispatch.
  //
  // = DESCRIPTION
  //     This implementation is necessary for cases where the
  //     <ACE_ReactorEx> is run in a multi-threaded program.  In this
  //     case, we need to be able to unblock WaitForMultipleObjects()
  //     when updates occur other than in the main <ACE_ReactorEx>
  //     thread.  To do this, we signal an auto-reset event the
  //     <ACE_ReactorEx> is listening on.  If an <ACE_Event_Handler>
  //     and <ACE_Reactor_Mask> is passed to <notify>, the appropriate
  //     <handle_*> method is dispatched.
{
public:
  ACE_ReactorEx_Notify (void);
  // Constructor

  int open (ACE_ReactorEx &reactorEx);
  // Initialization

  int notify (ACE_Event_Handler *eh = 0,
	      ACE_Reactor_Mask mask = ACE_Event_Handler::EXCEPT_MASK,
	      ACE_Time_Value *timeout = 0);
  // Special trick to unblock WaitForMultipleObjects() when updates
  // occur.  All we do is enqueue <eh> and <mask> onto the
  // <ACE_Message_Queue> and wakeup the ReactorEx by signaling its
  // <ACE_Event> handle.  The <ACE_Time_Value> indicates how long to
  // blocking trying to notify the <Reactor>.  If <timeout> == 0, the
  // caller will block until action is possible, else will wait until
  // the relative time specified in <timeout> elapses).

  virtual ACE_HANDLE get_handle (void) const;
  // Returns a handle to the <ACE_Auto_Event>.

  void max_notify_iterations (int);
  // Set the maximum number of times that the
  // <ACE_ReactorEx_Notify::handle_input> method will iterate and
  // dispatch the <ACE_Event_Handlers> that are passed in via the
  // notify queue before breaking out of its
  // <ACE_Message_Queue::dequeue> loop.  By default, this is set to
  // -1, which means "iterate until the queue is empty."  Setting this
  // to a value like "1 or 2" will increase "fairness" (and thus
  // prevent starvation) at the expense of slightly higher dispatching
  // overhead.

  int max_notify_iterations (void);
  // Get the maximum number of times that the
  // <ACE_ReactorEx_Notify::handle_input> method will iterate and
  // dispatch the <ACE_Event_Handlers> that are passed in via the
  // notify queue before breaking out of its
  // <ACE_Message_Queue::dequeue> loop.

private:
  virtual int handle_signal (int signum, siginfo_t * = 0, ucontext_t * = 0);
  // Called when the notification event waited on by <ACE_ReactorEx>
  // is signaled.  This dequeues all pending <ACE_Event_Handlers> and
  // dispatches them.

  ACE_Auto_Event wakeup_one_thread_;
  // An auto event is used so that we can <signal> it to wakeup one
  // thread up (e.g., when the <notify> method is called).

#if defined (ACE_WIN32)  // because Sun C++ 4.1 can't cope with this declaration:
  ACE_Message_Queue<ACE_MT_SYNCH> message_queue_;
#endif /* ACE_WIN32 */
  // Message queue that keeps track of pending <ACE_Event_Handlers>.
  // This queue must be thread-safe because it can be called by
  // multiple threads of control.

  int max_notify_iterations_;
  // Keeps track of the maximum number of times that the
  // <ACE_ReactorEx_Notify::handle_input> method will iterate and
  // dispatch the <ACE_Event_Handlers> that are passed in via the
  // notify queue before breaking out of its
  // <ACE_Message_Queue::dequeue> loop.  By default, this is set to
  // -1, which means "iterate until the queue is empty."
};

#if defined (ACE_WIN32)
class ACE_Export ACE_ReactorEx
  // = TITLE
  //     An object oriented event demultiplexor and event handler
  //     ReactorEx for Win32 WaitForMultipleObjects
  //     
  // = DESCRIPTION
  //     The ACE_ReactorEx is an object-oriented event demultiplexor
  //     and event handler ReactorEx.  The sources of events that the
  //     ACE_ReactorEx waits for and dispatches includes I/O events,
  //     general Win32 synchronization events (such as mutexes,
  //     semaphores, threads, etc.) and timer events.
{
  friend class ACE_ReactorEx_Handler_Repository;
public:
  enum 
  {
    DEFAULT_SIZE = MAXIMUM_WAIT_OBJECTS - 2
    // Default size of the ReactorEx's handle table. Two slots will be
    // added to the <size> parameter in the constructor and open
    // methods which will store handles used for internal management
    // purposes.
  };
  
  // = Initialization and termination methods.

  ACE_ReactorEx (ACE_Sig_Handler * = 0,
		 ACE_Timer_Queue * = 0);
  // Initialize <ACE_ReactorEx> with the default size.

  ACE_ReactorEx (size_t size, 
		 int unused = 0,
		 ACE_Sig_Handler * = 0,
		 ACE_Timer_Queue * = 0);
  // Initialize <ACE_ReactorEx> with size <size>.  Two slots will be
  // added to the <size> parameter which will store handles used for
  // internal management purposes.

  virtual int open (size_t size = DEFAULT_SIZE, 
		    int restart = 0, 
		    ACE_Sig_Handler * = 0,
		    ACE_Timer_Queue * = 0);
  // Initialize <ACE_ReactorEx> with size <size>.  Two slots will be
  // added to the <size> parameter which will store handles used for
  // internal management purposes.

  virtual int close (void);
  // Close down the ReactorEx and release all of its resources.

  virtual ~ACE_ReactorEx (void);
  // Close down the ReactorEx and release all of its resources.
  
  // = Event loop drivers.  

  virtual int handle_events (ACE_Time_Value *max_wait_time = 0,
			     int alertable = 0);
  // This event loop driver blocks for up to <max_wait_time> before
  // returning.  It will return earlier if timer events, I/O events,
  // or signal events occur.  Note that <max_wait_time> can be 0, in
  // which case this method blocks indefinitely until events occur.
  //
  // <max_wait_time> is decremented to reflect how much time this call
  // took.  For instance, if a time value of 3 seconds is passed to
  // handle_events and an event occurs after 2 seconds,
  // <max_wait_time> will equal 1 second.  This can be used if an
  // application wishes to handle events for some fixed amount of
  // time.
  // 
  // WaitForMultipleObjectsEx> is used as the demultiplexing call
  // 
  // Returns the total number of <ACE_Event_Handler>s that were
  // dispatched, 0 if the <max_wait_time> elapsed without dispatching
  // any handlers, or -1 if an error occurs.

  virtual int handle_events (ACE_Time_Value &max_wait_time,
			     int alertable = 0);
  // This method is just like the one above, except the
  // <max_wait_time> value is a reference and can therefore never be
  // NULL.

  // = Register and remove Handlers. 

  virtual int register_handler (ACE_Event_Handler *eh, 
				ACE_HANDLE handle = ACE_INVALID_HANDLE);
  // Register an <ACE_Event_Handler> <eh>.  If <handle> ==
  // <ACE_INVALID_HANDLE> the <ACE_ReactorEx> will call the
  // <get_handle> method of <eh> to extract the underlying I/O handle.
 
  virtual int remove_handler (ACE_Event_Handler *eh,
			      ACE_Reactor_Mask mask = 0);
  // Removes <eh> from the <ACE_ReactorEx>.  Note that the
  // <ACE_ReactorEx> will call the <get_handle> method of <eh> to
  // extract the underlying I/O handle.  If <mask> ==
  // <ACE_Event_Handler::DONT_CALL> then the <handle_close> method of
  // the <eh> is not invoked.

  virtual int remove_handler (ACE_HANDLE handle, 
			      ACE_Reactor_Mask mask = 0);
  // Removes <handle> from the <ACE_ReactorEx>.  If <mask> ==
  // <ACE_Event_Handler::DONT_CALL> then the <handle_close> method of
  // the <eh> is not invoked.

  // Removes the <mask> bind of <Event_Handler> whose handle is
  // <handle> from the Reactor.  If there are no more bindings for
  // this <eh> then it is removed from the Reactor.

  // = Timer management. 

  virtual int schedule_timer (ACE_Event_Handler *eh,
			      const void *arg,
			      const ACE_Time_Value &delta,
			      const ACE_Time_Value &interval = ACE_Time_Value::zero);  
  // Schedule an <event_handler> that will expire after <delay> amount
  // of time.  If it expires then <arg> is passed in as the value to
  // the <event_handler>'s <handle_timeout> callback method.  If
  // <interval> is != to <ACE_Time_Value::zero> then it is used to
  // reschedule the <event_handler> automatically.  This method
  // returns a <timer_id> that uniquely identifies the <event_handler>
  // in an internal list.  This <timer_id> can be used to cancel an
  // <event_handler> before it expires.  The cancellation ensures that
  // <timer_ids> are unique up to values of greater than 2 billion
  // timers.  As long as timers don't stay around longer than this
  // there should be no problems with accidentally deleting the wrong
  // timer.  Returns -1 on failure (which is guaranteed never to be a
  // valid <timer_id>.

  virtual int cancel_timer (ACE_Event_Handler *event_handler);
  // Cancel all Event_Handlers that match the address of
  // <event_handler>.  Returns number of handler's cancelled.

  virtual int cancel_timer (int timer_id, const void **arg = 0);
  // Cancel the single Event_Handler that matches the <timer_id> value
  // (which was returned from the schedule method).  If arg is
  // non-NULL then it will be set to point to the ``magic cookie''
  // argument passed in when the Event_Handler was registered.  This
  // makes it possible to free up the memory and avoid memory leaks.
  // Returns 1 if cancellation succeeded and 0 if the <timer_id>
  // wasn't found.

  // = Notification methods.

  int notify (ACE_Event_Handler * = 0, 
	      ACE_Reactor_Mask = ACE_Event_Handler::EXCEPT_MASK,
	      ACE_Time_Value * = 0);
  // Wakeup one <ACE_ReactorEx> thread if it is currently blocked in
  // <WaitForMultipleObjects>.  The <ACE_Time_Value> indicates how
  // long to blocking trying to notify the <Reactor>.  If <timeout> ==
  // 0, the caller will block until action is possible, else will wait
  // until the relative time specified in <timeout> elapses).

  void max_notify_iterations (int);
  // Set the maximum number of times that the
  // <ACE_ReactorEx_Notify::handle_input> method will iterate and
  // dispatch the <ACE_Event_Handlers> that are passed in via the
  // notify queue before breaking out of its
  // <ACE_Message_Queue::dequeue> loop.  By default, this is set to
  // -1, which means "iterate until the queue is empty."  Setting this
  // to a value like "1 or 2" will increase "fairness" (and thus
  // prevent starvation) at the expense of slightly higher dispatching
  // overhead.

  int max_notify_iterations (void);
  // Get the maximum number of times that the
  // <ACE_ReactorEx_Notify::handle_input> method will iterate and
  // dispatch the <ACE_Event_Handlers> that are passed in via the
  // notify queue before breaking out of its
  // <ACE_Message_Queue::dequeue> loop.

  void dump (void) const;
  // Dump the state of an object.

  ACE_thread_t owner (void);
  // Return the ID of the "owner" thread.

  void owner (ACE_thread_t new_owner);
  // Transfers ownership of the ReactorEx to the <new_owner>. The
  // transfer will not complete until all threads are ready (just like
  // the handle set).

  void wakeup_all_threads (void);
  // Wake up all threads in WaitForMultipleObjects so that they can
  // reconsult the handle set

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:
  virtual int ok_to_wait (ACE_Time_Value *max_wait_time,
			  int alertable);
  // Check to see if it is ok to enter ::WaitForMultipleObjects().
  
  virtual int wait_for_multiple_events (ACE_Time_Value *max_wait_time,
					int alertable);
  // Wait for timer and I/O events to occur.

  virtual int dispatch (int wait_status);
  // Dispatches the timers and I/O handlers.

  int dispatch_handles (size_t index);
  // Dispatches any active handles from handles_[<index>] to
  // handles_[active_handles_] using <WaitForMultipleObjects> to poll
  // through our handle set looking for active handles.

  int dispatch_handler (int index);
  // Dispatches a single handler.  Returns 0 on success, -1 if the
  // handler was removed.

private:
  ACE_ReactorEx (const ACE_ReactorEx &);
  ACE_ReactorEx &operator = (const ACE_ReactorEx &);
  // Deny access since member-wise won't work...

  int calculate_timeout (ACE_Time_Value *time);
  // Used to caluculate the next timeout

  int update_state (void);
  // Update the state of the handler repository

  int new_owner (void);
  // Check to see if we have a new owner

  int change_owner (void);
  // Set owner to new owner

  ACE_Timer_Queue *timer_queue_;
  // Defined as a pointer to allow overriding by derived classes...

  int delete_timer_queue_;
  // Keeps track of whether we should delete the timer queue (if we
  // didn't create it, then we don't delete it).

  int delete_handler_rep_;
  // Keeps track of whether we should delete the handler repository

  ACE_Process_Mutex lock_;
  // Synchronization for the ACE_Reactor.
  // A Process Mutex is used here because of two reasons:
  // (a) The implementation of ACE_Thread_Mutex uses CriticalSections
  //     CriticalSections are not waitable using ::WaitForMultipleObjects
  // (b) This is really not a process mutex because it is not
  //     named. No other process can use this mutex.

  ACE_ReactorEx_Handler_Repository handler_rep_;
  // Table that maps <ACE_HANDLEs> to <ACE_Event_Handler *>'s.

  ACE_ReactorEx_Notify notify_handler_;
  // Used when <notify> is called.

  ACE_Manual_Event ok_to_wait_;
  // A manual event used to block threads from proceeding into
  // WaitForMultipleObjects

  ACE_Manual_Event wakeup_all_threads_;
  // A manual event is used so that we can wake everyone up (e.g.,
  // when <ACE_Event_Handlers> are bounded and unbound from the
  // handler repository).

  ACE_Wakeup_All_Threads_Handler wakeup_all_threads_handler_;
  // Used when <wakeup_all_threads_> is signaled

  ACE_Auto_Event waiting_to_change_state_;
  // The changing thread waits on this event, till all threads are not
  // active anymore 

  int active_threads_;
  // Count of currently active threads

  ACE_thread_t owner_;
  // The thread which is "owner" of the ReactorEx. The owner concept
  // is used because we don't want multiple threads to try to expire
  // timers. Therefore the "owner" thread is the only one allowed to
  // expire timers. Also, the owner thread is the only thread which
  // waits on the notify handle. Note that the ownership can be
  // transferred.

  ACE_thread_t new_owner_;
  // The owner to be of the ReactorEx

  ACE_thread_t change_state_thread_;
  // This is the thread which is responsible for the changing the
  // state of the <ReactorEx> handle set

  ACE_HANDLE atomic_wait_array_ [2];
  // This is an array of ACE_HANDLEs which keep track of the <lock_>
  // and <ok_to_wait_> handles

  int open_for_business_;
  // This flag is used to keep track of whether we are already closed.
};

#else /* NOT win32 */
class ACE_Export ACE_ReactorEx
{
public:
  virtual int handle_events (void) { return -1; }
  virtual int handle_events (ACE_Time_Value &) { return -1; }
  int notify (ACE_Event_Handler * = 0,
	      ACE_Reactor_Mask = ACE_Event_Handler::EXCEPT_MASK)
  { return 0; }
};

#endif /* ACE_WIN32 */

#if defined (__ACE_INLINE__)
#include "ace/ReactorEx.i"
#endif /* __ACE_INLINE__ */
#endif /* ACE_REACTOREX_H */
