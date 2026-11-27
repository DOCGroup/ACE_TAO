#include "tao/Exclusive_TMS.h"
#include "tao/Reply_Dispatcher.h"
#include "tao/debug.h"
#include "tao/Transport.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Exclusive_TMS::TAO_Exclusive_TMS (TAO_Transport *transport)
  : TAO_Transport_Mux_Strategy (transport)
{
}

TAO_Exclusive_TMS::~TAO_Exclusive_TMS () = default;

// Generate and return an unique request id for the current
// invocation. We can actually return a predecided ULong, since we
// allow only one invocation over this connection at a time.
CORBA::ULong
TAO_Exclusive_TMS::request_id ()
{
  ACE_MT (ACE_GUARD_RETURN (TAO_SYNCH_RECURSIVE_MUTEX, ace_mon, this->lock_, 0));

  this->request_sent_ = false;
  this->transport_idle_ = false;

  ++this->request_id_generator_;

  // if TAO_Transport::bidirectional_flag_
  //  ==  1 --> originating side
  //  ==  0 --> other side
  //  == -1 --> no bi-directional connection was negotiated
  // The originating side must have an even request ID, and the other
  // side must have an odd request ID.  Make sure that is the case.
  int const bidir_flag =
    this->transport_->bidirectional_flag ();

  if ((bidir_flag == 1 && ACE_ODD (this->request_id_generator_))
       || (bidir_flag == 0 && ACE_EVEN (this->request_id_generator_)))
    {
      ++this->request_id_generator_;
    }

  if (TAO_debug_level > 4)
    {
      TAOLIB_DEBUG ((LM_DEBUG,
                     ACE_TEXT ("TAO (%P|%t) - Exclusive_TMS::request_id - [%d]\n"),
                     this->request_id_generator_));
    }

  return this->request_id_generator_;
}

// Bind the handler with the request id.
int
TAO_Exclusive_TMS::bind_dispatcher (CORBA::ULong request_id,
                                    ACE_Intrusive_Auto_Ptr<TAO_Reply_Dispatcher> rd)
{
  ACE_MT (ACE_GUARD_RETURN (TAO_SYNCH_RECURSIVE_MUTEX, ace_mon, this->lock_, -1));

  this->request_id_ = request_id;
  this->rd_.reset (rd.get ());

  return 0;
}

bool
TAO_Exclusive_TMS::has_request ()
{
  ACE_MT (ACE_GUARD_RETURN (TAO_SYNCH_RECURSIVE_MUTEX, ace_mon, this->lock_, false));

  return this->rd_.get () != nullptr;
}

int
TAO_Exclusive_TMS::unbind_dispatcher (CORBA::ULong request_id)
{
  ACE_MT (ACE_GUARD_RETURN (TAO_SYNCH_RECURSIVE_MUTEX, ace_mon, this->lock_, -1));

  if (!this->rd_ || this->request_id_ != request_id)
    {
      return -1;
    }

  this->rd_.release ();

  return 0;
}

int
TAO_Exclusive_TMS::dispatch_reply (TAO_Pluggable_Reply_Params &params)
{
  ACE_Intrusive_Auto_Ptr<TAO_Reply_Dispatcher> rd {};

  {
    ACE_MT (ACE_GUARD_RETURN (TAO_SYNCH_RECURSIVE_MUTEX, ace_mon, this->lock_, -1));

    // Check the ids.
    if (!this->rd_ || this->request_id_ != params.request_id_)
      {
        if (TAO_debug_level > 0)
          {
            TAOLIB_DEBUG ((LM_DEBUG,
                           ACE_TEXT ("TAO (%P|%t) - Exclusive_TMS::dispatch_reply - [%d] != [%d]\n"),
                           this->request_id_, params.request_id_));
          }

        // The return value 0 informs the transport that the mux strategy
        // did not find the right reply handler.
        return 0;
      }

    rd.reset (this->rd_.get ());
    this->request_id_ = 0; // @@ What is a good value???
    this->rd_.release ();
  }

  // Dispatch the reply.
  // Returns 1 on success, -1 on failure.
  return rd->dispatch_reply (params);
}

int
TAO_Exclusive_TMS::reply_timed_out (CORBA::ULong request_id)
{
  ACE_Intrusive_Auto_Ptr<TAO_Reply_Dispatcher> rd {};

  {
    ACE_MT (ACE_GUARD_RETURN (TAO_SYNCH_RECURSIVE_MUTEX, ace_mon, this->lock_, -1));

    // Check the ids.
    if (!this->rd_ || this->request_id_ != request_id)
      {
        if (TAO_debug_level > 0)
          {
            TAOLIB_DEBUG ((LM_DEBUG,
                           ACE_TEXT ("TAO (%P|%t) - Exclusive_TMS::reply_timed_out - [%d] != [%d]\n"),
                           this->request_id_, request_id));
          }

        // The return value 0 informs the transport that the mux strategy
        // did not find the right reply handler.
        return 0;
      }

    rd.reset (this->rd_.get ());
    this->request_id_ = 0; // @@ What is a good value???
    this->rd_.release ();
  }

  rd->reply_timed_out ();

  return 0;
}

bool
TAO_Exclusive_TMS::idle_after_send ()
{
  bool make_idle {};

  {
    ACE_MT (ACE_GUARD_RETURN (TAO_SYNCH_RECURSIVE_MUTEX, ace_mon, this->lock_, false));

    this->request_sent_ = true;

    // If there is no reply dispatcher (possible when the reply arrived
    // before sending completed), release the transport now.
    if (!this->rd_ && !this->transport_idle_)
      {
        this->transport_idle_ = true;
        make_idle = true;
      }
  }

  if (make_idle && this->transport_ != nullptr)
    {
      // Let WS know we're finished.
      this->transport_->wait_strategy ()->finished_request ();

      (void) this->transport_->make_idle ();
    }

  return make_idle;
}

bool
TAO_Exclusive_TMS::idle_after_reply ()
{
  bool make_idle {};

  {
    ACE_MT (ACE_GUARD_RETURN (TAO_SYNCH_RECURSIVE_MUTEX, ace_mon, this->lock_, false));

    if (this->request_sent_ && !this->transport_idle_)
      {
        this->transport_idle_ = true;
        make_idle = true;
      }
  }

  if (make_idle && this->transport_ != nullptr)
    {
      // Let WS know we're finished.
      this->transport_->wait_strategy ()->finished_request ();

      (void) this->transport_->make_idle ();
    }

  // The send completion path will idle the transport if it is still active.
  return true;
}

void
TAO_Exclusive_TMS::connection_closed ()
{
  ACE_Intrusive_Auto_Ptr<TAO_Reply_Dispatcher> rd {};

  {
    ACE_MT (ACE_GUARD (TAO_SYNCH_RECURSIVE_MUTEX, ace_mon, this->lock_));

    rd.reset (this->rd_.get ());
  }

  if (rd)
    {
      rd->connection_closed ();
    }
}

TAO_END_VERSIONED_NAMESPACE_DECL
