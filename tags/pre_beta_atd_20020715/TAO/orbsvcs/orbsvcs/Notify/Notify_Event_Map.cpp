// $Id$

#include "orbsvcs/ESF/ESF_Proxy_Collection.h"
#include "Notify_Event.h"
#include "Notify_Event_Map.h"
#include "Notify_Factory.h"
#include "Notify_Collection.h"
#include "Notify_Event_Manager_Objects_Factory.h"
#include "Notify_Collection_Factory.h"

#if ! defined (__ACE_INLINE__)
#include "Notify_Event_Map.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Notify, Notify_Event_Map, "$Id$")

TAO_Notify_Event_Map::TAO_Notify_Event_Map (TAO_Notify_EMO_Factory* emo_factory)
  :default_subscription_list_ (0),
   subscription_change_listeners_ (0),
   publication_change_listeners_ (0),
   emo_factory_ (emo_factory),
   collection_factory_ (TAO_Notify_Factory::get_collection_factory ())
{
}

TAO_Notify_Event_Map::~TAO_Notify_Event_Map ()
{
  // delete all event listener lists in the event map.
  EVENT_RECIPIENT_MAP::ITERATOR iterator (this->event_recipient_map_);

  for (EVENT_RECIPIENT_MAP::ENTRY *entry = 0;
       iterator.next (entry) != 0;
       iterator.advance ())
    {
      delete entry->int_id_;
    }

  delete default_subscription_list_;
  delete subscription_change_listeners_;
  delete publication_change_listeners_;
}

void
TAO_Notify_Event_Map::init (ACE_ENV_SINGLE_ARG_DECL)
{
  this->default_subscription_list_ =
    this->collection_factory_->create_event_listener_list (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
  // Create the default list of listeners.

  this->subscription_change_listeners_
    = this->collection_factory_->create_update_listener_list (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->publication_change_listeners_
    = this->collection_factory_->create_update_listener_list (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_Notify_Event_Map::shutdown (ACE_ENV_SINGLE_ARG_DECL)
{
  // Shutdown all event listener lists in the event map.
  EVENT_RECIPIENT_MAP::ITERATOR iterator (this->event_recipient_map_);

  for (EVENT_RECIPIENT_MAP::ENTRY *entry = 0;
       iterator.next (entry) != 0;
       iterator.advance ())
    {
      entry->int_id_->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
    }

  default_subscription_list_->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  subscription_change_listeners_->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  publication_change_listeners_->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_Notify_Event_Map::subscribe_for_events (TAO_Notify_EventListener* event_listener, TAO_Notify_EventType_List& update, const CosNotification::EventTypeSeq & added ACE_ENV_ARG_DECL)
{
  TAO_Notify_EventListener_List* event_listener_list;
  TAO_Notify_EventType event_type;

  for (CORBA::ULong index = 0; index < added.length (); index++)
    {
      event_type = added[index];
#if 0
      ACE_DEBUG ((LM_DEBUG, "subscribing %x for event: %s, %s\n",
                 event_listener,
                  event_type.event_type_.domain_name.in (),
                  event_type.event_type_.type_name.in ()));
#endif
      if (event_type.is_special ())
        event_listener_list = default_subscription_list_;
      else if (this->event_recipient_map_.find (event_type,
                                                event_listener_list) == -1)
        {
          // create the list.
          TAO_Notify_EventListener_List* new_list =
            this->collection_factory_->create_event_listener_list (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_CHECK;

          // add the list to the recipient map.
          this->event_recipient_map_.bind (event_type, new_list);
          event_listener_list = new_list;
        }

      event_listener_list->connected (event_listener ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      // mirror changes in the subscription list
      if (this->subscription_list_.insert (event_type) == 0)
        update.insert (event_type);
    }
}

void
TAO_Notify_Event_Map::unsubscribe_from_events (TAO_Notify_EventListener* event_listener, TAO_Notify_EventType_List &update, const CosNotification::EventTypeSeq & removed ACE_ENV_ARG_DECL)
{
  TAO_Notify_EventListener_List* event_listener_list;
  TAO_Notify_EventType event_type;

  for (CORBA::ULong index = 0; index < removed.length (); index++)
    {
      event_type = removed[index];

      // find out if there is a event listener list for this event type.
      if (event_type.is_special ())
        event_listener_list = this->default_subscription_list_;
      else if (this->event_recipient_map_.find (event_type,
                                                event_listener_list) == -1)
        continue;

      // remove <event_listener> from the list.

      event_listener_list->disconnected (event_listener ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      // If this was the last entry in the list then we should remove
      // the corrsponding event_type from the recipient_map.
      // However i'm going to leave the blank entries based on the assumption
      // that a system will have a fixed set of event types.
      // This way we save that cost of creating a new entry in the map and
      // its corresponding listener list when an event type seen before
      // shows up again.
      // Later we could add logic to check at non-time-critical points, if
      // the map has grown too big and prune it. (lazy evaluation).

      // mirror changes in the subscription list
      if (this->subscription_list_.remove (event_type) == 0)
        update.insert (event_type); // modify the update list.
    }
}

void
TAO_Notify_Event_Map::register_for_publication_updates (TAO_Notify_UpdateListener* update_listener ACE_ENV_ARG_DECL)
{
  this->publication_change_listeners_->connected (update_listener ACE_ENV_ARG_PARAMETER);
}

void
TAO_Notify_Event_Map::register_for_subscription_updates (TAO_Notify_UpdateListener* update_listener ACE_ENV_ARG_DECL)
{
  this->subscription_change_listeners_->connected (update_listener ACE_ENV_ARG_PARAMETER);
}

void
TAO_Notify_Event_Map:: unregister_from_subscription_updates (TAO_Notify_UpdateListener* update_listener ACE_ENV_ARG_DECL)
{
  this->subscription_change_listeners_->disconnected (update_listener ACE_ENV_ARG_PARAMETER);
}

void
TAO_Notify_Event_Map::unregister_from_publication_updates (TAO_Notify_UpdateListener* update_listener ACE_ENV_ARG_DECL)
{
  this->publication_change_listeners_->disconnected (update_listener ACE_ENV_ARG_PARAMETER);
}

CosNotification::EventTypeSeq*
TAO_Notify_Event_Map::obtain_offered_types (void)
{
  CosNotification::EventTypeSeq* event_type_seq;

  ACE_NEW_RETURN (event_type_seq,
                  CosNotification::EventTypeSeq(this->publication_list_.size ()),
                  0);

  this->publication_list_.populate (*event_type_seq);

  return event_type_seq;
}

CosNotification::EventTypeSeq*
TAO_Notify_Event_Map::obtain_subscription_types (void)
{
  CosNotification::EventTypeSeq* event_type_seq;

  ACE_NEW_RETURN (event_type_seq,
                  CosNotification::EventTypeSeq(this->subscription_list_.size ()),
                  0);

  this->subscription_list_.populate (*event_type_seq);

  return event_type_seq;
}

void
TAO_Notify_Event_Map::update_publication_list (const CosNotification::EventTypeSeq & added, const CosNotification::EventTypeSeq & removed, TAO_Notify_EventType_List &added_list, TAO_Notify_EventType_List &removed_list ACE_ENV_ARG_DECL_NOT_USED/*ACE_ENV_SINGLE_ARG_PARAMETER*/)
{
  TAO_Notify_EventType event_type;

  CORBA::ULong len = removed.length ();
  CORBA::ULong i = 0 ;
  for (; i < len; ++i)
    {
      event_type = removed[i];
      if (this->publication_list_.remove (event_type) == 0)
        {
          removed_list.insert (event_type);
        }
    }

  len = added.length ();
  for (i = 0; i < len; i++)
    {
      event_type = added[i];
      if (this->publication_list_.insert (event_type) == 0)
        {
          added_list.insert (event_type);
        }
    }
}
