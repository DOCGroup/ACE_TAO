//
// $Id$
//
#include "Payload_Receiver.h"

ACE_RCSID(Big_Request_Muxing, Payload_Receiver, "$Id$")

Payload_Receiver::Payload_Receiver (int expected)
  : message_count_ (0)
  , expected_(expected)
{
}

void
Payload_Receiver::more_data (const Test::Payload& payload ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (payload.length() > 0)
  {
    this->message_count_++;
  }
}

int
Payload_Receiver::count() const
{
  return message_count_;
}
