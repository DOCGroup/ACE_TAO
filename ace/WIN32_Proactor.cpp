// $Id$

#define ACE_BUILD_DLL

// ACE_RCSID(ace, Proactor, "$Id$")

#include "ace/WIN32_Proactor.h"

#if (defined (ACE_WIN32) && !defined (ACE_HAS_WINCE))
// WIN implemenatation of the Proactor.

#include "ace/Log_Msg.h"
#include "ace/Object_Manager.h"

#if !defined (__ACE_INLINE__)
#include "ace/WIN32_Proactor.i"
#endif /* __ACE_INLINE__ */

ACE_WIN32_Proactor::ACE_WIN32_Proactor (size_t number_of_threads,
                                        int used_with_reactor_event_loop)
  : completion_port_ (0),
    // This *MUST* be 0, *NOT* ACE_INVALID_HANDLE !!!
    number_of_threads_ (number_of_threads),
    used_with_reactor_event_loop_ (used_with_reactor_event_loop)
{
  // Create the completion port.
  this->completion_port_ = ::CreateIoCompletionPort (INVALID_HANDLE_VALUE,
                                                     this->completion_port_,
                                                     0,
                                                     this->number_of_threads_);
  if (this->completion_port_ == 0)
    ACE_ERROR ((LM_ERROR,
                ASYS_TEXT ("%p\n"),
                ASYS_TEXT ("CreateIoCompletionPort")));
}

ACE_WIN32_Proactor::~ACE_WIN32_Proactor (void)
{
  this->close ();
}

int
ACE_WIN32_Proactor::close (void)
{
  // Close the completion port
  if (this->completion_port_ != 0)
    {
      int result = ACE_OS::close (this->completion_port_);
      this->completion_port_ = 0;
      return result;
    }

  return 0;
}

int
ACE_WIN32_Proactor::register_handle (ACE_HANDLE handle,
                                     const void *completion_key)
{
  // No locking is needed here as no state changes.
  ACE_HANDLE cp = ::CreateIoCompletionPort (handle,
                                            this->completion_port_,
                                            (u_long) completion_key,
                                            this->number_of_threads_);
  if (cp == 0)
    {
      errno = ::GetLastError ();
      // If errno == ERROR_INVALID_PARAMETER, then this handle was
      // already registered.
      if (errno != ERROR_INVALID_PARAMETER)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ASYS_TEXT ("%p\n"),
                           ASYS_TEXT ("CreateIoCompletionPort")), -1);
    }
  return 0;
}

ACE_Asynch_Read_Stream_Impl *
ACE_WIN32_Proactor::create_asynch_read_stream (void)
{
  ACE_Asynch_Read_Stream_Impl *implementation = 0;
  ACE_NEW_RETURN (implementation,
                  ACE_WIN32_Asynch_Read_Stream (this),
                  0);
  return implementation;
}

ACE_Asynch_Write_Stream_Impl *
ACE_WIN32_Proactor::create_asynch_write_stream (void)
{
  ACE_Asynch_Write_Stream_Impl *implementation = 0;
  ACE_NEW_RETURN (implementation,
                  ACE_WIN32_Asynch_Write_Stream (this),
                  0);
  return implementation;
}

ACE_Asynch_Read_File_Impl *
ACE_WIN32_Proactor::create_asynch_read_file (void)
{
  ACE_Asynch_Read_File_Impl *implementation = 0;
  ACE_NEW_RETURN (implementation,
                  ACE_WIN32_Asynch_Read_File (this),
                  0);
  return  implementation;
}

ACE_Asynch_Write_File_Impl *
ACE_WIN32_Proactor::create_asynch_write_file (void)
{
  ACE_Asynch_Write_File_Impl *implementation = 0;
  ACE_NEW_RETURN (implementation,
                  ACE_WIN32_Asynch_Write_File (this),
                  0);
  return  implementation;
}

ACE_Asynch_Accept_Impl *
ACE_WIN32_Proactor::create_asynch_accept (void)
{
  ACE_Asynch_Accept_Impl *implementation = 0;
  ACE_NEW_RETURN (implementation,
                  ACE_WIN32_Asynch_Accept (this),
                  0);
  return implementation;
}

ACE_Asynch_Transmit_File_Impl *
ACE_WIN32_Proactor::create_asynch_transmit_file (void)
{
  ACE_Asynch_Transmit_File_Impl *implementation = 0;
  ACE_NEW_RETURN (implementation,
                  ACE_WIN32_Asynch_Transmit_File (this),
                  0);
  return  implementation;
}

ACE_Asynch_Read_Stream_Result_Impl *
ACE_WIN32_Proactor::create_asynch_read_stream_result (ACE_Handler &handler,
                                                      ACE_HANDLE handle,
                                                      ACE_Message_Block &message_block,
                                                      u_long bytes_to_read,
                                                      const void* act,
                                                      ACE_HANDLE event,
                                                      int priority,
                                                      int signal_number)
{
  ACE_Asynch_Read_Stream_Result_Impl *implementation;
  ACE_NEW_RETURN (implementation,
                  ACE_WIN32_Asynch_Read_Stream_Result (handler,
                                                       handle,
                                                       message_block,
                                                       bytes_to_read,
                                                       act,
                                                       event,
                                                       priority,
                                                       signal_number),
                  0);
  return implementation;
}

ACE_Asynch_Write_Stream_Result_Impl *
ACE_WIN32_Proactor::create_asynch_write_stream_result (ACE_Handler &handler,
                                                       ACE_HANDLE handle,
                                                       ACE_Message_Block &message_block,
                                                       u_long bytes_to_write,
                                                       const void* act,
                                                       ACE_HANDLE event,
                                                       int priority,
                                                       int signal_number)
{
  ACE_Asynch_Write_Stream_Result_Impl *implementation;
  ACE_NEW_RETURN (implementation,
                  ACE_WIN32_Asynch_Write_Stream_Result (handler,
                                                        handle,
                                                        message_block,
                                                        bytes_to_write,
                                                        act,
                                                        event,
                                                        priority,
                                                        signal_number),
                  0);
  return implementation;
}

ACE_Asynch_Read_File_Result_Impl *
ACE_WIN32_Proactor::create_asynch_read_file_result (ACE_Handler &handler,
                                                    ACE_HANDLE handle,
                                                    ACE_Message_Block &message_block,
                                                    u_long bytes_to_read,
                                                    const void* act,
                                                    u_long offset,
                                                    u_long offset_high,
                                                    ACE_HANDLE event,
                                                    int priority,
                                                    int signal_number)
{
  ACE_Asynch_Read_File_Result_Impl *implementation;
  ACE_NEW_RETURN (implementation,
                  ACE_WIN32_Asynch_Read_File_Result (handler,
                                                     handle,
                                                     message_block,
                                                     bytes_to_read,
                                                     act,
                                                     offset,
                                                     offset_high,
                                                     event,
                                                     priority,
                                                     signal_number),
                  0);
  return implementation;
}

ACE_Asynch_Write_File_Result_Impl *
ACE_WIN32_Proactor::create_asynch_write_file_result (ACE_Handler &handler,
                                                     ACE_HANDLE handle,
                                                     ACE_Message_Block &message_block,
                                                     u_long bytes_to_write,
                                                     const void* act,
                                                     u_long offset,
                                                     u_long offset_high,
                                                     ACE_HANDLE event,
                                                     int priority,
                                                     int signal_number)
{
  ACE_Asynch_Write_File_Result_Impl *implementation;
  ACE_NEW_RETURN (implementation,
                  ACE_WIN32_Asynch_Write_File_Result (handler,
                                                      handle,
                                                      message_block,
                                                      bytes_to_write,
                                                      act,
                                                      offset,
                                                      offset_high,
                                                      event,
                                                      priority,
                                                      signal_number),
                  0);
  return implementation;
}

ACE_Asynch_Accept_Result_Impl *
ACE_WIN32_Proactor::create_asynch_accept_result (ACE_Handler &handler,
                                                 ACE_HANDLE listen_handle,
                                                 ACE_HANDLE accept_handle,
                                                 ACE_Message_Block &message_block,
                                                 u_long bytes_to_read,
                                                 const void* act,
                                                 ACE_HANDLE event,
                                                 int priority,
                                                 int signal_number)
{
  ACE_Asynch_Accept_Result_Impl *implementation;
  ACE_NEW_RETURN (implementation,
                  ACE_WIN32_Asynch_Accept_Result (handler,
                                                  listen_handle,
                                                  accept_handle,
                                                  message_block,
                                                  bytes_to_read,
                                                  act,
                                                  event,
                                                  priority,
                                                  signal_number),
                  0);
  return implementation;
}

ACE_Asynch_Transmit_File_Result_Impl *
ACE_WIN32_Proactor::create_asynch_transmit_file_result (ACE_Handler &handler,
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
                                                        int signal_number)
{
  ACE_Asynch_Transmit_File_Result_Impl *implementation;
  ACE_NEW_RETURN (implementation,
                  ACE_WIN32_Asynch_Transmit_File_Result (handler,
                                                         socket,
                                                         file,
                                                         header_and_trailer,
                                                         bytes_to_write,
                                                         offset,
                                                         offset_high,
                                                         bytes_per_send,
                                                         flags,
                                                         act,
                                                         event,
                                                         priority,
                                                         signal_number),
                  0);
  return implementation;
}

ACE_Asynch_Result_Impl *
ACE_WIN32_Proactor::create_asynch_timer (ACE_Handler &handler,
                                         const void *act,
                                         const ACE_Time_Value &tv,
                                         ACE_HANDLE event,
                                         int priority,
                                         int signal_number)
{
  ACE_Asynch_Result_Impl *implementation;
  ACE_NEW_RETURN (implementation,
                  ACE_WIN32_Asynch_Timer (handler,
                                          act,
                                          tv,
                                          event,
                                          priority,
                                          signal_number),
                  0);
  return implementation;
}

int
ACE_WIN32_Proactor::handle_signal (int, siginfo_t *, ucontext_t *)
{
  // Perform a non-blocking "poll" for all the I/O events that have
  // completed in the I/O completion queue.

  ACE_Time_Value timeout (0, 0);
  int result = 0;

  while (1)
    {
      result = this->handle_events (timeout);
      if (result != 0 || errno == ETIME)
        break;
    }

  // If our handle_events failed, we'll report a failure to the
  // Reactor.
  return result == -1 ? -1 : 0;
}

int
ACE_WIN32_Proactor::handle_close (ACE_HANDLE handle,
                                  ACE_Reactor_Mask close_mask)
{
  ACE_UNUSED_ARG (close_mask);
  ACE_UNUSED_ARG (handle);

  return this->close ();
}

ACE_HANDLE
ACE_WIN32_Proactor::get_handle (void) const
{
  if (this->used_with_reactor_event_loop_)
    return this->event_.handle ();
  else
    return 0;
}

int
ACE_WIN32_Proactor::handle_events (ACE_Time_Value &wait_time)
{
  // Decrement <wait_time> with the amount of time spent in the method
  ACE_Countdown_Time countdown (&wait_time);
  return this->handle_events (wait_time.msec ());
}

int
ACE_WIN32_Proactor::handle_events (void)
{
  return this->handle_events (ACE_INFINITE);
}

int
ACE_WIN32_Proactor::handle_events (unsigned long milli_seconds)
{
  ACE_OVERLAPPED *overlapped = 0;
  u_long bytes_transferred = 0;
  u_long completion_key = 0;

  // Get the next asynchronous operation that completes
  BOOL result = ::GetQueuedCompletionStatus (this->completion_port_,
                                             &bytes_transferred,
                                             &completion_key,
                                             &overlapped,
                                             milli_seconds);
  if (result == FALSE && overlapped == 0)
    {
      errno = ::GetLastError ();

      if (errno == WAIT_TIMEOUT)
        {
          errno = ETIME;
          return 0;
        }
      else
        ACE_ERROR_RETURN ((LM_ERROR,
                           ASYS_TEXT ("%p\n"),
                           ASYS_TEXT ("GetQueuedCompletionStatus")),
                          -1);
    }
  else
    {
      // Narrow the result.
      ACE_WIN32_Asynch_Result *asynch_result = (ACE_WIN32_Asynch_Result *) overlapped;

      // If errors happen, grab the error.
      if (result == FALSE)
        errno = ::GetLastError ();
      else
        errno = 0;

      this->application_specific_code (asynch_result,
                                       bytes_transferred,
                                       result,
                                       (void *) completion_key,
                                       errno);
    }
  return 1;
}

void
ACE_WIN32_Proactor::application_specific_code (ACE_WIN32_Asynch_Result *asynch_result,
                                               u_long bytes_transferred,
                                               int success,
                                               const void *completion_key,
                                               u_long error)
{
  ACE_SEH_TRY
    {
      // Call completion hook
      asynch_result->complete (bytes_transferred,
                               success,
                               (void *) completion_key,
                               error);
    }
  ACE_SEH_FINALLY
    {
      // This is crucial to prevent memory leaks
      delete asynch_result;
    }
}

int
ACE_WIN32_Proactor::post_completion (ACE_WIN32_Asynch_Result *result)
{
  // Grab the event associated with the Proactor
  HANDLE handle = this->get_handle ();
  
  // If Proactor event is valid, signal it
  if (handle != ACE_INVALID_HANDLE &&
      handle != 0)
    ACE_OS::event_signal (&handle);

  // Post a completion
  if (::PostQueuedCompletionStatus (this->completion_port_, // completion port
                                    0,            // number of bytes tranferred
                                    0,            // completion key
                                    result      // overlapped
                                    ) == FALSE)
    {
      delete result;
      ACE_ERROR_RETURN ((LM_ERROR, "PostQueuedCompletionStatus failed\n"), -1);
    }

  return 0;
}

int
ACE_WIN32_Proactor::wake_up_dispatch_threads (void)
{
  return 0;
}

int
ACE_WIN32_Proactor::close_dispatch_threads (int)
{
  return 0;
}

size_t
ACE_WIN32_Proactor::number_of_threads (void) const
{
  return this->number_of_threads_;
}

void
ACE_WIN32_Proactor::number_of_threads (size_t threads)
{
  this->number_of_threads_ = threads;
}

// *********************************************************************

ACE_WIN32_Asynch_Timer::ACE_WIN32_Asynch_Timer (ACE_Handler &handler,
                                                const void *act,
                                                const ACE_Time_Value &tv,
                                                ACE_HANDLE event,
                                                int priority,
                                                int signal_number)
  : ACE_Asynch_Result_Impl (),
    ACE_WIN32_Asynch_Result (handler, act, event, 0, 0, priority,
                             signal_number),
    time_ (tv)
{
}

void
ACE_WIN32_Asynch_Timer::complete (u_long bytes_transferred,
                                  int success,
                                  const void *completion_key,
                                  u_long error)
{
    ACE_UNUSED_ARG (error);
  ACE_UNUSED_ARG (completion_key);
  ACE_UNUSED_ARG (success);
  ACE_UNUSED_ARG (bytes_transferred);

  this->handler_.handle_time_out (this->time_, this->act ());
}

#endif /* ACE_WIN32 */
