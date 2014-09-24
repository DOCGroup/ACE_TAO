// $Id$

#include "tao/Muxed_TMS.h"
#include "tao/Reply_Dispatcher.h"
#include "tao/debug.h"
#include "tao/Transport.h"
#include "tao/ORB_Core.h"
#include "tao/Client_Strategy_Factory.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Muxed_TMS::TAO_Muxed_TMS (TAO_Transport *transport)
  : TAO_Transport_Mux_Strategy (transport)
    , lock_ (0)
    , request_id_generator_ (0)
    , orb_core_ (transport->orb_core ())
    , dispatcher_table_ (this->orb_core_->client_factory ()->reply_dispatcher_table_size ())
{
  this->lock_ =
    this->orb_core_->client_factory ()->create_transport_mux_strategy_lock ();
}

TAO_Muxed_TMS::~TAO_Muxed_TMS (void)
{
  delete this->lock_;
}

// Generate and return an unique request id for the current
// invocation.
CORBA::ULong
TAO_Muxed_TMS::request_id (void)
{
  // @@ What is a good error return value?
  ACE_GUARD_RETURN (ACE_Lock,
                    ace_mon,
                    *this->lock_,
                    0);

  ++this->request_id_generator_;

  // if TAO_Transport::bidirectional_flag_
  //  ==  1 --> originating side
  //  ==  0 --> other side
  //  == -1 --> no bi-directional connection was negotiated
  // The originating side must have an even request ID, and the other
  // side must have an odd request ID.  Make sure that is the case.
  int const bidir_flag = this->transport_->bidirectional_flag ();

  if ((bidir_flag == 1 && ACE_ODD (this->request_id_generator_))
       || (bidir_flag == 0 && ACE_EVEN (this->request_id_generator_)))
    ++this->request_id_generator_;

  if (TAO_debug_level > 4)
    TAOLIB_DEBUG ((LM_DEBUG,
                "TAO (%P|%t) - Muxed_TMS[%d]::request_id, <%d>\n",
                this->transport_->id (),
                this->request_id_generator_));

  return this->request_id_generator_;
}

/// Bind the dispatcher with the request id.
int
TAO_Muxed_TMS::bind_dispatcher (CORBA::ULong request_id,
                                ACE_Intrusive_Auto_Ptr<TAO_Reply_Dispatcher> rd)
{
  ACE_GUARD_RETURN (ACE_Lock,
                    ace_mon,
                    *this->lock_,
                    -1);

  if (rd == 0)
    {
      if (TAO_debug_level > 0)
        {
          TAOLIB_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) - TAO_Muxed_TMS::bind_dispatcher, ")
                      ACE_TEXT ("null reply dispatcher\n")));
        }
      return 0;
    }

  int const result = this->dispatcher_table_.bind (request_id, rd);

  if (result != 0)
    {
      if (TAO_debug_level > 0)
        TAOLIB_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO (%P|%t) - TAO_Muxed_TMS::bind_dispatcher, ")
                    ACE_TEXT ("bind dispatcher failed: result = %d, request id = %d\n"),
                    result, request_id));

      return -1;
    }

  return 0;
}

int
TAO_Muxed_TMS::unbind_dispatcher (CORBA::ULong request_id)
{
  ACE_GUARD_RETURN (ACE_Lock,
                    ace_mon,
                    *this->lock_,
                    -1);

  return this->dispatcher_table_.unbind (request_id);
}

bool
TAO_Muxed_TMS::has_request (void)
{
  ACE_GUARD_RETURN (ACE_Lock,
                    ace_mon,
                    *this->lock_,
                    false);

  return this->dispatcher_table_.current_size () > 0;
}

int
TAO_Muxed_TMS::dispatch_reply (TAO_Pluggable_Reply_Params &params)
{
  int result = 0;
  ACE_Intrusive_Auto_Ptr<TAO_Reply_Dispatcher> rd(0);

  // Grab the reply dispatcher for this id.
  {
    ACE_GUARD_RETURN (ACE_Lock,
                      ace_mon,
                      *this->lock_,
                      -1);
    result = this->dispatcher_table_.unbind (params.request_id_, rd);
  }

    if (result == 0 && rd)
      {
        if (TAO_debug_level > 8)
          TAOLIB_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) - TAO_Muxed_TMS::dispatch_reply, ")
                      ACE_TEXT ("id = %d\n"),
                      params.request_id_));

        // Dispatch the reply.
        // They return 1 on success, and -1 on failure.
        result = rd->dispatch_reply (params);
      }
    else
      {
        if (TAO_debug_level > 0)
          TAOLIB_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) - TAO_Muxed_TMS::dispatch_reply, ")
                      ACE_TEXT ("unbind dispatcher failed, id %d: result = %d\n"),
                      params.request_id_,
                      result));

      // Result = 0 means that the mux strategy was not able
      // to find a registered reply handler, either because the reply
      // was not our reply - just forget about it - or it was ours, but
      // the reply timed out - just forget about the reply.
      result = 0;
    }


  return result;
}

int
TAO_Muxed_TMS::reply_timed_out (CORBA::ULong request_id)
{
  int result = 0;
  ACE_Intrusive_Auto_Ptr<TAO_Reply_Dispatcher> rd(0);

  // Grab the reply dispatcher for this id.
  {
    ACE_GUARD_RETURN (ACE_Lock,
                      ace_mon,
                      *this->lock_,
                      -1);

    result = this->dispatcher_table_.unbind (request_id, rd);
  }

  if (result == 0 && rd)
    {
      if (TAO_debug_level > 8)
        {
          TAOLIB_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) - TAO_Muxed_TMS::reply_timed_out, ")
                      ACE_TEXT ("id = %d\n"),
                      request_id));
        }

      // Do not move it outside the scope of the lock. A follower thread
      // could have timedout unwinding the stack and the reply
      // dispatcher, and that would mean the present thread could be left
      // with a dangling pointer and may crash. To safeguard against such
      // cases we dispatch with the lock held.
      // Dispatch the reply.
      rd->reply_timed_out ();
    }
  else
    {
      if (TAO_debug_level > 0)
        TAOLIB_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO (%P|%t) - TAO_Muxed_TMS::reply_timed_out, ")
                    ACE_TEXT ("unbind dispatcher failed, id %d: result = %d\n"),
                    request_id,
                    result));

      // Result = 0 means that the mux strategy was not able
      // to find a registered reply handler, either because the reply
      // was not our reply - just forget about it - or it was ours, but
      // the reply timed out - just forget about the reply.
      result = 0;
    }

  return result;
}


bool
TAO_Muxed_TMS::idle_after_send (void)
{
  // Irrespective of whether we are successful or not we need to
  // return true. If *this* class is not successful in idling the
  // transport no one can.
  if (this->transport_ != 0)
    (void) this->transport_->make_idle ();

  return true;
}

bool
TAO_Muxed_TMS::idle_after_reply (void)
{
  return false;
}

void
TAO_Muxed_TMS::connection_closed (void)
{
  ACE_GUARD (ACE_Lock,
             ace_mon,
             *this->lock_);

  int retval = 0;
  do
    {
      retval = this->clear_cache_i ();
    }
  while (retval != -1);
}

int
TAO_Muxed_TMS::clear_cache_i (void)
{
  if (this->dispatcher_table_.current_size () == 0)
    return -1;

  REQUEST_DISPATCHER_TABLE::ITERATOR const end =
    this->dispatcher_table_.end ();

  ACE_Unbounded_Stack <ACE_Intrusive_Auto_Ptr<TAO_Reply_Dispatcher> > ubs;

  for (REQUEST_DISPATCHER_TABLE::ITERATOR i =
         this->dispatcher_table_.begin ();
       i != end;
       ++i)
    {
      ubs.push ((*i).int_id_);
    }

  this->dispatcher_table_.unbind_all ();
  size_t const sz = ubs.size ();

  for (size_t k = 0 ; k != sz ; ++k)
    {
      ACE_Intrusive_Auto_Ptr<TAO_Reply_Dispatcher> rd(0);

      if (ubs.pop (rd) == 0)
        {
          rd->connection_closed ();
        }
    }

  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL
