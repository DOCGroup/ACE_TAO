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

  // = Timer management.
  virtual long schedule_timer (ACE_Handler &handler,
			       const void *act,
			       const ACE_Time_Value &time) = 0;
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
					 const ACE_Time_Value &interval) = 0;

  // Same as above except <interval> it is used to reschedule the
  // <handler> automatically.

  virtual long schedule_timer (ACE_Handler &handler,
			       const void *act,
			       const ACE_Time_Value &time,
			       const ACE_Time_Value &interval) = 0;
  // This combines the above two methods into one. Mostly for backward
  // compatibility.

  virtual int cancel_timer (ACE_Handler &handler,
			    int dont_call_handle_close) = 0;
  // Cancel all timers associated with this <handler>.  Returns number
  // of timers cancelled.

  virtual int cancel_timer (long timer_id,
			    const void **act,
			    int dont_call_handle_close) = 0;
  // Cancel the single <ACE_Handler> that matches the <timer_id> value
  // (which was returned from the <schedule> method).  If <act> is
  // non-NULL then it will be set to point to the ``magic cookie''
  // argument passed in when the <Handler> was registered.  This makes
  // it possible to free up the memory and avoid memory leaks.
  // Returns 1 if cancellation succeeded and 0 if the <timer_id>
  // wasn't found.

  virtual int handle_events (ACE_Time_Value &wait_time) = 0;
  // Dispatch a single set of events.  If <wait_time> elapses before
  // any events occur, return.  Return 0 on success, non-zero (-1) on
  // timeouts/errors and errno is set accordingly.

  virtual int handle_events (void) = 0;
  // Block indefinitely until at least one event is dispatched.
  // Return 0 on success, non-zero (-1) on timeouts/errors and errno
  // is set accordingly.
  
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
                                                                                ACE_HANDLE event,
                                                                                int priority = 0) = 0;
  // Create the correct implementation class for ACE_Asynch_Read_Stream::Result class. 
  
  virtual ACE_Asynch_Write_Stream_Result_Impl *create_asynch_write_stream_result (ACE_Handler &handler,
                                                                                  ACE_HANDLE handle,
                                                                                  ACE_Message_Block &message_block,
                                                                                  u_long bytes_to_write,
                                                                                  const void* act,
                                                                                  ACE_HANDLE event,
                                                                                  int priority = 0) = 0;
  // Create the correct implementation class for ACE_Asynch_Write_Stream::Result.
    
  virtual ACE_Asynch_Read_File_Result_Impl *create_asynch_read_file_result (ACE_Handler &handler,
                                                                            ACE_HANDLE handle,
                                                                            ACE_Message_Block &message_block,
                                                                            u_long bytes_to_read,
                                                                            const void* act,
                                                                            u_long offset,
                                                                            u_long offset_high,
                                                                            ACE_HANDLE event,
                                                                            int priority = 0) = 0;
  // Create the correct implementation class for ACE_Asynch_Read_File::Result.  
  
  virtual ACE_Asynch_Write_File_Result_Impl *create_asynch_write_file_result (ACE_Handler &handler,
                                                                              ACE_HANDLE handle,
                                                                              ACE_Message_Block &message_block,
                                                                              u_long bytes_to_write,
                                                                              const void* act,
                                                                              u_long offset,
                                                                              u_long offset_high,
                                                                              ACE_HANDLE event,
                                                                              int priority = 0) = 0;
    // Create the correct implementation class for ACE_Asynch_Write_File::Result.
  
  virtual ACE_Asynch_Accept_Result_Impl *create_asynch_accept_result (ACE_Handler &handler,
                                                                      ACE_HANDLE listen_handle,
                                                                      ACE_HANDLE accept_handle,
                                                                      ACE_Message_Block &message_block,
                                                                      u_long bytes_to_read,
                                                                      const void* act,
                                                                      ACE_HANDLE event,
                                                                      int priority = 0) = 0;
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
                                                                                    int priority = 0) = 0;
  // Create the correct implementation class for ACE_Asynch_Transmit_File::Result.
};
#endif /* (ACE_WIN32 && ACE_HAS_WINCE) || ACE_HAS_AIO_CALLS */
#endif /* ACE_PROACTOR_IMPL_H */
