// $Id$
// ==========================================================================
//
// = LIBRARY
//   Orbsvcs
//
// = FILENAME
//   Notify_Event_Manager.h
//
// = DESCRIPTION
//   An Event Manager for the Notification Service.
//
// = AUTHOR
//    Pradeep Gore <pradeep@cs.wustl.edu>
//
// ==========================================================================

#ifndef TAO_NOTIFY_EVENT_MANAGER
#define TAO_NOTIFY_EVENT_MANAGER
#include "ace/pre.h"

#include "Notify_Listeners.h"
#include "Notify_Types.h"
#include "ace/Containers_T.h"

class TAO_Notify_EventChannel_i;

class TAO_Notify_Event_Manager
{
  // = TITLE
  //   TAO_Notify_Event_Manager
  //
  // = DESCRIPTION
  //   Handles all aspects of event propogation thru the channel.
  //   Also keeps track of publications and subscriptions.

 public:
  // = Initialization and termination code.
  TAO_Notify_Event_Manager (TAO_Notify_EventChannel_i* parent);
  // Constructor.

  ~TAO_Notify_Event_Manager ();
  // Destructor.

  void init (CORBA::Environment &ACE_TRY_ENV);
  // Init

  // = Publish/Subscribe management
  // = Subscription
  void subscribe_for_events (TAO_Notify_Event_Listener* event_listener, EVENTTYPE_LIST* current, const CosNotification::EventTypeSeq & added, const CosNotification::EventTypeSeq & removed, CORBA::Environment &ACE_TRY_ENV);
  // Subscribes <event_listener> for events <added>.
  // Unsubscribes <event_listener> for events <removed>.
  // Updates the <current> subscription.

  void subscribe_for_events (EVENT_LISTENER_LIST& event_listener_list, EVENTTYPE_LIST* current, const CosNotification::EventTypeSeq & added, const CosNotification::EventTypeSeq & removed, CORBA::Environment &ACE_TRY_ENV);
  // Subscribes the whole group.

  // = Publications
  void update_publication_list (const CosNotification::EventTypeSeq & added, const CosNotification::EventTypeSeq & removed, CORBA::Environment &ACE_TRY_ENV);
  // Suppliers can send anonymous requests to the Event Manager to indicate
  // what kind of events they expect to produce.

  // = Updates
  void unregister_from_subscription_updates (TAO_Notify_Update_Listener* update_listener, CORBA::Environment &ACE_TRY_ENV);
  // Unregister from subscription updates.

  void register_for_subscription_updates (TAO_Notify_Update_Listener* update_listener, CORBA::Environment &ACE_TRY_ENV);
  // Registers the subscription update listener with the Event Manager.

  void register_for_publication_updates (TAO_Notify_Update_Listener* update_listener, CORBA::Environment &ACE_TRY_ENV);
  // Registers the publication update listener with the Event Manager.

  void unregister_from_publication_updates (TAO_Notify_Update_Listener* update_listener, CORBA::Environment &ACE_TRY_ENV);
  // Unregister from publication updates.

  CosNotification::EventTypeSeq* obtain_offered_types(void);
  // Obtain the publication list.

  CosNotification::EventTypeSeq* obtain_subscription_types (void);
  // Obtain the subscription list.

  // = Event forwarding methods.
  void push (TAO_Notify_Event& event, CORBA::Environment &ACE_TRY_ENV);
  // Delivers the event to listeners subscribed for <event>

 protected:
  // = Helpers
  void subscribe_for_events_i (TAO_Notify_Event_Listener* event_listener, EVENTTYPE_LIST* current, EVENTTYPE_LIST& update, const CosNotification::EventTypeSeq & added, CORBA::Environment &ACE_TRY_ENV);

  void subscribe_for_events_i (EVENT_LISTENER_LIST& event_listener_list, EVENTTYPE_LIST* current, EVENTTYPE_LIST& update, const CosNotification::EventTypeSeq & added, CORBA::Environment &ACE_TRY_ENV);

  void unsubscribe_from_events_i (TAO_Notify_Event_Listener* event_listener, EVENTTYPE_LIST* current, EVENTTYPE_LIST &update, const CosNotification::EventTypeSeq & removed, CORBA::Environment &ACE_TRY_ENV);

  void unsubscribe_from_events_i (EVENT_LISTENER_LIST& event_listener_list, EVENTTYPE_LIST* current, EVENTTYPE_LIST &update, const CosNotification::EventTypeSeq & removed, CORBA::Environment &ACE_TRY_ENV);

  // = Event and updates dispatching methods.
  void dispatch_event_i (TAO_Notify_Event& event, EVENT_LISTENER_LIST* event_listener_list, CORBA::Environment &ACE_TRY_ENV);
  // Dispatches <event> to the list.

  void dispatch_updates_i (UPDATE_LISTENER_LIST& update_listener_list, EVENTTYPE_LIST& added, EVENTTYPE_LIST& removed, CORBA::Environment &ACE_TRY_ENV);
  // Dispatch the updates to the <update_listener_list>

  // = Typedefs
  typedef ACE_Hash_Map_Manager <TAO_Notify_EventType, EVENT_LISTENER_LIST*, ACE_SYNCH_MUTEX> EVENT_RECIPIENT_MAP;

  // = Data Members
  TAO_Notify_EventChannel_i* event_channel_;
  // The Event Channel that we're managing for.

  EVENT_RECIPIENT_MAP event_recipient_map_;
  // A Map of event types and the groups of event listeners interested in them.
  // The keys of the map are a list of events that consumers have currently
  // subscribed for.

  EVENT_LISTENER_LIST default_subscription_list_;
  // Save a reference to the default list, we don't want to spend time
  // looking for it in the <event_recipient_map>.

  EVENTTYPE_LIST publication_list_;
  // The list of event types that are being currently published by suppliers.

  EVENTTYPE_LIST subscription_list_;
  // The list of event types that are being currently subscribed to by
  // consumers. This list is the same as the list of keys in the
  // <event_recepient_map>. We keep a copy of that list here to reduce
  // contention for the map which will be accessed for every event that
  // enters the system.

  UPDATE_LISTENER_LIST subscription_change_listeners_;
  // This is a list of listeners that want to be notified if the subsciptions
  // from consumers changes the <event_recipient_map_> keys.

  UPDATE_LISTENER_LIST publication_change_listeners_;
  // This is a list of listeners that want to be notified if the publications
  // from suppliers changes the <publication_list_>.
};

#include "ace/post.h"
#endif /* TAO_NOTIFY_EVENT_MANAGER */
