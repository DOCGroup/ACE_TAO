//
// $Id$
//
#include "Sender.h"
#include "Sender_Task.h"

ACE_RCSID(LongWrites, Sender, "$Id$")

Sender::Sender (int test_type)
  : test_type_ (test_type)
  , receiver_count_ (0)
  , receiver_length_ (16)
  , shutdown_called_ (0)
  , event_count_ (0)
{
  ACE_NEW (this->receivers_, Test::Receiver_var[this->receiver_length_]);
}

Sender::~Sender (void)
{
  delete[] this->receivers_;
}

int
Sender::test_done (CORBA::ULong message_count)
{
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->mutex_, 0);
  return
    (this->event_count_ != 0
     && this->receiver_count_ != 0
     && this->shutdown_called_ != 0
     && (4 * this->receiver_count_
           * this->event_count_ <= message_count));
}

int
Sender::shutdown_called (void)
{
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->mutex_, 0);
  return this->shutdown_called_;
}

void
Sender::add_receiver (Test::Receiver_ptr receiver,
                      CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_GUARD (ACE_SYNCH_MUTEX, ace_mon, this->mutex_);
  if (this->receiver_count_ == this->receiver_length_)
    {
      this->receiver_length_ *= 2;
      Test::Receiver_var *tmp;
      ACE_NEW (tmp, Test::Receiver_var[this->receiver_length_]);
      for (size_t i = 0; i != this->receiver_count_; ++i)
        tmp[i] = this->receivers_[i];
      delete[] this->receivers_;
      this->receivers_ = tmp;
    }
  this->receivers_[this->receiver_count_++] =
    Test::Receiver::_duplicate (receiver);
}

void
Sender::send_events (CORBA::Long event_count,
                     CORBA::ULong event_size,
                     CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  {
    ACE_GUARD (ACE_SYNCH_MUTEX, ace_mon, this->mutex_);
    this->event_count_ = event_count;
  }
  ACE_Thread_Manager thr_mgr;
  Sender_Task task (this,
                    event_count,
                    event_size,
                    &thr_mgr);

  if (task.activate (THR_NEW_LWP | THR_JOINABLE,
                     4, 1) == -1)
    return;

  ACE_TRY
    {
      int argc = 0;
      CORBA::ORB_var orb = CORBA::ORB_init (argc, 0, "", ACE_TRY_ENV);
      ACE_TRY_CHECK;

      ACE_Time_Value tv (60, 0);
      orb->run (tv, ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
    }
  ACE_ENDTRY;

  thr_mgr.wait ();
}

int
Sender::run_test (CORBA::Long event_count,
                  CORBA::ULong event_size)
{
  ACE_DECLARE_NEW_CORBA_ENV;

  Test::Payload payload(event_size); payload.length(event_size);
  for (CORBA::Long i = 0; i != event_count; ++i)
    {
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) - running iteration %d\n", i));
      for (size_t j = 0; j != this->receiver_count_; ++j)
        {
          ACE_TRY
            {
              if (this->test_type_ == Sender::TEST_ONEWAY)
                {
                  this->receivers_[j]->receive_data_oneway (payload,
                                                            ACE_TRY_ENV);
                  ACE_TRY_CHECK;
                }
              else if (this->test_type_ == Sender::TEST_WRITE)
                {
                  this->receivers_[j]->receive_data (payload,
                                                     ACE_TRY_ENV);
                  ACE_TRY_CHECK;
                }
              else
                {
                  Test::Payload_var retval =
                    this->receivers_[j]->return_data (payload,
                                                      ACE_TRY_ENV);
                  ACE_TRY_CHECK;
                }
            }
          ACE_CATCHANY
            {
              return -1;
            }
          ACE_ENDTRY;
        }
    }
  return 0;
}

void
Sender::shutdown (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_GUARD (ACE_SYNCH_MUTEX, ace_mon, this->mutex_);
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) shutting down\n"));
  this->shutdown_called_ = 1;
}
