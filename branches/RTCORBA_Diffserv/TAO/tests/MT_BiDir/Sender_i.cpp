#include "Sender_i.h"



ACE_RCSID(MT_BiDir,
          Sender_i,
          "$Id$")


Sender_i::Sender_i (int no_clients,
                    ACE_Manual_Event &event)
  : event_ (event)
  , receivers_ (0)
  , no_clients_ (no_clients)
  , last_index_ (0)
  , payload_ (32768)
{
  this->receivers_ =
    new Receiver *[no_clients * sizeof (Receiver_ptr)];
  /*ACE_NEW (this->receivers_,
    Receiver ** [no_clients * sizeof Receiver *]);*/

  // Okay to have a magic number...
  this->payload_.length (32768);

  for (CORBA::ULong j = 0; j != this->payload_.length (); ++j)
    this->payload_[j] = (j % 256);
}


CORBA::Long
Sender_i::receiver_object (Receiver_ptr recv
                           ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Sender::Table_Full))
{
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX,
                    ace_mon,
                    this->mutex_,-1);

  if (this->no_clients_ == this->last_index_)
    ACE_THROW_RETURN (Sender::Table_Full (),
                      -1);

  this->receivers_[this->last_index_] =
    Receiver::_duplicate (recv);

  ++this->last_index_;

  if (this->last_index_ == this->no_clients_)
    {
      cout << "About to signal events " << this->last_index_ << endl;
      cout << "About to signal events " << this->no_clients_ << endl;
      this->event_.signal ();
    }

  return this->last_index_;
}


void
Sender_i::send_message (ACE_ENV_SINGLE_ARG_DECL)
{
  // NOTE:No synchronization with purpose. Synchrnozing this is
  // going to spoil the whole purpose of this test.
  int send_to =
    ACE_OS::rand () % this->no_clients_;

  this->receivers_[send_to]->receive_payload (this->payload_
                                              ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}
