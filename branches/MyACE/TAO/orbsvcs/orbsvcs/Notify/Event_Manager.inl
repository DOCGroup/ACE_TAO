// $Id$

#include "ProxyConsumer.h"
#include "ProxySupplier.h"
#include "Event_Map_T.h"

ACE_INLINE TAO_Notify_Consumer_Map*
TAO_Notify_Event_Manager::consumer_map (void)
{
  return this->consumer_map_;
}

ACE_INLINE TAO_Notify_Supplier_Map*
TAO_Notify_Event_Manager::supplier_map (void)
{
  return this->supplier_map_;
}

ACE_INLINE const TAO_Notify_EventTypeSeq&
TAO_Notify_Event_Manager::offered_types (void)
{
  return this->supplier_map_->event_types ();
}

ACE_INLINE const TAO_Notify_EventTypeSeq&
TAO_Notify_Event_Manager::subscription_types (void)
{
  return this->consumer_map_->event_types ();
}

/********************************************************************************/

ACE_INLINE TAO_Notify_ProxyConsumer_Update_Worker::TAO_Notify_ProxyConsumer_Update_Worker (const TAO_Notify_EventTypeSeq& added, const TAO_Notify_EventTypeSeq& removed)
  :added_ (added), removed_ (removed)
{
}

ACE_INLINE void
TAO_Notify_ProxyConsumer_Update_Worker::work (TAO_Notify_ProxyConsumer* proxy ACE_ENV_ARG_DECL)
{
  proxy->types_changed (added_, removed_ ACE_ENV_ARG_PARAMETER);
}

/********************************************************************************/

ACE_INLINE TAO_Notify_ProxySupplier_Update_Worker::TAO_Notify_ProxySupplier_Update_Worker (const TAO_Notify_EventTypeSeq& added, const TAO_Notify_EventTypeSeq& removed)
  :added_ (added), removed_ (removed)
{
}

ACE_INLINE void
TAO_Notify_ProxySupplier_Update_Worker::work (TAO_Notify_ProxySupplier* proxy ACE_ENV_ARG_DECL)
{
  proxy->types_changed (added_, removed_ ACE_ENV_ARG_PARAMETER);
}

/********************************************************************************/
