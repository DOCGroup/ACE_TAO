// $Id$


#include "tao/Muxed_TMS.h"
#include "tao/Reply_Dispatcher.h"
#include "tao/GIOP_Message_Version.h"
#include "tao/debug.h"
#include "tao/Pluggable_Messaging_Utils.h"
#include "Transport.h"
#include "ORB_Core.h"
#include "Client_Strategy_Factory.h"

ACE_RCSID(tao, Muxed_TMS, "$Id$")

TAO_Muxed_TMS::TAO_Muxed_TMS (TAO_Transport *transport)
  : TAO_Transport_Mux_Strategy (transport)
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
  ACE_GUARD_RETURN (ACE_Lock, ace_mon,
                    *this->lock_, 0);

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
                "TAO (%P|%t) - Muxed_TMS[%d]::request_id, <%d>\n",
                this->transport_->id (),
                this->request_id_generator_));

  return this->request_id_generator_;
}

// Bind the dispatcher with the request id.
int
TAO_Muxed_TMS::bind_dispatcher (CORBA::ULong request_id,
                                TAO_Reply_Dispatcher *rd)
{
  ACE_GUARD_RETURN (ACE_Lock,
                    ace_mon,
                    *this->lock_,
                    -1);

  int result =
    this->dispatcher_table_.bind (request_id, rd);

  if (result != 0)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%P | %t):TAO_Muxed_TMS::bind_dispatcher: ")
                    ACE_TEXT ("bind dispatcher failed: result = %d, request id = %d \n"),
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
  TAO_Reply_Dispatcher *rd = 0;

  // @@TODO: WTH are we sending the rd in? We can just unbind using
  // the request_id
  return this->dispatcher_table_.unbind (request_id, rd);
}

int
TAO_Muxed_TMS::dispatch_reply (TAO_Pluggable_Reply_Params &params)
{
  int result = 0;
  TAO_Reply_Dispatcher *rd = 0;

  // Grab the reply dispatcher for this id.
  {
    ACE_GUARD_RETURN (ACE_Lock,
                      ace_mon,
                      *this->lock_,
                      -1);

    result =
      this->dispatcher_table_.unbind (params.request_id_, rd);

    if (TAO_debug_level > 8)
      ACE_DEBUG ((LM_DEBUG,
                  "TAO (%P|%t)- TAO_Muxed_TMS::dispatch_reply, "
                  "id = %d\n",
                  params.request_id_));

    if (result != 0)
      {
        if (TAO_debug_level > 0)
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P | %t):TAO_Muxed_TMS::dispatch_reply: ")
                      ACE_TEXT ("unbind dispatcher failed: result = %d\n"),
                      result));

        // This return value means that the mux strategy was not able
        // to find a registered reply handler, either because the reply
        // was not our reply - just forget about it - or it was ours, but
        // the reply timed out - just forget about the reply.
        return 0;
      }


    if (TAO_debug_level > 8)
      ACE_DEBUG ((LM_DEBUG,
                  "TAO (%P|%t)- TAO_Muxed_TMS::dispatch_reply, "
                  "id = %d\n",
                  params.request_id_));

    if (result != 0)
      {
        if (TAO_debug_level > 0)
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P | %t):TAO_Muxed_TMS::dispatch_reply: ")
                      ACE_TEXT ("unbind dispatcher failed: result = %d\n"),
                      result));

        // This return value means that the mux strategy was not able
        // to find a registered reply handler, either because the reply
        // was not our reply - just forget about it - or it was ours, but
        // the reply timed out - just forget about the reply.
        return 0;
      }

    // Do not move it outside the scope of the lock. A follower thread
    // could have timedout unwinding teh stack and the reply
    // dispatcher and that would mean the present thread could be left
    // with a dangling pointer and may crash. To safeguard againt such
    // cases we dispatch with the lock held.
    // Dispatch the reply.
    // They return 1 on success, and -1 on failure.
    result =  rd->dispatch_reply (params);
  }

  return result;
}

int
TAO_Muxed_TMS::idle_after_send (void)
{
  if (this->transport_ != 0)
    return this->transport_->make_idle ();

  return 0;
}

int
TAO_Muxed_TMS::idle_after_reply (void)
{
  // No op.
  return 0;
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
      retval = this->clear_cache ();
    }
  while (retval != -1);

}

int
TAO_Muxed_TMS::clear_cache (void)
{
  if (this->dispatcher_table_.current_size () == 0)
    return -1;

  REQUEST_DISPATCHER_TABLE::ITERATOR end =
    this->dispatcher_table_.end ();

  ACE_Unbounded_Stack <TAO_Reply_Dispatcher *> ubs;

  for (REQUEST_DISPATCHER_TABLE::ITERATOR i =
         this->dispatcher_table_.begin ();
       i != end;
       ++i)
    {
      CORBA::ULong request_id = (*i).ext_id_;

      ubs.push ((*i).int_id_);
    }

  this->dispatcher_table_.unbind_all ();
  size_t sz =  ubs.size ();

  for (size_t k = 0 ; k != sz ; ++k)
    {
      TAO_Reply_Dispatcher *rd = 0;

      ubs.pop (rd);

      rd->connection_closed ();
    }

  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Hash_Map_Manager_Ex <CORBA::ULong,
                                        TAO_Reply_Dispatcher *,
                                        ACE_Hash <CORBA::ULong>,
                                        ACE_Equal_To <CORBA::ULong>,
                                        ACE_Null_Mutex>;

template class ACE_Hash_Map_Entry<CORBA::ULong,
                                  TAO_Reply_Dispatcher *>;

template class ACE_Hash_Map_Iterator_Base_Ex<CORBA::ULong,
                                             TAO_Reply_Dispatcher *,
                                             ACE_Hash<unsigned int>,
                                             ACE_Equal_To<unsigned int>,
                                             ACE_Null_Mutex>;

template class ACE_Hash_Map_Iterator_Ex<CORBA::ULong,
                                        TAO_Reply_Dispatcher*,
                                        ACE_Hash<CORBA::ULong>,
                                        ACE_Equal_To<CORBA::ULong>,
                                        ACE_Null_Mutex>;

template class ACE_Hash_Map_Reverse_Iterator_Ex<CORBA::ULong,
                                                TAO_Reply_Dispatcher*,
                                                ACE_Hash<CORBA::ULong>,
                                                ACE_Equal_To<CORBA::ULong>,
                                                ACE_Null_Mutex>;

template class ACE_Unbounded_Stack<TAO_Reply_Dispatcher*>;
template class ACE_Node<TAO_Reply_Dispatcher*>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Hash_Map_Manager_Ex <CORBA::ULong, TAO_Reply_Dispatcher *, ACE_Hash <CORBA::ULong>, ACE_Equal_To <CORBA::ULong>, ACE_Null_Mutex>

#pragma instantiate ACE_Hash_Map_Entry<CORBA::ULong, TAO_Reply_Dispatcher *>

#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<unsigned int, TAO_Reply_Dispatcher *, ACE_Hash<unsigned int>, ACE_Equal_To<unsigned int>, ACE_Null_Mutex>

#pragma instantiate ACE_Hash_Map_Iterator_Ex<CORBA::ULong, TAO_Reply_Dispatcher*, ACE_Hash<CORBA::ULong>, ACE_Equal_To<CORBA::ULong>, ACE_Null_Mutex>

#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<CORBA::ULong, TAO_Reply_Dispatcher*, ACE_Hash<CORBA::ULong>, ACE_Equal_To<CORBA::ULong>, ACE_Null_Mutex>


#pragma instantiate ACE_Unbounded_Stack<TAO_Reply_Dispatcher*>

#pragma instantiate ACE_Node<TAO_Reply_Dispatcher*>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
