/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Notification
//
// = FILENAME
//   Notify_Event_Map.h
//
// = DESCRIPTION
//   Stores information about subscription mappings
//
// = AUTHOR
//   Pradeep Gore <pradeep@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_NOTIFY_Event_Map_HEADER_H
#define TAO_NOTIFY_Event_Map_HEADER_H
#include "ace/pre.h"

#include "orbsvcs/CosNotificationC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Notify_Collection.h"
#include "notify_export.h"
#include "orbsvcs/ESF/ESF_Worker.h"

#include "ace/Hash_Map_Manager.h"

class TAO_Notify_EventListener;
class TAO_Notify_UpdateListener;
class TAO_Notify_EventType;
class TAO_Notify_Collection_Factory;
class TAO_Notify_EMO_Factory;

class TAO_Notify_Export TAO_Notify_Event_Map
{
  // = TITLE
  //   TAO_Notify_Event_Map
  //
  // = DESCRIPTION
  //   This is a compound container consisting of:
  //   - A "recipient" map : mapping between an event and its subscriibers
  //   - Lists of current events being subscribed, published.
  //   - Lists of subscriptions, publications update listeners.
  //
public:
  TAO_Notify_Event_Map (TAO_Notify_EMO_Factory* emo_factory);
  // Constructor.

  virtual ~TAO_Notify_Event_Map ();
  // Destructor.

  void init (ACE_ENV_SINGLE_ARG_DECL);
  // Init

  void shutdown (ACE_ENV_SINGLE_ARG_DECL);
  // Shutdown releases all listeners.

  // = Subscribe and Unsubscribe methods.
  void subscribe_for_events (TAO_Notify_EventListener* event_listener, TAO_Notify_EventType_List& update, const CosNotification::EventTypeSeq & added ACE_ENV_ARG_DECL);

  void unsubscribe_from_events (TAO_Notify_EventListener* event_listener, TAO_Notify_EventType_List &update, const CosNotification::EventTypeSeq & removed ACE_ENV_ARG_DECL);

  // = Publish and Unpublish methods
  // Later:
  void update_publication_list (const CosNotification::EventTypeSeq & added, const CosNotification::EventTypeSeq & removed, TAO_Notify_EventType_List &added_list, TAO_Notify_EventType_List &removed_list ACE_ENV_ARG_DECL);
  // Suppliers can send anonymous requests to the Event Manager to indicate
  // what kind of events they expect to produce.

  // = Subscription Updates
  void register_for_subscription_updates (TAO_Notify_UpdateListener* update_listener ACE_ENV_ARG_DECL);
  // Registers the subscription update listener with the Event Manager.

  void unregister_from_subscription_updates (TAO_Notify_UpdateListener* update_listener ACE_ENV_ARG_DECL);
  // Unregister from subscription updates.

  // = Publication Updates
  void register_for_publication_updates (TAO_Notify_UpdateListener* update_listener ACE_ENV_ARG_DECL);
  // Registers the publication update listener with the Event Manager.

  void unregister_from_publication_updates (TAO_Notify_UpdateListener* update_listener ACE_ENV_ARG_DECL);
  // Unregister from publication updates.

  // = Subscription list lookup
  int find (TAO_Notify_EventType const & event_type, TAO_Notify_EventListener_List*& event_listener_list);

  // = Accessors
  CosNotification::EventTypeSeq* obtain_offered_types(void);
  // Obtain the publication list.

  CosNotification::EventTypeSeq* obtain_subscription_types (void);
  // Obtain the subscription list.

  TAO_Notify_UpdateListener_List* subscription_change_listeners (void);
  TAO_Notify_UpdateListener_List* publication_change_listeners (void);
  TAO_Notify_EventListener_List* default_subscription_list (void);

protected:
  // = Typedefs
  typedef ACE_Hash_Map_Manager <TAO_Notify_EventType,
    TAO_Notify_EventListener_List*, TAO_SYNCH_MUTEX> EVENT_RECIPIENT_MAP;

  // = Data Members
  EVENT_RECIPIENT_MAP event_recipient_map_;
  // A Map of event types and the groups of event listeners interested in them.
  // The keys of the map are a list of events that consumers have currently
  // subscribed for.

  TAO_Notify_EventListener_List* default_subscription_list_;
  // Save a reference to the default list, we don't want to spend time
  // looking for it in the <event_recipient_map>.

  TAO_Notify_EventType_List publication_list_;
  // The list of event types that are being currently published by suppliers.

  TAO_Notify_EventType_List subscription_list_;
  // The list of event types that are being currently subscribed to by
  // consumers. This list is the same as the list of keys in the
  // <event_recepient_map>. We keep a copy of that list here to reduce
  // contention for the map which will be accessed for every event that
  // enters the system.

  TAO_Notify_UpdateListener_List* subscription_change_listeners_;
  // This is a list of listeners that want to be notified if the subsciptions
  // from consumers changes the <event_recipient_map_> keys.

  TAO_Notify_UpdateListener_List* publication_change_listeners_;
  // This is a list of listeners that want to be notified if the publications
  // from suppliers changes the <publication_list_>.

  TAO_Notify_EMO_Factory* emo_factory_;
  // Evenet Manager Objects factory.

  TAO_Notify_Collection_Factory* collection_factory_;
  // Collection objects factory.
};

/********************************************************************/

#if defined (__ACE_INLINE__)
#include "Notify_Event_Map.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NOTIFY_Event_Map_HEADER_H */
