//
// $Id$
//
#include "Session.h"

ACE_RCSID(Big_Oneways, Session, "$Id$")

Session::Session (Test::Session_Control_ptr control)
  : control_ (Test::Session_Control::_duplicate (control))
  , running_ (0)
  , thread_count_ (0)
  , payload_size_ (0)
  , message_count_ (0)
  , active_thread_count_ (0)
  , expected_messages_ (0)
  , task_ (this)
{
}

Session::~Session (void)
{
}

int
Session::svc (void)
{
  /// Automatically decrease the reference count at the end of the
  /// thread
  PortableServer::ServantBase_var auto_decrement (this);

  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      // Use the same payload over and over
      Test::Payload payload (this->payload_size_);
      payload.length (this->payload_size_);

      // Get the number of peers just once.
      CORBA::ULong session_count =
        this->other_sessions_.length ();

      for (CORBA::ULong i = 0; i != this->message_count_; ++i)
        {
#if 0
          if (i % 500 == 0)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "(%P|%t) Session::svc, "
                          "sending message %d\n",
                          i));
            }
#endif /* 0 */
          for (CORBA::ULong j = 0; j != session_count; ++j)
            {
              this->other_sessions_[j]->receive_payload (payload,
                                                         ACE_TRY_ENV);
              ACE_TRY_CHECK;
            }
        }

      ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->mutex_, -1);
      this->active_thread_count_--;
      this->check_for_termination (ACE_TRY_ENV);
      ACE_TRY_CHECK;

    }
  ACE_CATCHANY
    {
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

void
Session::start (const Test::Session_List &other_sessions,
                CORBA::ULong payload_size,
                CORBA::ULong thread_count,
                CORBA::ULong message_count,
                CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Test::Already_Running,
                   Test::No_Peers))
{
  if (other_sessions.length () == 0)
    ACE_THROW (Test::No_Peers ());

  ACE_GUARD (ACE_SYNCH_MUTEX, ace_mon, this->mutex_);
  if (this->running_)
    ACE_THROW (Test::Already_Running ());

  this->other_sessions_ = other_sessions;
  this->payload_size_ = payload_size;
  this->thread_count_ = thread_count;
  this->message_count_ = message_count;

  this->expected_messages_ =
    (other_sessions.length ()
     * this->thread_count_
     * this->message_count_);

  for (CORBA::ULong i = 0; i != this->thread_count_; ++i)
    {
      // Increase the reference count because the new thread will have
      // access to this object....
      ACE_TRY
        {
          this->_add_ref (ACE_TRY_ENV);
          ACE_TRY_CHECK;
          if (this->task_.activate (
                          THR_NEW_LWP | THR_JOINABLE, 1, 1) == -1)
            {
              this->_remove_ref (ACE_TRY_ENV);
              ACE_TRY_CHECK;
            }
          else
            {
              this->running_ = 1;
              this->active_thread_count_++;
            }
        }
      ACE_CATCHANY {} ACE_ENDTRY;
    }

  if (this->running_ == 0)
    {
      /// None of the threads are running, this session is useless at
      /// this point, report the problem and destroy the local objects
      this->terminate (0, ACE_TRY_ENV);
      ACE_CHECK;
    }
}

void
Session::receive_payload (const Test::Payload &the_payload,
                          CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (the_payload.length () != this->payload_size_)
    {
      ACE_ERROR ((LM_ERROR,
                  "ERROR: (%P|%t) Session::receive_payload, "
                  "unexpected payload size (%d != %d)\n",
                  the_payload.length (), this->payload_size_));
    }
  ACE_GUARD (ACE_SYNCH_MUTEX, ace_mon, this->mutex_);
  this->expected_messages_--;

#if 0
  int verbose = this->expected_messages_ % 500 == 0;
  if (this->expected_messages_ < 500)
    verbose = (this->expected_messages_ % 100 == 0);
  if (this->expected_messages_ < 100)
    verbose = (this->expected_messages_ % 10 == 0);
  if (this->expected_messages_ < 10)
    verbose = 1;

  if (verbose)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) Session::receive_payload, "
                  "%d messages to go\n",
                  this->expected_messages_));
    }
#endif /* 0 */
  this->check_for_termination (ACE_TRY_ENV);
}

void
Session::check_for_termination (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC (())
{
  if (this->expected_messages_ > 0
      || this->active_thread_count_ > 0)
    return;

  this->terminate (1, ACE_TRY_ENV);
}

void
Session::terminate (CORBA::Boolean success,
                    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC (())
{
  // Make sure local resources are released
  ACE_TRY_EX(LOCAL)
    {
      PortableServer::POA_var poa =
        this->_default_POA (ACE_TRY_ENV);
      ACE_CHECK;
      PortableServer::ObjectId_var oid =
        poa->servant_to_id (this, ACE_TRY_ENV);
      ACE_CHECK;
      poa->deactivate_object (oid.in (), ACE_TRY_ENV);
      ACE_CHECK;
    }
  ACE_CATCHANY {} ACE_ENDTRY;

  // Make sure that global resources are released
  ACE_TRY_EX(GLOBAL)
    {
      this->control_->session_finished (success,
                                        ACE_TRY_ENV);
      ACE_TRY_CHECK_EX(GLOBAL);
    }
  ACE_CATCHANY {} ACE_ENDTRY;

}
