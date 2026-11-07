#include "Sender_i.h"
#include "ace/Manual_Event.h"
#include <new>

Sender_i::Sender_i (CORBA::ULong no_clients,
                    ACE_Manual_Event &event)
  : event_ (event)
  , no_clients_ (no_clients)
  , payload_ (32768)
{
  this->receivers_.reset (new (std::nothrow) Receiver_var[no_clients]);
  if (!this->receivers_)
    throw CORBA::NO_MEMORY ();

  // Okay to have a magic number...
  this->payload_.length (32768);

  for (CORBA::ULong j {}; j != this->payload_.length (); ++j)
    this->payload_[j] = (j % 256);
}

CORBA::Long
Sender_i::receiver_object (Receiver_ptr recv)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                    ace_mon,
                    this->mutex_, -1);

  if (this->no_clients_ == this->last_index_)
    throw Sender::Table_Full ();

  this->receivers_[this->last_index_] =
    Receiver::_duplicate (recv);

  ++this->last_index_;

  if (this->last_index_ == this->no_clients_)
    {
      this->event_.signal ();
    }

  return this->last_index_;
}


void
Sender_i::send_message ()
{
  // NOTE:No synchronization with purpose. Synchrnozing this is
  // going to spoil the whole purpose of this test.
  CORBA::ULong const send_to =
    static_cast<CORBA::ULong> (ACE_OS::rand () % this->no_clients_);

  this->receivers_[send_to]->receive_payload (this->payload_);
}
