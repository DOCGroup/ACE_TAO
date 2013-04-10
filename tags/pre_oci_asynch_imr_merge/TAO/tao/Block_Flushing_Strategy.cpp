// $Id$

#include "tao/Block_Flushing_Strategy.h"
#include "tao/Transport.h"
#include "tao/Queued_Message.h"
#include "tao/Connection_Handler.h"
#include "tao/ORB_Time_Policy.h"

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
TAO_Block_Flushing_Strategy::call_handle_output (TAO_Transport *transport,
    const TAO::Transport::Drain_Constraints &dc)
{
  switch (transport->handle_output (dc).dre_)
    {
    case TAO_Transport::DR_ERROR: return -1;
    case TAO_Transport::DR_QUEUE_EMPTY: // won't happen, fall-through anyway
    case TAO_Transport::DR_OK: return 0;
    case TAO_Transport::DR_WOULDBLOCK:
      {
        TAO::ORB_Countdown_Time counter (dc.timeout ());
        TAO_Connection_Handler &ch = *transport->connection_handler ();
        if (ch.handle_write_ready (dc.timeout ()) == -1)
          {
            return -1;
          }
        return 0;
      }
    }
  return 0;
}

int
TAO_Block_Flushing_Strategy::flush_message (TAO_Transport *transport,
                                            TAO_Queued_Message *msg,
                                            ACE_Time_Value *max_wait_time)
{
  TAO::Transport::Drain_Constraints dc (max_wait_time, true);
  while (!msg->all_data_sent ())
    {
      if (this->call_handle_output (transport, dc) == -1)
        return -1;
    }
  return 0;
}

int
TAO_Block_Flushing_Strategy::flush_transport (TAO_Transport *transport,
                                              ACE_Time_Value *max_wait_time)
{
  TAO::Transport::Drain_Constraints dc (max_wait_time, true);
  while (!transport->queue_is_empty ())
    {
      if (this->call_handle_output (transport, dc) == -1)
        return -1;
    }
  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL
