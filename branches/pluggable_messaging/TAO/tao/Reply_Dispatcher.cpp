// $Id$


#include "tao/Reply_Dispatcher.h"
#include "tao/ORB_Core.h"
#include "tao/Leader_Follower.h"
#include "tao/debug.h"

#if !defined (__ACE_INLINE__)
#include "tao/Reply_Dispatcher.i"
#endif /* __ACE_INLINE__ */

// Constructor.
TAO_Reply_Dispatcher::TAO_Reply_Dispatcher (void)
{
}

// Destructor.
TAO_Reply_Dispatcher::~TAO_Reply_Dispatcher (void)
{
}

TAO_GIOP_Message_State *
TAO_Reply_Dispatcher::message_state (void)
{
  return 0;
}

int
TAO_Reply_Dispatcher::leader_follower_condition_variable (TAO_Transport *)
{
  // no-op.
  return 0;
}

// *********************************************************************

// Constructor.
TAO_Synch_Reply_Dispatcher::TAO_Synch_Reply_Dispatcher (TAO_ORB_Core *orb_core,
                                                        IOP::ServiceContextList &sc)
  : reply_service_info_ (sc),
    message_state_ (orb_core),
    reply_received_ (0),
    leader_follower_condition_variable_ (0),
    orb_core_ (orb_core)
{
}

// Destructor.
TAO_Synch_Reply_Dispatcher::~TAO_Synch_Reply_Dispatcher (void)
{
}

// Dispatch the reply.
int
TAO_Synch_Reply_Dispatcher::dispatch_reply (CORBA::ULong reply_status,
                                            const TAO_GIOP_Version &version,
                                            IOP::ServiceContextList &reply_ctx,
                                            TAO_GIOP_Message_State *message_state)
{
  this->reply_received_ = 1;

  this->reply_status_ = reply_status;
  this->version_ = version;

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

  // If condition variable is present, then we are doing leader
  // follower model. Do all the necessary things.
  if (this->leader_follower_condition_variable_ != 0)
    {
      TAO_Leader_Follower& leader_follower =
        this->orb_core_->leader_follower ();

      // We *must* remove it when we signal it so the same condition
      // is not signalled for both wake up as a follower and as the
      // next leader.
      // The follower may not be there if the reply is received while
      // the consumer is not yet waiting for it (i.e. it send the
      // request but has not blocked to receive the reply yet).
      // Ignore errors.
      (void) leader_follower.remove_follower (this->leader_follower_condition_variable_);

      (void) this->leader_follower_condition_variable_->signal ();
    }

  return 1;
}

TAO_GIOP_Message_State *
TAO_Synch_Reply_Dispatcher::message_state (void)
{
  return &this->message_state_;
}

TAO_InputCDR &
TAO_Synch_Reply_Dispatcher::reply_cdr (void)
{
  return this->message_state_.cdr;
}

int &
TAO_Synch_Reply_Dispatcher::reply_received (void)
{
  return this->reply_received_;
}

int
TAO_Synch_Reply_Dispatcher::leader_follower_condition_variable (TAO_Transport *transport)
{
  this->leader_follower_condition_variable_ =
    transport->leader_follower_condition_variable ();
  return 0;
}

// *********************************************************************

#if (TAO_HAS_CORBA_MESSAGING == 1)

#if (TAO_HAS_AMI_CALLBACK == 1) || (TAO_HAS_AMI_POLLER == 1)

// Constructor.
TAO_Asynch_Reply_Dispatcher::TAO_Asynch_Reply_Dispatcher (const TAO_Reply_Handler_Skeleton &reply_handler_skel,
                                                          Messaging::ReplyHandler_ptr reply_handler_ptr)
  : reply_handler_skel_ (reply_handler_skel),
    reply_handler_ (reply_handler_ptr),
    transport_ (0)
{
}

// Destructor.
TAO_Asynch_Reply_Dispatcher::~TAO_Asynch_Reply_Dispatcher (void)
{
  if (this->transport_ != 0)
    this->transport_->idle_after_reply ();
}

// Dispatch the reply.
int
TAO_Asynch_Reply_Dispatcher::dispatch_reply (CORBA::ULong reply_status,
                                             const TAO_GIOP_Version &version,
                                             IOP::ServiceContextList &reply_ctx,
                                             TAO_GIOP_Message_State *message_state)
{
  this->reply_status_ = reply_status;
  this->version_ = version;
  this->message_state_ = message_state;

  // Steal the buffer, that way we don't do any unnecesary copies of
  // this data.
  CORBA::ULong max = reply_ctx.maximum ();
  CORBA::ULong len = reply_ctx.length ();
  IOP::ServiceContext* context_list = reply_ctx.get_buffer (1);
  this->reply_service_info_.replace (max, len, context_list, 1);


  if (TAO_debug_level >= 4)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "(%P | %t):TAO_Asynch_Reply_Dispatcher::dispatch_reply:\n"));
    }

  CORBA::ULong reply_error = TAO_AMI_REPLY_NOT_OK;
  switch (reply_status)
    {
    case TAO_GIOP_NO_EXCEPTION:
      reply_error = TAO_AMI_REPLY_OK;
      break;
    case TAO_GIOP_USER_EXCEPTION:
      reply_error = TAO_AMI_REPLY_USER_EXCEPTION;
      break;
    case TAO_GIOP_SYSTEM_EXCEPTION:
      reply_error = TAO_AMI_REPLY_SYSTEM_EXCEPTION;
      break;
    default:
    case TAO_GIOP_LOCATION_FORWARD:
      // @@ Michael: Not even the spec mentions this case.
      //             We have to think about this case.
      // Handle the forwarding and return so the stub restarts the
      // request!
      reply_error = TAO_AMI_REPLY_NOT_OK;
      break;
    }

  ACE_TRY_NEW_ENV
    {
      // Call the Reply Handler's skeleton.
      reply_handler_skel_ (this->message_state_->cdr,
                           this->reply_handler_,
                           reply_error,
                           ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      if (TAO_debug_level >= 4)
        ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                             "Exception during reply handler");
    }
  ACE_ENDTRY;

  // This was dynamically allocated. Now the job is done. Commit
  // suicide here.
  delete this;

  return 1;
}

TAO_GIOP_Message_State *
TAO_Asynch_Reply_Dispatcher::message_state (void)
{
  return this->message_state_;
}

#endif /* TAO_HAS_AMI_CALLBACK == 1 || TAO_HAS_AMI_POLLER == 1 */

#endif /* TAO_HAS_CORBA_MESSAGING == 1 */

// *********************************************************************

#if !defined (TAO_HAS_MINIMUM_CORBA)

// Constructor.
TAO_DII_Deferred_Reply_Dispatcher::TAO_DII_Deferred_Reply_Dispatcher (const CORBA::Request_ptr req)
  : req_ (req),
    transport_ (0)
{
}

// Destructor.
TAO_DII_Deferred_Reply_Dispatcher::~TAO_DII_Deferred_Reply_Dispatcher (void)
{
  if (this->transport_ != 0)
    this->transport_->idle_after_reply ();
}

// Dispatch the reply.
int
TAO_DII_Deferred_Reply_Dispatcher::dispatch_reply (
    CORBA::ULong reply_status,
    const TAO_GIOP_Version &version,
    IOP::ServiceContextList &reply_ctx,
    TAO_GIOP_Message_State *message_state
  )
{
  this->reply_status_ = reply_status;
  this->version_ = version;
  this->message_state_ = message_state;

  // Steal the buffer, that way we don't do any unnecesary copies of
  // this data.
  CORBA::ULong max = reply_ctx.maximum ();
  CORBA::ULong len = reply_ctx.length ();
  IOP::ServiceContext* context_list = reply_ctx.get_buffer (1);
  this->reply_service_info_.replace (max, len, context_list, 1);


  if (TAO_debug_level >= 4)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ASYS_TEXT ("(%P | %t):TAO_Asynch_Reply_Dispatcher::dispatch_reply:\n")));
    }

  ACE_TRY_NEW_ENV
    {
      // Call the Request back and send the reply data.
      this->req_->handle_response (this->message_state_->cdr,
                                   reply_status,
                                   ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      if (TAO_debug_level >= 4)
        ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                             "Exception during reply handler");
    }
  ACE_ENDTRY;

  // This was dynamically allocated. Now the job is done. Commit
  // suicide here.
  delete this;

  return 1;
}

TAO_GIOP_Message_State *
TAO_DII_Deferred_Reply_Dispatcher::message_state (void)
{
  return this->message_state_;
}

#endif /* TAO_HAS_MINIMUM_CORBA */
