// $Id$

#include "tao/Reply_Dispatcher.h"
#include "tao/debug.h"

#if !defined (__ACE_INLINE__)
#include "tao/Reply_Dispatcher.i"
#endif /* __ACE_INLINE__ */

// Constructor.
TAO_Reply_Dispatcher::TAO_Reply_Dispatcher (void)
  //  : reply_received_ (0)
{
}

// Destructor.
TAO_Reply_Dispatcher::~TAO_Reply_Dispatcher (void)
{
}

TAO_GIOP_Message_State *
TAO_Reply_Dispatcher::message_state (void) const
{
  return 0;
}

// int
// TAO_Reply_Dispatcher::reply_received (void) const
// {
//   return reply_received_;
// }
 
// *********************************************************************

// Constructor.
TAO_Synch_Reply_Dispatcher::TAO_Synch_Reply_Dispatcher (void)
  : message_state_ (0),
    reply_cdr_ (0)
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
                                            TAO_GIOP_ServiceContextList &reply_ctx,
                                            TAO_GIOP_Message_State *message_state)
{
  // this->reply_received_ = 1;

  this->reply_status_ = reply_status;
  this->version_ = version;
  this->message_state_ = message_state;

  // Steal the buffer, that way we don't do any unnecesary copies of
  // this data.
  CORBA::ULong max = reply_ctx.maximum ();
  CORBA::ULong len = reply_ctx.length ();
  TAO_GIOP_ServiceContext* context_list = reply_ctx.get_buffer (1);
  this->reply_ctx_.replace (max, len, context_list, 1);

  // @@ Unnecessary copying should be avoided here (Alex).
  // @@ Carlos: I am confused about implementing this one. In the
  //    MUXED TMS, this mess state is going to get deleted, as soon as
  //    we return from here. So we need to save the CDR. But in the
  //    Exclusive case, the message state will be there. So, how do we
  //    correctly own the CDR here? (Alex).
  ACE_NEW_RETURN (this->reply_cdr_,
                  TAO_InputCDR (message_state->cdr.steal_contents ()),
                  0);
  
  return 0;
}

TAO_GIOP_Message_State *
TAO_Synch_Reply_Dispatcher::message_state (void) const
{
  return this->message_state_;
}

TAO_InputCDR &
TAO_Synch_Reply_Dispatcher::reply_cdr (void) const
{
  return *this->reply_cdr_;
}


// *********************************************************************
#if defined (TAO_HAS_CORBA_MESSAGING) && defined (TAO_POLLER)
// Constructor.
TAO_Asynch_Reply_Dispatcher::TAO_Asynch_Reply_Dispatcher (const TAO_Reply_Handler_Skeleton &reply_handler_skel,
                                                          Messaging::ReplyHandler_ptr reply_handler_ptr)
  : reply_handler_skel_ (reply_handler_skel),
    reply_handler_ (reply_handler_ptr)
{
}

// Destructor.
TAO_Asynch_Reply_Dispatcher::~TAO_Asynch_Reply_Dispatcher (void)
{
}

// Dispatch the reply.
int
TAO_Asynch_Reply_Dispatcher::dispatch_reply (CORBA::ULong reply_status,
                                             const TAO_GIOP_Version &version,
                                             TAO_GIOP_ServiceContextList &reply_ctx,
                                             TAO_GIOP_Message_State *message_state)
{
  // this->reply_received_ = 1;
  
  this->reply_status_ = reply_status;
  this->version_ = version;
  this->message_state_ = message_state;

  // Steal the buffer, that way we don't do any unnecesary copies of
  // this data.
  CORBA::ULong max = reply_ctx.maximum ();
  CORBA::ULong len = reply_ctx.length ();
  TAO_GIOP_ServiceContext* context_list = reply_ctx.get_buffer (1);
  this->reply_ctx_.replace (max, len, context_list, 1);

  if (TAO_debug_level >= 4)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "(%P | %t):TAO_Asynch_Reply_Dispatcher::dispatch_reply:\n"));
    }
  
  ACE_DECLARE_NEW_CORBA_ENV;
  
  // Call the Reply Handler's skeleton.
  reply_handler_skel_ (message_state_->cdr,
                       reply_handler_,
                       ACE_TRY_ENV);
  
  // This was dynamically allocated. Now the job is done. Commit 
  // suicide here.  
  delete this;
                                  
  return 0;
}

TAO_GIOP_Message_State *
TAO_Asynch_Reply_Dispatcher::message_state (void) const
{
  return this->message_state_;
}
#endif /* TAO_HAS_CORBA_MESSAGING && TAO_POLLER */
