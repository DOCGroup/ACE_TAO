// $Id$

#include "tao/Synch_Reply_Dispatcher.h"
#include "tao/ORB_Core.h"
#include "tao/Wait_Strategy.h"

#if !defined (__ACE_INLINE__)
#include "tao/Synch_Reply_Dispatcher.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(tao, Synch_Reply_Dispatcher, "$Id$")

// Constructor.
TAO_Synch_Reply_Dispatcher::TAO_Synch_Reply_Dispatcher (TAO_ORB_Core *orb_core,
                                                        IOP::ServiceContextList &sc)
  : reply_service_info_ (sc),
    reply_status_ (100), //Just a invalid reply status
    message_state_ (orb_core),
    reply_received_ (0),
    orb_core_ (orb_core),
    wait_strategy_ (0),
    leader_follower_condition_variable_ (0)
{
}

// Destructor.
TAO_Synch_Reply_Dispatcher::~TAO_Synch_Reply_Dispatcher (void)
{
}

TAO_InputCDR &
TAO_Synch_Reply_Dispatcher::reply_cdr (void)
{
  return this->message_state_.cdr;
}

int&
TAO_Synch_Reply_Dispatcher::reply_received (void)
{
  return this->reply_received_;
}

int
TAO_Synch_Reply_Dispatcher::dispatch_reply (CORBA::ULong reply_status,
                                            const TAO_GIOP_Version & /*version*/,
                                            IOP::ServiceContextList &reply_ctx,
                                            TAO_GIOP_Message_State *message_state)
{
  this->reply_status_ = reply_status;
  //this->version_ = version;

  // Steal the buffer, that way we don't do any unnecesary copies of
  // this data.
  CORBA::ULong max = reply_ctx.maximum ();
  CORBA::ULong len = reply_ctx.length ();
  IOP::ServiceContext* context_list = reply_ctx.get_buffer (1);
  this->reply_service_info_.replace (max, len, context_list, 1);

  // Must reset the message state, it is possible that the same reply
  // dispatcher is used because the request must be re-sent.
  this->message_state_.reset (0);

  if (&this->message_state_ != message_state)
    {
      // The Transport Mux Strategy did not use our Message_State to
      // receive the event, possibly because it is muxing multiple
      // requests over the same connection.

      // Steal the buffer so that no copying is done.
      this->message_state_.cdr.steal_from (message_state->cdr);

      // There is no need to copy the other fields!
    }

  if (this->wait_strategy_ != 0)
    {
      if (this->wait_strategy_->reply_dispatched (
                  this->reply_received_,
                  this->leader_follower_condition_variable_) == -1)
        return -1;
    }

  return 1;
}

TAO_GIOP_Message_State *
TAO_Synch_Reply_Dispatcher::message_state (void)
{
  return &this->message_state_;
}

void
TAO_Synch_Reply_Dispatcher::dispatcher_bound (TAO_Transport *transport)
{
  this->wait_strategy_ = transport->wait_strategy ();
  this->leader_follower_condition_variable_ =
    transport->wait_strategy ()->leader_follower_condition_variable ();
}

void
TAO_Synch_Reply_Dispatcher::connection_closed (void)
{
  if (this->wait_strategy_ != 0)
    {
      this->wait_strategy_->connection_closed (
                  this->reply_received_,
                  this->leader_follower_condition_variable_);
    }
}
