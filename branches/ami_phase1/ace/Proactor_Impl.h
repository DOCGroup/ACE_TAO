/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    Proactor_Impl.h
//
// = AUTHOR
//    Alexander Babu Arulanthu <alex@cs.wustl.edu>
//
// ============================================================================

#if !defined (ACE_PROACTOR_IMPL_H)
#define ACE_PROACTOR_IMPL_H

#include "ace/OS.h"

#if ((defined (ACE_WIN32) && !defined (ACE_HAS_WINCE)) || (defined (ACE_HAS_AIO_CALLS)))
// This only works on Win32 platforms and on Unix platforms supporting
// aio calls.  

#include "ace/Asynch_IO.h"
#include "ace/Reactor.h"

class ACE_Export ACE_Proactor_Impl : public ACE_Event_Handler
{
  // = TITLE
  // 
  //     A manager for asynchronous event demultiplexing. This class
  //     is the base class for all the concrete implementation
  //     classes. 
  //
  // = DESCRIPTION
  // 
  //     See the Proactor pattern description at
  //     http://www.cs.wustl.edu/~schmidt/proactor.ps.gz for more
  //     details.
  
public:
  virtual ~ACE_Proactor_Impl (void) {}
  // Virtual destruction.

  virtual int close (void) = 0;
  // Close the IO completion port.

  virtual int register_handle (ACE_HANDLE handle,
			       const void *completion_key) = 0;
  // This method adds the <handle> to the I/O completion port. This
  // function is a no-op function for Unix systems.

  virtual int handle_events (ACE_Time_Value &wait_time) = 0;
  // Dispatch a single set of events.  If <wait_time> elapses before
  // any events occur, return 0.  Return 1 on success i.e., when a
  // completion is dispatched, non-zero (-1) on errors and errno is
  // set accordingly.
  
  virtual int handle_events (void) = 0;
  // Block indefinitely until at least one event is dispatched.
  // Dispatch a single set of events.  If <wait_time> elapses before
  // any events occur, return 0.  Return 1 on success i.e., when a
  // completion is dispatched, non-zero (-1) on errors and errno is
  // set accordingly.
 
  virtual int wake_up_dispatch_threads (void) = 0;
  // Add wakeup dispatch threads (reinit).

  virtual int close_dispatch_threads (int wait) = 0;
  // Close all dispatch threads.

  virtual size_t number_of_threads (void) const = 0;
  virtual void number_of_threads (size_t threads) = 0;
  // Number of thread used as a parameter to CreatIoCompletionPort. 

  virtual ACE_HANDLE get_handle (void) const = 0;
  // Get the event handle.
  
  //
  // = Factory methods for the operations
  //
  // Note that the user does not have to use or know about these
  // methods.

  virtual ACE_Asynch_Read_Stream_Impl *create_asynch_read_stream (void) = 0;
    // Create the correct implementation class for doing Asynch_Read_Stream. 

  virtual ACE_Asynch_Write_Stream_Impl *create_asynch_write_stream (void) = 0;
  // Create the correct implementation class for doing Asynch_Write_Stream. 
  
  virtual ACE_Asynch_Read_File_Impl *create_asynch_read_file (void) = 0;
  // Create the correct implementation class for doing Asynch_Read_File. 
  
  virtual ACE_Asynch_Write_File_Impl *create_asynch_write_file (void) = 0;
    // Create the correct implementation class for doing Asynch_Write_File. 

  virtual ACE_Asynch_Accept_Impl *create_asynch_accept (void) = 0;
  // Create the correct implementation class for doing Asynch_Accept. 
  
  virtual ACE_Asynch_Transmit_File_Impl *create_asynch_transmit_file (void) = 0;
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
                                                                                ACE_HANDLE event = ACE_INVALID_HANDLE,
                                                                                int priority = 0,
                                                                                int signal_number = ACE_SIGRTMIN) = 0;
  // Create the correct implementation class for ACE_Asynch_Read_Stream::Result class. 
  
  virtual ACE_Asynch_Write_Stream_Result_Impl *create_asynch_write_stream_result (ACE_Handler &handler,
                                                                                  ACE_HANDLE handle,
                                                                                  ACE_Message_Block &message_block,
                                                                                  u_long bytes_to_write,
                                                                                  const void* act,
                                                                                  ACE_HANDLE event = ACE_INVALID_HANDLE,
                                                                                  int priority = 0,
                                                                                  int signal_number = ACE_SIGRTMIN) = 0;
  // Create the correct implementation class for ACE_Asynch_Write_Stream::Result.
    
  virtual ACE_Asynch_Read_File_Result_Impl *create_asynch_read_file_result (ACE_Handler &handler,
                                                                            ACE_HANDLE handle,
                                                                            ACE_Message_Block &message_block,
                                                                            u_long bytes_to_read,
                                                                            const void* act,
                                                                            u_long offset,
                                                                            u_long offset_high,
                                                                            ACE_HANDLE event = ACE_INVALID_HANDLE,
                                                                            int priority = 0,
                                                                            int signal_number = ACE_SIGRTMIN) = 0;
  // Create the correct implementation class for ACE_Asynch_Read_File::Result.  
  
  virtual ACE_Asynch_Write_File_Result_Impl *create_asynch_write_file_result (ACE_Handler &handler,
                                                                              ACE_HANDLE handle,
                                                                              ACE_Message_Block &message_block,
                                                                              u_long bytes_to_write,
                                                                              const void* act,
                                                                              u_long offset,
                                                                              u_long offset_high,
                                                                              ACE_HANDLE event = ACE_INVALID_HANDLE,
                                                                              int priority = 0,
                                                                              int signal_number = ACE_SIGRTMIN) = 0;
    // Create the correct implementation class for ACE_Asynch_Write_File::Result.
  
  virtual ACE_Asynch_Accept_Result_Impl *create_asynch_accept_result (ACE_Handler &handler,
                                                                      ACE_HANDLE listen_handle,
                                                                      ACE_HANDLE accept_handle,
                                                                      ACE_Message_Block &message_block,
                                                                      u_long bytes_to_read,
                                                                      const void* act,
                                                                      ACE_HANDLE event = ACE_INVALID_HANDLE,
                                                                      int priority = 0,
                                                                      int signal_number = ACE_SIGRTMIN) = 0;
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
                                                                                    ACE_HANDLE event = ACE_INVALID_HANDLE,
                                                                                    int priority = 0,
                                                                                    int signal_number = ACE_SIGRTMIN) = 0;
  // Create the correct implementation class for ACE_Asynch_Transmit_File::Result.

  virtual ACE_Asynch_Result_Impl *create_asynch_timer (ACE_Handler &handler,
                                                       const void *act,
                                                       const ACE_Time_Value &tv,
                                                       ACE_HANDLE event = ACE_INVALID_HANDLE,
                                                       int priority = 0,
                                                       int signal_number = 0) = 0;
  // Create the correct implementation object for the Timer
  // result. POSIX_SIG_Proactor will create a Timer object with a
  // meaningful signal number, if you leave the signal number as 0. 
};

#endif /* (ACE_WIN32 && ACE_HAS_WINCE) || ACE_HAS_AIO_CALLS */
#endif /* ACE_PROACTOR_IMPL_H */
