//
// $Id$
//
#include "Payload_Receiver.h"

Payload_Receiver::Payload_Receiver (void)
  : message_count_ (0)
  , maybe_lost_count_ (0)
{
}

void
Payload_Receiver::more_data (
  const Test::Payload& payload,
  CORBA::Boolean maybe_lost)
{
  if (payload.length() > 0)
    {
      if (maybe_lost)
        {
          ++this->maybe_lost_count_;
        }
      else
        {
          ++this->message_count_;
        }
    }
}

void
Payload_Receiver::ping (void)
{
}

int
Payload_Receiver::count (bool maybe_lost) const
{
  return (maybe_lost) ?
         maybe_lost_count_.value ()
       : message_count_.value ();
}
