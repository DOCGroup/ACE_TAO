// $Id$
#include "Notify_Event_Manager.h"
#include "Notify_EventChannel_i.h"

ACE_RCSID(Notify, Notify_Event_Manager, "$Id$")

TAO_Notify_Event_Manager::TAO_Notify_Event_Manager (TAO_Notify_EventChannel_i* event_channel)
  :event_channel_ (event_channel)
{
  // No-Op.
}

TAO_Notify_Event_Manager::~TAO_Notify_Event_Manager ()
{
  // No-Op.
}

void
TAO_Notify_Event_Manager::init (CORBA::Environment &/*ACE_TRY_ENV*/)
{
  // No-Op.
}

void
TAO_Notify_Event_Manager::subscribe_for_events (TAO_Notify_EventListener* event_listener, TAO_Notify_EventType_List* current, const CosNotification::EventTypeSeq & added, const CosNotification::EventTypeSeq & removed, CORBA::Environment &ACE_TRY_ENV)
{
  TAO_Notify_EventType_List added_update, removed_update;

  this->subscribe_for_events_i (event_listener, current, added_update,
                                added, ACE_TRY_ENV);
  ACE_CHECK;

  this->unsubscribe_from_events_i (event_listener, current, removed_update,
                                   removed, ACE_TRY_ENV);
  ACE_CHECK;

  // if either of the lists are *not* empty, send updates
  if (added_update.is_empty () == 0 || removed_update.is_empty () == 0)
    this->dispatch_updates_i (this->subscription_change_listeners_,
                              added_update, removed_update, ACE_TRY_ENV);
}

void
TAO_Notify_Event_Manager::subscribe_for_events (TAO_Notify_EventListener_List& event_listener_list, TAO_Notify_EventType_List* current, const CosNotification::EventTypeSeq & added, const CosNotification::EventTypeSeq & removed, CORBA::Environment &ACE_TRY_ENV)
{
  TAO_Notify_EventType_List added_update, removed_update;

  this->subscribe_for_events_i (event_listener_list, current, added_update,
                                added, ACE_TRY_ENV);
  ACE_CHECK;

  this->unsubscribe_from_events_i (event_listener_list, current,
                                   removed_update, removed, ACE_TRY_ENV);
  ACE_CHECK;

  // if either of the lists are *not* empty, send updates
  if (added_update.is_empty () == 0 || removed_update.is_empty () == 0)
    this->dispatch_updates_i (this->subscription_change_listeners_,
                              added_update, removed_update, ACE_TRY_ENV);
}

void
TAO_Notify_Event_Manager::subscribe_for_events_i (TAO_Notify_EventListener* event_listener, TAO_Notify_EventType_List* current, TAO_Notify_EventType_List& update, const CosNotification::EventTypeSeq & added, CORBA::Environment &ACE_TRY_ENV)
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
        event_listener_list = &default_subscription_list_;
      else if (this->event_recipient_map_.find (event_type,
                                                event_listener_list) == -1)
        {
          // create the list.
          TAO_Notify_EventListener_List* new_list;

          ACE_NEW_THROW_EX (new_list,
                            TAO_Notify_EventListener_List (),
                            CORBA::NO_MEMORY ());

          // add the list to the recipient map.
          this->event_recipient_map_.bind (event_type, new_list);
          event_listener_list = new_list;
        }

      event_listener_list->insert (event_listener);

      // mirror changes in the subscription list
      if (this->subscription_list_.insert (event_type) == 0)
        update.insert (event_type);

      // update the caller's current subscription list
      if (current != 0)
        current->insert (event_type);
    }
}

void
TAO_Notify_Event_Manager::subscribe_for_events_i (TAO_Notify_EventListener_List& list_to_add, TAO_Notify_EventType_List* current, TAO_Notify_EventType_List& update, const CosNotification::EventTypeSeq & added, CORBA::Environment &ACE_TRY_ENV)
{
  TAO_Notify_EventListener_List* event_listener_list;
  TAO_Notify_EventType event_type;

  for (CORBA::ULong index = 0; index < added.length (); index++)
    {
      event_type = added[index];

      if (event_type.is_special ())
        event_listener_list = &default_subscription_list_;
      else if (this->event_recipient_map_.find (event_type,
                                                event_listener_list) == -1)
        {
          // create the list.
          TAO_Notify_EventListener_List* new_list;

          ACE_NEW_THROW_EX (new_list,
                            TAO_Notify_EventListener_List (),
                            CORBA::NO_MEMORY ());

          // add the list to the recipient map.
          this->event_recipient_map_.bind (event_type, new_list);
          event_listener_list = new_list;
        }

      TAO_Notify_EventListener_List::ITERATOR iter (list_to_add);

      TAO_Notify_EventListener** event_listener;

      for (iter.first (); iter.next (event_listener); iter.advance ())
        {
#if 0
          ACE_DEBUG ((LM_DEBUG, "subscribing %x for event: %s, %s\n",
                      *event_listener,
                      event_type.event_type_.domain_name.in (),
                      event_type.event_type_.type_name.in ()));
#endif
          ACE_ASSERT (*event_listener != 0);
          event_listener_list->insert (*event_listener);
        }

      // mirror changes in the subscription list
      if (this->subscription_list_.insert (event_type) == 0)
        update.insert (event_type);

      // update the caller's current subscription list
      if (current != 0)
        current->insert (event_type);
    }
}

void
TAO_Notify_Event_Manager::unsubscribe_from_events_i (TAO_Notify_EventListener* event_listener, TAO_Notify_EventType_List* current, TAO_Notify_EventType_List &update, const CosNotification::EventTypeSeq & removed, CORBA::Environment &/*ACE_TRY_ENV*/)
{
  TAO_Notify_EventListener_List* event_listener_list;
  TAO_Notify_EventType event_type;

  for (CORBA::ULong index = 0; index < removed.length (); index++)
    {
      event_type = removed[index];

      // find out if there is a event listener list for this event type.
      if (event_type.is_special ())
        event_listener_list = &this->default_subscription_list_;
      else if (this->event_recipient_map_.find (event_type,
                                                event_listener_list) == -1)
        continue;

      // remove <event_listener> from the list.

      event_listener_list->remove (event_listener);

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

      if (current != 0)
        current->remove (event_type);
    }
}

void
TAO_Notify_Event_Manager::unsubscribe_from_events_i (TAO_Notify_EventListener_List& list_to_add, TAO_Notify_EventType_List* current, TAO_Notify_EventType_List &update, const CosNotification::EventTypeSeq & removed, CORBA::Environment &/*ACE_TRY_ENV*/)
{
  TAO_Notify_EventListener_List* event_listener_list;
  TAO_Notify_EventType event_type;

  for (CORBA::ULong index = 0; index < removed.length (); index++)
    {
      event_type = removed[index];

      // find out if there is a event listener list for this event type.
      if (event_type.is_special ())
        event_listener_list = &this->default_subscription_list_;
      else if (this->event_recipient_map_.find (event_type,
                                                event_listener_list) == -1)
        continue;

      TAO_Notify_EventListener_List::ITERATOR iter (list_to_add);

      TAO_Notify_EventListener** event_listener;

      for (iter.first (); iter.next (event_listener); iter.advance ())
        {
          ACE_ASSERT (*event_listener != 0);
          // remove <event_listener> from the list.
          event_listener_list->remove (*event_listener);
        }

      // mirror changes in the subscription list
      if (this->subscription_list_.remove (event_type) == 0)
        update.insert (event_type); // modify the update list.

      if (current != 0)
        current->remove (event_type);
    }
}

void
TAO_Notify_Event_Manager::push (TAO_Notify_Event& event, CORBA::Environment &ACE_TRY_ENV)
{
  // If the event is *not* the special event
  // send it to the list that matches it.
  // In any case send it to the default list.
#if 0
  ACE_DEBUG ((LM_DEBUG, "finding a match for event: %s, %s\n",
              event.event_type ().event_type_.domain_name.in (),
              event.event_type ().event_type_.type_name.in ()));
#endif
  if (!event.is_special_event_type ())
    {
      TAO_Notify_EventListener_List* subscription_list;
      // find the subscription list for <event_type>
      if (this->event_recipient_map_.find (event.event_type (),
                                           subscription_list) == 0)
        {
          this->dispatch_event_i (event, subscription_list, ACE_TRY_ENV);
        }
    }
  // Those subscribed for the default events get everything.
  if (this->default_subscription_list_.is_empty () == 0)
    {
      this->dispatch_event_i (event,
                              &default_subscription_list_, ACE_TRY_ENV);
      ACE_CHECK;
    }
}

void
TAO_Notify_Event_Manager::dispatch_event_i (TAO_Notify_Event &event, TAO_Notify_EventListener_List* proxy_list, CORBA::Environment &ACE_TRY_ENV)
{
  TAO_Notify_EventListener_List::ITERATOR iter (*proxy_list);

  TAO_Notify_EventListener** event_listener;

  for (iter.first (); iter.next (event_listener); iter.advance ())
    {
      ACE_ASSERT (*event_listener != 0);
      (*event_listener)->dispatch_event (event, ACE_TRY_ENV);
    }
}

void
TAO_Notify_Event_Manager::dispatch_updates_i (TAO_Notify_UpdateListener_List& update_listener_list, TAO_Notify_EventType_List& added, TAO_Notify_EventType_List& removed, CORBA::Environment &ACE_TRY_ENV)
{
  TAO_Notify_UpdateListener_List::ITERATOR iter (update_listener_list);

  TAO_Notify_UpdateListener** update_listener;

  for (iter.first (); iter.next (update_listener); iter.advance ())
    {
      (*update_listener)->dispatch_update (added, removed, ACE_TRY_ENV);
    }
}

void
TAO_Notify_Event_Manager::register_for_publication_updates (TAO_Notify_UpdateListener* update_listener, CORBA::Environment& /*ACE_TRY_ENV*/)
{
  this->publication_change_listeners_.insert (update_listener);
}

void
TAO_Notify_Event_Manager::register_for_subscription_updates (TAO_Notify_UpdateListener* update_listener, CORBA::Environment& /*ACE_TRY_ENV*/)
{
  this->subscription_change_listeners_.insert (update_listener);
}

void
TAO_Notify_Event_Manager:: unregister_from_subscription_updates (TAO_Notify_UpdateListener* update_listener, CORBA::Environment& /*ACE_TRY_ENV*/)
{
  this->subscription_change_listeners_.remove (update_listener);
}

void
TAO_Notify_Event_Manager::unregister_from_publication_updates (TAO_Notify_UpdateListener* update_listener, CORBA::Environment& /*ACE_TRY_ENV*/)
{
  this->publication_change_listeners_.remove (update_listener);
}

CosNotification::EventTypeSeq*
TAO_Notify_Event_Manager::obtain_offered_types (void)
{
  CosNotification::EventTypeSeq* event_type_seq;

  ACE_NEW_RETURN (event_type_seq,
                  CosNotification::EventTypeSeq(this->publication_list_.size ()),
                  0);

  this->publication_list_.populate (*event_type_seq);

  return event_type_seq;
}

CosNotification::EventTypeSeq*
TAO_Notify_Event_Manager::obtain_subscription_types (void)
{
  CosNotification::EventTypeSeq* event_type_seq;

  ACE_NEW_RETURN (event_type_seq,
                  CosNotification::EventTypeSeq(this->subscription_list_.size ()),
                  0);

  this->subscription_list_.populate (*event_type_seq);

  return event_type_seq;
}

void
TAO_Notify_Event_Manager::update_publication_list (const CosNotification::EventTypeSeq & added, const CosNotification::EventTypeSeq & removed, CORBA::Environment &ACE_TRY_ENV)
{
  TAO_Notify_EventType event_type;

  TAO_Notify_EventType_List added_list;
  TAO_Notify_EventType_List removed_list;

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

  // if either of the lists are *not* empty, send updates
  if (added_list.is_empty () == 0 || removed_list.is_empty () == 0)
    this->dispatch_updates_i (this->publication_change_listeners_,
                              added_list, removed_list, ACE_TRY_ENV);
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Hash_Map_Entry<TAO_Notify_EventType, TAO_Notify_EventListener_List *>;
template class ACE_Hash_Map_Manager<TAO_Notify_EventType, TAO_Notify_EventListener_List *,ACE_SYNCH_MUTEX>;
template class ACE_Hash_Map_Manager_Ex<TAO_Notify_EventType, TAO_Notify_EventListener_List *,ACE_Hash<TAO_Notify_EventType>, ACE_Equal_To<TAO_Notify_EventType>,ACE_SYNCH_MUTEX>;
template class ACE_Hash_Map_Iterator<TAO_Notify_EventType, TAO_Notify_EventListener_List *,ACE_SYNCH_MUTEX>;
template class ACE_Hash_Map_Iterator_Ex<TAO_Notify_EventType, TAO_Notify_EventListener_List *,ACE_Hash<TAO_Notify_EventType>, ACE_Equal_To<TAO_Notify_EventType>,ACE_SYNCH_MUTEX>;
template class ACE_Hash_Map_Iterator_Base_Ex<TAO_Notify_EventType, TAO_Notify_EventListener_List *,ACE_Hash<TAO_Notify_EventType>, ACE_Equal_To<TAO_Notify_EventType>,ACE_SYNCH_MUTEX>;
template class ACE_Hash_Map_Reverse_Iterator<TAO_Notify_EventType, TAO_Notify_EventListener_List *,ACE_SYNCH_MUTEX>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<TAO_Notify_EventType, TAO_Notify_EventListener_List *,ACE_Hash<TAO_Notify_EventType>, ACE_Equal_To<TAO_Notify_EventType>,ACE_SYNCH_MUTEX>;
template class ACE_Hash<TAO_Notify_EventType>;
template class ACE_Equal_To<TAO_Notify_EventType>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Hash_Map_Entry<TAO_Notify_EventType, TAO_Notify_EventListener_List *>
#pragma instantiate ACE_Hash_Map_Manager<TAO_Notify_EventType, TAO_Notify_EventListener_List *,ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Manager_Ex<TAO_Notify_EventType, TAO_Notify_EventListener_List *,ACE_Hash<TAO_Notify_EventType>, ACE_Equal_To<TAO_Notify_EventType>,ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator<TAO_Notify_EventType, TAO_Notify_EventListener_List *,ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<TAO_Notify_EventType, TAO_Notify_EventListener_List *,ACE_Hash<TAO_Notify_EventType>, ACE_Equal_To<TAO_Notify_EventType>,ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<TAO_Notify_EventType, TAO_Notify_EventListener_List *,ACE_Hash<TAO_Notify_EventType>, ACE_Equal_To<TAO_Notify_EventType>,ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<TAO_Notify_EventType, TAO_Notify_EventListener_List *,ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<TAO_Notify_EventType, TAO_Notify_EventListener_List *,ACE_Hash<TAO_Notify_EventType>, ACE_Equal_To<TAO_Notify_EventType>,ACE_SYNCH_MUTEX>
#pragma instantiate template ACE_Hash<TAO_Notify_EventType>
#pragma instantiate template ACE_Equal_To<TAO_Notify_EventType>
#endif /*ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
