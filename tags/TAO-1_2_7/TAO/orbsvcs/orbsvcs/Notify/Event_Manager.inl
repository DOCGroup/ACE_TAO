// $Id$

#include "ProxyConsumer.h"
#include "ProxySupplier.h"
#include "Event_Map_T.h"

ACE_INLINE TAO_NS_Consumer_Map*
TAO_NS_Event_Manager::consumer_map (void)
{
  return this->consumer_map_;
}

ACE_INLINE TAO_NS_Supplier_Map*
TAO_NS_Event_Manager::supplier_map (void)
{
  return this->supplier_map_;
}

ACE_INLINE const TAO_NS_EventTypeSeq&
TAO_NS_Event_Manager::offered_types (void)
{
  return this->supplier_map_->event_types ();
}

ACE_INLINE const TAO_NS_EventTypeSeq&
TAO_NS_Event_Manager::subscription_types (void)
{
  return this->consumer_map_->event_types ();
}

/********************************************************************************/

ACE_INLINE TAO_NS_ProxyConsumer_Update_Worker::TAO_NS_ProxyConsumer_Update_Worker (const TAO_NS_EventTypeSeq& added, const TAO_NS_EventTypeSeq& removed)
  :added_ (added), removed_ (removed)
{
}

ACE_INLINE void
TAO_NS_ProxyConsumer_Update_Worker::work (TAO_NS_ProxyConsumer* proxy ACE_ENV_ARG_DECL)
{
  proxy->types_changed (added_, removed_ ACE_ENV_ARG_PARAMETER);
}

/********************************************************************************/

ACE_INLINE TAO_NS_ProxySupplier_Update_Worker::TAO_NS_ProxySupplier_Update_Worker (const TAO_NS_EventTypeSeq& added, const TAO_NS_EventTypeSeq& removed)
  :added_ (added), removed_ (removed)
{
}

ACE_INLINE void
TAO_NS_ProxySupplier_Update_Worker::work (TAO_NS_ProxySupplier* proxy ACE_ENV_ARG_DECL)
{
  proxy->types_changed (added_, removed_ ACE_ENV_ARG_PARAMETER);
}

/********************************************************************************/
