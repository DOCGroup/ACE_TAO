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
  //     ACE_SIG_Proactor specialize this class for each strategy.

  friend class ACE_POSIX_SIG_Asynch_Accept_Handler;
  // For <POSIX_SIG_Asynch_Accept> operation, this handler class does
  // the actual work, has to register the real-time signal with the
  // Proactor. 

public:
  virtual ~ACE_POSIX_Proactor (void);
  // Virtual destructor.

  virtual int close (void);
  // Close down the Proactor.

  virtual int register_handle (ACE_HANDLE handle,
			       const void *completion_key);
  // This function is a no-op function for Unix systems. Returns 0.
  
  virtual int post_completion (ACE_POSIX_Asynch_Result *result) = 0;
  // Post a result to the completion port of the Proactor.  If errors
  // occur, the result will be deleted by this method.  If successful,
  // the result will be deleted by the Proactor when the result is
  // removed from the completion port. Therefore, the result should
  // have been dynamically allocated and should be orphaned by the
  // user once this method is called.

  int wake_up_dispatch_threads (void);
  // Add wakeup dispatch threads (reinit).

  int close_dispatch_threads (int wait);
  // Close all dispatch threads.

  size_t number_of_threads (void) const;
  void number_of_threads (size_t threads);
  // Number of thread used as a parameter to CreatIoCompletionPort.

  virtual ACE_HANDLE get_handle (void) const;
  // Get the event handle. This is a no-op in POSIX. Returns
  // ACE_INVALID_HANDLE. 

  // Methods used to create Asynch_IO_Result objects. We create the right
  // objects here in these methods.

  virtual ACE_Asynch_Read_Stream_Result_Impl *create_asynch_read_stream_result (ACE_Handler &handler,
                                                                                ACE_HANDLE handle,
                                                                                ACE_Message_Block &message_block,
                                                                                u_long bytes_to_read,
                                                                                const void* act,
                                                                                ACE_HANDLE event = ACE_INVALID_HANDLE,
                                                                                int priority = 0,
                                                                                int signal_number = ACE_SIGRTMIN);

  virtual ACE_Asynch_Write_Stream_Result_Impl *create_asynch_write_stream_result (ACE_Handler &handler,
                                                                                  ACE_HANDLE handle,
                                                                                  ACE_Message_Block &message_block,
                                                                                  u_long bytes_to_write,
                                                                                  const void* act,
                                                                                  ACE_HANDLE event = ACE_INVALID_HANDLE,
                                                                                  int priority = 0,
                                                                                  int signal_number = ACE_SIGRTMIN);

  virtual ACE_Asynch_Read_File_Result_Impl *create_asynch_read_file_result (ACE_Handler &handler,
                                                                            ACE_HANDLE handle,
                                                                            ACE_Message_Block &message_block,
                                                                            u_long bytes_to_read,
                                                                            const void* act,
                                                                            u_long offset,
                                                                            u_long offset_high,
                                                                            ACE_HANDLE event = ACE_INVALID_HANDLE,
                                                                            int priority = 0,
                                                                            int signal_number = ACE_SIGRTMIN);

  virtual ACE_Asynch_Write_File_Result_Impl *create_asynch_write_file_result (ACE_Handler &handler,
                                                                              ACE_HANDLE handle,
                                                                              ACE_Message_Block &message_block,
                                                                              u_long bytes_to_write,
                                                                              const void* act,
                                                                              u_long offset,
                                                                              u_long offset_high,
                                                                              ACE_HANDLE event = ACE_INVALID_HANDLE,
                                                                              int priority = 0,
                                                                              int signal_number = ACE_SIGRTMIN);

  virtual ACE_Asynch_Accept_Result_Impl *create_asynch_accept_result (ACE_Handler &handler,
                                                                      ACE_HANDLE listen_handle,
                                                                      ACE_HANDLE accept_handle,
                                                                      ACE_Message_Block &message_block,
                                                                      u_long bytes_to_read,
                                                                      const void* act,
                                                                      ACE_HANDLE event = ACE_INVALID_HANDLE,
                                                                      int priority = 0,
                                                                      int signal_number = ACE_SIGRTMIN);

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
                                                                                    ACE_HANDLE event = ACE_INVALID_HANDLE,
                                                                                    int priority = 0,
                                                                                    int signal_number = ACE_SIGRTMIN);
  
  virtual ACE_Asynch_Result_Impl *create_asynch_timer (ACE_Handler &handler,
                                                       const void *act,
                                                       const ACE_Time_Value &tv,
                                                       ACE_HANDLE event = ACE_INVALID_HANDLE,
                                                       int priority = 0,
                                                       int signal_number = ACE_SIGRTMIN);
  // Create a timer result object which can be used with the Timer
  // mechanism of the Proactor. 
  
protected:
  ACE_POSIX_Proactor (void);
  // Constructor.

  void application_specific_code (ACE_POSIX_Asynch_Result *asynch_result,
				  u_long bytes_transferred,
				  int success,
				  const void *completion_key,
				  u_long error);
  // Protect against structured exceptions caused by user code when
  // dispatching handles.
};

// Forward declarations.
class ACE_AIOCB_Notify_Pipe_Manager;

class ACE_Export ACE_POSIX_AIOCB_Proactor : public ACE_POSIX_Proactor
{
  // = TITLE
  //
  //     This Proactor makes use of Asynchronous I/O Control Blocks
  //     (AIOCB) to notify/get the completion status of the <aio_>
  //     operations issued.
  //
  // = DESCRIPTION
  //

  friend class ACE_AIOCB_Notify_Pipe_Manager;
  // Handler needs to call application specific code.
  
  friend class ACE_POSIX_AIOCB_Asynch_Operation;
  // This class does the registering of Asynch Operations with the
  // Proactor which is necessary in the AIOCB strategy.

  // friend class ACE_POSIX_AIOCB_Asynch_Accept_Handler;
  // For <Asynch_Accept> operation class, this helper class takes care
  // of doing the <Asynch_Accept>. 

public:
  ACE_POSIX_AIOCB_Proactor (void);
  // Constructor.

  virtual ~ACE_POSIX_AIOCB_Proactor (void);
  // Destructor.

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
  // before any events occur, return 0. Return 1 if a completion
  // dispatched. Return -1 on errors.

  void application_specific_code (ACE_POSIX_Asynch_Result *asynch_result,
				  u_long bytes_transferred,
				  int success,
				  const void *completion_key,
				  u_long error);
  // We will call the base class's application_specific_code from
  // here.
  
  int register_aio_with_proactor (ACE_POSIX_Asynch_Result *result);
  // If the ptr is o, just check whether there is any slot free and
  // return 0 if yes, else return -1. If a valid ptr is passed, keep it
  // in a free slot.

  ACE_AIOCB_Notify_Pipe_Manager* aiocb_notify_pipe_manager_;
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
  //     POSIX Real Time signals. <sigtimedwait>/<sigwaitinfo> call is
  //     used to get the notify/get the completions.
  //     The real-time signals that are going to be used with this
  //     Proactor should be given apriori in the constructor, so that
  //     those signals can be masked from asynchornous delivery. 
  //
  // = DESCRIPTION
  //

  friend class ACE_POSIX_SIG_Asynch_Operation;
  // This class does the registering of Asynch Operations with the
  // Proactor which is necessary in the SIG strategy, because we need
  // to store the signal number.

public:
  ACE_POSIX_SIG_Proactor (void);
  // This constructor masks only the <ACE_SIGRTMIN>
  // real-time signal. Only this signal should be used to issue
  // asynchronous operations using this Proctor.

  ACE_POSIX_SIG_Proactor (const sigset_t mask_set);
  // This constructor should be used to tell the Proactor to mask and
  // wait for the real-time signals specified in this set. Only these
  // signals should be used by the asynchronous operations when they
  // use this Proactor.

  virtual ~ACE_POSIX_SIG_Proactor (void);
  // Destructor.

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

  virtual ACE_Asynch_Result_Impl *create_asynch_timer (ACE_Handler &handler,
                                                       const void *act,
                                                       const ACE_Time_Value &tv,
                                                       ACE_HANDLE event = ACE_INVALID_HANDLE,
                                                       int priority = 0,
                                                       int signal_number = ACE_SIGRTMIN) ;
  // If <signal_number> is -1, check with the Proactor and use one of
  // the signals that is present in the mask set (i.e. the signals for
  // which the Proactor will be waiting) of the Proactor. If there are
  // more than one signal, the higher numbered signal will be chosen.

protected:
  int setup_signal_handler (int signal_number) const;
  // To setup the handler for a real-time signbal.

  static void null_handler (int signal_number, siginfo_t *info, void *context);
  // Dummy signal handler. This wont get called at all, since we are
  // going to be masking the signal in all the threads.
  
  int mask_all (void) const;
  // To mask all the signals in a thread.

  virtual int handle_events (unsigned long milli_seconds);
  // Dispatch a single set of events.  If <milli_seconds> elapses
  // before any events occur, return 0. Return 1 if a completion is
  // dispatched. Return -1 on errors.

  sigset_t RT_completion_signals_;
  // These signals are used for completion notification by the
  // Proactor. The signals specified while issueing <Asynch
  // Operation>s are stored here in this set. These signals are masked
  // for a thread when it calls the Proactor::handle_events.
};

class ACE_Export ACE_POSIX_Asynch_Timer : public ACE_POSIX_Asynch_Result
{
  // = TITLE
  //     This class is posted to the completion port when a timer
  //     expires. When the <complete method> of this object is
  //     called, the <handler>'s <handle_timeout> method will be
  //     called. 
  
  friend class ACE_POSIX_Proactor;
  friend class ACE_POSIX_SIG_Proactor;
  // The factory method for this class is with the POSIX_Proactor 
  // class. 

protected:
  ACE_POSIX_Asynch_Timer (ACE_Handler &handler,
                          const void *act,
                          const ACE_Time_Value &tv,
                          ACE_HANDLE event = ACE_INVALID_HANDLE,
                          int priority = 0,
                          int signal_number = ACE_SIGRTMIN);
  // Constructor.
  
  virtual ~ACE_POSIX_Asynch_Timer (void) {}
  // Destructor.

  virtual void complete (u_long bytes_transferred,
                         int success,
                         const void *completion_key,
                         u_long error = 0);
  // This method calls the <handler>'s handle_timeout method.

  ACE_Time_Value time_;
  // Time value requested by caller
};

#if defined (__ACE_INLINE__)
#include "ace/POSIX_Proactor.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_HAS_AIO_CALLS */
#endif /* ACE_POSIX_PROACTOR_H */
