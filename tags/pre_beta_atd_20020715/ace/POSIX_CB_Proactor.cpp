/* -*- C++ -*- */
// $Id$

#include "ace/POSIX_CB_Proactor.h"

#if defined (ACE_HAS_AIO_CALLS) && defined (__sgi)

#include "ace/Task_T.h"
#include "ace/Log_Msg.h"
#include "ace/Object_Manager.h"

#if !defined (__ACE_INLINE__)
#include "ace/POSIX_CB_Proactor.i"
#endif /* __ACE_INLINE__ */

ACE_POSIX_CB_Proactor::ACE_POSIX_CB_Proactor (size_t max_aio_operations)
  : ACE_POSIX_AIOCB_Proactor (max_aio_operations, 
                              ACE_POSIX_Proactor::PROACTOR_CB),
    sema_ (0)
{
  // we should start pseudo-asynchronous accept task
  // one per all future acceptors

  this->get_asynch_pseudo_task ().start ();
}

// Destructor.
ACE_POSIX_CB_Proactor::~ACE_POSIX_CB_Proactor (void)
{
  this->close ();
}

int
ACE_POSIX_CB_Proactor::handle_events (ACE_Time_Value &wait_time)
{
  // Decrement <wait_time> with the amount of time spent in the method
  ACE_Countdown_Time countdown (&wait_time);
  return this->handle_events (wait_time.msec ());
}

int
ACE_POSIX_CB_Proactor::handle_events (void)
{
  return this->handle_events (ACE_INFINITE);
}

int
ACE_POSIX_CB_Proactor::handle_events (unsigned long milli_seconds)
{

  int result_wait=0;

  // Wait for the signals.
  if (milli_seconds == ACE_INFINITE)
    {
      result_wait = this->sema_.acquire();
    }
  else
    {
      // Wait for <milli_seconds> amount of time.
      ACE_Time_Value abs_time = ACE_OS::gettimeofday ()
                              + ACE_Time_Value ( milli_seconds);

      result_wait = this->sema_.acquire(abs_time);
    }



  // Check for errors
  // but let continue work in case of errors
  // we should check "post_completed" queue
  if ( result_wait == -1)
    {
      if (errno != ETIME &&   // timeout
          errno != EINTR )    // interrupted system call
        ACE_ERROR ((LM_ERROR,
                    "%N:%l:(%P | %t)::%p\n",
                    "ACE_POSIX_CB_Proactor::handle_events:"
                    "semaphore acquire failed"
                  ));
    }

  size_t index = 0;          // start index to scan aiocb list
  size_t count = aiocb_list_max_size_;  // max number to iterate

  int error_status = 0;
  int return_status = 0;

  int ret_aio = 0;
  int ret_que = 0;

  for (;; ret_aio++)
    {
      ACE_POSIX_Asynch_Result * asynch_result =
          find_completed_aio (error_status,
                              return_status,
                              index,
                              count);

      if (asynch_result == 0)
          break;

      // Call the application code.
      this->application_specific_code (asynch_result,
                                     return_status, // Bytes transferred.
                                     1,             // Success
                                     0,             // No completion key.
                                     error_status); // Error
     }

  // process post_completed results
  ret_que = this->process_result_queue ();

  // Uncomment this  if you want to test
  // and research the behavior of you system
  // ACE_DEBUG ((LM_DEBUG,
  //            "(%t) NumAIO=%d NumQueue=%d\n",
  //             ret_aio, ret_que));

  return ret_aio + ret_que > 0 ? 1 : 0;
}

void ACE_POSIX_CB_Proactor::aio_completion_func ( sigval_t cb_data )
{
#if defined (__FreeBSD__)
  ACE_POSIX_CB_Proactor * impl = ACE_static_cast (ACE_POSIX_CB_Proactor *, cb_data.sigval_ptr);
#else
  ACE_POSIX_CB_Proactor * impl = ACE_static_cast (ACE_POSIX_CB_Proactor *, cb_data.sival_ptr);
#endif

  if ( impl != 0 )
    impl->notify_completion (0);
}

int
ACE_POSIX_CB_Proactor::notify_completion(int  sig_num)
{
  ACE_UNUSED_ARG (sig_num);

  return this->sema_.release();
}


int
ACE_POSIX_CB_Proactor::allocate_aio_slot (ACE_POSIX_Asynch_Result *result)
{
  size_t i = 0;

  //try to find free slot as usual, starting from 0
  for (i = 0; i < this->aiocb_list_max_size_; i++)
    if (result_list_[i] == 0)
      break;

  if (i >= this->aiocb_list_max_size_)
    ACE_ERROR_RETURN ((LM_ERROR,
              "%N:%l:(%P | %t)::\n"
              "ACE_POSIX_CB_Proactor::allocate_aio_slot "
              "internal Proactor error 1\n"),
              -1);

  int retval = ACE_static_cast (int, i);

  // setup OS notification methods for this aio
  // store index!!, not pointer in signal info
  result->aio_sigevent.sigev_notify = SIGEV_CALLBACK;
  result->aio_sigevent.sigev_func   = aio_completion_func ;

#if defined (__FreeBSD__)
  result->aio_sigevent.sigev_value.sigval_ptr = this ;
#else
  result->aio_sigevent.sigev_value.sival_ptr = this ;
#endif /* __FreeBSD__ */

  return retval;
}

int
ACE_POSIX_CB_Proactor::get_result_status ( ACE_POSIX_Asynch_Result* asynch_result,
                                           int & error_status,
                                           int & return_status )
{  
  return ACE_POSIX_AIOCB_Proactor::get_result_status (asynch_result,
                                                  error_status,
                                                  return_status );
}

int
ACE_POSIX_CB_Proactor::cancel_aiocb (ACE_POSIX_Asynch_Result *result)
{
  return ACE_POSIX_AIOCB_Proactor::cancel_aiocb (result);
}

int
ACE_POSIX_CB_Proactor::cancel_aio (ACE_HANDLE handle)
{
  return ACE_POSIX_AIOCB_Proactor::cancel_aio (handle);
}

#endif /* ACE_HAS_AIO_CALLS && __sgi */
