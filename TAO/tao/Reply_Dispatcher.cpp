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

TAO_InputCDR *
TAO_Reply_Dispatcher::cdr (void) const
{
  return 0;
}

// *********************************************************************

// Constructor.
TAO_Synch_Reply_Dispatcher::TAO_Synch_Reply_Dispatcher (TAO_InputCDR* cdr)
{
  this->cdr_ = cdr;
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
                                            TAO_InputCDR*)
{
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

TAO_InputCDR *
TAO_Synch_Reply_Dispatcher::cdr (void) const
{
  return this->cdr_;
}
