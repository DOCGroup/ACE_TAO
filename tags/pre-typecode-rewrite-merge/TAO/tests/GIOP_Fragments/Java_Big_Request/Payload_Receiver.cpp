//
// $Id$
//
#include "Payload_Receiver.h"

ACE_RCSID(Big_Request_Muxing, Payload_Receiver, "$Id$")

Payload_Receiver::Payload_Receiver (CORBA::ORB_ptr orb)
 : count_ (0),
   orb_(CORBA::ORB::_duplicate (orb))
{
}

void
Payload_Receiver::more_data (const Test::Payload &payload
                             ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Test::Payload_Receiver::Invalid_Payload))
{
  ++this->count_;

  CORBA::ULong length = payload.length ();
  for (CORBA::ULong i = 0; i < length; ++i)
    {
      if (payload[i] != 'A')
        {
          ACE_THROW (Test::Payload_Receiver::Invalid_Payload ());
        }
    }
}


void
Payload_Receiver::shutdown (ACE_ENV_SINGLE_ARG_DECL)
   ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0 ACE_ENV_ARG_PARAMETER);
}


int
Payload_Receiver::get_count () const
{
  return count_;
}
