// $Id$

#define ACE_BUILD_DLL
#include "ace/SUN_Proactor.h"

#if defined (ACE_HAS_AIO_CALLS) && defined (sun)

#include "ace/Task_T.h"
#include "ace/Log_Msg.h"
#include "ace/Object_Manager.h"

#if !defined (__ACE_INLINE__)
#include "ace/SUN_Proactor.i"
#endif /* __ACE_INLINE__ */

ACE_SUN_Proactor::ACE_SUN_Proactor (size_t max_aio_operations)
  : ACE_POSIX_AIOCB_Proactor (max_aio_operations , 0)
{
  // To provide correct virtual calls.
  create_notify_manager ();
}

// Destructor.
ACE_SUN_Proactor::~ACE_SUN_Proactor (void)
{
  // to provide correct virtual calls
  delete_notify_manager ();
}

int
ACE_SUN_Proactor::handle_events (ACE_Time_Value &wait_time)
{
  return this->handle_events (wait_time.msec ());
}

int
ACE_SUN_Proactor::handle_events (void)
{
  return this->handle_events (ACE_INFINITE);
}

int
ACE_SUN_Proactor::handle_events (u_long milli_seconds)
{
  aio_result_t *result = 0;

  if (milli_seconds == ACE_INFINITE)
    result = aiowait (0);
  else
    {
      struct timeval timeout;
      timeout.tv_sec = milli_seconds / 1000;
      timeout.tv_usec = (milli_seconds - (timeout.tv_sec * 1000)) * 1000;
      result = aiowait (&timeout);
    }

  if (ACE_reinterpret_cast (long, result) == -1)
    // Check errno  for  EINVAL,EAGAIN,EINTR ??
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%N:%l:(%P | %t)::%p\n",
                       "ACE_SUN_Proactor::handle_events:"
                       "aiowait failed"),
                      0);
   
  if (ACE_reinterpret_cast (long, result) == -1)
    return 0; // timeout

  int error_status = 0;
  int return_status = 0;

  ACE_POSIX_Asynch_Result *asynch_result =
    find_completed_aio (result,
                        error_status,
                        return_status);

  if (asynch_result == 0)
    return 0;

  // Call the application code.
  this->application_specific_code (asynch_result,
                                   return_status, // Bytes transferred.
                                   1,             // Success
                                   0,             // No completion key.
                                   error_status); // Error
  return 1;    
}

ACE_POSIX_Asynch_Result *
ACE_SUN_Proactor::find_completed_aio (aio_result_t *result,
                                       int &error_status,
                                       int &return_status)
{
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, mutex_, 0));

  size_t ai;
  error_status = -1;
  return_status= 0;

  //  we call find_completed_aio always with result != 0
	
  for (ai = 0; ai < aiocb_list_max_size_; ai++)
    if (result == &aiocb_list_[ai]->aio_resultp)
      break;

  if (ai >= aiocb_list_max_size_)   // not found 
    return 0;

  error_status = result->aio_errno;
  return_status= result->aio_return;	

  if (error_status == -1)   // should never be
    {
      ACE_ERROR ((LM_ERROR,
                  "%N:%l:(%P | %t)::%p\n",
                  "ACE_SUN_Proactor::find_completed_aio:"
                  "<aio_errno> has failed\n"));

      aiocb_list_[ai] = 0;
      result_list_[ai] = 0;
      aiocb_list_cur_size_--;
			
      return 0;
    }

  if (error_status == EINPROGRESS) // should never be
    return 0;   

  if (error_status == ECANCELED)
    return_status = 0;

  if (return_status == -1) 
    {
      // was ACE_ERROR_RETURN
      ACE_ERROR ((LM_ERROR,   
                  "%N:%l:(%P | %t)::%p\n",
                  "ACE_SUN_Proactor::find_completed_aio:"
                  "<aio_return> failed\n"));
      return_status = 0; // zero bytes transferred
    }

  ACE_POSIX_Asynch_Result *asynch_result = result_list_[ai];

  aiocb_list_[ai] = 0;
  result_list_[ai] = 0;
  aiocb_list_cur_size_--;

  return asynch_result;
}

int
ACE_SUN_Proactor::start_aio (ACE_POSIX_Asynch_Result *result,
                             int op)
{
  ACE_TRACE ("ACE_SUN_Proactor::start_aio");

  int ret_val;
  const ACE_TCHAR *ptype;

  // Start IO

  switch (op)
    {
    case 0 : 
      ptype = "read";
      ret_val = aioread (result->aio_fildes,
                        (char *) result->aio_buf,
                        result->aio_nbytes,
                        result->aio_offset,
                        SEEK_SET,
                        &result->aio_resultp);
      break;

    case 1 :
      ptype = "write"; 
      ret_val = aiowrite (result->aio_fildes,
                         (char *) result->aio_buf,
                         result->aio_nbytes,
                         result->aio_offset,
                         SEEK_SET,
                         &result->aio_resultp);
      break;

    default:
      ptype = "?????"; 
      ret_val = -1;
      break;
    }
	
  if (ret_val == -1)
    ACE_ERROR ((LM_ERROR,
                "%N:%l:(%P | %t)::start_aio: aio%s %p\n",
                ptype,
                "queueing failed\n"));
  return ret_val;
}

#endif /* ACE_HAS_AIO_CALLS && sun */

