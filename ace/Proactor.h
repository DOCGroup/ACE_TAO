/* -*- C++ -*- */
// $Id: Proactor.h,v

// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    Proactor.h
//
// = AUTHOR
//    Irfan Pyarali (irfan@cs.wustl.edu)
//    Tim Harrison (harrison@cs.wustl.edu)
// 
// ============================================================================

#if !defined (ACE_PROACTOR_H)
#define ACE_PROACTOR_H

#include "ace/Asynch_IO.h"
#include "ace/Thread_Manager.h"
#include "ace/Event_Handler.h"

#include "ace/Timer_Queue.h"
#include "ace/Timer_List.h"
#include "ace/Timer_Heap.h"
#include "ace/Timer_Wheel.h"

#if defined (ACE_WIN32)
// This only works on Win32 platforms

// Forward declarations.
class ACE_Asynch_Result;
class ACE_Proactor_Timer_Handler;
class ACE_Proactor;

class ACE_Export ACE_Proactor_Handle_Timeout_Upcall
  // = TITLE 
  //      Functor for Timer_Queues.
  //
  // = DESCRIPTION
  //
  //      This class implements the functor required by the Timer
  //      Queue to call <handle_timeout> on ACE_Handlers.
{
  friend class ACE_Proactor;
  // Proactor has special privileges
  // Access needed to: proactor ()

public:
  typedef ACE_Timer_Queue_T<ACE_Handler *, 
                            ACE_Proactor_Handle_Timeout_Upcall, 
                            ACE_SYNCH_RECURSIVE_MUTEX> TIMER_QUEUE;
    
  ACE_Proactor_Handle_Timeout_Upcall (void);
  // Constructor

  int timeout (TIMER_QUEUE &timer_queue,
	       ACE_Handler *handler,
	       const void *arg,
	       const ACE_Time_Value &cur_time);
  // This method is called when the timer expires
    
  int cancellation (TIMER_QUEUE &timer_queue,
		    ACE_Handler *handler);
  // This method is called when the timer is canceled

  int deletion (TIMER_QUEUE &timer_queue,
                ACE_Handler *handler,
                const void *arg);
  // This method is called when the timer queue is destroyed and
  // the timer is still contained in it

protected:
  int proactor (ACE_Proactor &proactor);
  // Set the proactor. This will fail, if one is already set!

  ACE_Proactor *proactor_;
  // Handle to the proactor. This is needed for the completion port.
};

class ACE_Export ACE_Proactor : public ACE_Event_Handler
  //     
  // = TITLE
  //
  //     A Proactor for asynchronous I/O events.
  // 
  // = DESCRIPTION
  //     
  //     A manager for the I/O completion port.
{
  friend class ACE_Proactor_Timer_Handler;
  // Timer Handler has special privileges because
  // Access needed to: thr_mgr_

  friend class ACE_Proactor_Handle_Timeout_Upcall;  
  // Access needed to: Asynch_Timer, and completion_port_

public:

  // Here are the typedef for Timer_Queue, Timer_List, and Timer_Heap
  // for the Proactor (add to the ease of use of these template
  // classes).

  typedef ACE_Timer_Queue_T<ACE_Handler *, 
                            ACE_Proactor_Handle_Timeout_Upcall, 
                            ACE_SYNCH_RECURSIVE_MUTEX> Timer_Queue;
  typedef ACE_Timer_Queue_Iterator_T<ACE_Handler *, 
                                     ACE_Proactor_Handle_Timeout_Upcall, 
                                     ACE_SYNCH_RECURSIVE_MUTEX> Timer_Queue_Iterator;

  typedef ACE_Timer_List_T<ACE_Handler *, 
                           ACE_Proactor_Handle_Timeout_Upcall, 
                           ACE_SYNCH_RECURSIVE_MUTEX> Timer_List;
  typedef ACE_Timer_List_Iterator_T<ACE_Handler *, 
                                    ACE_Proactor_Handle_Timeout_Upcall, 
                                    ACE_SYNCH_RECURSIVE_MUTEX> Timer_List_Iterator;

  typedef ACE_Timer_Heap_T<ACE_Handler *, 
                            ACE_Proactor_Handle_Timeout_Upcall, 
                            ACE_SYNCH_RECURSIVE_MUTEX> Timer_Heap;
  typedef ACE_Timer_Heap_Iterator_T<ACE_Handler *, 
                                    ACE_Proactor_Handle_Timeout_Upcall, 
                                    ACE_SYNCH_RECURSIVE_MUTEX> Timer_Heap_Iterator;

  typedef ACE_Timer_Wheel_T<ACE_Handler *, 
                            ACE_Proactor_Handle_Timeout_Upcall, 
                            ACE_SYNCH_RECURSIVE_MUTEX> Timer_Wheel;
  typedef ACE_Timer_Wheel_Iterator_T<ACE_Handler *, 
                                     ACE_Proactor_Handle_Timeout_Upcall, 
                                     ACE_SYNCH_RECURSIVE_MUTEX> Timer_Wheel_Iterator;

  ACE_Proactor (size_t number_of_threads = 0, 
		Timer_Queue *tq = 0,
		int used_with_reactorEx_event_loop = 0);
  // A do nothing constructor.
  
  virtual ~ACE_Proactor (void);
  // Virtual destruction.
  
  static ACE_Proactor *instance (size_t threads = 0);
  // Get pointer to a process-wide <ACE_Proactor>.  <threads> should
  // be part of another method.  It's only here because I'm just a
  // grad student and not in charge.  No, I'm not bitter about this.

  static ACE_Proactor *instance (ACE_Proactor *);
  // Set pointer to a process-wide <ACE_Proactor> and return existing
  // pointer.

  static void close_singleton (void);
  // Delete the dynamically allocated Singleton

  // = Proactor event loop management methods.
  static int run_event_loop (void);
  // Run the event loop until the <ACE_Proactor::handle_events>
  // method returns -1 or the <end_event_loop> method
  // is invoked.

  static int run_event_loop (ACE_Time_Value &tv);
  // Run the event loop until the <ACE_Proactor::handle_events>
  // method returns -1, the <end_event_loop> method
  // is invoked, or the <ACE_Time_Value> expires.

  static int end_event_loop (void);
  // Instruct the <ACE_Proactor::instance> to terminate its event loop.

  static sig_atomic_t event_loop_done (void);
  // Report if the <ACE_Proactor::instance> event loop is finished.

  virtual int close (void);
  // Close the IO completion port
  
  virtual int register_handle (ACE_HANDLE handle, 
			       const void *completion_key);
  // This method adds the <handle> to the I/O completion port
  
  // = Timer management. 
  virtual long schedule_timer (ACE_Handler &handler, 
			       const void *act,
			       const ACE_Time_Value &time);
  // Schedule a <handler> that will expire after <time>.  If it
  // expires then <act> is passed in as the value to the <handler>'s
  // <handle_timeout> callback method.  This method returns a
  // <timer_id>. This <timer_id> can be used to cancel a timer before
  // it expires.  The cancellation ensures that <timer_ids> are unique
  // up to values of greater than 2 billion timers.  As long as timers
  // don't stay around longer than this there should be no problems
  // with accidentally deleting the wrong timer.  Returns -1 on
  // failure (which is guaranteed never to be a valid <timer_id>.

  virtual long schedule_repeating_timer (ACE_Handler &handler, 
					 const void *act,
					 const ACE_Time_Value &interval);  
  
  // Same as above except <interval> it is used to reschedule the
  // <handler> automatically.
  
  virtual long schedule_timer (ACE_Handler &handler, 
			       const void *act,
			       const ACE_Time_Value &time,
			       const ACE_Time_Value &interval);
  // This combines the above two methods into one. Mostly for backward
  // compatibility.

  virtual int cancel_timer (ACE_Handler &handler,
			    int dont_call_handle_close = 1);
  // Cancel all timers associated with this <handler>.  Returns number
  // of timers cancelled.

  virtual int cancel_timer (long timer_id, 
			    const void **act = 0,
			    int dont_call_handle_close = 1);
  // Cancel the single <ACE_Handler> that matches the <timer_id> value
  // (which was returned from the <schedule> method).  If <act> is
  // non-NULL then it will be set to point to the ``magic cookie''
  // argument passed in when the <Handler> was registered.  This makes
  // it possible to free up the memory and avoid memory leaks.
  // Returns 1 if cancellation succeeded and 0 if the <timer_id>
  // wasn't found.

  virtual int handle_events (ACE_Time_Value &wait_time);
  // Dispatch a single set of events.  If <wait_time> elapses before
  // any events occur, return.

  virtual int handle_events (void);
  // Block indefinitely until at least one event is dispatched.

  int wake_up_dispatch_threads (void);
  // Add wakeup dispatch threads (reinit).

  int close_dispatch_threads (int wait);
  // Close all dispatch threads.

  size_t number_of_threads (void) const;
  void number_of_threads (size_t threads);
  // Number of thread used as a parameter to CreatIoCompletionPort

  Timer_Queue *timer_queue (void) const;
  void timer_queue (Timer_Queue *);
  // Get/Set timer queue

  virtual ACE_HANDLE get_handle (void) const;
  // Get the event handle.
  
protected:
  
  virtual int handle_signal (int signum, siginfo_t * = 0, ucontext_t * = 0);
  // Called when object is signaled by OS (either via UNIX signals or
  // when a Win32 object becomes signaled).

  virtual int handle_close (ACE_HANDLE handle,
			    ACE_Reactor_Mask close_mask);
  // Called when object is removed from the ACE_Reactor

  void application_specific_code (ACE_Asynch_Result *asynch_result,
				  u_long bytes_transferred,
				  int success,
				  const void *completion_key,
				  u_long error);
  // Protect against structured exceptions caused by user code when
  // dispatching handles
  
  virtual int handle_events (unsigned long milli_seconds);
  // Dispatch a single set of events.  If <milli_seconds> elapses
  // before any events occur, return.
  
  class ACE_Export Asynch_Timer : protected ACE_Asynch_Result
    //     
    // = TITLE
    //
    //     This class is posted to the completion port when a timer
    //     expires. When the complete method of this object is called,
    //     the <handler>'s handle_timeout method will be called.
    // 
    {
      friend class ACE_Proactor_Handle_Timeout_Upcall;
      // Timer Handler has special privileges
      // Access needed to: convert Asynch_Timer into an OVERLAPPED

    public:
      Asynch_Timer (ACE_Handler &handler,
		    const void *act,
		    const ACE_Time_Value &tv, 
		    ACE_HANDLE event);
      
    protected:
      virtual void complete (u_long bytes_transferred,
			     int success,
			     const void *completion_key,
			     u_long error = 0);  
      // This method calls the <handler>'s handle_timeout method 
      
      ACE_Time_Value time_;
      // Time value requested by caller
    };
  
  ACE_HANDLE completion_port_;
  // Handle for the completion port
  
  size_t number_of_threads_;
  // This number is passed to the CreatIOCompletionPort() system call

  Timer_Queue *timer_queue_;
  // Timer Queue 

  int delete_timer_queue_;
  // Flag on whether to delete the timer queue

  ACE_Proactor_Timer_Handler *timer_handler_;
  // Handles timeouts events

  ACE_Thread_Manager thr_mgr_;
  // This will manage the thread in the Timer_Handler

  ACE_Auto_Event event_;
  // This event is used in conjunction with ReactorEx when we try to
  // integrate the event loops of ReactorEx and the Proactor.

  int used_with_reactorEx_event_loop_;
  // Flag that indicates whether we are used in conjunction with
  // ReactorEx

private:
  static ACE_Proactor *proactor_;
  // Pointer to a process-wide <ACE_Proactor>.

  static int delete_proactor_;
  // Must delete the <proactor_> if non-0.

  static sig_atomic_t end_event_loop_;
  // Terminate the proactor event loop.
};

#if defined (__ACE_INLINE__)
#include "ace/Proactor.i"
#endif /* __ACE_INLINE__ */

#else /* NOT WIN32 */

class ACE_Export ACE_Proactor
{
public:
  class Timer_Queue {};
  ACE_Proactor (size_t /* number_of_threads */ = 0,
		Timer_Queue * /* tq */ = 0) {}
  virtual int handle_events (void) { return -1; }
  virtual int handle_events (ACE_Time_Value &) { return -1; }

  static ACE_Proactor *instance (size_t threads = 0);
  // Placeholder to enable compilation on non-Win32 platforms

  static ACE_Proactor *instance (ACE_Proactor *);
  // Placeholder to enable compilation on non-Win32 platforms

  static void close_singleton (void);
  // Placeholder to enable compilation on non-Win32 platforms

  static int run_event_loop (void);
  // Placeholder to enable compilation on non-Win32 platforms

  static int run_event_loop (ACE_Time_Value &tv);
  // Placeholder to enable compilation on non-Win32 platforms

  static int end_event_loop (void);
  // Placeholder to enable compilation on non-Win32 platforms

  static sig_atomic_t event_loop_done (void);
  // Placeholder to enable compilation on non-Win32 platforms
};

#endif /* ACE_WIN32 */
#endif /* ACE_PROACTOR_H */

