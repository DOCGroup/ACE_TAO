// $Id$

#include "tao/Reply_Dispatcher.h"

// Constructor.
TAO_Reply_Dispatcher::TAO_Reply_Dispatcher (void)
  : request_id_ (0),
    cdr_ (0)
{
}

// Destructor.
TAO_Reply_Dispatcher::~TAO_Reply_Dispatcher (void)
{
}

void
TAO_Reply_Dispatcher::request_id (CORBA::ULong request_id)
{
  this->request_id_ = request_id;
}

CORBA::ULong
TAO_Reply_Dispatcher::request_id (void) const
{
  return this->request_id_;
}

void
TAO_Reply_Dispatcher::reply_status (CORBA::ULong reply_status)
{
  this->reply_status_ = reply_status;
}

// Get the reply status.
CORBA::ULong
TAO_Reply_Dispatcher::reply_status (void) const
{
  return this->reply_status_;
}

// Set the CDR which the has the reply message.
void
TAO_Reply_Dispatcher::cdr (TAO_InputCDR *cdr)
{
  this->cdr_ = cdr;
}

// Get the CDR stream.
TAO_InputCDR *
TAO_Reply_Dispatcher::cdr (void) const
{
  return this->cdr_;
}

// *********************************************************************

// Constructor.
TAO_Synch_Reply_Dispatcher::TAO_Synch_Reply_Dispatcher (void)
{
}

// Destructor.
TAO_Synch_Reply_Dispatcher::~TAO_Synch_Reply_Dispatcher (void)
{
}

// Dispatch the reply using the Stub object.
int
TAO_Synch_Reply_Dispatcher::dispatch_reply (void)
{
  // NO OP.
  return 0;
}
