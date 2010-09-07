// -*- C++ -*-
// $Id$

#include "tao/Reactive_Flushing_Strategy.h"
#include "tao/Transport.h"
#include "tao/ORB_Core.h"
#include "tao/Queued_Message.h"

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
  try
    {
      TAO_ORB_Core * const orb_core = transport->orb_core ();

      while (!msg->all_data_sent () && result >= 0)
        {
          result = orb_core->run (max_wait_time, 1);

          if (max_wait_time != 0) {
            if (*max_wait_time <= ACE_Time_Value::zero) {
              errno = ETIME;
              result = -1;
            }
          }
        }
    }
  catch (const ::CORBA::Exception&)
    {
      return -1;
    }

  return result;
}

int
TAO_Reactive_Flushing_Strategy::flush_transport (TAO_Transport *transport
                                                 , ACE_Time_Value *max_wait_time)
{
  try
    {
      TAO_ORB_Core * const orb_core = transport->orb_core ();

      while (!transport->queue_is_empty ())
        {
          if (orb_core->run (max_wait_time, 1) == -1)
            return -1;

          if (max_wait_time != 0) {
            if (*max_wait_time <= ACE_Time_Value::zero) {
              errno = ETIME;
              return -1;
            }
          }
        }
    }
  catch (const ::CORBA::Exception&)
    {
      return -1;
    }

  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL
