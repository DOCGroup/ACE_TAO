// Proactor.cpp
// $Id: Proactor.cpp,v

#define ACE_BUILD_DLL
#include "ace/Proactor.h"

#if defined (ACE_WIN32)
// This only works on Win32 platforms

#include "ace/Timer_Queue.h"
#include "ace/Log_Msg.h"
#include "ace/Asynch_IO.h"

#if !defined (__ACE_INLINE__)
#include "ace/Proactor.i"
#endif /* __ACE_INLINE__ */

ACE_Proactor::ACE_Proactor (size_t number_of_threads, 
			    ACE_Timer_Queue *tq)
  : completion_port_ (0), // This *MUST* be 0, *NOT* ACE_INVALID_HANDLE!!!!
    number_of_threads_ (number_of_threads)
{
  this->completion_port_ = ::CreateIoCompletionPort (INVALID_HANDLE_VALUE,						     
						     this->completion_port_,
						     0,
						     this->number_of_threads_);
  if (this->completion_port_ == 0)
    ACE_ERROR ((LM_ERROR, "%p\n", "CreateIoCompletionPort"));
}

ACE_Proactor::~ACE_Proactor (void)
{
  this->close ();
}

int 
ACE_Proactor::close (void)
{
  // Close the completion port
  if (this->completion_port_ != 0)
    {
      int result = ACE_OS::close (this->completion_port_);
      this->completion_port_ = 0;
      return result;
    }
  else
    return 0;
}

int 
ACE_Proactor::register_handle (ACE_HANDLE handle, 
			       const void *completion_key)
{
  // No locking is needed here as no state changes
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
	ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "CreateIoCompletionPort"), -1);
    }
  return 0;
}

int 
ACE_Proactor::handle_events (ACE_Time_Value &wait_time)
{
  return 0;
}

int 
ACE_Proactor::handle_events (void)
{
  OVERLAPPED *overlapped = 0;
  u_long bytes_transferred = 0;
  u_long completion_key = 0;

  // Get the next asynchronous operation that completes
  BOOL result = ::GetQueuedCompletionStatus (this->completion_port_,
					     &bytes_transferred,
					     &completion_key,
					     &overlapped,
					     INFINITE);


  if (result == FALSE && overlapped == 0)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "GetQueuedCompletionStatus"), -1);
  else
    {
      // Narrow result
      ACE_Asynch_Result *asynch_result = (ACE_Asynch_Result *) overlapped;
      // If errors happen, grab the error
      if (result == FALSE)
	errno = ::GetLastError ();
      
      this->application_specific_code (asynch_result,
				       bytes_transferred,
				       result,
				       (void *) completion_key,
				       errno);
    }
  return 0;
}

void
ACE_Proactor::application_specific_code (ACE_Asynch_Result *asynch_result,
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
			       errno);
    }
  ACE_SEH_FINALLY
    {
      // This is crucial to prevent memory leaks
      delete asynch_result;
    }
}

int 
ACE_Proactor::run_proactor_event_loop (void)
{
  return 0;
}

int 
ACE_Proactor::run_event_loop (ACE_Time_Value &)
{
  return 0;
}

int 
ACE_Proactor::end_event_loop (void)
{
  return 0;
}

sig_atomic_t 
ACE_Proactor::event_loop_done (void)
{
  return 0;
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
  return this->number_of_threads_;
}

void 
ACE_Proactor::number_of_threads (size_t threads)
{
  this->number_of_threads_ = threads;
}

#endif /* ACE_WIN32 */
