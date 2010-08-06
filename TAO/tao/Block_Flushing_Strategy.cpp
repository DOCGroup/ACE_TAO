// $Id$

#include "tao/Block_Flushing_Strategy.h"
#include "tao/Transport.h"
#include "tao/Queued_Message.h"

ACE_RCSID(tao, Block_Flushing_Strategy, "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

int
TAO_Block_Flushing_Strategy::schedule_output (TAO_Transport *)
{
  return MUST_FLUSH;
}

int
TAO_Block_Flushing_Strategy::cancel_output (TAO_Transport *)
{
  return 0;
}

int
TAO_Block_Flushing_Strategy::flush_message (TAO_Transport *transport,
                                            TAO_Queued_Message *msg,
                                            ACE_Time_Value *max_wait_time)
{
  while (!msg->all_data_sent ())
    {
      if (transport->handle_output (max_wait_time) == -1)
        return -1;
    }
  return 0;
}

int
TAO_Block_Flushing_Strategy::flush_transport (TAO_Transport *transport
                                              , ACE_Time_Value *max_wait_time)
{
  while (!transport->queue_is_empty ())
    {
      if (transport->handle_output (max_wait_time) == -1)
        return -1;
    }
  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL
