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

  int result = 0;

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      while (ch->keep_waiting ())
        {
          result =
            this->orb_core_->run (max_wait_time, 1 ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          // Did we timeout? If so, stop running the loop.
          if (result == 0 &&
              max_wait_time != 0 &&
              *max_wait_time == ACE_Time_Value::zero)
            {
              errno = ETIME;
              result = -1;
              break;
            }

          // Other errors? If so, stop running the loop.
          if (result == -1)
            break;
        }
    }
  ACE_CATCHANY
    {
      result = -1;
    }
  ACE_ENDTRY;

  // Set the result.
  if (ch->error_detected () && result != -1)
    result = -1;

  return result;
}
