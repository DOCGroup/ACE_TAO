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
#include "ace/Event_Handler.h"

#if (defined (ACE_WIN32) && !defined (ACE_HAS_WINCE))
// WIN32 implementation of the Proactor.

#include "ace/Proactor_Impl.h"

// Forward declarations.
class ACE_WIN32_Asynch_Result;
class ACE_WIN32_Proactor_Timer_Handler;

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
  ACE_WIN32_Proactor (size_t number_of_threads = 0,
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
                                                                                int priority,
                                                                                int signal_number = 0);

  virtual ACE_Asynch_Write_Stream_Result_Impl *create_asynch_write_stream_result (ACE_Handler &handler,
                                                                                  ACE_HANDLE handle,
                                                                                  ACE_Message_Block &message_block,
                                                                                  u_long bytes_to_write,
                                                                                  const void* act,
                                                                                  ACE_HANDLE event,
                                                                                  int priority,
                                                                                  int signal_number = 0);

  virtual ACE_Asynch_Read_File_Result_Impl *create_asynch_read_file_result (ACE_Handler &handler,
                                                                            ACE_HANDLE handle,
                                                                            ACE_Message_Block &message_block,
                                                                            u_long bytes_to_read,
                                                                            const void* act,
                                                                            u_long offset,
                                                                            u_long offset_high,
                                                                            ACE_HANDLE event,
                                                                            int priority,
                                                                            int signal_number = 0);

  virtual ACE_Asynch_Write_File_Result_Impl *create_asynch_write_file_result (ACE_Handler &handler,
                                                                              ACE_HANDLE handle,
                                                                              ACE_Message_Block &message_block,
                                                                              u_long bytes_to_write,
                                                                              const void* act,
                                                                              u_long offset,
                                                                              u_long offset_high,
                                                                              ACE_HANDLE event,
                                                                              int priority,
                                                                              int signal_number = 0);

  virtual ACE_Asynch_Accept_Result_Impl *create_asynch_accept_result (ACE_Handler &handler,
                                                                      ACE_HANDLE listen_handle,
                                                                      ACE_HANDLE accept_handle,
                                                                      ACE_Message_Block &message_block,
                                                                      u_long bytes_to_read,
                                                                      const void* act,
                                                                      ACE_HANDLE event,
                                                                      int priority,
                                                                      int signal_number = 0);

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
                                                                                    int priority,
                                                                                    int signal_number = 0);
  
  virtual ACE_Asynch_Result_Impl *create_asynch_timer (ACE_Handler &handler,
                                                       const void *act,
                                                       const ACE_Time_Value &tv,
                                                       ACE_HANDLE event,
                                                       int priority,
                                                       int signal_number = 0);
  // Create a timer result object which can be used with the Timer
  // mechanism of the Proactor. 

protected:
  virtual int handle_signal (int signum, siginfo_t * = 0, ucontext_t * = 0);
  // Called when object is signaled by OS (either via UNIX signals or
  // when a Win32 object becomes signaled).

  virtual int handle_close (ACE_HANDLE handle,
			    ACE_Reactor_Mask close_mask);
  // Called when object is removed from the ACE_Reactor.

  virtual int handle_events (unsigned long milli_seconds);
  // Dispatch a single set of events.  If <milli_seconds> elapses
  // before any events occur, return 0. Return 1 if a completion is
  // dispatched. Return -1 on errors.  

  void application_specific_code (ACE_WIN32_Asynch_Result *asynch_result,
				  u_long bytes_transferred,
				  int success,
				  const void *completion_key,
				  u_long error);
  // Protect against structured exceptions caused by user code when
  // dispatching handles.

  virtual int post_wakeup_completions (int how_many);
  // Post <how_many> completions to the completion port so that all
  // threads can wake up. This is used in conjunction with the
  // <run_event_loop>. 

  ACE_HANDLE completion_port_;
  // Handle for the completion port. Unix doesnt have completion
  // ports.

  size_t number_of_threads_;
  // This number is passed to the <CreatIOCompletionPort> system
  // call.

  ACE_Auto_Event event_;
  // This event is used in conjunction with Reactor when we try to
  // integrate the event loops of Reactor and the Proactor.

  int used_with_reactor_event_loop_;
  // Flag that indicates whether we are used in conjunction with
  // Reactor.
  
  ACE_Handler wakeup_handler_;
  // Handler to handle the wakeups. This works in conjunction with the
  // <ACE_Proactor::run_event_loop>. 
};

class ACE_Export ACE_WIN32_Asynch_Timer : public ACE_WIN32_Asynch_Result
{
  // = TITLE
  //     This class is posted to the completion port when a timer
  //     expires. When the complete method of this object is
  //     called, the <handler>'s handle_timeout method will be
  //     called.

  friend class ACE_WIN32_Proactor;
  // The factory method for this class is with the POSIX_Proactor
  // class.
 
protected:
  ACE_WIN32_Asynch_Timer (ACE_Handler &handler,
                          const void *act,
                          const ACE_Time_Value &tv,
                          ACE_HANDLE event = ACE_INVALID_HANDLE,
                          int priority = 0,
                          int signal_number = 0);
  // Constructor.
  
  virtual void complete (u_long bytes_transferred,
                         int success,
                         const void *completion_key,
                         u_long error = 0);
  // This method calls the <handler>'s handle_timeout method.

  ACE_Time_Value time_;
  // Time value requested by caller
};

#if defined (__ACE_INLINE__)
#include "ace/WIN32_Proactor.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_WIN32 */
#endif /* ACE_PROACTOR_H */
