// -*- C++ -*-
// $Id$

#include "Block_Flushing_Strategy.h"
#include "Transport.h"
#include "Queued_Message.h"

ACE_RCSID(tao, Block_Flushing_Strategy, "$Id$")

int
TAO_Block_Flushing_Strategy::schedule_output (TAO_Transport *)
{
  return 0;
}

int
TAO_Block_Flushing_Strategy::cancel_output (TAO_Transport *)
{
  return 0;
}

int
TAO_Block_Flushing_Strategy::flush_message (TAO_Transport *transport,
                                            TAO_Queued_Message *msg)
{
  while (!msg->all_data_sent ())
    {
      int result = transport->handle_output ();
      if (result == -1)
        return -1;
    }
  return 0;
}
