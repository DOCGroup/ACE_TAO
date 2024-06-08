#include "Payload_Receiver.h"

Payload_Receiver::Payload_Receiver (CORBA::ORB_ptr orb)
 : count_ (0),
   orb_(CORBA::ORB::_duplicate (orb))
{
}

void
Payload_Receiver::more_data (const Test::Payload &payload)
{
  ++this->count_;

  CORBA::ULong length = payload.length ();
  for (CORBA::ULong i = 0; i < length; ++i)
    {
      if (payload[i] != 'A')
        {
          throw Test::Payload_Receiver::Invalid_Payload ();
        }
    }
}


void
Payload_Receiver::shutdown ()
{
  this->orb_->shutdown (false);
}


int
Payload_Receiver::get_count () const
{
  return count_;
}
