// $Id$

#include "tao/IIOP_RMS.h"

TAO_IIOP_Request_Multiplexing_Strategy::TAO_IIOP_Request_Multiplexing_Strategy (void)
  : cdr_ (0)
{
}

TAO_IIOP_Request_Multiplexing_Strategy::~TAO_IIOP_Request_Multiplexing_Strategy (void)
{
}

TAO_InputCDR *
TAO_IIOP_Request_Multiplexing_Strategy::get_cdr_stream (void)
{
  return cdr_;
}

// *********************************************************************

TAO_IIOP_Muxed_RMS::TAO_IIOP_Muxed_RMS (void)
{
}

TAO_IIOP_Muxed_RMS::~TAO_IIOP_Muxed_RMS (void)
{
  // @@ delete ???
}

// Generate and return an unique request id for the current
// invocation.  
CORBA::ULong
TAO_IIOP_Muxed_RMS::request_id (void)
{
  // @@ 
  return 0;
}

// Bind the dispatcher with the request id.
int
TAO_IIOP_Muxed_RMS::bind_dispatcher (CORBA::ULong request_id,
                                     TAO_IIOP_Reply_Dispatcher *rh)
{
  // @@
  return 0;
}

// Find the Reply Dispatcher.
TAO_IIOP_Reply_Dispatcher*
TAO_IIOP_Muxed_RMS::find_dispatcher (CORBA::ULong request_id)
{
  // @@ 
  return 0;
}
  
void
TAO_IIOP_Muxed_RMS::set_cdr_stream (TAO_InputCDR *Cdr)
{
  // @@
}


void
TAO_IIOP_Muxed_RMS::destroy_cdr_stream (void)
{
  // @@ Implement.
  // delete cdr;
  // cdr = 0;
}

// *********************************************************************

TAO_IIOP_Exclusive_RMS::TAO_IIOP_Exclusive_RMS (void)
  : request_id_ (931232), // @@ Hardcoding.
    rd_ (0)
{
}

TAO_IIOP_Exclusive_RMS::~TAO_IIOP_Exclusive_RMS (void)
{
}

// Generate and return an unique request id for the current
// invocation. We can actually return a predecided ULong, since we
// allow only one invocation over this connection at a time.
CORBA::ULong
TAO_IIOP_Exclusive_RMS::request_id (void)
{
  return this->request_id_;
}

// Bind the handler with the request id.
int
TAO_IIOP_Exclusive_RMS::bind_dispatcher (CORBA::ULong request_id,
                                         TAO_IIOP_Reply_Dispatcher *rd)
{
  this->request_id_ = request_id;
  this->rd_ = rd;
  return 0;
}

// Find the Reply Handler.
TAO_IIOP_Reply_Dispatcher *
TAO_IIOP_Exclusive_RMS::find_dispatcher (CORBA::ULong request_id)
{
  if (this->request_id_ != request_id)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%N:%l:TAO_IIOP_Exclusive_RMS::find_handler: ",
                       "Failed to find the handler\n"),
                      0);

  return this->rd_;
}


// Set the CDR stream.
void
TAO_IIOP_Exclusive_RMS::set_cdr_stream (TAO_InputCDR *cdr)
{
  this->cdr_ = cdr;
}

// NOOP function.
void
TAO_IIOP_Exclusive_RMS::destroy_cdr_stream (void)
{
}
