// $Id$
#include "Notify_Event_Manager.h"
#include "Notify_EventChannel_i.h"

TAO_Notify_Event_Manager::TAO_Notify_Event_Manager (TAO_Notify_EventChannel_i* event_channel)
  :event_channel_ (event_channel),
   default_event_type_ ("*", "*"),
   default_subscription_list_ (0)
{
  // No-Op.
}

TAO_Notify_Event_Manager::~TAO_Notify_Event_Manager ()
{
  // No-Op.
}

void
TAO_Notify_Event_Manager::init (CORBA::Environment &ACE_TRY_ENV)
{
  // create the default list.
  ACE_NEW_THROW_EX (this->default_subscription_list_,
                    EVENT_LISTENER_LIST (),
                    CORBA::NO_MEMORY ());
  // add the list to the recipient map.
  this->event_recipient_map_.bind (this->default_event_type_,
                                   this->default_subscription_list_);
}

void
TAO_Notify_Event_Manager::subscribe_for_default_events (TAO_Notify_Event_Listener* event_listener, CORBA::Environment &/*ACE_TRY_ENV*/)
{
  this->default_subscription_list_->insert (event_listener);
}

void
TAO_Notify_Event_Manager::subscribe_for_events (TAO_Notify_Event_Listener* event_listener, EVENTTYPE_LIST& initial, CORBA::Environment &ACE_TRY_ENV)
{
  EVENTTYPE_LIST added_ret, removed_ret;

  this->process_added_list_i (event_listener, initial, added_ret, ACE_TRY_ENV);
  ACE_CHECK;

  // if the returned list is *not* empty, send updates
  if (added_ret.is_empty () == 0)
    this->dispatch_updates_i (this->subscription_change_listeners_,
                              added_ret, removed_ret, ACE_TRY_ENV);
}

void
TAO_Notify_Event_Manager::update_subscription_for_events (TAO_Notify_Event_Listener* event_listener, EVENTTYPE_LIST& added, EVENTTYPE_LIST& removed, CORBA::Environment &ACE_TRY_ENV)
{
  EVENTTYPE_LIST added_ret, removed_ret;

  this->process_added_list_i (event_listener, added, added_ret, ACE_TRY_ENV);
  this->process_removed_list_i (event_listener, removed, removed_ret,
                                ACE_TRY_ENV);

  // if either of the lists are *not* empty, send updates
  if (added_ret.is_empty () == 0 || removed_ret.is_empty () == 0)
    this->dispatch_updates_i (this->subscription_change_listeners_,
                              added_ret, removed_ret, ACE_TRY_ENV);
}

void
TAO_Notify_Event_Manager::process_added_list_i (TAO_Notify_Event_Listener* event_listener, EVENTTYPE_LIST& added, EVENTTYPE_LIST& added_ret, CORBA::Environment &ACE_TRY_ENV)
{
  EVENT_LISTENER_LIST* event_listener_list;
  TAO_Notify_EventType* event_type;

  // iterate over the <added> list first.
  EVENTTYPE_LIST_ITER iter (added);

  for (iter.first (); iter.next (event_type) == 1; iter.advance ())
    {
      if (this->event_recipient_map_.find (*event_type,
                                           event_listener_list) == -1)
        {
          // create the list.
          EVENT_LISTENER_LIST* new_list;

          ACE_NEW_THROW_EX (new_list,
                            EVENT_LISTENER_LIST (),
                            CORBA::NO_MEMORY ());

          // insert the listener in the list
          new_list->insert (event_listener);

          // add the list to the recipient map.
          this->event_recipient_map_.bind (*event_type, new_list);
        }
      else
        event_listener_list->insert (event_listener);

      // mirror changes in the subscription list
      if (this->subscription_list_.insert (*event_type) == 0)
        added_ret.insert (*event_type);
    } // for
}

void
TAO_Notify_Event_Manager::process_removed_list_i (TAO_Notify_Event_Listener* event_listener, EVENTTYPE_LIST& removed, EVENTTYPE_LIST& removed_ret, CORBA::Environment &/*ACE_TRY_ENV*/)
{
  EVENT_LISTENER_LIST* event_listener_list;
  TAO_Notify_EventType* event_type;

  // iterate over the <removed> list first.
  EVENTTYPE_LIST_ITER iter (removed);

  for (iter.first (); iter.next (event_type) == 1; iter.advance ())
    {
      // find out if there is a event listener list for this event type.
      if (this->event_recipient_map_.find (*event_type,
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
      if (this->subscription_list_.remove (*event_type) == 0)
        removed_ret.insert (*event_type);
    }
}

void
TAO_Notify_Event_Manager::unsubscribe_from_events (TAO_Notify_Event_Listener* event_listener, CORBA::Environment &ACE_TRY_ENV)
{
  // Go through all the lists in the <recipient_map_> and remove
  // <event_listener> from each list.
  // TODO: keep a reverse map to tell which lists we belong to.?

  ACE_GUARD (ACE_SYNCH_MUTEX, ace_mon, event_recipient_map_.mutex ());
  ACE_CHECK;

  // Create an iterator
  EVENT_RECIPIENT_MAP::ITERATOR iter (event_recipient_map_);

  // Iterate over and populate the list.
  EVENT_RECIPIENT_MAP::ENTRY *hash_entry;

  CORBA::ULong len = event_recipient_map_.current_size ();

    for (CORBA::ULong i = 0; i < len; i++)
    {
      if (iter.next (hash_entry) == 1)
        hash_entry->int_id_->remove (event_listener);

      iter.advance ();
    }
}

void
TAO_Notify_Event_Manager::push (const CORBA::Any & data, CORBA::Environment &ACE_TRY_ENV)
{
  if (this->default_subscription_list_->is_empty () == 0)
    {
      this->dispatch_event (data,
                            default_subscription_list_, ACE_TRY_ENV);
      ACE_CHECK;
    }
}

void
TAO_Notify_Event_Manager::push (const CosNotification::StructuredEvent & notification, CORBA::Environment &ACE_TRY_ENV)
{
  TAO_Notify_EventType event_type;
  event_type = notification.header.fixed_header.event_type;

  EVENT_LISTENER_LIST* subscription_list;
  // find the subscription list for <event_type>
  if (this->event_recipient_map_.find (event_type,
                                       subscription_list) == 0)
    {
      this->dispatch_event (notification, subscription_list, ACE_TRY_ENV);
    }

  // Those subscribed for the default events get everything.
  if (this->default_subscription_list_->is_empty () == 0)
    {
      this->dispatch_event (notification,
                            default_subscription_list_, ACE_TRY_ENV);
      ACE_CHECK;
    }
}

void
TAO_Notify_Event_Manager::dispatch_event (const CORBA::Any & data, EVENT_LISTENER_LIST* proxy_list, CORBA::Environment &ACE_TRY_ENV)
{
  EVENT_LISTENER_LIST_ITER iter (*proxy_list);

  TAO_Notify_Event_Listener** event_listener; // don't be afraid...

  for (iter.first (); iter.next (event_listener); iter.advance ())
    {
      (*event_listener)->dispatch_event (data, ACE_TRY_ENV);
    }
}

void
TAO_Notify_Event_Manager::dispatch_event (const CosNotification::StructuredEvent & notification, EVENT_LISTENER_LIST* proxy_list, CORBA::Environment &ACE_TRY_ENV)
{
  EVENT_LISTENER_LIST_ITER iter (*proxy_list);

  TAO_Notify_Event_Listener** event_listener; // don't be afraid...

  for (iter.first (); iter.next (event_listener); iter.advance ())
    {
      (*event_listener)->dispatch_event (notification, ACE_TRY_ENV);
    }
}

void
TAO_Notify_Event_Manager::dispatch_updates_i (UPDATE_LISTENER_LIST& update_listener_list, EVENTTYPE_LIST& added, EVENTTYPE_LIST& removed, CORBA::Environment &/*ACE_TRY_ENV*/)
{
  UPDATE_LISTENER_LIST_ITER iter (update_listener_list);

  TAO_Notify_Update_Listener** update_listener;

  for (iter.first (); iter.next (update_listener); iter.advance ())
    {
      (*update_listener)->dispatch_update (added, removed);//, ACE_TRY_ENV);
    }
}

void
TAO_Notify_Event_Manager::register_for_publication_updates (TAO_Notify_Update_Listener* update_listener, CORBA::Environment& /*ACE_TRY_ENV*/)
{
  this->publication_change_listeners_.insert (update_listener);
}

void
TAO_Notify_Event_Manager::register_for_subscription_updates (TAO_Notify_Update_Listener* update_listener, CORBA::Environment& /*ACE_TRY_ENV*/)
{
  this->subscription_change_listeners_.insert (update_listener);
}

void
TAO_Notify_Event_Manager:: unregister_from_subscription_updates (TAO_Notify_Update_Listener* update_listener, CORBA::Environment& /*ACE_TRY_ENV*/)
{
  this->subscription_change_listeners_.remove (update_listener);
}

void
TAO_Notify_Event_Manager::unregister_from_publication_updates (TAO_Notify_Update_Listener* update_listener, CORBA::Environment& /*ACE_TRY_ENV*/)
{
  this->publication_change_listeners_.remove (update_listener);
}

void
TAO_Notify_Event_Manager::update_publication_list (const CosNotification::EventTypeSeq & added, const CosNotification::EventTypeSeq & removed, CORBA::Environment &ACE_TRY_ENV)
{
  TAO_Notify_EventType event_type;

  EVENTTYPE_LIST added_list;
  EVENTTYPE_LIST removed_list;

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
      if (this->subscription_list_.insert (event_type) == 0)
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

template class ACE_Hash_Map_Entry<TAO_Notify_EventType, EVENT_LISTENER_LIST *>;
template class ACE_Hash_Map_Manager<TAO_Notify_EventType, EVENT_LISTENER_LIST *,ACE_SYNCH_MUTEX>;
template class ACE_Hash_Map_Manager_Ex<TAO_Notify_EventType, EVENT_LISTENER_LIST *,ACE_Hash<TAO_Notify_EventType>, ACE_Equal_To<TAO_Notify_EventType>,ACE_SYNCH_MUTEX>;
template class ACE_Hash_Map_Iterator<TAO_Notify_EventType, EVENT_LISTENER_LIST *,ACE_SYNCH_MUTEX>;
template class ACE_Hash_Map_Iterator_Ex<TAO_Notify_EventType, EVENT_LISTENER_LIST *,ACE_Hash<TAO_Notify_EventType>, ACE_Equal_To<TAO_Notify_EventType>,ACE_SYNCH_MUTEX>;
template class ACE_Hash_Map_Iterator_Base_Ex<TAO_Notify_EventType, EVENT_LISTENER_LIST *,ACE_Hash<TAO_Notify_EventType>, ACE_Equal_To<TAO_Notify_EventType>,ACE_SYNCH_MUTEX>;
template class ACE_Hash_Map_Reverse_Iterator<TAO_Notify_EventType, EVENT_LISTENER_LIST *,ACE_SYNCH_MUTEX>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<TAO_Notify_EventType, EVENT_LISTENER_LIST *,ACE_Hash<TAO_Notify_EventType>, ACE_Equal_To<TAO_Notify_EventType>,ACE_SYNCH_MUTEX>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Hash_Map_Entry<TAO_Notify_EventType, EVENT_LISTENER_LIST *>
#pragma instantiate ACE_Hash_Map_Manager<TAO_Notify_EventType, EVENT_LISTENER_LIST *,ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Manager_Ex<TAO_Notify_EventType, EVENT_LISTENER_LIST *,ACE_Hash<TAO_Notify_EventType>, ACE_Equal_To<TAO_Notify_EventType>,ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator<TAO_Notify_EventType, EVENT_LISTENER_LIST *,ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<TAO_Notify_EventType, EVENT_LISTENER_LIST *,ACE_Hash<TAO_Notify_EventType>, ACE_Equal_To<TAO_Notify_EventType>,ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<TAO_Notify_EventType, EVENT_LISTENER_LIST *,ACE_Hash<TAO_Notify_EventType>, ACE_Equal_To<TAO_Notify_EventType>,ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<TAO_Notify_EventType, EVENT_LISTENER_LIST *,ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<TAO_Notify_EventType, EVENT_LISTENER_LIST *,ACE_Hash<TAO_Notify_EventType>, ACE_Equal_To<TAO_Notify_EventType>,ACE_SYNCH_MUTEX>

#endif /*ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
