// $Id$

#define ACE_BUILD_DLL
#include "ace/Proactor.h"
#include "ace/Proactor_Impl.h"
#include "ace/Object_Manager.h"

ACE_RCSID(ace, Proactor, "$Id$")

#if ((defined (ACE_WIN32) && !defined (ACE_HAS_WINCE)) || (defined (ACE_HAS_AIO_CALLS)))
// This only works on Win32 platforms and on Unix platforms with aio
// calls.
#include "ace/Task_T.h"
#include "ace/Log_Msg.h"

#if defined (ACE_HAS_AIO_CALLS)
#   include "ace/POSIX_Proactor.h"
#else /* !ACE_HAS_AIO_CALLS */
#   include "ace/WIN32_Proactor.h"
#endif /* ACE_HAS_AIO_CALLS */

#if !defined (__ACE_INLINE__)
#include "ace/Proactor.i"
#endif /* __ACE_INLINE__ */

// Process-wide ACE_Proactor.
ACE_Proactor *ACE_Proactor::proactor_ = 0;

// Controls whether the Proactor is deleted when we shut down (we can
// only delete it safely if we created it!)
int ACE_Proactor::delete_proactor_ = 0;

// Terminate the eventloop.
sig_atomic_t ACE_Proactor::end_event_loop_ = 0;

ACE_Proactor::ACE_Proactor (ACE_Proactor_Impl *implementation,
                            int delete_implementation)
  : implementation_ (0),
    delete_implementation_ (delete_implementation)
{
  this->implementation (implementation);

  if (this->implementation () == 0)
    {
#if defined (ACE_HAS_AIO_CALLS)
      // POSIX Proactor.
  #if defined (ACE_POSIX_AIOCB_PROACTOR)
      ACE_NEW (implementation, ACE_POSIX_AIOCB_Proactor);
  #elif defined (ACE_POSIX_SIG_PROACTOR)
      ACE_NEW (implementation, ACE_POSIX_SIG_Proactor);
  #else /* Default is to use the AIOCB one */
      ACE_NEW (implementation, ACE_POSIX_AIOCB_Proactor);
  #endif
#elif (defined (ACE_WIN32) && !defined (ACE_HAS_WINCE))
      // WIN_Proactor.
      ACE_NEW (implementation, ACE_WIN32_Proactor);
#endif /* ACE_HAS_AIO_CALLS */
      this->implementation (implementation);
      this->delete_implementation_ = 1;
    }
}

ACE_Proactor::~ACE_Proactor (void)
{
  if (this->delete_implementation_)
    delete this->implementation ();
}

ACE_Proactor *
ACE_Proactor::instance (size_t threads)
{
  ACE_TRACE ("ACE_Proactor::instance");

  if (ACE_Proactor::proactor_ == 0)
    {
      // Perform Double-Checked Locking Optimization.
      ACE_MT (ACE_GUARD_RETURN (ACE_Recursive_Thread_Mutex, ace_mon,
				*ACE_Static_Object_Lock::instance (),
                                0));

      if (ACE_Proactor::proactor_ == 0)
	{
	  ACE_NEW_RETURN (ACE_Proactor::proactor_,
                          ACE_Proactor,
                          0);
	  ACE_Proactor::delete_proactor_ = 1;
	}
    }
  return ACE_Proactor::proactor_;
}

ACE_Proactor *
ACE_Proactor::instance (ACE_Proactor *r)
{
  ACE_TRACE ("ACE_Proactor::instance");

  ACE_MT (ACE_GUARD_RETURN (ACE_Recursive_Thread_Mutex, ace_mon,
			    *ACE_Static_Object_Lock::instance (), 0));

  ACE_Proactor *t = ACE_Proactor::proactor_;

  // We can't safely delete it since we don't know who created it!
  ACE_Proactor::delete_proactor_ = 0;

  ACE_Proactor::proactor_ = r;
  return t;
}

void
ACE_Proactor::close_singleton (void)
{
  ACE_TRACE ("ACE_Proactor::close_singleton");

  ACE_MT (ACE_GUARD (ACE_Recursive_Thread_Mutex, ace_mon,
		     *ACE_Static_Object_Lock::instance ()));

  if (ACE_Proactor::delete_proactor_)
    {
      delete ACE_Proactor::proactor_;
      ACE_Proactor::proactor_ = 0;
      ACE_Proactor::delete_proactor_ = 0;
    }
}

int
ACE_Proactor::run_event_loop (void)
{
  ACE_TRACE ("ACE_Proactor::run_event_loop");

  while (ACE_Proactor::end_event_loop_ == 0)
    {
      int result = ACE_Proactor::instance ()->handle_events ();

      if (ACE_Service_Config::reconfig_occurred ())
	ACE_Service_Config::reconfigure ();

      else if (result == -1)
	return -1;
    }

  return 0;
}

// Handle events for -tv- time.  handle_events updates -tv- to reflect
// time elapsed, so do not return until -tv- == 0, or an error occurs.
int
ACE_Proactor::run_event_loop (ACE_Time_Value &tv)
{
  ACE_TRACE ("ACE_Proactor::run_event_loop");

  while (ACE_Proactor::end_event_loop_ == 0
         && tv != ACE_Time_Value::zero)
    {
      int result = ACE_Proactor::instance ()->handle_events (tv);

      if (ACE_Service_Config::reconfig_occurred ())
	ACE_Service_Config::reconfigure ();

      // An error has occurred.
      else if (result == -1)
	return result;
    }

  return 0;
}

int
ACE_Proactor::end_event_loop (void)
{
  ACE_TRACE ("ACE_Proactor::end_event_loop");
  ACE_Proactor::end_event_loop_ = 1;
  //  ACE_Proactor::instance()->notify ();
  return 0;
}

int
ACE_Proactor::event_loop_done (void)
{
  ACE_TRACE ("ACE_Proactor::event_loop_done");
  return ACE_Proactor::end_event_loop_ != 0;
}

int
ACE_Proactor::close (void)
{
  return this->implementation ()->close ();
}

int
ACE_Proactor::register_handle (ACE_HANDLE handle,
			       const void *completion_key)
{
  return this->implementation ()->register_handle (handle,
                                                   completion_key);
}

long
ACE_Proactor::schedule_timer (ACE_Handler &handler,
			      const void *act,
			      const ACE_Time_Value &time)
{
  return this->implementation ()->schedule_timer (handler,
                                                  act,
                                                  time,
                                                  ACE_Time_Value::zero);
}

long
ACE_Proactor::schedule_repeating_timer (ACE_Handler &handler,
					const void *act,
					const ACE_Time_Value &interval)
{
  return this->implementation ()->schedule_timer (handler,
                                                  act,
                                                  interval,
                                                  interval);
}

long
ACE_Proactor::schedule_timer (ACE_Handler &handler,
			      const void *act,
			      const ACE_Time_Value &time,
			      const ACE_Time_Value &interval)
{
  return this->implementation ()->schedule_timer (handler,
                                                  act,
                                                  time,
                                                  interval);
}

int
ACE_Proactor::cancel_timer (long timer_id,
			    const void **arg,
			    int dont_call_handle_close)
{
  return this->implementation ()->cancel_timer (timer_id,
                                                arg,
                                                dont_call_handle_close);
}

int
ACE_Proactor::cancel_timer (ACE_Handler &handler,
			    int dont_call_handle_close)
{
  return this->implementation ()->cancel_timer (handler,
                                                dont_call_handle_close);
}

int
ACE_Proactor::handle_events (ACE_Time_Value &wait_time)
{
  return implementation ()->handle_events (wait_time);
}

int
ACE_Proactor::handle_events (void)
{
  return this->implementation ()->handle_events ();
}

int
ACE_Proactor::wake_up_dispatch_threads (void)
{
  return 0;
}

int
ACE_Proactor::close_dispatch_threads (int)
{
  return 0;
}

size_t
ACE_Proactor::number_of_threads (void) const
{
  return this->implementation ()->number_of_threads ();
}

void
ACE_Proactor::number_of_threads (size_t threads)
{
  this->implementation ()->number_of_threads (threads);
}

ACE_HANDLE
ACE_Proactor::get_handle (void) const
{
  return this->implementation ()->get_handle ();
}

ACE_Proactor_Impl *
ACE_Proactor::implementation (void) const
{
  return this->implementation_;
}


ACE_Asynch_Read_Stream_Impl *
ACE_Proactor::create_asynch_read_stream (void)
{
  return this->implementation ()->create_asynch_read_stream ();
}

ACE_Asynch_Write_Stream_Impl *
ACE_Proactor::create_asynch_write_stream (void)
{
  return this->implementation ()->create_asynch_write_stream ();
}

ACE_Asynch_Read_File_Impl *
ACE_Proactor::create_asynch_read_file (void)
{
  return this->implementation ()->create_asynch_read_file ();
}

ACE_Asynch_Write_File_Impl *
ACE_Proactor::create_asynch_write_file (void)
{
  return this->implementation ()->create_asynch_write_file ();
}

ACE_Asynch_Accept_Impl *
ACE_Proactor::create_asynch_accept (void)
{
  return this->implementation ()->create_asynch_accept ();
}

ACE_Asynch_Transmit_File_Impl *
ACE_Proactor::create_asynch_transmit_file (void)
{
  return this->implementation ()->create_asynch_transmit_file ();
}

ACE_Asynch_Read_Stream_Result_Impl *
ACE_Proactor::create_asynch_read_stream_result (ACE_Handler &handler,
                                                ACE_HANDLE handle,
                                                ACE_Message_Block &message_block,
                                                u_long bytes_to_read,
                                                const void* act,
                                                ACE_HANDLE event,
                                                int priority)
{
  return this->implementation ()->create_asynch_read_stream_result (handler,
                                                                    handle,
                                                                    message_block,
                                                                    bytes_to_read,
                                                                    act,
                                                                    event,
                                                                    priority);
}


ACE_Asynch_Write_Stream_Result_Impl *
ACE_Proactor::create_asynch_write_stream_result (ACE_Handler &handler,
                                                 ACE_HANDLE handle,
                                                 ACE_Message_Block &message_block,
                                                 u_long bytes_to_write,
                                                 const void* act,
                                                 ACE_HANDLE event,
                                                 int priority)

{
  return this->implementation ()->create_asynch_write_stream_result (handler,
                                                                     handle,
                                                                     message_block,
                                                                     bytes_to_write,
                                                                     act,
                                                                     event,
                                                                     priority);
}



ACE_Asynch_Read_File_Result_Impl *
ACE_Proactor::create_asynch_read_file_result (ACE_Handler &handler,
                                              ACE_HANDLE handle,
                                              ACE_Message_Block &message_block,
                                              u_long bytes_to_read,
                                              const void* act,
                                              u_long offset,
                                              u_long offset_high,
                                              ACE_HANDLE event,
                                              int priority)

{
  return this->implementation ()->create_asynch_read_file_result (handler,
                                                                  handle,
                                                                  message_block,
                                                                  bytes_to_read,
                                                                  act,
                                                                  offset,
                                                                  offset_high,
                                                                  event,
                                                                  priority);
}



ACE_Asynch_Write_File_Result_Impl *
ACE_Proactor::create_asynch_write_file_result (ACE_Handler &handler,
                                               ACE_HANDLE handle,
                                               ACE_Message_Block &message_block,
                                               u_long bytes_to_write,
                                               const void* act,
                                               u_long offset,
                                               u_long offset_high,
                                               ACE_HANDLE event,
                                               int priority)

{
  return this->implementation ()->create_asynch_write_file_result (handler,
                                                                   handle,
                                                                   message_block,
                                                                   bytes_to_write,
                                                                   act,
                                                                   offset,
                                                                   offset_high,
                                                                   event,
                                                                   priority);
}


ACE_Asynch_Accept_Result_Impl *
ACE_Proactor::create_asynch_accept_result (ACE_Handler &handler,
                                           ACE_HANDLE listen_handle,
                                           ACE_HANDLE accept_handle,
                                           ACE_Message_Block &message_block,
                                           u_long bytes_to_read,
                                           const void* act,
                                           ACE_HANDLE event,
                                           int priority)

{
  return this->implementation ()->create_asynch_accept_result (handler,
                                                               listen_handle,
                                                               accept_handle,
                                                               message_block,
                                                               bytes_to_read,
                                                               act,
                                                               event,
                                                               priority);
}

ACE_Asynch_Transmit_File_Result_Impl *
ACE_Proactor::create_asynch_transmit_file_result (ACE_Handler &handler,
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
                                                  int priority)

{
  return this->implementation ()->create_asynch_transmit_file_result (handler,
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
                                                                      priority);
}

void
ACE_Proactor::implementation (ACE_Proactor_Impl *implementation)
{
  this->implementation_ = implementation;
}

#else /* !ACE_WIN32 || !ACE_HAS_AIO_CALLS */
ACE_Proactor *
ACE_Proactor::instance (size_t threads)
{
  ACE_UNUSED_ARG (threads);
  return 0;
}

ACE_Proactor *
ACE_Proactor::instance (ACE_Proactor *)
{
  return 0;
}

void
ACE_Proactor::close_singleton (void)
{
}

int
ACE_Proactor::run_event_loop (void)
{
  // not implemented
  return -1;
}

int
ACE_Proactor::run_event_loop (ACE_Time_Value &tv)
{
  // not implemented
  ACE_UNUSED_ARG (tv);
  return -1;
}

int
ACE_Proactor::end_event_loop (void)
{
  // not implemented
  return -1;
}

sig_atomic_t
ACE_Proactor::event_loop_done (void)
{
  return sig_atomic_t (1);
}
#endif /* ACE_WIN32 || ACE_HAS_AIO_CALLS*/
