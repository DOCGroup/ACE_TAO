/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    WIN_Proactor.h
//
// = AUTHOR
//    Irfan Pyarali (irfan@cs.wustl.edu),
//    Tim Harrison (harrison@cs.wustl.edu) and
//    Alexander Babu Arulanthu <alex@cs.wustl.edu>
//
// ============================================================================

#if !defined (ACE_WIN32_PROACTOR_H)
#define ACE_WIN32_PROACTOR_H

#include "ace/OS.h"
#include "ace/WIN32_Asynch_IO.h"
#include "ace/Thread_Manager.h"
#include "ace/Event_Handler.h"

#if (defined (ACE_WIN32) && !defined (ACE_HAS_WINCE))
// WIN implementation of the Proactor.

#include "ace/Timer_Queue.h"
#include "ace/Timer_List.h"
#include "ace/Timer_Heap.h"
#include "ace/Timer_Wheel.h"

#include "ace/Proactor_Impl.h"

// Forward declarations.
class ACE_WIN32_Asynch_Result;
//class ACE_WIN32_Asynch_Operation;
class ACE_WIN32_Proactor_Timer_Handler;
class ACE_WIN32_Proactor;

class ACE_Export ACE_WIN32_Proactor_Handle_Timeout_Upcall
{
  // = TITLE
  //      Functor for <ACE_Timer_Queue>.
  //
  // = DESCRIPTION
  //      This class implements the functor required by the Timer
  //      Queue to call <handle_timeout> on ACE_Handlers.
public:
  friend class ACE_WIN32_Proactor;
  // Proactor has special privileges, access needed to: proactor ().

  typedef ACE_Timer_Queue_T<ACE_Handler *,
    ACE_WIN32_Proactor_Handle_Timeout_Upcall,
    ACE_SYNCH_RECURSIVE_MUTEX>
  TIMER_QUEUE;

  ACE_WIN32_Proactor_Handle_Timeout_Upcall (void);
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
  int win32_proactor (ACE_WIN32_Proactor &win32_proactor);
  // Set the proactor. This will fail, if one is already set!

  ACE_WIN32_Proactor *win32_proactor_;
  // Handle to the proactor. This is needed for the completion port.
};

class ACE_Export ACE_WIN32_Proactor : public ACE_Proactor_Impl
{
  // = TITLE
  //     A manager for asynchronous event demultiplexing.
  //
  // = DESCRIPTION
  //     See the Proactor pattern description at
  //     http://www.cs.wustl.edu/~schmidt/proactor.ps.gz for more
  //     details.
public:
  friend class ACE_WIN32_Proactor_Timer_Handler;
  // Timer Handler has special privileges because Access needed to:
  // thr_mgr_

  friend class ACE_WIN32_Proactor_Handle_Timeout_Upcall;
  // Access needed to: Asynch_Timer, and completion_port_.

  // = Here are the typedefs that the <ACE_WIN32_Proactor> uses.

  typedef ACE_Timer_Queue_T<ACE_Handler *,
    ACE_WIN32_Proactor_Handle_Timeout_Upcall,
    ACE_SYNCH_RECURSIVE_MUTEX>
  TIMER_QUEUE;
  typedef ACE_Timer_Queue_Iterator_T<ACE_Handler *,
    ACE_WIN32_Proactor_Handle_Timeout_Upcall,
    ACE_SYNCH_RECURSIVE_MUTEX>
  TIMER_QUEUE_ITERATOR;
  typedef ACE_Timer_List_T<ACE_Handler *,
    ACE_WIN32_Proactor_Handle_Timeout_Upcall,
    ACE_SYNCH_RECURSIVE_MUTEX>
  TIMER_LIST;
  typedef ACE_Timer_List_Iterator_T<ACE_Handler *,
    ACE_WIN32_Proactor_Handle_Timeout_Upcall,
    ACE_SYNCH_RECURSIVE_MUTEX>
  TIMER_LIST_ITERATOR;
  typedef ACE_Timer_Heap_T<ACE_Handler *,
    ACE_WIN32_Proactor_Handle_Timeout_Upcall,
    ACE_SYNCH_RECURSIVE_MUTEX>
  TIMER_HEAP;
  typedef ACE_Timer_Heap_Iterator_T<ACE_Handler *,
    ACE_WIN32_Proactor_Handle_Timeout_Upcall,
    ACE_SYNCH_RECURSIVE_MUTEX>
  TIMER_HEAP_ITERATOR;
  typedef ACE_Timer_Wheel_T<ACE_Handler *,
    ACE_WIN32_Proactor_Handle_Timeout_Upcall,
    ACE_SYNCH_RECURSIVE_MUTEX>
  TIMER_WHEEL;
  typedef ACE_Timer_Wheel_Iterator_T<ACE_Handler *,
    ACE_WIN32_Proactor_Handle_Timeout_Upcall,
    ACE_SYNCH_RECURSIVE_MUTEX>
  TIMER_WHEEL_ITERATOR;

  ACE_WIN32_Proactor (size_t number_of_threads = 0,
                      TIMER_QUEUE *tq = 0,
                      int used_with_reactor_event_loop = 0);
  // A do nothing constructor.

  virtual ~ACE_WIN32_Proactor (void);
  // Virtual destruction.

  virtual int close (void);
  // Close the IO completion port.

  virtual int register_handle (ACE_HANDLE handle,
			       const void *completion_key);
  // This method adds the <handle> to the I/O completion port. This
  // function is a no-op function for Unix systems.

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
  // any events occur, return.  Return 0 on success, non-zero (-1) on
  // timeouts/errors and errno is set accordingly.

  virtual int handle_events (void);
  // Block indefinitely until at least one event is dispatched.
  // Return 0 on success, non-zero (-1) on timeouts/errors and errno
  // is set accordingly.

  virtual int post_completion (ACE_WIN32_Asynch_Result *result);
  // Post a result to the completion port of the Proactor.  If errors
  // occur, the result will be deleted by this method.  If successful,
  // the result will be deleted by the Proactor when the result is
  // removed from the completion port.  Therefore, the result should
  // have been dynamically allocated and should be orphaned by the
  // user once this method is called.

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

  virtual ACE_Asynch_Read_Stream_Impl *create_asynch_read_stream (void);

  virtual ACE_Asynch_Write_Stream_Impl *create_asynch_write_stream (void);

  virtual ACE_Asynch_Read_File_Impl *create_asynch_read_file (void);

  virtual ACE_Asynch_Write_File_Impl *create_asynch_write_file (void);

  virtual ACE_Asynch_Accept_Impl *create_asynch_accept (void);

  virtual ACE_Asynch_Transmit_File_Impl *create_asynch_transmit_file (void);

  // Methods used to create Asynch_IO_Result objects. We create the right
  // objects here in these methods.

  virtual ACE_Asynch_Read_Stream_Result_Impl *create_asynch_read_stream_result (ACE_Handler &handler,
                                                                                ACE_HANDLE handle,
                                                                                ACE_Message_Block &message_block,
                                                                                u_long bytes_to_read,
                                                                                const void* act,
                                                                                ACE_HANDLE event,
                                                                                int priority = 0);

  virtual ACE_Asynch_Write_Stream_Result_Impl *create_asynch_write_stream_result (ACE_Handler &handler,
                                                                                  ACE_HANDLE handle,
                                                                                  ACE_Message_Block &message_block,
                                                                                  u_long bytes_to_write,
                                                                                  const void* act,
                                                                                  ACE_HANDLE event,
                                                                                  int priority = 0);

  virtual ACE_Asynch_Read_File_Result_Impl *create_asynch_read_file_result (ACE_Handler &handler,
                                                                            ACE_HANDLE handle,
                                                                            ACE_Message_Block &message_block,
                                                                            u_long bytes_to_read,
                                                                            const void* act,
                                                                            u_long offset,
                                                                            u_long offset_high,
                                                                            ACE_HANDLE event,
                                                                            int priority = 0);

  virtual ACE_Asynch_Write_File_Result_Impl *create_asynch_write_file_result (ACE_Handler &handler,
                                                                              ACE_HANDLE handle,
                                                                              ACE_Message_Block &message_block,
                                                                              u_long bytes_to_write,
                                                                              const void* act,
                                                                              u_long offset,
                                                                              u_long offset_high,
                                                                              ACE_HANDLE event,
                                                                              int priority = 0);

  virtual ACE_Asynch_Accept_Result_Impl *create_asynch_accept_result (ACE_Handler &handler,
                                                                      ACE_HANDLE listen_handle,
                                                                      ACE_HANDLE accept_handle,
                                                                      ACE_Message_Block &message_block,
                                                                      u_long bytes_to_read,
                                                                      const void* act,
                                                                      ACE_HANDLE event,
                                                                      int priority = 0);

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
                                                                                    int priority = 0);

protected:
  virtual int handle_signal (int signum, siginfo_t * = 0, ucontext_t * = 0);
  // Called when object is signaled by OS (either via UNIX signals or
  // when a Win32 object becomes signaled).

  virtual int handle_close (ACE_HANDLE handle,
			    ACE_Reactor_Mask close_mask);
  // Called when object is removed from the ACE_Reactor.

  virtual int handle_events (unsigned long milli_seconds);
  // Dispatch a single set of events.  If <milli_seconds> elapses
  // before any events occur, return.

  void application_specific_code (ACE_WIN32_Asynch_Result *asynch_result,
				  u_long bytes_transferred,
				  int success,
				  const void *completion_key,
				  u_long error);
  // Protect against structured exceptions caused by user code when
  // dispatching handles.


  ACE_HANDLE completion_port_;
  // Handle for the completion port. Unix doesnt have completion
  // ports.

  size_t number_of_threads_;
  // This number is passed to the <CreatIOCompletionPort> system
  // call.

  TIMER_QUEUE *timer_queue_;
  // Timer Queue.

  int delete_timer_queue_;
  // Flag on whether to delete the timer queue.

  ACE_WIN32_Proactor_Timer_Handler *timer_handler_;
  // Handles timeouts events.

  ACE_Thread_Manager thr_mgr_;
  // This will manage the thread in the Timer_Handler.

  ACE_Auto_Event event_;
  // This event is used in conjunction with Reactor when we try to
  // integrate the event loops of Reactor and the Proactor.

  int used_with_reactor_event_loop_;
  // Flag that indicates whether we are used in conjunction with
  // Reactor.
};

class ACE_Export ACE_WIN32_Asynch_Timer : protected ACE_WIN32_Asynch_Result
{
  // = TITLE
  //     This class is posted to the completion port when a timer
  //     expires. When the complete method of this object is
  //     called, the <handler>'s handle_timeout method will be
  //     called.
public:
  friend class ACE_WIN32_Proactor_Handle_Timeout_Upcall;
  // Timer Handler has special privileges
  // Access needed to: convert Asynch_Timer into an OVERLAPPED

  ACE_WIN32_Asynch_Timer (ACE_Handler &handler,
                          const void *act,
                          const ACE_Time_Value &tv,
                          ACE_HANDLE event = ACE_INVALID_HANDLE);

protected:
  virtual void complete (u_long bytes_transferred,
                         int success,
                         const void *completion_key,
                         u_long error = 0);
  // This method calls the <handler>'s handle_timeout method

  ACE_Time_Value time_;
  // Time value requested by caller
};

#if defined (__ACE_INLINE__)
#include "ace/WIN32_Proactor.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_WIN32 */
#endif /* ACE_PROACTOR_H */
