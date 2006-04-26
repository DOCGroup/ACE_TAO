// $Id$

#include "tao/Exclusive_TMS.h"
#include "tao/Reply_Dispatcher.h"
#include "tao/debug.h"
#include "tao/Transport.h"

ACE_RCSID (tao,
           Exclusive_TMS,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Exclusive_TMS::TAO_Exclusive_TMS (TAO_Transport *transport)
  : TAO_Transport_Mux_Strategy (transport),
    request_id_generator_ (0),
    has_request_ (0),
    request_id_ (0),
    rd_ (0)
{
}

TAO_Exclusive_TMS::~TAO_Exclusive_TMS (void)
{
}

// Generate and return an unique request id for the current
// invocation. We can actually return a predecided ULong, since we
// allow only one invocation over this connection at a time.
CORBA::ULong
TAO_Exclusive_TMS::request_id (void)
{
  ++this->request_id_generator_;

  // if TAO_Transport::bidirectional_flag_
  //  ==  1 --> originating side
  //  ==  0 --> other side
  //  == -1 --> no bi-directional connection was negotiated
  // The originating side must have an even request ID, and the other
  // side must have an odd request ID.  Make sure that is the case.
  int bidir_flag =
    this->transport_->bidirectional_flag ();

  if ((bidir_flag == 1 && ACE_ODD (this->request_id_generator_))
       || (bidir_flag == 0 && ACE_EVEN (this->request_id_generator_)))
    ++this->request_id_generator_;

  if (TAO_debug_level > 4)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("TAO (%P|%t) - Exclusive_TMS::request_id - <%d>\n"),
                this->request_id_generator_));

  return this->request_id_generator_;
}

// Bind the handler with the request id.
int
TAO_Exclusive_TMS::bind_dispatcher (CORBA::ULong request_id,
                                    TAO_Reply_Dispatcher *rd)
{
  this->has_request_ = 1;
  this->request_id_ = request_id;
  this->rd_ = rd;

  return 0;
}

int
TAO_Exclusive_TMS::unbind_dispatcher (CORBA::ULong request_id)
{
  if (!this->has_request_ || this->request_id_ != request_id)
    return -1;
  this->has_request_ = 0;
  this->request_id_ = 0;
  this->rd_ = 0;

  return 0;
}

int
TAO_Exclusive_TMS::dispatch_reply (TAO_Pluggable_Reply_Params &params)
{
  // Check the ids.
  if (!this->has_request_ || this->request_id_ != params.request_id_)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%P|%t) TAO_Exclusive_TMS::dispatch_reply - <%d != %d>\n"),
                    this->request_id_, params.request_id_));

      // The return value 0 informs the transport that the mux strategy
      // did not find the right reply handler.
      return 0;
    }

  TAO_Reply_Dispatcher *rd = this->rd_;
  this->has_request_ = 0;
  this->request_id_ = 0; // @@ What is a good value???
  this->rd_ = 0;

  // Dispatch the reply.
  // Returns 1 on success, -1 on failure.
  return rd->dispatch_reply (params);
}

bool
TAO_Exclusive_TMS::idle_after_send (void)
{
  return false;
}

bool
TAO_Exclusive_TMS::idle_after_reply (void)
{
  // Irrespective of whether we are successful or not we need to
  // return true. If *this* class is not successfull in idling the
  // transport no one can.
  if (this->transport_ != 0)
    (void) this->transport_->make_idle ();

  return true;
}

void
TAO_Exclusive_TMS::connection_closed (void)
{
  if (this->rd_ != 0)
    this->rd_->connection_closed ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
