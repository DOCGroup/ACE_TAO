//$Id$
#include "Notify_Listeners.h"
#include "Notify_Event_Map.h"
#include "Notify_Event_Processor.h"
#include "Notify_AdminProperties.h"
#include "tao/corba.h"

ACE_INLINE void
TAO_Notify_Event_Manager::process_event  (TAO_Notify_Event* event, TAO_Notify_EventSource* event_source, CORBA::Environment &ACE_TRY_ENV)
{
  if (admin_properties_->reject_new_events () == 1)
    ACE_THROW (CORBA::IMP_LIMIT ());

  this->event_processor_->evaluate_source_filter (event, event_source, ACE_TRY_ENV);
  // Start by checking if the event passes through the Source's filter.
}

ACE_INLINE void
TAO_Notify_Event_Manager::register_for_publication_updates (TAO_Notify_UpdateListener* update_listener, CORBA::Environment& ACE_TRY_ENV)
{
  this->event_map_->register_for_publication_updates (update_listener,
                                                     ACE_TRY_ENV);
}

ACE_INLINE void
TAO_Notify_Event_Manager::register_for_subscription_updates (TAO_Notify_UpdateListener* update_listener, CORBA::Environment& ACE_TRY_ENV)
{
  this->event_map_->register_for_subscription_updates (update_listener,
                                                      ACE_TRY_ENV);
}

ACE_INLINE void
TAO_Notify_Event_Manager:: unregister_from_subscription_updates (TAO_Notify_UpdateListener* update_listener, CORBA::Environment& ACE_TRY_ENV)
{
  this->event_map_->unregister_from_subscription_updates (update_listener,
                                                         ACE_TRY_ENV);
}

ACE_INLINE void
TAO_Notify_Event_Manager::unregister_from_publication_updates (TAO_Notify_UpdateListener* update_listener, CORBA::Environment& ACE_TRY_ENV)
{
  this->event_map_->unregister_from_publication_updates(update_listener,
                                                       ACE_TRY_ENV);
}

ACE_INLINE TAO_Notify_Event_Map*
TAO_Notify_Event_Manager::event_map (void)
{
  return this->event_map_;
}

ACE_INLINE TAO_Notify_Event_Processor*
TAO_Notify_Event_Manager::event_processor (void)
{
  return this->event_processor_;
}

ACE_INLINE CosNotification::EventTypeSeq*
TAO_Notify_Event_Manager::obtain_offered_types (void)
{
  return this->event_map_->obtain_offered_types ();
}

ACE_INLINE CosNotification::EventTypeSeq*
TAO_Notify_Event_Manager::obtain_subscription_types (void)
{
  return this->event_map_->obtain_subscription_types ();
}

ACE_INLINE TAO_Notify_AdminProperties*
TAO_Notify_Event_Manager::admin_properties (void)
{
  return this->admin_properties_;
}
