//
// $Id$
//
#include "Payload_Receiver.h"

ACE_RCSID(Big_Request_Muxing, Payload_Receiver, "$Id$")

Payload_Receiver::Payload_Receiver (void)
  :  message_count_ (0)
  ,  byte_count_ (0)
{
}

void
Payload_Receiver::more_data (const Test::Payload &payload
                             TAO_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->message_count_++;
  this->byte_count_ += payload.length ();
}

CORBA::Long
Payload_Receiver::get_message_count (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->message_count_;
}
