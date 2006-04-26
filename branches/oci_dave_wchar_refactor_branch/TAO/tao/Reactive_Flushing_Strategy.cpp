// -*- C++ -*-
// $Id$

#include "tao/Reactive_Flushing_Strategy.h"
#include "tao/Transport.h"
#include "tao/ORB_Core.h"
#include "tao/Queued_Message.h"

ACE_RCSID (tao,
           Reactive_Flushing_Strategy,
           "$Id$")


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

int
TAO_Reactive_Flushing_Strategy::schedule_output (TAO_Transport *transport)
{
  return transport->schedule_output_i ();
}

int
TAO_Reactive_Flushing_Strategy::cancel_output (TAO_Transport *transport)
{
  return transport->cancel_output_i ();
}

int
TAO_Reactive_Flushing_Strategy::flush_message (TAO_Transport *transport,
                                               TAO_Queued_Message *msg,
                                               ACE_Time_Value *max_wait_time)
{
  int result = 0;

  // @@ Should we pass this down?  Can we?
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      TAO_ORB_Core * const orb_core = transport->orb_core ();

      while (!msg->all_data_sent () && result >= 0)
        {
          result = orb_core->run (max_wait_time, 1 ACE_ENV_ARG_PARAMETER);
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

int
TAO_Reactive_Flushing_Strategy::flush_transport (TAO_Transport *transport)
{
  // @@ Should we pass this down?  Can we?
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      TAO_ORB_Core * const orb_core = transport->orb_core ();

      while (!transport->queue_is_empty ())
        {
          int const result = orb_core->run (0, 1 ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          if (result == -1)
            return -1;
        }
    }
  ACE_CATCHANY
    {
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL
