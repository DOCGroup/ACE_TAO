// $Id$

#include "Delivery_Method_Lookup.h"

#if ! defined (__ACE_INLINE__)
#include "Delivery_Method_Lookup.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(RT_Notify, TAO_Notify_Method_Request_Lookup, "$Id$")

#include "Event_Map_T.h"
#include "ProxySupplier.h"
#include "ProxyConsumer.h"

#include "orbsvcs/ESF/ESF_Proxy_Collection.h"

#include "tao/debug.h"

namespace TAO_NOTIFY
{

//////////////////
// Delivery Request
Delivery_Method_Lookup::Delivery_Method_Lookup (const TAO_NOTIFY::Delivery_Request_Ptr & delivery_request,
                                                            TAO_Notify_ProxyConsumer* proxy_consumer)
  : Delivery_Method (delivery_request)
  , TAO_ESF_Worker<TAO_Notify_ProxySupplier> ()  // gcc 3.2 makes me do this
  , proxy_consumer_ (proxy_consumer)
  , refcountable_guard_ (*proxy_consumer)
{
  delivery_request->set_delivery_type (persistence_code);
  IdVec ids(3);
  proxy_consumer->get_id_path (ids);
  delivery_request->set_destination_id (ids);
}

Delivery_Method_Lookup::Delivery_Method_Lookup( const Delivery_Method_Lookup & rhs)
  : Delivery_Method (rhs.delivery_request_)
  , TAO_ESF_Worker<TAO_Notify_ProxySupplier> ()  // gcc 3.2 makes me do this
  , proxy_consumer_ (rhs.proxy_consumer_)
  , refcountable_guard_ (*rhs.proxy_consumer_)
{
}

Delivery_Method_Lookup::~Delivery_Method_Lookup ()
{
}

//static
Delivery_Method_Lookup *
Delivery_Method_Lookup::create (
  const Delivery_Request_Ptr& delivery_request,
  TAO_Notify_EventChannelFactory &ecf,
  TAO_InputCDR & cdr
  ACE_ENV_ARG_DECL)
{
  bool ok = true;
  Delivery_Method_Lookup * result = 0;
  CORBA::ULong count;
  if (cdr.read_ulong (count))
  {
    IdVec id_path (count);
    for (size_t nid = 0; ok && nid < count; ++nid)
    {
      TAO_Notify_Object_Id id = 0;
      if ( cdr.read_long (id))
      {
        id_path.push_back (id);
      }
      else
      {
        ok = false;
      }
    }

    if (ok)
    {
      TAO_Notify_ProxyConsumer * proxy_consumer = ecf.find_proxy_consumer (id_path,
        0 ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN(0);
      if (proxy_consumer != 0)
      {
        ACE_NEW_NORETURN (result,
          Delivery_Method_Lookup (delivery_request, proxy_consumer));
      }
      else
      {
        ACE_ERROR ((LM_ERROR,
          ACE_TEXT ("(%P|%t) Delivery_Method_Lookup::unmarshal: unknown proxy id\n")
          ));
      }
    }
    else
    {
      ACE_ERROR ((LM_ERROR,
        ACE_TEXT ("(%P|%t) Delivery_Method_Lookup::unmarshal: Cant read proxy id path\n")
        ));
    }
  }
  return result;
}


TAO_Notify_Method_Request*
Delivery_Method_Lookup::copy (void)
{
  return new Delivery_Method_Lookup (*this);
}

int
Delivery_Method_Lookup::execute (ExecuteOption eo ACE_ENV_ARG_DECL)
{
  ACE_UNUSED_ARG (eo);
  ACE_ASSERT(this->delivery_request_ != 0);
  ACE_ASSERT(this->proxy_consumer_ != 0);
  ACE_ASSERT(! this->delivery_request_->event().null());
  ACE_ASSERT(eo == TAO_Notify_Method_Request::EXECUTE);

  // Report request complete when this method exits. If the request isn't filtered
  // then a new request will be created for each destination consumer.
  Delivery_Method_Complete complete (this->delivery_request_);

  // Note : The ProxyConsumer may very well shutdown() before we get here. It should still be
  // usable for checking filters, and we still want to deliver the events that were pushed.

  TAO_Notify_Event_Ptr pevent = this->delivery_request_->event ();
  CORBA::Boolean val =  this->proxy_consumer_->check_filters (*pevent ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN(0);

  if (TAO_debug_level > 1)
    ACE_DEBUG ((LM_DEBUG, "Proxyconsumer %x filter eval result = %d\n",this->proxy_consumer_ , val));

  // Filter failed - do nothing.
  if (val == 0)
  {
    return 0;
  }

  TAO_Notify_Consumer_Map* map = this->proxy_consumer_->map();
  ACE_ASSERT(map != 0);

  TAO_Notify_Consumer_Map::ENTRY* entry = map->find (pevent->type () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  TAO_Notify_ProxySupplier_Collection* consumers = 0;

  if (entry != 0)
  {
    consumers = entry->collection ();

    if (consumers != 0)
    {
      consumers->for_each (this ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN(0);
    }

    map->release (entry);
  }

  // Get the consumers subscribed to all events.
  consumers = map->broadcast_collection ();

  if (consumers != 0)
  {
    consumers->for_each (this ACE_ENV_ARG_PARAMETER);
  }
  return 0;
}

void
Delivery_Method_Lookup::work (TAO_Notify_ProxySupplier* proxy ACE_ENV_ARG_DECL_NOT_USED)
{
  ACE_ASSERT(proxy != 0);
  ACE_ASSERT(this->delivery_request_ != 0);
  ACE_ASSERT(this->delivery_request_->routing_slip() != 0);
  this->delivery_request_->routing_slip()->dispatch (proxy, true);
}

} //namespace TAO_NOTIFY
