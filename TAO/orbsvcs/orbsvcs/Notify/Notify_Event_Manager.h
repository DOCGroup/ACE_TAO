/* -*- C++ -*- */
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
#include "orbsvcs/CosNotificationC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "notify_export.h"
#include "Notify_Listeners.h"
#include "Notify_Collection.h"

class TAO_Notify_EventChannel_i;
class TAO_Notify_Event;
class TAO_Notify_Event_Processor;
class TAO_Notify_Event_Map;
class TAO_Notify_Update_Dispatcher;
class TAO_Notify_EventSource;
class TAO_Notify_EventListener;
class TAO_Notify_Worker_Task;
class TAO_Notify_EMO_Factory;
class TAO_Notify_Event_Processor;
class TAO_Notify_AdminProperties;

class TAO_Notify_Export TAO_Notify_Event_Manager
{
  // = TITLE
  //   TAO_Notify_Event_Manager
  //
  // = DESCRIPTION
  //   Handles all aspects of event propogation thru the channel.
  //   Also keeps track of publications and subscriptions.

 public:
  // = Initialization and termination code.
  TAO_Notify_Event_Manager (TAO_Notify_EventChannel_i* parent,
                            TAO_Notify_EMO_Factory* emo_factory);
  // Constructor.

  ~TAO_Notify_Event_Manager ();
  // Destructor.

  void init (TAO_ENV_SINGLE_ARG_DECL);
  // Init

  void shutdown (TAO_ENV_SINGLE_ARG_DECL);
  // Shutdown operations.

  // = Publish/Subscribe management
  // = Subscription
  void subscribe_for_events (TAO_Notify_EventListener* event_listener, const CosNotification::EventTypeSeq & added, const CosNotification::EventTypeSeq & removed TAO_ENV_ARG_DECL);
  // Subscribes <event_listener> for events <added>.
  // Unsubscribes <event_listener> for events <removed>.

  // = Publications
  void update_publication_list (const CosNotification::EventTypeSeq & added, const CosNotification::EventTypeSeq & removed TAO_ENV_ARG_DECL);
  // Suppliers can send anonymous requests to the Event Manager to indicate
  // what kind of events they expect to produce.

  // = Updates
  void register_for_subscription_updates (TAO_Notify_UpdateListener* update_listener TAO_ENV_ARG_DECL);
  // Registers the subscription update listener with the Event Manager.

  void unregister_from_subscription_updates (TAO_Notify_UpdateListener* update_listener TAO_ENV_ARG_DECL);
  // Unregister from subscription updates.

  void register_for_publication_updates (TAO_Notify_UpdateListener* update_listener TAO_ENV_ARG_DECL);
  // Registers the publication update listener with the Event Manager.

  void unregister_from_publication_updates (TAO_Notify_UpdateListener* update_listener TAO_ENV_ARG_DECL);
  // Unregister from publication updates.

  // = Accessors
  CosNotification::EventTypeSeq* obtain_offered_types (void);
  // Obtain the publication list.

  CosNotification::EventTypeSeq* obtain_subscription_types (void);
  // Obtain the subscription list.

  TAO_Notify_Event_Map* event_map (void);
  // Get the event map.

  TAO_Notify_Event_Processor* event_processor (void);
  // Get the Event Processor.

  TAO_Notify_AdminProperties* admin_properties (void);
  // Get the Admin Properties.

  TAO_Notify_EMO_Factory* resource_factory (void);
  // Get the Resource Factory.

  // = Event forwarding methods.
  void process_event (TAO_Notify_Event* event,
                      TAO_Notify_EventSource* event_source
                      TAO_ENV_ARG_DECL);
  // Delivers the event to listeners subscribed for <event>
  // <event_source> is the <event> source to the Event Manager.

  void update_task_admins (void);

protected:
  // = Event dispatching methods.
  void dispatch_updates_i (TAO_Notify_UpdateListener_List* update_listener_list,
                           TAO_Notify_EventType_List& added,
                           TAO_Notify_EventType_List& removed
                           TAO_ENV_ARG_DECL);
  // Dispatch the updates to the <update_listener_list>

  // = Data members.
  TAO_Notify_EventChannel_i * event_channel_;
  // The Event Channel that we're managing for.

  TAO_Notify_Event_Map* event_map_;
  // Container for event <-> source/sinks mappings.

  TAO_Notify_Event_Processor * event_processor_;
  // Handles processing of events.

  TAO_Notify_Worker_Task * updates_dispatching_task_;
  // Dispatches updates to update listeners.

  TAO_Notify_EMO_Factory* emo_factory_;
  // Event manager objects factory.

  ACE_Lock* lock_;
  // The locking strategy.

  TAO_Notify_AdminProperties* admin_properties_;
  // Admin properties.
};

/**************************************************************************/

class TAO_Notify_Export TAO_Notify_Update_Worker : public TAO_ESF_Worker<TAO_Notify_UpdateListener>
{
  // = TITLE
  //   TAO_Notify_Update_Worker
  //
  // = DESCRIPTION
  //   Worker to send update commands to the updates dispatching task.
  //
 public:
  // = Initialization and termination code.
  TAO_Notify_Update_Worker (TAO_Notify_Worker_Task * updates_dispatching_task, TAO_Notify_EventType_List& added, TAO_Notify_EventType_List& removed);

  // = TAO_ESF_Worker method
  void work (TAO_Notify_UpdateListener* listener TAO_ENV_ARG_DECL);
 protected:
  // = Data members.
  TAO_Notify_EventType_List& added_;
  TAO_Notify_EventType_List& removed_;
  // Data to transmit.

  TAO_Notify_Worker_Task * updates_dispatching_task_;
  // Update dispatcher
};

#if defined (__ACE_INLINE__)
#include "Notify_Event_Manager.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NOTIFY_EVENT_MANAGER */
