// -*- C++ -*-
// $Id$

#include "Reactive_Flushing_Strategy.h"
#include "Transport.h"
#include "ORB_Core.h"
#include "Queued_Message.h"
#include "debug.h"

ACE_RCSID(tao, Reactive_Flushing_Strategy, "$Id$")

int
TAO_Reactive_Flushing_Strategy::schedule_output (TAO_Transport *transport)
{
  transport->schedule_output ();
}

int
TAO_Reactive_Flushing_Strategy::cancel_output (TAO_Transport *transport)
{
  transport->cancel_output ();
}

int
TAO_Reactive_Flushing_Strategy::flush_message (TAO_Transport *transport,
                                               TAO_Queued_Message *msg)
{
  TAO_ORB_Core *orb_core = transport->orb_core ();

  int result = 0;
  // @@ Should we pass this down?  Can we?
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      while (!msg->done () && result > 0)
        {
          result = orb_core->run (0, 1, ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      return -1;
    }
  ACE_ENDTRY;

  return result;
}
