// $Id$

#include "tao/Reply_Dispatcher.h"

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
TAO_Synch_Reply_Dispatcher::
    TAO_Synch_Reply_Dispatcher (TAO_GIOP_Message_State* message_state)
  : message_state_ (message_state)
{
}

// Destructor.
TAO_Synch_Reply_Dispatcher::~TAO_Synch_Reply_Dispatcher (void)
{
}

// Dispatch the reply.
int
TAO_Synch_Reply_Dispatcher::dispatch_reply (CORBA::ULong reply_status,
                                            const TAO_GIOP_Version& version,
                                            TAO_GIOP_ServiceContextList& reply_ctx,
                                            TAO_GIOP_Message_State* message_state)
{
  ACE_ASSERT (message_state == this->message_state_);
  // @@ Notice that the message is ignored because we assume that
  //    the message_state is the same we are giving down to the ORB to
  //    use.... I.E. this class cannot be used with Muxed stream, but
  //    chances are that the way to implement that will change several
  //    times in the next few weeks.
  this->reply_status_ = reply_status;
  this->version_ = version;

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
