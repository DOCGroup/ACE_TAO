// $Id$

#include "Method_Request_Lookup.h"

#if ! defined (__ACE_INLINE__)
#include "Method_Request_Lookup.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Notify, TAO_NS_Method_Request_Lookup, "$Id$")

#include "tao/debug.h"
#include "Consumer_Map.h"
#include "ProxySupplier.h"
#include "ProxyConsumer.h"
#include "Proxy.h"
#include "Admin.h"
#include "SupplierAdmin.h"

TAO_NS_Method_Request_Lookup::TAO_NS_Method_Request_Lookup (const TAO_NS_Event_var& event, TAO_NS_ProxyConsumer* proxy_consumer, TAO_NS_Consumer_Map* map)
  : TAO_NS_Method_Request_Event (event), proxy_consumer_ (proxy_consumer), map_ (map),
    refcountable_guard_ (*proxy_consumer)
{
}

TAO_NS_Method_Request_Lookup::~TAO_NS_Method_Request_Lookup ()
{
}

TAO_NS_Method_Request*
TAO_NS_Method_Request_Lookup::copy (void)
{
  /// @@use factory
  return new TAO_NS_Method_Request_Lookup (this->event_, this->proxy_consumer_, this->map_);
}

int
TAO_NS_Method_Request_Lookup::execute (ACE_ENV_SINGLE_ARG_DECL)
{
  if (this->proxy_consumer_->has_shutdown ())
    return 0; // If we were shutdown while waiting in the queue, return with no action.

  TAO_NS_Admin* parent = this->proxy_consumer_->supplier_admin ();

  CORBA::Boolean val =  this->proxy_consumer_->check_filters (this->event_,
                                                              parent->filter_admin (),
                                                              parent->filter_operator ()
                                                              ACE_ENV_ARG_PARAMETER);

  if (TAO_debug_level > 1)
    ACE_DEBUG ((LM_DEBUG, "Proxyconsumer %x filter eval result = %d",this->proxy_consumer_ , val));

  // Filter failed - do nothing.
  if (val == 0)
    return 0;

  TAO_NS_Consumer_Map::ENTRY* entry = map_->find (this->event_->type () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  TAO_NS_ProxySupplier_Collection* consumers = 0;

  if (entry != 0)
  {
    consumers = entry->collection ();

    if (consumers != 0)
      consumers->for_each (this ACE_ENV_ARG_PARAMETER);

    this->map_->release (entry);
  }

  // Get the default consumers
  consumers = map_->broadcast_collection ();

  if (consumers != 0)
    consumers->for_each (this ACE_ENV_ARG_PARAMETER);

  return 0;
}

void
TAO_NS_Method_Request_Lookup::work (TAO_NS_ProxySupplier* proxy_supplier ACE_ENV_ARG_DECL_NOT_USED)
{
  proxy_supplier->push (this->event_);
}
