/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    POSIX_Proactor.h
//
// = AUTHOR
//    Irfan Pyarali <irfan@cs.wustl.edu>,
//    Tim Harrison <harrison@cs.wustl.edu> and
//    Alexander Babu Arulanthu <alex@cs.wustl.edu>
//
// ============================================================================

#if !defined (ACE_POSIX_PROACTOR_H)
#define ACE_POSIX_PROACTOR_H

#include "ace/Proactor_Impl.h"

#if defined (ACE_HAS_AIO_CALLS)
// POSIX implementation of Proactor depends on the <aio_> family of
// system calls.

#include "ace/Free_List.h"
#include "ace/Pipe.h"
#include "ace/POSIX_Asynch_IO.h"

class ACE_Export ACE_POSIX_Proactor : public ACE_Proactor_Impl
{
  // = TITLE
  //
  //     POSIX implementation of the Proactor.
  //
  // = DESCRIPTION
  //
  //     There are two different strategies by which Proactor can get
  //     to know the completion of <aio> operations. One is based on
  //     Asynchronous I/O Control Blocks (AIOCB) where a list of
  //     AIOCBs are stored and completion status of the corresponding
  //     operations are queried on them. The other one is based on
  //     POSIX Real Time signals. This class abstracts out the common
  //     code needed for both the strategies. ACE_AIOCB_Proactor and
  //     ACE_SIG_Proactor specialize this.

public:
  virtual ~ACE_POSIX_Proactor (void);
  // Virtual destructor.

  virtual int close (void);
  // Close down the Proactor.

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
			    int dont_call_handle_close);
  // Cancel all timers associated with this <handler>.  Returns number
  // of timers cancelled.

  virtual int cancel_timer (long timer_id,
			    const void **act,
			    int dont_call_handle_close);
  // Cancel the single <ACE_Handler> that matches the <timer_id> value
  // (which was returned from the <schedule> method).  If <act> is
  // non-NULL then it will be set to point to the ``magic cookie''
  // argument passed in when the <Handler> was registered.  This makes
  // it possible to free up the memory and avoid memory leaks.
  // Returns 1 if cancellation succeeded and 0 if the <timer_id>
  // wasn't found.

  virtual int post_completion (ACE_POSIX_Asynch_Result *result) = 0;
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

#if 0
  Timer_Queue *timer_queue (void) const;
  void timer_queue (Timer_Queue *);
  // Get/Set timer queue.
#endif /* 0 */

  virtual ACE_HANDLE get_handle (void) const;
  // Get the event handle.

  // Methods used to create Asynch_IO_Result objects. We create the right
  // objects here in these methods.

  virtual ACE_Asynch_Read_Stream_Result_Impl *create_asynch_read_stream_result (ACE_Handler &handler,
                                                                                ACE_HANDLE handle,
                                                                                ACE_Message_Block &message_block,
                                                                                u_long bytes_to_read,
                                                                                const void* act,
                                                                                ACE_HANDLE event,
                                                                                int priority);

  virtual ACE_Asynch_Write_Stream_Result_Impl *create_asynch_write_stream_result (ACE_Handler &handler,
                                                                                  ACE_HANDLE handle,
                                                                                  ACE_Message_Block &message_block,
                                                                                  u_long bytes_to_write,
                                                                                  const void* act,
                                                                                  ACE_HANDLE event,
                                                                                  int priority);

  virtual ACE_Asynch_Read_File_Result_Impl *create_asynch_read_file_result (ACE_Handler &handler,
                                                                            ACE_HANDLE handle,
                                                                            ACE_Message_Block &message_block,
                                                                            u_long bytes_to_read,
                                                                            const void* act,
                                                                            u_long offset,
                                                                            u_long offset_high,
                                                                            ACE_HANDLE event,
                                                                            int priority);

  virtual ACE_Asynch_Write_File_Result_Impl *create_asynch_write_file_result (ACE_Handler &handler,
                                                                              ACE_HANDLE handle,
                                                                              ACE_Message_Block &message_block,
                                                                              u_long bytes_to_write,
                                                                              const void* act,
                                                                              u_long offset,
                                                                              u_long offset_high,
                                                                              ACE_HANDLE event,
                                                                              int priority);

  virtual ACE_Asynch_Accept_Result_Impl *create_asynch_accept_result (ACE_Handler &handler,
                                                                      ACE_HANDLE listen_handle,
                                                                      ACE_HANDLE accept_handle,
                                                                      ACE_Message_Block &message_block,
                                                                      u_long bytes_to_read,
                                                                      const void* act,
                                                                      ACE_HANDLE event,
                                                                      int priority);

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
protected:
  ACE_POSIX_Proactor (void);
  // Constructor.

#if 0
  virtual int handle_signal (int signum, siginfo_t * = 0, ucontext_t * = 0);
  // Called when object is signaled by OS (either via UNIX signals or
  // when a Win32 object becomes signaled).

  virtual int handle_close (ACE_HANDLE handle,
			    ACE_Reactor_Mask close_mask);
  // Called when object is removed from the ACE_Reactor.
#endif /* 0 */

  void application_specific_code (ACE_POSIX_Asynch_Result *asynch_result,
				  u_long bytes_transferred,
				  int success,
				  const void *completion_key,
				  u_long error);
  // Protect against structured exceptions caused by user code when
  // dispatching handles.

#if 0
  Timer_Queue *timer_queue_;
  // Timer Queue.

  int delete_timer_queue_;
  // Flag on whether to delete the timer queue.

  ACE_Proactor_Timer_Handler *timer_handler_;
  // Handles timeouts events.

  ACE_Thread_Manager thr_mgr_;
  // This will manage the thread in the Timer_Handler.

  ACE_Auto_Event event_;
  // This event is used in conjunction with Reactor when we try to
  // integrate the event loops of Reactor and the Proactor.

  int used_with_reactor_event_loop_;
  // Flag that indicates whether we are used in conjunction with
  // Reactor.
#endif /* 0 */
};

// Forward declarations.
class ACE_AIO_Accept_Handler;

class ACE_Export ACE_POSIX_AIOCB_Proactor : public ACE_POSIX_Proactor
{
  // = TITLE
  //
  //     This Proactor makes use of Asynchronous I/O Control Blocks
  //     (AIOCB) to get the completion status of the <aio_> operations
  //     issued.
  //
  // = DESCRIPTION
  //

  friend class ACE_AIO_Accept_Handler;
  // Handler needs to call application specific code.

  friend class ACE_POSIX_AIOCB_Asynch_Operation;
  // This class does the registering of Asynch Operations with the
  // Proactor which is necessary in the AIOCB strategy.

  friend class ACE_POSIX_AIOCB_Asynch_Accept_Handler;
  // This is the helper class to <Asynch_Accept> class, takes care of
  // doing the <Asynch_Accept>.

public:
  ACE_POSIX_AIOCB_Proactor (void);
  // Constructor.

  virtual ~ACE_POSIX_AIOCB_Proactor (void);
  // Destructor.

  virtual int handle_events (ACE_Time_Value &wait_time);
  // Dispatch a single set of events.  If <wait_time> elapses before
  // any events occur, return.  Return 0 on success, non-zero (-1) on
  // timeouts/errors and errno is set accordingly.

  virtual int handle_events (void);
  // Block indefinitely until at least one event is dispatched.
  // Return 0 on success, non-zero (-1) on timeouts/errors and errno
  // is set accordingly.

  virtual int post_completion (ACE_POSIX_Asynch_Result *result);
  // Post a result to the completion port of the Proactor.

  // = Methods used to create Asynch_IO objects. We create the right
  // objects here in these methods.

  virtual ACE_Asynch_Read_Stream_Impl *create_asynch_read_stream (void);

  virtual ACE_Asynch_Write_Stream_Impl *create_asynch_write_stream (void);

  virtual ACE_Asynch_Read_File_Impl *create_asynch_read_file (void);

  virtual ACE_Asynch_Write_File_Impl *create_asynch_write_file (void);

  virtual ACE_Asynch_Accept_Impl *create_asynch_accept (void);

  virtual ACE_Asynch_Transmit_File_Impl *create_asynch_transmit_file (void);

protected:
  virtual int notify_asynch_accept (ACE_POSIX_Asynch_Accept_Result* result);
  // Asynch_Accept calls this function to notify an accept to the
  // Proactor.

  virtual int handle_events (unsigned long milli_seconds);
  // Dispatch a single set of events.  If <milli_seconds> elapses
  // before any events occur, return.

  void application_specific_code (ACE_POSIX_Asynch_Result *asynch_result,
				  u_long bytes_transferred,
				  int success,
				  const void *completion_key,
				  u_long error);
  // We will call the base class's application_specific_code from
  // here.

  int register_aio_with_proactor (aiocb *aiocbptr);
  // If the ptr is o, just check whether there is any slot free and
  // return 0 if yes, else return -1. If a valid ptr is passed, keep it
  // in a free slot.

  ACE_AIO_Accept_Handler* aio_accept_handler_;
  // This class takes care of doing <accept> when we use
  // AIO_CONTROL_BLOCKS strategy.

  aiocb *aiocb_list_ [ACE_RTSIG_MAX];
  // Use an array to keep track of all the aio's issued
  // currently. We'll limit the array size to Maximum RT signals that
  // can be queued in a process.  This is the upper limit how many aio
  // operations can be pending at a time.

  size_t aiocb_list_max_size_;
  // To maintain the maximum size of the array (list).

  size_t aiocb_list_cur_size_;
  // To maintain the current size of the array (list).
};

class ACE_Export ACE_POSIX_SIG_Proactor : public ACE_POSIX_Proactor
{
  // = TITLE
  //
  //     This Proactor implementation does compeltion querying using
  //     POSIX Real Time signals. <sigtimedwait> call is used to get
  //     the notify/get the completions.
  //
  // = DESCRIPTION
  //
public:
  ACE_POSIX_SIG_Proactor (void);
  // Constructor.

  virtual ~ACE_POSIX_SIG_Proactor (void);
  // Destructor.

  virtual int handle_events (ACE_Time_Value &wait_time);
  // Dispatch a single set of events.  If <wait_time> elapses before
  // any events occur, return.  Return 0 on success, non-zero (-1) on
  // timeouts/errors and errno is set accordingly.

  virtual int handle_events (void);
  // Block indefinitely until at least one event is dispatched.
  // Return 0 on success, non-zero (-1) on timeouts/errors and errno
  // is set accordingly.

  virtual int post_completion (ACE_POSIX_Asynch_Result *result);
  // Post a result to the completion port of the Proactor.

  // = Methods used to create Asynch_IO objects. We create the right
  // objects here in these methods.

  virtual ACE_Asynch_Read_Stream_Impl *create_asynch_read_stream (void);

  virtual ACE_Asynch_Write_Stream_Impl *create_asynch_write_stream (void);

  virtual ACE_Asynch_Read_File_Impl *create_asynch_read_file (void);

  virtual ACE_Asynch_Write_File_Impl *create_asynch_write_file (void);

  virtual ACE_Asynch_Accept_Impl *create_asynch_accept (void);

  virtual ACE_Asynch_Transmit_File_Impl *create_asynch_transmit_file (void);

protected:
  virtual int handle_events (unsigned long milli_seconds);
  // Dispatch a single set of events.  If <milli_seconds> elapses
  // before any events occur, return.

  sigset_t RT_completion_signals_;
  // These signals are used for completion notification by the
  // Proactor. ACE_SIG_AIO is the only signal used here right now. We
  // use this variable instread of hard-coding ACE_SIG_AIO.
};

#if defined (__ACE_INLINE__)
#include "ace/POSIX_Proactor.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_HAS_AIO_CALLS */
#endif /* ACE_POSIX_PROACTOR_H */
