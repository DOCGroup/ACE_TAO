// $Id$
#include "Method_Request_Lookup_Base.h"

#if ! defined (__ACE_INLINE__)
#include "Method_Request_Lookup_Base.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID (Notify, TAO_Notify_Method_Request_Lookup_Base, "$Id$")


#include "tao/debug.h"
#include "Event_Manager.h"
#include "ProxyConsumer.h"
#include "SupplierAdmin.h"
#include "Consumer_Map.h"

TAO_Notify_Method_Request_Lookup_Base::TAO_Notify_Method_Request_Lookup_Base (
      const TAO_Notify_Event * event,
      TAO_Notify_ProxyConsumer * proxy)
  : event_ (event)
  , proxy_consumer_ (proxy)
{
}

TAO_Notify_Method_Request_Lookup_Base::~TAO_Notify_Method_Request_Lookup_Base ()
{
}

void
TAO_Notify_Method_Request_Lookup_Base::work (
  TAO_Notify_ProxySupplier* proxy_supplier
  ACE_ENV_ARG_DECL)
{
  proxy_supplier->push (this->event_, true ACE_ENV_ARG_PARAMETER);
}

TAO_Notify_Method_Request_Lookup_Base::execute_i (ACE_ENV_SINGLE_ARG_DECL)
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
