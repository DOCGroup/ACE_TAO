// $Id$

#include "Event_Manager.h"

template <class EVENT, class PROXY, class EVENT_PARAM, class PROXY_PARAM> ACE_INLINE int
TAO_NS_Method_Request_Lookup_T<EVENT, PROXY, EVENT_PARAM, PROXY_PARAM>::execute_i (ACE_ENV_SINGLE_ARG_DECL)
{
  if (this->proxy_consumer_->has_shutdown ())
    return 0; // If we were shutdown while waiting in the queue, return with no action.

  TAO_NS_Admin* parent = this->proxy_consumer_->supplier_admin ();

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
  TAO_NS_Consumer_Map* map = this->proxy_consumer_->event_manager ()->consumer_map ();

  TAO_NS_Consumer_Map::ENTRY* entry = map->find (this->event_->type () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  TAO_NS_ProxySupplier_Collection* consumers = 0;

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
