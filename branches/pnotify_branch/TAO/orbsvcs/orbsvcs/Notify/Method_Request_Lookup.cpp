// $Id$

#include "Method_Request_Lookup.h"

#if ! defined (__ACE_INLINE__)
#include "Method_Request_Lookup.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Notify, TAO_Notify_Method_Request_Lookup, "$Id$")

#include "tao/debug.h"
#include "Consumer_Map.h"
#include "ProxySupplier.h"
#include "ProxyConsumer.h"
#include "Proxy.h"
#include "Admin.h"
#include "SupplierAdmin.h"
#include "Event_Manager.h"
#include "Method_Request_Dispatch.h"

TAO_Notify_Method_Request_Lookup::TAO_Notify_Method_Request_Lookup (
      const TAO_Notify_Event * event,
      TAO_Notify_ProxyConsumer * proxy)
  : TAO_Notify_Method_Request_Event (event)
  , proxy_consumer_ (proxy)
{
}

TAO_Notify_Method_Request_Lookup::~TAO_Notify_Method_Request_Lookup ()
{
}

void
TAO_Notify_Method_Request_Lookup::work (
  TAO_Notify_ProxySupplier* proxy_supplier
  ACE_ENV_ARG_DECL)
{
  TAO_Notify_Method_Request_Dispatch_No_Copy request (this->event_, proxy_supplier, true);
  proxy_supplier->deliver (request ACE_ENV_ARG_PARAMETER);
}

TAO_Notify_Method_Request_Lookup::execute_i (ACE_ENV_SINGLE_ARG_DECL)
{
  if (this->proxy_consumer_->has_shutdown ())
    return 0; // If we were shutdown while waiting in the queue, return with no action.

  TAO_Notify_SupplierAdmin* parent = this->proxy_consumer_->supplier_admin ();

  CORBA::Boolean val =  this->proxy_consumer_->check_filters (this->event_,
                                                             parent->filter_admin (),
                                                             parent->filter_operator ()
                                                             ACE_ENV_ARG_PARAMETER);

  if (TAO_debug_level > 1)
    ACE_DEBUG ((LM_DEBUG, "Proxyconsumer %x filter eval result = %d",&this->proxy_consumer_ , val));

  // Filter failed - do nothing.
  if (val == 0)
    return 0;

  // The map of subscriptions.
  TAO_Notify_Consumer_Map* map = this->proxy_consumer_->event_manager ()->consumer_map ();

  TAO_Notify_Consumer_Map::ENTRY* entry = map->find (this->event_->type () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  TAO_Notify_ProxySupplier_Collection* consumers = 0;

  if (entry != 0)
  {
    consumers = entry->collection ();

    if (consumers != 0)
      consumers->for_each (this ACE_ENV_ARG_PARAMETER);

    map->release (entry);
  }

  // Get the default consumers
  consumers = map->broadcast_collection ();

  if (consumers != 0)
    consumers->for_each (this ACE_ENV_ARG_PARAMETER);

  return 0;
}

/****************************************************************/

TAO_Notify_Method_Request_Lookup_Queueable::TAO_Notify_Method_Request_Lookup_Queueable (
      const TAO_Notify_Event_var& event,
      TAO_Notify_ProxyConsumer* proxy_consumer)
  : TAO_Notify_Method_Request_Queueable (event.get ())
  , TAO_Notify_Method_Request_Lookup (event.get (), proxy_consumer)
  , event_var_ (event)
  , proxy_guard_ (proxy_consumer)
{
}

TAO_Notify_Method_Request_Lookup_Queueable::~TAO_Notify_Method_Request_Lookup_Queueable ()
{
}

int
TAO_Notify_Method_Request_Lookup_Queueable::execute (ACE_ENV_SINGLE_ARG_DECL)
{
  return this->execute_i (ACE_ENV_SINGLE_ARG_PARAMETER);
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
TAO_Notify_Method_Request_Lookup_No_Copy::execute (ACE_ENV_SINGLE_ARG_DECL)
{
  return this->execute_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

TAO_Notify_Method_Request_Queueable*
TAO_Notify_Method_Request_Lookup_No_Copy::copy (ACE_ENV_SINGLE_ARG_DECL)
{
  TAO_Notify_Method_Request_Queueable* request;

  const TAO_Notify_Event* event_copy = this->event_->queueable_copy (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  TAO_Notify_Event_Copy_var event_var (event_copy);

  ACE_NEW_THROW_EX (request,
                    TAO_Notify_Method_Request_Lookup_Queueable (event_var, this->proxy_consumer_),
                    CORBA::INTERNAL ());

  return request;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class TAO_Notify_Method_Request_Lookup_T<const TAO_Notify_Event_var
, TAO_Notify_ProxyConsumer_Guard
, const TAO_Notify_Event_var&
, TAO_Notify_ProxyConsumer*>;

template class TAO_Notify_Method_Request_Lookup_T<const TAO_Notify_Event*
, TAO_Notify_ProxyConsumer*
, const TAO_Notify_Event*
, TAO_Notify_ProxyConsumer*>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate TAO_Notify_Method_Request_Lookup_T<const TAO_Notify_Event_var
, TAO_Notify_ProxyConsumer_Guard
, const TAO_Notify_Event_var&
, TAO_Notify_ProxyConsumer*>

#pragma instantiate TAO_Notify_Method_Request_Lookup_T<const TAO_Notify_Event*
, TAO_Notify_ProxyConsumer*
, const TAO_Notify_Event*
, TAO_Notify_ProxyConsumer*>

#endif /*ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
