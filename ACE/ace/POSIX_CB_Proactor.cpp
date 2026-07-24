#include "ace/POSIX_CB_Proactor.h"

#if defined (ACE_HAS_AIO_CALLS) && !defined (ACE_HAS_BROKEN_SIGEVENT_STRUCT)

#include "ace/Task_T.h"
#include "ace/Log_Category.h"
#include "ace/Object_Manager.h"
#include "ace/OS_NS_sys_time.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_POSIX_CB_Proactor::Notification_State::Notification_State (ACE_SYNCH_SEMAPHORE &sema)
  : zero_pending_ (this->mutex_),
    sema_ (&sema),
    pending_callbacks_ (0),
    ref_count_ (0)
{
}

void
ACE_POSIX_CB_Proactor::Notification_State::add_pending (void)
{
  ACE_GUARD (ACE_Thread_Mutex, ace_mon, this->mutex_);
  ++this->ref_count_;
  ++this->pending_callbacks_;
}

void
ACE_POSIX_CB_Proactor::Notification_State::complete_one (void)
{
  this->finish_pending_i (true);
}

void
ACE_POSIX_CB_Proactor::Notification_State::abandon_pending (void)
{
  this->finish_pending_i (false);
}

size_t
ACE_POSIX_CB_Proactor::Notification_State::pending (void)
{
  ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->mutex_, 0);
  return this->pending_callbacks_;
}

int
ACE_POSIX_CB_Proactor::Notification_State::wait_for_pending_zero (const ACE_Time_Value *abstime)
{
  ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->mutex_, -1);

  while (this->pending_callbacks_ != 0)
    if (this->zero_pending_.wait (abstime) == -1)
      return -1;

  return 0;
}

void
ACE_POSIX_CB_Proactor::Notification_State::add_ref (void)
{
  ACE_GUARD (ACE_Thread_Mutex, ace_mon, this->mutex_);
  ++this->ref_count_;
}

void
ACE_POSIX_CB_Proactor::Notification_State::remove_ref (void)
{
  bool destroy = false;
  {
    ACE_GUARD (ACE_Thread_Mutex, ace_mon, this->mutex_);
    if (this->ref_count_ != 0 && --this->ref_count_ == 0)
      destroy = true;
  }

  if (destroy)
    delete this;
}

void
ACE_POSIX_CB_Proactor::Notification_State::detach (void)
{
  ACE_GUARD (ACE_Thread_Mutex, ace_mon, this->mutex_);
  this->sema_ = 0;
}

void
ACE_POSIX_CB_Proactor::Notification_State::finish_pending_i (bool signal_waiter)
{
  bool destroy = false;
  {
    ACE_GUARD (ACE_Thread_Mutex, ace_mon, this->mutex_);

    if (signal_waiter && this->sema_ != 0)
      this->sema_->release ();

    if (this->pending_callbacks_ != 0)
      {
        --this->pending_callbacks_;
        if (this->pending_callbacks_ == 0)
          this->zero_pending_.broadcast ();
      }

    if (this->ref_count_ != 0 && --this->ref_count_ == 0)
      destroy = true;
  }

  if (destroy)
    delete this;
}

ACE_POSIX_CB_Proactor::ACE_POSIX_CB_Proactor (size_t max_aio_operations)
  : ACE_POSIX_AIOCB_Proactor (max_aio_operations,
                              ACE_POSIX_Proactor::PROACTOR_CB),
    sema_ ((unsigned int) 0),
    notification_state_ (0)
{
  ACE_NEW (this->notification_state_, Notification_State (this->sema_));
  this->notification_state_->add_ref ();

  // we should start pseudo-asynchronous accept task
  // one per all future acceptors

  this->get_asynch_pseudo_task ().start ();
}

// Destructor.
ACE_POSIX_CB_Proactor::~ACE_POSIX_CB_Proactor (void)
{
  this->close ();
}

ACE_POSIX_Proactor::Proactor_Type
ACE_POSIX_CB_Proactor::get_impl_type (void)
{
  return PROACTOR_CB;
}

void ACE_POSIX_CB_Proactor::aio_completion_func (sigval cb_data)
{
  Notification_State *state =
    static_cast<Notification_State *> (cb_data.sival_ptr);
  if (state != 0)
    state->complete_one ();
}

#if defined (ACE_HAS_SIG_C_FUNC)
extern "C" void
ACE_POSIX_CB_Proactor_aio_completion (sigval cb_data)
{
  ACE_POSIX_CB_Proactor::aio_completion_func (cb_data);
}
#endif /* ACE_HAS_SIG_C_FUNC */

int
ACE_POSIX_CB_Proactor::close (void)
{
  int const result = ACE_POSIX_AIOCB_Proactor::close ();

  Notification_State *state = this->notification_state_;
  if (state != 0)
    {
      this->notification_state_ = 0;

      ACE_Time_Value const settle_timeout (0, 500000);
      ACE_Time_Value const deadline =
        ACE_OS::gettimeofday () + settle_timeout;
      (void) state->wait_for_pending_zero (&deadline);
      state->detach ();
      state->remove_ref ();
    }

  return result;
}

int
ACE_POSIX_CB_Proactor::handle_events (ACE_Time_Value &wait_time)
{
  // Decrement <wait_time> with the amount of time spent in the method
  ACE_Countdown_Time countdown (&wait_time);
  return this->handle_events_i (wait_time.msec ());
}

int
ACE_POSIX_CB_Proactor::handle_events (void)
{
  return this->handle_events_i (ACE_INFINITE);
}

int
ACE_POSIX_CB_Proactor::notify_completion (int /* sig_num */)
{
  return this->sema_.release();
}

int
ACE_POSIX_CB_Proactor::post_completion (ACE_POSIX_Asynch_Result *result)
{
  ACE_MT (ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->mutex_, -1));

  int const rc = this->putq_result (result);
  if (rc == 0)
    this->sema_.release ();
  return rc;
}

void
ACE_POSIX_CB_Proactor::abandon_pending_aio (void)
{
  if (this->notification_state_ != 0)
    this->notification_state_->abandon_pending ();
}


ssize_t
ACE_POSIX_CB_Proactor::allocate_aio_slot (ACE_POSIX_Asynch_Result *result)
{
  ssize_t slot = ACE_POSIX_AIOCB_Proactor::allocate_aio_slot (result);
  if (slot == -1)
    return -1;

  // setup OS notification methods for this aio
  // @@ TODO: This gets the completion method back to this proactor to
  // find the completed aiocb. It would be so much better to not only get
  // the proactor, but the aiocb as well.
  result->aio_sigevent.sigev_notify = SIGEV_THREAD;
#  if defined (ACE_HAS_SIG_C_FUNC)
  result->aio_sigevent.sigev_notify_function =
    ACE_POSIX_CB_Proactor_aio_completion;
#  else
  result->aio_sigevent.sigev_notify_function = aio_completion_func;
#  endif /* ACE_HAS_SIG_C_FUNC */
  result->aio_sigevent.sigev_notify_attributes = 0;

  result->aio_sigevent.sigev_value.sival_ptr = this->notification_state_;

  return slot;
}

int
ACE_POSIX_CB_Proactor::handle_events_i (u_long milli_seconds)
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
                              + ACE_Time_Value (0, milli_seconds * 1000);

      result_wait = this->sema_.acquire(abs_time);
    }

  // Check for errors
  // but let continue work in case of errors
  // we should check "post_completed" queue
  if (result_wait == -1)
    {
      int const lerror = errno;
      if (lerror != ETIME &&   // timeout
          lerror != EINTR )    // interrupted system call
        ACELIB_ERROR ((LM_ERROR,
                    ACE_TEXT("%N:%l:(%P | %t)::%p\n"),
                    ACE_TEXT("ACE_POSIX_CB_Proactor::handle_events:")
                    ACE_TEXT("semaphore acquire failed")
                  ));
    }

  size_t index = 0;          // start index to scan aiocb list
  size_t count = this->aiocb_list_max_size_;  // max number to iterate

  int error_status = 0;
  size_t return_status = 0;

  int ret_aio = 0;
  int ret_que = 0;

  for (;; ret_aio++)
    {
      ACE_POSIX_Asynch_Result * asynch_result =
          this->find_completed_aio (error_status,
                                    return_status,
                                    index,
                                    count);

      if (asynch_result == 0)
          break;

      // Call the application code.
      this->application_specific_code (asynch_result,
                                       return_status, // Bytes transferred.
                                       0,             // No completion key.
                                       error_status); // Error
     }

  // process post_completed results
  ret_que = this->process_result_queue ();


  return ret_aio + ret_que > 0 ? 1 : 0;
}

int
ACE_POSIX_CB_Proactor::start_aio (ACE_POSIX_Asynch_Result *result,
                                  ACE_POSIX_Proactor::Opcode op)
{
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->mutex_, -1));

  int ret_val = (aiocb_list_cur_size_ >= aiocb_list_max_size_) ? -1 : 0;

  if (result == 0)
    return -1;

  switch (op)
    {
    case ACE_POSIX_Proactor::ACE_OPCODE_READ:
      result->aio_lio_opcode = LIO_READ;
      break;

    case ACE_POSIX_Proactor::ACE_OPCODE_WRITE:
      result->aio_lio_opcode = LIO_WRITE;
      break;

    default:
      ACELIB_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("%N:%l:(%P|%t)::")
                         ACE_TEXT ("start_aio: Invalid op code %d\n"),
                         op),
                        -1);
    }

  if (ret_val != 0)
    {
      errno = EAGAIN;
      return -1;
    }

  ssize_t const slot = this->allocate_aio_slot (result);
  if (slot < 0)
    return -1;

  size_t const index = static_cast<size_t> (slot);
  this->result_list_[index] = result;
  ++this->aiocb_list_cur_size_;

  if (this->notification_state_ != 0)
    this->notification_state_->add_pending ();

  ret_val = this->start_aio_i (result);
  switch (ret_val)
    {
    case 0:
      this->aiocb_list_[index] = result;
      return 0;

    case 1:
      ++this->num_deferred_aiocb_;
      return 0;

    default:
      this->abandon_pending_aio ();
      break;
    }

  this->result_list_[index] = 0;
  --this->aiocb_list_cur_size_;
  return -1;
}

ACE_END_VERSIONED_NAMESPACE_DECL

#endif /* ACE_HAS_AIO_CALLS && !ACE_HAS_BROKEN_SIGEVENT_STRUCT */
