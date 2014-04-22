// $Id$

#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/Notify/Method_Request_Lookup.h"
#include "orbsvcs/Notify/Consumer_Map.h"
#include "orbsvcs/Notify/ProxySupplier.h"
#include "orbsvcs/Notify/ProxyConsumer.h"
#include "orbsvcs/Notify/Proxy.h"
#include "orbsvcs/Notify/Admin.h"
#include "orbsvcs/Notify/SupplierAdmin.h"
#include "orbsvcs/Notify/Method_Request_Dispatch.h"
#include "orbsvcs/Notify/Delivery_Request.h"
#include "orbsvcs/Notify/EventChannelFactory.h"
#include "orbsvcs/Notify/Event_Manager.h"
#include "orbsvcs/Notify/Factory.h"

#include "orbsvcs/ESF/ESF_Proxy_Collection.h"

#include "tao/debug.h"
#include "tao/CDR.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Notify_Method_Request_Lookup::TAO_Notify_Method_Request_Lookup (
      const TAO_Notify_Event * event,
      TAO_Notify_ProxyConsumer * proxy)
  : TAO_Notify_Method_Request_Event (event)
  , proxy_consumer_ (proxy)
{
}

TAO_Notify_Method_Request_Lookup::TAO_Notify_Method_Request_Lookup (
      const TAO_Notify::Delivery_Request_Ptr& delivery,
      TAO_Notify_ProxyConsumer * proxy)
  : TAO_Notify_Method_Request_Event (delivery)
  , proxy_consumer_ (proxy)
{
}

TAO_Notify_Method_Request_Lookup::~TAO_Notify_Method_Request_Lookup ()
{
}

void
TAO_Notify_Method_Request_Lookup::work (
  TAO_Notify_ProxySupplier* proxy_supplier)
{
  if (delivery_request_.get () == 0)
  {
    TAO_Notify_Method_Request_Dispatch_No_Copy request (*this, proxy_supplier, true);
    proxy_supplier->deliver (request);
  }
  else
  {
    delivery_request_->dispatch (proxy_supplier, true);
  }
}

int TAO_Notify_Method_Request_Lookup::execute_i (void)
{
  if (this->proxy_consumer_->has_shutdown ())
    return 0; // If we were shutdown while waiting in the queue, return with no action.

  TAO_Notify_SupplierAdmin& parent = this->proxy_consumer_->supplier_admin ();

  CORBA::Boolean val =  this->proxy_consumer_->check_filters (this->event_,
                                                             parent.filter_admin (),
                                                             parent.filter_operator ());

  if (TAO_debug_level > 1)
    ORBSVCS_DEBUG ((LM_DEBUG, ACE_TEXT("Notify (%P|%t) Proxyconsumer %x filter ")
                          ACE_TEXT("eval result = %d\n"),
                          &this->proxy_consumer_ , val));

  // Filter failed - do nothing.
  if (!val)
    return 0;

  // The map of subscriptions.
  TAO_Notify_Consumer_Map& map = this->proxy_consumer_->event_manager ().consumer_map ();

  TAO_Notify_Consumer_Map::ENTRY* entry = map.find (this->event_->type ());

  TAO_Notify_ProxySupplier_Collection* consumers = 0;

  if (entry != 0)
  {
    consumers = entry->collection ();

    if (consumers != 0)
      {
        consumers->for_each (this);
      }

    map.release (entry);
  }

  // Get the default consumers
  consumers = map.broadcast_collection ();

  if (consumers != 0)
    {
      consumers->for_each (this);
    }
  this->complete ();
  return 0;
}

/// Static method used to reconstruct a Method Request Dispatch
TAO_Notify_Method_Request_Lookup_Queueable *
TAO_Notify_Method_Request_Lookup::unmarshal (
  TAO_Notify::Delivery_Request_Ptr & delivery_request,
  TAO_Notify_EventChannelFactory &ecf,
  TAO_InputCDR & cdr)
{
  bool ok = true;
  TAO_Notify_Method_Request_Lookup_Queueable * result = 0;
  CORBA::ULong count;
  if (cdr.read_ulong (count))
  {
    TAO_Notify::IdVec id_path (count);
    for (size_t nid = 0; ok && nid < count; ++nid)
    {
      TAO_Notify_Object::ID id = 0;
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
      TAO_Notify_ProxyConsumer * proxy_consumer = ecf.find_proxy_consumer (
        id_path,
        0);
      if (proxy_consumer != 0)
      {
        ACE_NEW_NORETURN (result,
          TAO_Notify_Method_Request_Lookup_Queueable (delivery_request, proxy_consumer));
      }
      else
      {
        ORBSVCS_ERROR ((LM_ERROR,
          ACE_TEXT ("(%P|%t) TAO_Notify_Method_Request_Lookup_No_Copy::unmarshal: unknown proxy id\n")
          ));
      }
    }
    else
    {
      ORBSVCS_ERROR ((LM_ERROR,
        ACE_TEXT ("(%P|%t) TAO_Notify_Method_Request_Lookup_No_Copy::unmarshal: Cant read proxy id path\n")
        ));
    }
  }
  return result;

}


/****************************************************************/

TAO_Notify_Method_Request_Lookup_Queueable::TAO_Notify_Method_Request_Lookup_Queueable (
      const TAO_Notify_Event::Ptr& event,
      TAO_Notify_ProxyConsumer* proxy_consumer)
  : TAO_Notify_Method_Request_Lookup (event.get (), proxy_consumer)
  , TAO_Notify_Method_Request_Queueable (event.get ())
  , event_var_ (event)
  , proxy_guard_ (proxy_consumer)
{
}

TAO_Notify_Method_Request_Lookup_Queueable::TAO_Notify_Method_Request_Lookup_Queueable (
      TAO_Notify::Delivery_Request_Ptr & request,
      TAO_Notify_ProxyConsumer * proxy_consumer)
  : TAO_Notify_Method_Request_Lookup (request, proxy_consumer)
  , TAO_Notify_Method_Request_Queueable (request->event ().get ())
  , event_var_ (request->event ())
  , proxy_guard_ (proxy_consumer)
{
}


TAO_Notify_Method_Request_Lookup_Queueable::~TAO_Notify_Method_Request_Lookup_Queueable ()
{
}

int
TAO_Notify_Method_Request_Lookup_Queueable::execute (void)
{
  return this->execute_i ();
}

/******************************************************************************************************/

TAO_Notify_Method_Request_Lookup_No_Copy::TAO_Notify_Method_Request_Lookup_No_Copy (
        const TAO_Notify_Event* event,
        TAO_Notify_ProxyConsumer* proxy_consumer)
  : TAO_Notify_Method_Request_Lookup (event, proxy_consumer)
{
}

TAO_Notify_Method_Request_Lookup_No_Copy::~TAO_Notify_Method_Request_Lookup_No_Copy ()
{
}

int
TAO_Notify_Method_Request_Lookup_No_Copy::execute (void)
{
  return this->execute_i ();
}

TAO_Notify_Method_Request_Queueable*
TAO_Notify_Method_Request_Lookup_No_Copy::copy (void)
{
  TAO_Notify_Method_Request_Queueable* request;

  TAO_Notify_Event::Ptr event(this->event_->queueable_copy());

  ACE_NEW_THROW_EX (request,
                    TAO_Notify_Method_Request_Lookup_Queueable (event, this->proxy_consumer_),
                    CORBA::INTERNAL ());

  return request;
}

TAO_END_VERSIONED_NAMESPACE_DECL
