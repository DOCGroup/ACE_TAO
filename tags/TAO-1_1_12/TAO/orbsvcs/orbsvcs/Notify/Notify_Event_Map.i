//$Id$

#include "ace/Containers_T.h"
#include "orbsvcs/ESF/ESF_Proxy_Collection.h"
#include "Notify_Event.h"

ACE_INLINE int
TAO_Notify_Event_Map::find (TAO_Notify_EventType const & event_type, TAO_Notify_EventListener_List*& event_listener_list)
{
   return this->event_recipient_map_.find (event_type, event_listener_list);
}

ACE_INLINE TAO_Notify_UpdateListener_List*
TAO_Notify_Event_Map::publication_change_listeners (void)
{
  return publication_change_listeners_;
}

ACE_INLINE TAO_Notify_UpdateListener_List*
TAO_Notify_Event_Map::subscription_change_listeners (void)
{
  return subscription_change_listeners_;
}

ACE_INLINE TAO_Notify_EventListener_List*
TAO_Notify_Event_Map::default_subscription_list (void)
{
  return default_subscription_list_;
}
