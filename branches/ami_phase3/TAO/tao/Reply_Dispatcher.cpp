// $Id$

#include "tao/Reply_Dispatcher.h"
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
TAO_Reply_Dispatcher::message_state (void) const
{
  return 0;
}

// *********************************************************************

// Constructor.
TAO_Synch_Reply_Dispatcher::TAO_Synch_Reply_Dispatcher (void)
  : message_state_ (0)
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
  this->reply_status_ = reply_status;
  this->version_ = version;
  this->message_state_ = message_state;

  // Steal the buffer, that way we don't do any unnecesary copies of
  // this data.
  CORBA::ULong max = reply_ctx.maximum ();
  CORBA::ULong len = reply_ctx.length ();
  TAO_GIOP_ServiceContext* context_list = reply_ctx.get_buffer (1);
  this->reply_ctx_.replace (max, len, context_list, 1);

  return 0;
}

TAO_GIOP_Message_State *
TAO_Synch_Reply_Dispatcher::message_state (void) const
{
  return this->message_state_;
}

// *********************************************************************

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
  this->reply_status_ = reply_status;
  this->version_ = version;
  this->message_state_ = message_state;

  // Steal the buffer, that way we don't do any unnecesary copies of
  // this data.
  CORBA::ULong max = reply_ctx.maximum ();
  CORBA::ULong len = reply_ctx.length ();
  TAO_GIOP_ServiceContext* context_list = reply_ctx.get_buffer (1);
  this->reply_ctx_.replace (max, len, context_list, 1);

  if (TAO_debug_level > 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "%N:%l:TAO_Asynch_Reply_Dispatcher::dispatch_reply:\n"));
    }
  
  ACE_DECLARE_NEW_CORBA_ENV;
  
  // Call the Reply Handler's skeleton.
  reply_handler_skel_ (message_state_->cdr,
                       reply_handler_,
                       ACE_TRY_ENV);
                                  
  return 0;
}

TAO_GIOP_Message_State *
TAO_Asynch_Reply_Dispatcher::message_state (void) const
{
  return this->message_state_;
}
