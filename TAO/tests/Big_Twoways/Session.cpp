#include "Session.h"
#include "tao/debug.h"

Session::Session (Test::Session_Control_ptr control,
                  CORBA::ULong payload_size,
                  CORBA::ULong thread_count,
                  CORBA::ULong message_count,
                  CORBA::ULong peer_count)
  : control_ (Test::Session_Control::_duplicate (control))
  , running_ (0)
  , payload_size_ (payload_size)
  , thread_count_ (thread_count)
  , message_count_ (message_count)
  , active_thread_count_ (0)
  , expected_messages_ (thread_count * message_count * (peer_count - 1))
  , task_ (this)
  , barrier_ (thread_count + 1)
{
}

Session::~Session ()
{
}

int
Session::svc ()
{
  this->barrier_.wait ();

  /// Automatically decrease the reference count at the end of the
  /// thread
  PortableServer::ServantBase_var auto_decrement (this);
  CORBA::ULong i = 0;

  try
    {
      // Use the same payload over and over
      Test::Payload payload (this->payload_size_);
      payload.length (this->payload_size_);

      // Get the number of peers just once.
      CORBA::ULong session_count =
        this->other_sessions_.length ();

      for (; i != this->message_count_; ++i)
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
              Test::Payload_var received =
                this->other_sessions_[j]->echo_payload (payload);
            }
        }

      {
        ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->mutex_, -1);
        this->active_thread_count_--;
        if (this->more_work ())
          {
            return 0;
          }
      }
      this->terminate (1);
    }
  catch (const CORBA::Exception& ex)
    {
      ACE_ERROR ((LM_ERROR,
                          "(%P|%t) ERROR: Session::svc, "
                          "send %d messages out of %d\n",
                          i, message_count_));
      ex._tao_print_exception ("Session::svc - ");
      return -1;
    }

  return 0;
}

void
Session::start (const Test::Session_List &other_sessions)
{
  if (other_sessions.length () == 0)
    throw Test::No_Peers ();

  {
    ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->mutex_);
    if (this->running_)
      throw Test::Already_Running ();

    this->other_sessions_ = other_sessions;

    for (CORBA::ULong i = 0; i != this->thread_count_; ++i)
      {
        // Increase the reference count because the new thread will have
        // access to this object....
        try
          {
            this->_add_ref ();

            if (this->task_.activate (
                    THR_NEW_LWP | THR_JOINABLE, 1, 1) == -1)
              {
                this->_remove_ref ();
              }
            else
              {
                this->running_ = 1;
                this->active_thread_count_++;
              }
          }
        catch (const CORBA::Exception& ex)
          {
            ex._tao_print_exception ("Session::start, ignored");
          }
      }

    if (this->active_thread_count_ != this->thread_count_)
      return;
  }

  this->validate_connections ();

  this->barrier_.wait ();

  if (this->running_ != 0)
    return;

  /// None of the threads are running, this session is useless at
  /// this point, report the problem and destroy the local objects
  this->terminate (0);
}

Test::Payload *
Session::echo_payload (const Test::Payload &the_payload)
{
  if (the_payload.length () != this->payload_size_)
    {
      ACE_ERROR ((LM_ERROR,
                  "ERROR: (%P|%t) Session::echo_payload, "
                  "unexpected payload size (%d != %d)\n",
                  the_payload.length (), this->payload_size_));
    }

  Test::Payload_var retval (new Test::Payload (the_payload));

  {
    ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->mutex_,
                      retval._retn ());
    this->expected_messages_--;

    int verbose = 0;
#if 0
    verbose = this->expected_messages_ % 500 == 0;
    if (this->expected_messages_ < 500)
      verbose = (this->expected_messages_ % 100 == 0);
    if (this->expected_messages_ < 100)
    verbose = (this->expected_messages_ % 10 == 0);
#endif /* 0 */
    if (this->expected_messages_ < 5)
      verbose = 1;

    if (verbose)
      {
        ACE_DEBUG ((LM_DEBUG,
                    "(%P|%t) Session::echo_payload, "
                    "%d messages to go\n",
                    this->expected_messages_));
      }
    if (this->more_work ())
      return retval._retn ();
  }
  this->terminate (1);

  return retval._retn ();
}


void
Session::destroy ()
{
  // Make sure local resources are released

  PortableServer::POA_var poa =
    this->_default_POA ();
  PortableServer::ObjectId_var oid =
    poa->servant_to_id (this);
  poa->deactivate_object (oid.in ());
}

int
Session::more_work () const
{
  if (this->expected_messages_ > 0
      || this->active_thread_count_ > 0
      || this->running_ == 0)
    return 1;

  return 0;
}

void
Session::validate_connections ()
{
  const CORBA::ULong session_count =
    this->other_sessions_.length ();
  for (CORBA::ULong j = 0; j != session_count; ++j)
    {
      try
        {
#if (TAO_HAS_CORBA_MESSAGING == 1)
          CORBA::PolicyList_var unused;
          this->other_sessions_[j]->_validate_connection (unused);
#else
          (void) this->other_sessions_[j]->_is_a ("Not_An_IDL_Type");
#endif /* TAO_HAS_MESSAGING == 1 */
        }
      catch (const CORBA::Exception&)
        {
        }
    }
}

void
Session::terminate (CORBA::Boolean success)
{
  // Make sure that global resources are released
  try
    {
      this->control_->session_finished (success);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Session::terminate, ignored");
    }

}
