#include "Reactive_Connect_Strategy.h"
#include "Connection_Handler.h"
#include "Transport.h"
#include "ORB_Core.h"
#include "debug.h"
#include "ace/Synch_Options.h"

ACE_RCSID(tao,
          Reactive_Connect_Strategy,
          "$Id$")


TAO_Reactive_Connect_Strategy::TAO_Reactive_Connect_Strategy (
    TAO_ORB_Core *orb_core)
  : TAO_Connect_Strategy (orb_core)
{
}

TAO_Reactive_Connect_Strategy::~TAO_Reactive_Connect_Strategy (void)
{
}

void
TAO_Reactive_Connect_Strategy::synch_options (ACE_Time_Value *timeout,
                                             ACE_Synch_Options &options)
{
  if (timeout != 0)
    {
      options.set (ACE_Synch_Options::USE_REACTOR,
                   *timeout);
    }
  else
    {
      // Making it sure it is blocking.
      options.set (ACE_Synch_Options::USE_REACTOR,
                   ACE_Time_Value::zero);
    }
}


int
TAO_Reactive_Connect_Strategy::wait (TAO_Connection_Handler *ch,
                                     ACE_Time_Value *max_wait_time)
{
  if (TAO_debug_level > 2)
    ACE_DEBUG ((LM_DEBUG,
                ACE_LIB_TEXT ("(%P|%t) Waiting in the Reactor for")
                ACE_LIB_TEXT (" connection completion - wait () \n")));

  TAO_Transport *transport =
    ch->transport ();

  int result = 0;

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      TAO_ORB_Core *orb_core = transport->orb_core ();

      while (!ch->is_connect_complete () &&
             result >= 0)
        {
          result = orb_core->run (max_wait_time, 1 ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      result = -1;
    }
  ACE_ENDTRY;

  // Set the result.
  if (ch->is_connect_successful () == 0 && result != -1)
    result = -1;

  return result;
}
