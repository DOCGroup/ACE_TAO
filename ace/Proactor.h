/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    Proactor.h
//
// = AUTHOR
//    Irfan Pyarali <irfan@cs.wustl.edu>,
//    Tim Harrison <harrison@cs.wustl.edu> and
//    Alexander Babu Arulanthu <alex@cs.wustl.edu>
//
// ============================================================================

#if !defined (ACE_PROACTOR_H)
#define ACE_PROACTOR_H

#include "ace/OS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if ((defined (ACE_WIN32) && !defined (ACE_HAS_WINCE)) || (defined (ACE_HAS_AIO_CALLS)))
// This only works on Win32 platforms and on Unix platforms supporting
// POSIX aio calls.

#include "ace/Asynch_IO.h"
#include "ace/Asynch_IO_Impl.h"
#include "ace/Thread_Manager.h"
#include "ace/Timer_Queue.h"
#include "ace/Timer_List.h"
#include "ace/Timer_Heap.h"
#include "ace/Timer_Wheel.h"

// Forward declarations.
class ACE_Proactor_Impl;
class ACE_Proactor_Timer_Handler;

class ACE_Export ACE_Proactor_Handle_Timeout_Upcall
{
  // = TITLE
  //      Functor for <ACE_Timer_Queue>.
  //
  // = DESCRIPTION
  //      This class implements the functor required by the Timer
  //      Queue to call <handle_timeout> on ACE_Handlers.
  
  typedef ACE_Timer_Queue_T<ACE_Handler *,
                            ACE_Proactor_Handle_Timeout_Upcall,
                            ACE_SYNCH_RECURSIVE_MUTEX>
  TIMER_QUEUE;
  // Type def for the timer queue.
  
  friend class ACE_Proactor;
  // The main Proactor class has special permissions.

public:
  ACE_Proactor_Handle_Timeout_Upcall (void);
  // Constructor.
  
  int timeout (TIMER_QUEUE &timer_queue,
	       ACE_Handler *handler,
	       const void *arg,
	       const ACE_Time_Value &cur_time);
  // This method is called when the timer expires.

  int cancellation (TIMER_QUEUE &timer_queue,
		    ACE_Handler *handler);
  // This method is called when the timer is canceled.

  int deletion (TIMER_QUEUE &timer_queue,
                ACE_Handler *handler,
                const void *arg);
  // This method is called when the timer queue is destroyed and the
  // timer is still contained in it.

protected:
  int proactor (ACE_Proactor &proactor);
  // Set the proactor. This will fail, if one is already set!

  ACE_Proactor *proactor_;
  // Handle to the proactor. This is needed for posting a timer result
  // to the Proactor's completion queue.
};

class ACE_Export ACE_Proactor
{
  // = TITLE
  //     A manager for asynchronous event demultiplexing.
  //
  // = DESCRIPTION
  //     See the Proactor pattern description at
  //     http://www.cs.wustl.edu/~schmidt/proactor.ps.gz for more
  //     details.

  // = Here are the private typedefs that the <ACE_Proactor> uses.

  typedef ACE_Timer_Queue_Iterator_T<ACE_Handler *,
    ACE_Proactor_Handle_Timeout_Upcall,
    ACE_SYNCH_RECURSIVE_MUTEX>
  TIMER_QUEUE_ITERATOR;
  typedef ACE_Timer_List_T<ACE_Handler *,
    ACE_Proactor_Handle_Timeout_Upcall,
    ACE_SYNCH_RECURSIVE_MUTEX>
  TIMER_LIST;
  typedef ACE_Timer_List_Iterator_T<ACE_Handler *,
    ACE_Proactor_Handle_Timeout_Upcall,
    ACE_SYNCH_RECURSIVE_MUTEX>
  TIMER_LIST_ITERATOR;
  typedef ACE_Timer_Heap_T<ACE_Handler *,
    ACE_Proactor_Handle_Timeout_Upcall,
    ACE_SYNCH_RECURSIVE_MUTEX>
  TIMER_HEAP;
  typedef ACE_Timer_Heap_Iterator_T<ACE_Handler *,
    ACE_Proactor_Handle_Timeout_Upcall,
    ACE_SYNCH_RECURSIVE_MUTEX>
  TIMER_HEAP_ITERATOR;
  typedef ACE_Timer_Wheel_T<ACE_Handler *,
    ACE_Proactor_Handle_Timeout_Upcall,
    ACE_SYNCH_RECURSIVE_MUTEX>
  TIMER_WHEEL;
  typedef ACE_Timer_Wheel_Iterator_T<ACE_Handler *,
    ACE_Proactor_Handle_Timeout_Upcall,
    ACE_SYNCH_RECURSIVE_MUTEX>
  TIMER_WHEEL_ITERATOR;
  
  // = Friendship.
  
  friend class ACE_Proactor_Timer_Handler;
  // Timer handler runs a thread and manages the timers, on behalf of
  // the Proactor.

public:
  typedef ACE_Timer_Queue_T<ACE_Handler *,
    ACE_Proactor_Handle_Timeout_Upcall,
    ACE_SYNCH_RECURSIVE_MUTEX>
  TIMER_QUEUE;
  // Public type.

  ACE_Proactor (ACE_Proactor_Impl *implementation = 0,
                TIMER_QUEUE *tq = 0,
                int delete_implementation = 0);
  // A do nothing constructor.

  virtual ~ACE_Proactor (void);
  // Virtual destruction.

  static ACE_Proactor *instance (size_t threads = 0);
  // Get pointer to a process-wide <ACE_Proactor>.  <threads> should
  // be part of another method.

  static ACE_Proactor *instance (ACE_Proactor *);
  // Set pointer to a process-wide <ACE_Proactor> and return existing
  // pointer.

  static void close_singleton (void);
  // Delete the dynamically allocated Singleton.

  // = Proactor event loop management methods.

  static int run_event_loop (void);
  // Run the event loop until the <ACE_Proactor::handle_events> method
  // returns -1 or the <end_event_loop> method is invoked.

  static int run_event_loop (ACE_Time_Value &tv);
  // Run the event loop until the <ACE_Proactor::handle_events> method
  // returns -1, the <end_event_loop> method is invoked, or the
  // <ACE_Time_Value> expires.

  static int end_event_loop (void);
  // Instruct the <ACE_Proactor::instance> to terminate its event
  // loop.

  static int event_loop_done (void);
  // Report if the <ACE_Proactor::instance> event loop is finished.

  virtual int close (void);
  // Close the IO completion port.

  virtual int register_handle (ACE_HANDLE handle,
			       const void *completion_key);
  // This method adds the <handle> to the I/O completion port. This
  // function is a no-op function for Unix systems and returns 0;

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
  // failure (which is guaranteed never to be a valid <timer_id>).

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
  // any events occur, return 0.  Return 1 on success i.e., when a
  // completion is dispatched, non-zero (-1) on errors and errno is
  // set accordingly. 

  virtual int handle_events (void);
  // Block indefinitely until at least one event is dispatched.
  // Dispatch a single set of events.  If <wait_time> elapses before
  // any events occur, return 0.  Return 1 on success i.e., when a
  // completion is dispatched, non-zero (-1) on errors and errno is
  // set accordingly. 

  int wake_up_dispatch_threads (void);
  // Add wakeup dispatch threads (reinit).

  int close_dispatch_threads (int wait);
  // Close all dispatch threads.

  size_t number_of_threads (void) const;
  void number_of_threads (size_t threads);
  // Number of thread used as a parameter to CreatIoCompletionPort.

  TIMER_QUEUE *timer_queue (void) const;
  void timer_queue (TIMER_QUEUE *timer_queue);
  // Get/Set timer queue.
  
  virtual ACE_HANDLE get_handle (void) const;
  // Get the event handle.
  // It is a no-op in POSIX platforms and it returns
  // ACE_INVALID_HANDLE.

  virtual ACE_Proactor_Impl *implementation (void) const;
  // Get the implementation class.

  //
  // = Factory methods for the operations
  //
  // Note that the user does not have to use or know about these
  // methods.

  virtual ACE_Asynch_Read_Stream_Impl *create_asynch_read_stream (void);
  // Create the correct implementation class for doing Asynch_Read_Stream.

  virtual ACE_Asynch_Write_Stream_Impl *create_asynch_write_stream (void);
  // Create the correct implementation class for doing Asynch_Write_Stream.

  virtual ACE_Asynch_Read_File_Impl *create_asynch_read_file (void);
  // Create the correct implementation class for doing Asynch_Read_File.

  virtual ACE_Asynch_Write_File_Impl *create_asynch_write_file (void);
  // Create the correct implementation class for doing Asynch_Write_File.

  virtual ACE_Asynch_Accept_Impl *create_asynch_accept (void);
  // Create the correct implementation class for doing Asynch_Accept.

  virtual ACE_Asynch_Transmit_File_Impl *create_asynch_transmit_file (void);
  // Create the correct implementation class for doing Asynch_Transmit_File.

  //
  // = Factory methods for the results
  //
  // Note that the user does not have to use or know about these
  // methods unless they want to "fake" results.

  virtual ACE_Asynch_Read_Stream_Result_Impl *create_asynch_read_stream_result (ACE_Handler &handler,
                                                                                ACE_HANDLE handle,
                                                                                ACE_Message_Block &message_block,
                                                                                u_long bytes_to_read,
                                                                                const void* act,
                                                                                ACE_HANDLE event,
                                                                                int priority);
  // Create the correct implementation class for ACE_Asynch_Read_Stream::Result class.

  virtual ACE_Asynch_Write_Stream_Result_Impl *create_asynch_write_stream_result (ACE_Handler &handler,
                                                                                  ACE_HANDLE handle,
                                                                                  ACE_Message_Block &message_block,
                                                                                  u_long bytes_to_write,
                                                                                  const void* act,
                                                                                  ACE_HANDLE event,
                                                                                  int priority);
  // Create the correct implementation class for ACE_Asynch_Write_Stream::Result.

  virtual ACE_Asynch_Read_File_Result_Impl *create_asynch_read_file_result (ACE_Handler &handler,
                                                                            ACE_HANDLE handle,
                                                                            ACE_Message_Block &message_block,
                                                                            u_long bytes_to_read,
                                                                            const void* act,
                                                                            u_long offset,
                                                                            u_long offset_high,
                                                                            ACE_HANDLE event,
                                                                            int priority);
  // Create the correct implementation class for ACE_Asynch_Read_File::Result.

  virtual ACE_Asynch_Write_File_Result_Impl *create_asynch_write_file_result (ACE_Handler &handler,
                                                                              ACE_HANDLE handle,
                                                                              ACE_Message_Block &message_block,
                                                                              u_long bytes_to_write,
                                                                              const void* act,
                                                                              u_long offset,
                                                                              u_long offset_high,
                                                                              ACE_HANDLE event,
                                                                              int priority);
  // Create the correct implementation class for ACE_Asynch_Write_File::Result.

  virtual ACE_Asynch_Accept_Result_Impl *create_asynch_accept_result (ACE_Handler &handler,
                                                                      ACE_HANDLE listen_handle,
                                                                      ACE_HANDLE accept_handle,
                                                                      ACE_Message_Block &message_block,
                                                                      u_long bytes_to_read,
                                                                      const void* act,
                                                                      ACE_HANDLE event,
                                                                      int priority);
  // Create the correct implementation class for ACE_Asynch_Accept::Result.

  virtual ACE_Asynch_Transmit_File_Result_Impl *create_asynch_transmit_file_result (ACE_Handler &handler,
                                                                                    ACE_HANDLE socket,
                                                                                    ACE_HANDLE file,
                                                                                    ACE_Asynch_Transmit_File::Header_And_Trailer *header_and_trailer,
                                                                                    u_long bytes_to_write,
                                                                                    u_long offset,
                                                                                    u_long offset_high,
                                                                                    u_long bytes_per_send,
                                                                                    u_long flags,
                                                                                    const void *act,
                                                                                    ACE_HANDLE event,
                                                                                    int priority);
  // Create the correct implementation class for ACE_Asynch_Transmit_File::Result. 

  virtual ACE_Asynch_Result_Impl *create_asynch_timer (ACE_Handler &handler,
                                                       const void *act,
                                                       const ACE_Time_Value &tv,
                                                       ACE_HANDLE event,
                                                       int priority = 0);
  // Create a timer result object which can be used with the Timer
  // mechanism of the Proactor.

protected:
  virtual void implementation (ACE_Proactor_Impl *implementation);
  // Set the implementation class.

  ACE_Proactor_Impl *implementation_;
  // Delegation/implementation class that all methods will be
  // forwarded to.

  int delete_implementation_;
  // Flag used to indicate whether we are responsible for cleaning up
  // the implementation instance.

  static ACE_Proactor *proactor_;
  // Pointer to a process-wide <ACE_Proactor>.

  static int delete_proactor_;
  // Must delete the <proactor_> if non-0.
  
  ACE_Proactor_Timer_Handler *timer_handler_;
  // Handles timeout events. 
  
  ACE_Thread_Manager thr_mgr_;
  // This will manage the thread in the Timer_Handler.

  TIMER_QUEUE *timer_queue_;
  // Timer Queue.

  int delete_timer_queue_;
  // Flag on whether to delete the timer queue.

  static sig_atomic_t end_event_loop_;
  // Terminate the proactor event loop.

private:
  ACE_Proactor (const ACE_Proactor &);
  ACE_Proactor &operator= (const ACE_Proactor &);
  // Deny access since member-wise won't work...
};

#if defined (__ACE_INLINE__)
#include "ace/Proactor.i"
#endif /* __ACE_INLINE__ */

#else /* NOT WIN32 or POSIX with AIO features. */
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
#endif /* ACE_WIN32 && !ACE_HAS_WINCE || ACE_HAS_AIO_CALLS*/
#endif /* ACE_PROACTOR_H */
