// $Id$

#include "tao/Request_Mux_Strategy.h"
#include "tao/Reply_Dispatcher.h"
#include "tao/debug.h"

// @@ Alex: there is another aspect that is controlled by this
//    strategy: the demuxed version must idle() the transport
//    right after the request is sent, otherwise nobody else will be
//    able to use it.
//    The exclusive version must idle it after the reply is received,
//    to guarantee that nobody else is using it.
//    We may need to add a couple of methods to implement that.

TAO_Request_Mux_Strategy::TAO_Request_Mux_Strategy (void)
{
}

TAO_Request_Mux_Strategy::~TAO_Request_Mux_Strategy (void)
{
}

// *********************************************************************

TAO_Muxed_RMS::TAO_Muxed_RMS (void)
{
}

TAO_Muxed_RMS::~TAO_Muxed_RMS (void)
{
  // @@ delete ???
}

// Generate and return an unique request id for the current
// invocation.
CORBA::ULong
TAO_Muxed_RMS::request_id (void)
{
  // @@
  return 0;
}

// Bind the dispatcher with the request id.
int
TAO_Muxed_RMS::bind_dispatcher (CORBA::ULong request_id,
                                TAO_Reply_Dispatcher *rh)
{
  // @@
  return 0;
}

int
TAO_Muxed_RMS::dispatch_reply (CORBA::ULong request_id,
                               CORBA::ULong reply_status,
                               const TAO_GIOP_Version& version,
                               TAO_GIOP_ServiceContextList& reply_ctx,
                               TAO_InputCDR* cdr)
{
  // @@
  return -1;
}

TAO_InputCDR *
TAO_Muxed_RMS::get_cdr_stream (void)
{
  return 0;
}

void
TAO_Muxed_RMS::destroy_cdr_stream (TAO_InputCDR *)
{
  // @@ Implement.
  // delete cdr;
  // cdr = 0;
}

// *********************************************************************

TAO_Exclusive_RMS::TAO_Exclusive_RMS (void)
  : request_id_generator_ (0),
    request_id_ (0),
    rd_ (0)
{
}

TAO_Exclusive_RMS::~TAO_Exclusive_RMS (void)
{
}

// Generate and return an unique request id for the current
// invocation. We can actually return a predecided ULong, since we
// allow only one invocation over this connection at a time.
CORBA::ULong
TAO_Exclusive_RMS::request_id (void)
{
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->lock_, 0);
  return this->request_id_generator_++;
}

// Bind the handler with the request id.
int
TAO_Exclusive_RMS::bind_dispatcher (CORBA::ULong request_id,
                                    TAO_Reply_Dispatcher *rd)
{
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->lock_, -1);
  this->request_id_ = request_id;
  this->rd_ = rd;
  return 0;
}

int
TAO_Exclusive_RMS::dispatch_reply (CORBA::ULong request_id,
                                   CORBA::ULong reply_status,
                                   const TAO_GIOP_Version& version,
                                   TAO_GIOP_ServiceContextList& reply_ctx,
                                   TAO_InputCDR* cdr)
{
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->lock_, -1);
  if (this->request_id_ != request_id)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO_Exclusive_RMS::dispatch_reply - <%d != %d>\n",
                    this->request_id_, request_id));
      return -1;
    }

  TAO_Reply_Dispatcher *rd = this->rd_;
  this->request_id_ = 0xdeadbeef; // @@ What is a good value???
  this->rd_ = 0;

  return rd->dispatch_reply (reply_status,
                             version,
                             reply_ctx,
                             cdr);
}

TAO_InputCDR *
TAO_Exclusive_RMS::get_cdr_stream (void)
{
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->lock_, 0);
  if (this->rd_ == 0)
    return 0;

  return this->rd_->cdr ();
}

// NOOP function.
void
TAO_Exclusive_RMS::destroy_cdr_stream (TAO_InputCDR *)
{
}
