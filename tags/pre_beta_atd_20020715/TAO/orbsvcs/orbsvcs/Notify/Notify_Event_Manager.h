/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   Notify_Event_Manager.h
 *
 *  $Id$
 *
 * An Event Manager for the Notification Service.
 *
 *
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 */
//=============================================================================


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

/**
 * @class TAO_Notify_Event_Manager
 *
 * @brief TAO_Notify_Event_Manager
 *
 * Handles all aspects of event propogation thru the channel.
 * Also keeps track of publications and subscriptions.
 */
class TAO_Notify_Export TAO_Notify_Event_Manager
{

 public:
  // = Initialization and termination code.
  /// Constructor.
  TAO_Notify_Event_Manager (TAO_Notify_EventChannel_i* parent,
                            TAO_Notify_EMO_Factory* emo_factory);

  /// Destructor.
  ~TAO_Notify_Event_Manager ();

  /// Init
  void init (ACE_ENV_SINGLE_ARG_DECL);

  /// Shutdown operations.
  void shutdown (ACE_ENV_SINGLE_ARG_DECL);

  // = Publish/Subscribe management
  // = Subscription
  /// Subscribes <event_listener> for events <added>.
  /// Unsubscribes <event_listener> for events <removed>.
  void subscribe_for_events (TAO_Notify_EventListener* event_listener, const CosNotification::EventTypeSeq & added, const CosNotification::EventTypeSeq & removed ACE_ENV_ARG_DECL);

  // = Publications
  /// Suppliers can send anonymous requests to the Event Manager to indicate
  /// what kind of events they expect to produce.
  void update_publication_list (const CosNotification::EventTypeSeq & added, const CosNotification::EventTypeSeq & removed ACE_ENV_ARG_DECL);

  // = Updates
  /// Registers the subscription update listener with the Event Manager.
  void register_for_subscription_updates (TAO_Notify_UpdateListener* update_listener ACE_ENV_ARG_DECL);

  /// Unregister from subscription updates.
  void unregister_from_subscription_updates (TAO_Notify_UpdateListener* update_listener ACE_ENV_ARG_DECL);

  /// Registers the publication update listener with the Event Manager.
  void register_for_publication_updates (TAO_Notify_UpdateListener* update_listener ACE_ENV_ARG_DECL);

  /// Unregister from publication updates.
  void unregister_from_publication_updates (TAO_Notify_UpdateListener* update_listener ACE_ENV_ARG_DECL);

  // = Accessors
  /// Obtain the publication list.
  CosNotification::EventTypeSeq* obtain_offered_types (void);

  /// Obtain the subscription list.
  CosNotification::EventTypeSeq* obtain_subscription_types (void);

  /// Get the event map.
  TAO_Notify_Event_Map* event_map (void);

  /// Get the Event Processor.
  TAO_Notify_Event_Processor* event_processor (void);

  /// Get the Admin Properties.
  TAO_Notify_AdminProperties* admin_properties (void);

  /// Get the Resource Factory.
  TAO_Notify_EMO_Factory* resource_factory (void);

  // = Event forwarding methods.
  /// Delivers the event to listeners subscribed for <event>
  /// <event_source> is the <event> source to the Event Manager.
  void process_event (TAO_Notify_Event* event,
                      TAO_Notify_EventSource* event_source
                      ACE_ENV_ARG_DECL);

  void update_task_admins (void);

protected:
  // = Event dispatching methods.
  /// Dispatch the updates to the <update_listener_list>
  void dispatch_updates_i (TAO_Notify_UpdateListener_List* update_listener_list,
                           TAO_Notify_EventType_List& added,
                           TAO_Notify_EventType_List& removed
                           ACE_ENV_ARG_DECL);

  // = Data members.
  /// The Event Channel that we're managing for.
  TAO_Notify_EventChannel_i * event_channel_;

  /// Container for event <-> source/sinks mappings.
  TAO_Notify_Event_Map* event_map_;

  /// Handles processing of events.
  TAO_Notify_Event_Processor * event_processor_;

  /// Dispatches updates to update listeners.
  TAO_Notify_Worker_Task * updates_dispatching_task_;

  /// Event manager objects factory.
  TAO_Notify_EMO_Factory* emo_factory_;

  /// The locking strategy.
  ACE_Lock* lock_;

  /// Admin properties.
  TAO_Notify_AdminProperties* admin_properties_;
};

/**************************************************************************/

 /**
  * @class TAO_Notify_Update_Worker
  *
  * @brief TAO_Notify_Update_Worker
  *
  * Worker to send update commands to the updates dispatching task.
  */
class TAO_Notify_Export TAO_Notify_Update_Worker : public TAO_ESF_Worker<TAO_Notify_UpdateListener>
{
 public:
  // = Initialization and termination code.
  TAO_Notify_Update_Worker (TAO_Notify_Worker_Task * updates_dispatching_task, TAO_Notify_EventType_List& added, TAO_Notify_EventType_List& removed);

  // = TAO_ESF_Worker method
  void work (TAO_Notify_UpdateListener* listener ACE_ENV_ARG_DECL);
 protected:
  // = Data members.
  /// Data to transmit.
  TAO_Notify_EventType_List& added_;
  TAO_Notify_EventType_List& removed_;

  /// Update dispatcher
  TAO_Notify_Worker_Task * updates_dispatching_task_;
};

#if defined (__ACE_INLINE__)
#include "Notify_Event_Manager.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NOTIFY_EVENT_MANAGER */
