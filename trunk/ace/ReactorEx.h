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
#include "ace/Token.h"
#include "ace/Synch.h"

#if defined (ACE_MT_SAFE)

#if defined (ACE_REACTOREX_HAS_DEADLOCK_DETECTION)
#include "ace/Local_Tokens.h"
typedef ACE_Local_Mutex ACE_REACTOREX_MUTEX;
#else 
typedef ACE_Token ACE_REACTOREX_MUTEX;
#endif /* ACE_REACTOR_HAS_DEADLOCK_DETECTION */

// Forward decl.
class ACE_ReactorEx;
 
class ACE_Export ACE_ReactorEx_Handle_Set
  // = TITLE 
  //      Track handles we are interested for various events.
{
public:
};

class ACE_Export ACE_ReactorEx_Handler_Repository
  // = TITLE
  //     Used to map <ACE_HANDLE>s onto the appropriate
  //     <ACE_Event_Handler> *. 
  // 
{
public:
  ACE_ReactorEx_Handler_Repository (ACE_ReactorEx &);
  // Default "do-nothing" constructor.

  int open (size_t size);
  // Initialize the repository of the approriate <size>.
  
  int close (void);
  // Close down the handler repository.

  // = Search structure operations.

  ACE_Event_Handler *find (size_t index) const;
  // Return the <ACE_Event_Handler *> associated with <index>.  Return
  // 0 if <index> is invalid.

  int bind (ACE_HANDLE, ACE_Event_Handler *);
  // Bind the <ACE_Event_Handler *> to the <ACE_HANDLE>.

  int unbind (ACE_HANDLE, ACE_Reactor_Mask mask);
  // Remove the binding of <ACE_HANDLE> in accordance with the <mask>.

  void unbind_all (void);
  // Remove all bindings of <ACE_HANDLE, ACE_Event_Handler> tuples.

  // = Sanity checking.

  // Check the <handle> to make sure it's a valid ACE_HANDLE that
  // within the range of legal handles (i.e., >= 0 && < max_size_).
  int invalid_handle (ACE_HANDLE handle);

  // Check the <index> to make sure it's within the range of currently
  // registered HANDLEs (i.e., >= 0 && < max_handlep1_).
  int handle_in_range (size_t handle);

  // = Accessors.
  size_t max_handlep1 (void) const;
  // Maximum ACE_HANDLE value, plus 1.

  ACE_HANDLE *handles (void) const;
  // Pointer to the beginning of the current array of <ACE_HANDLE>
  // *'s.

  int remove_handler (size_t index,
		      ACE_Reactor_Mask mask = 0);
  // Removes the <ACE_Event_Handler> at <index> from the ReactorEx.
  // Note that the ReactorEx will call the <get_handle> method of <eh>
  // to extract the underlying I/O handle.  If <mask> ==
  // ACE_Event_Handler::DONT_CALL then the <handle_close> method of
  // the <eh> is not invoked.

  void dump (void) const;
  // Dump the state of an object.

private:
  ACE_ReactorEx &reactorEx_;
  // Reference to our <ReactorEx>.
  
  size_t max_size_;
  // Maximum number of handles.

  size_t max_handlep1_;
  // The highest currently active handle, plus 1.  Since this ranges
  // between 0 and <max_size_> it also is a count of the number of
  // active handles.

  ACE_HANDLE *handles_;
  // Array of <ACE_HANDLEs> passed to <WaitForMultipleObjects>.

  ACE_Event_Handler **event_handlers_;
  // Array of <ACE_Event_Handler> pointers that store the event
  // handlers to dispatch when the corresponding <handles_[i]> entry
  // becomes signaled.
};

class ACE_Export ACE_ReactorEx_Token : public ACE_REACTOREX_MUTEX
  // = TITLE
  //     Used as a synchronization mechanism to coordinate concurrent
  //     access to a Reactor object.
{
public:
  ACE_ReactorEx_Token (ACE_ReactorEx &r);

  virtual void sleep_hook (void);
  // Called just before the thread that's trying to do the work goes
  // to sleep.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  ACE_ReactorEx &reactorEx_;
};

#endif /* ACE_MT_SAFE */

#if defined (ACE_WIN32)

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
  //     <handle_*> method is dispatched in the context of the
  //     <ACE_ReactorEx> thread.
{
public:
  int notify (ACE_Event_Handler *eh = 0,
	      ACE_Reactor_Mask mask = ACE_Event_Handler::EXCEPT_MASK,
	      ACE_Time_Value *timeout = 0);
  // Special trick to unblock WaitForMultipleObjects() when updates
  // occur in somewhere other than the main <ACE_ReactorEx> thread.
  // All we do is enqueue <eh> and <mask> onto the <ACE_Message_Queue>
  // and wakeup the ReactorEx by signaling its <ACE_Event> handle.
  // The <ACE_Time_Value> indicates how long to blocking trying to
  // notify the <Reactor>.  If <timeout> == 0, the caller will block
  // until action is possible, else will wait until the relative time
  // specified in *<timeout> elapses).

private:
  virtual int handle_signal (int signum, siginfo_t * = 0, ucontext_t * = 0);
  // Called when the notification event waited on by <ACE_ReactorEx>
  // is signaled.  This dequeues all pending <ACE_Event_Handlers> and
  // dispatches them.

  virtual ACE_HANDLE get_handle (void) const;
  // Returns a handle to the <ACE_Auto_Event>.

  ACE_Auto_Event notify_event_;
  // A handle.

  ACE_Message_Queue<ACE_MT_SYNCH> message_queue_;
  // Message queue that keeps track of pending <ACE_Event_Handlers>.
  // This queue must be thread-safe because it can be called by
  // multiple threads of control.
};

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
public:
  enum 
  {
    DEFAULT_SIZE = MAXIMUM_WAIT_OBJECTS,
    // Default size of the ReactorEx's handle table.
  };

  // = Initialization and termination methods.

  ACE_ReactorEx (ACE_Sig_Handler * = 0,
		 ACE_Timer_Queue * = 0);
  // Initialize <ACE_ReactorEx> with the default size.

  ACE_ReactorEx (size_t size, 
		 int unused = 0,
		 ACE_Sig_Handler * = 0,
		 ACE_Timer_Queue * = 0);
  // Initialize <ACE_ReactorEx> with size <size>.

  virtual int open (size_t size = DEFAULT_SIZE, 
		    int restart = 0, 
		    ACE_Sig_Handler * = 0,
		    ACE_Timer_Queue * = 0);
  // Initialize <ACE_ReactorEx> with size <size>.

  virtual int close (void);
  // Close down the ReactorEx and release all of its resources.

  virtual ~ACE_ReactorEx (void);
  // Close down the ReactorEx and release all of its resources.
  
  // = Event loop drivers.  

  virtual int handle_events (ACE_Time_Value *max_wait_time = 0,
			     int wait_all = 0,
			     ACE_Event_Handler *wait_all_callback = 0,
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
  // If <wait_all> is TRUE, then handle_events will only dispatch the
  // handlers if *all* handles become active.  If a timeout occurs,
  // then no handlers will be dispatched.  If <wait_all_callback> is 0
  // then we dispatch the <handle_signal> method on each and every
  // registered HANDLE.  Otherwise, we just call back the
  // <handle_signal> method of the <wait_all_callback> object, after
  // first assigning the siginfo_t <si_handles_> argument to point to
  // the array of signaled handles.
  // 
  // If <alertable> is true, then <WaitForMultipleObjectsEx> is used
  // as the demultiplexing call, otherwise <WaitForMultipleObjects> is
  // used.
  // 
  // Returns the total number of <ACE_Event_Handler>s that were
  // dispatched, 0 if the <max_wait_time> elapsed without dispatching
  // any handlers, or -1 if an error occurs.

  virtual int handle_events (ACE_Time_Value &max_wait_time,
			     int wait_all = 0,
			     ACE_Event_Handler *wait_all_callback = 0,
			     int alertable = 0);
  // This method is just like the one above, except the
  // <max_wait_time> value is a reference and can therefore never be
  // NULL.

  // = Register and remove Handlers. 

  virtual int register_handler (ACE_Event_Handler *eh, 
				ACE_HANDLE handle = ACE_INVALID_HANDLE);
  // Register an Event_Handler <eh>.  If handle == ACE_INVALID_HANDLE
  // the <ReactorEx> will call the <get_handle> method of <eh> to
  // extract the underlying I/O handle.
 
  virtual int remove_handler (ACE_Event_Handler *eh,
			      ACE_Reactor_Mask mask = 0);
  // Removes <eh> from the ReactorEx.  Note that the ReactorEx will
  // call the <get_handle> method of <eh> to extract the underlying
  // I/O handle.  If <mask> == ACE_Event_Handler::DONT_CALL then the
  // <handle_close> method of the <eh> is not invoked.

  // = Timer management. 

  virtual int schedule_timer (ACE_Event_Handler *eh,
			      const void *arg,
			      const ACE_Time_Value &delta,
			      const ACE_Time_Value &interval = ACE_Time_Value::zero);  
  // Schedule an Event Handler <eh> that will expire after <delta>
  // amount of time.  If it expires then <arg> is passed in as the
  // value to <handle_timeout> method call on <eh>.  If <interval> is
  // != to ACE_Time_Value::zero then it is used to reschedule <eh>
  // automatically.  This method returns a timer handle that uniquely
  // identifies the <eh> in an internal list.  This timer handle can
  // be used to cancel an Event_Handler before it expires.  The
  // cancellation ensures that timer_ids are unique up to values of
  // greater than 2 billion timers.  As long as timers don't stay
  // around longer than this there should be no problems with
  // accidentally deleting the wrong timer.

  virtual int cancel_timer (ACE_Event_Handler *event_handler);
  // Cancel all Event_Handlers that match the address of
  // <event_handler>.

  virtual int cancel_timer (int timer_id, const void **arg = 0);
  // Cancel the single Event_Handler that matches the <timer_id> value
  // (which was returned from the schedule method).  If arg is
  // non-NULL then it will be set to point to the ``magic cookie''
  // argument passed in when the Event_Handler was registered.  This
  // makes it possible to free up the memory and avoid memory leaks.

  // = Notification methods.

  int notify (ACE_Event_Handler * = 0, 
	      ACE_Reactor_Mask = ACE_Event_Handler::EXCEPT_MASK,
	      ACE_Time_Value * = 0);
  // Wakeup <ACE_ReactorEx> if it is currently blocked in
  // <WaitForMultipleObjects>.  The <ACE_Time_Value> indicates how
  // long to blocking trying to notify the <Reactor>.  If <timeout> ==
  // 0, the caller will block until action is possible, else will wait
  // until the relative time specified in *<timeout> elapses).

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:
  virtual int wait_for_multiple_events (ACE_ReactorEx_Handle_Set &wait_set,
					ACE_Time_Value *max_wait_time,
					int wait_all,
					int alertable);
  // Wait for timer and I/O events to occur.

  virtual dispatch (int wait_status,
		    int wait_all,
		    ACE_Event_Handler *wait_all_callback,
		    ACE_ReactorEx_Handle_Set &dispatch_set);
  // Dispatches the timers and I/O handlers.

  int dispatch_handles (size_t index);
  // Dispatches any active handles from handles_[<index>] to
  // handles_[active_handles_] using <WaitForMultipleObjects> to poll
  // through our handle set looking for active handles.

  int dispatch_callbacks (ACE_Event_Handler *wait_all_callback);
  // This is called when the user called handle_events() with the
  // <wait_all> parameter enabled.  In this case, all the handlers are
  // now signaled.  If <wait_all_callback> is NULL then we dispatch
  // the <handle_signal> method on each and every HANDLE in the
  // dispatch array.  Otherwise, we just call back the <handle_signal>
  // method of the <wait_all_callback> object, after first assigning
  // the siginfo_t <si_handles_> argument to point to the array of
  // signaled handles.

  int dispatch_handler (int index);
  // Dispatches a single handler.  Returns 0 on success, -1 if the
  // handler was removed.

private:
  ACE_Timer_Queue *timer_queue_;
  // Defined as a pointer to allow overriding by derived classes...

  int delete_timer_queue_;
  // Keeps track of whether we should delete the timer queue (if we
  // didn't create it, then we don't delete it).

  ACE_ReactorEx_Token token_;
  // Synchronization token for the MT_SAFE ACE_Reactor.

  ACE_ReactorEx_Handler_Repository handler_rep_;
  // Table that maps <ACE_HANDLEs> to <ACE_Event_Handler *>'s.

  ACE_ReactorEx_Notify notify_handler_;
  // Called when notify is called.

  // Deny access since member-wise won't work...
  ACE_ReactorEx (const ACE_ReactorEx &);
  ACE_ReactorEx &operator = (const ACE_ReactorEx &);
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
