// $Id$


#include "tao/Muxed_TMS.h"
#include "tao/Reply_Dispatcher.h"
#include "tao/GIOP_Message_Version.h"
#include "tao/debug.h"
#include "tao/Pluggable_Messaging_Utils.h"
#include "Transport.h"

ACE_RCSID(tao, Muxed_TMS, "$Id$")


TAO_Muxed_TMS::TAO_Muxed_TMS (TAO_Transport *transport)
  : TAO_Transport_Mux_Strategy (transport),
    request_id_generator_ (0),
    orb_core_ (transport->orb_core ())
{
}

TAO_Muxed_TMS::~TAO_Muxed_TMS (void)
{
}

// Generate and return an unique request id for the current
// invocation.
CORBA::ULong
TAO_Muxed_TMS::request_id (void)
{
  // @@ What is a good error return value?
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon,
                    this->lock_, 0);
  return this->request_id_generator_++;
}

// Bind the dispatcher with the request id.
int
TAO_Muxed_TMS::bind_dispatcher (CORBA::ULong request_id,
                                TAO_Reply_Dispatcher *rd)
{
  int result = this->dispatcher_table_.bind (request_id, rd);

  if (result != 0)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%P | %t):TAO_Muxed_TMS::bind_dispatcher: ")
                    ACE_TEXT ("bind dispatcher failed: result = %d\n"),
                    result));

      return -1;
    }

  return 0;
}

void
TAO_Muxed_TMS::unbind_dispatcher (CORBA::ULong request_id)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock_);
  TAO_Reply_Dispatcher *rd = 0;
  (void) this->dispatcher_table_.unbind (request_id, rd);
}

int
TAO_Muxed_TMS::dispatch_reply (TAO_Pluggable_Reply_Params &params)
{
  // This message state should be the same as the one we have here,
  // which we gave to the Transport to read the message. Just a sanity
  // check here.
  //  ACE_ASSERT (message_state == this->message_state_);

  int result = 0;
  TAO_Reply_Dispatcher *rd = 0;

  // Grab the reply dispatcher for this id.
  {
    ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->lock_, -1);
    result = this->dispatcher_table_.unbind (params.request_id_, rd);
    //ACE_DEBUG ((LM_DEBUG,
    //            "\n(%P|%t) TAO_Muxed_TMS::dispatch_reply: id = %d\n",
    //            params.request_id_));
  }

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

  // Dispatch the reply.
  // They return 1 on success, and -1 on failure.
  return rd->dispatch_reply (params);

  // No need for idling Transport, it would have got idle'd soon after
  // sending the request.
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
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock_);
  // @@ This should be done using a mutex, the table

  REQUEST_DISPATCHER_TABLE::ITERATOR end =
    this->dispatcher_table_.end ();
  for (REQUEST_DISPATCHER_TABLE::ITERATOR i =
         this->dispatcher_table_.begin ();
       i != end;
       ++i)
    {
      (*i).int_id_->connection_closed ();
    }
  this->dispatcher_table_.unbind_all ();
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

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Hash_Map_Manager_Ex <CORBA::ULong, TAO_Reply_Dispatcher *, ACE_Hash <CORBA::ULong>, ACE_Equal_To <CORBA::ULong>, ACE_Null_Mutex>

#pragma instantiate ACE_Hash_Map_Entry<CORBA::ULong, TAO_Reply_Dispatcher *>

#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<unsigned int, TAO_Reply_Dispatcher *, ACE_Hash<unsigned int>, ACE_Equal_To<unsigned int>, ACE_Null_Mutex>

#pragma instantiate ACE_Hash_Map_Iterator_Ex<CORBA::ULong, TAO_Reply_Dispatcher*, ACE_Hash<CORBA::ULong>, ACE_Equal_To<CORBA::ULong>, ACE_Null_Mutex>

#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<CORBA::ULong, TAO_Reply_Dispatcher*, ACE_Hash<CORBA::ULong>, ACE_Equal_To<CORBA::ULong>, ACE_Null_Mutex>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
