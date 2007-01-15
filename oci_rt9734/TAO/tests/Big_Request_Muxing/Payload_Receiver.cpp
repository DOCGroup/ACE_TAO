//
// $Id$
//
#include "Payload_Receiver.h"

ACE_RCSID(Big_Request_Muxing, Payload_Receiver, "$Id$")

Payload_Receiver::Payload_Receiver ()
  : message_count_ (0),
    sync_none_message_count_ (0)
{
}

void
Payload_Receiver::more_data (const Test::Payload& payload
			     ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (payload.length() > 0)
  {
    ++this->message_count_;
  }
}

void
Payload_Receiver::sync_none_more_data (const Test::Payload& payload
			     ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (payload.length() > 0)
  {
    ++this->sync_none_message_count_;
  }
}

int
Payload_Receiver::count(bool sync_none) const
{
  if (sync_none)
    return sync_none_message_count_.value ();
  else
    return message_count_.value ();
}
