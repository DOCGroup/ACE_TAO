/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   Notify_Event_Processor.h
 *
 *  $Id$
 *
 *      The QoS properties of "Order Policy" and "Discard Policy" can be set
 *      per proxy.To accomodate this we now have an ACE_Task per proxy.
 *      The TAO_Notify_Event_Processor asks each event listener and source
 *      for the task object to send commands to.
 *
 *
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 */
//=============================================================================

//

#ifndef TAO_NOTIFY_EVENT_PROCESSOR_H
#define TAO_NOTIFY_EVENT_PROCESSOR_H

#include "ace/pre.h"
#include "ace/Module.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Stream.h"
#include "ace/streams.h"
#include "tao/corba.h"
#include "notify_export.h"

class TAO_Notify_Event;
class TAO_Notify_Event_Manager;
class TAO_Notify_EventSource;
class TAO_Notify_Worker_Task;
class TAO_Notify_EMO_Factory;
class TAO_Notify_EventListener;
class TAO_Notify_QoSAdmin_i;

 /**
  * @class TAO_Notify_Event_Processor
  *
  * @brief TAO_Notify_Event_Processor
  *
  * The Event Processor.
  */
class TAO_Notify_Export TAO_Notify_Event_Processor
{
 public:
  // = Initialialization and termination code.
  TAO_Notify_Event_Processor (TAO_Notify_Event_Manager* event_manager);
  virtual ~TAO_Notify_Event_Processor ();

  /// Init
  void init (TAO_Notify_QoSAdmin_i* qos_properties
             ACE_ENV_ARG_DECL);

  /// Shutdown operations.
  void shutdown (ACE_ENV_SINGLE_ARG_DECL);

  /// Event processing entry point.
  void evaluate_source_filter (TAO_Notify_Event* event,
                               TAO_Notify_EventSource* event_source
                               ACE_ENV_ARG_DECL);

  // = Callbacks for Source/Event Listeners.
  /// This method is called by an Event_Source after it has
  /// successfully evaluated its filter.
  void lookup_subscriptions (TAO_Notify_Event* event,
                             TAO_Notify_EventSource* event_source
                             ACE_ENV_ARG_DECL);

  /// This method is called by the subscription lookup command asking
  /// that <event> be delivered to <event_listener>.
  void evaluate_listener_filter (TAO_Notify_Event* event,
                                 TAO_Notify_EventListener* event_listener,
                                 CORBA::Boolean eval_parent
                                 ACE_ENV_ARG_DECL);

  /// This method is called by an Event_Listener after it has
  /// successfully evaluated its filter.
  void dispatch_event (TAO_Notify_Event* event,
                       TAO_Notify_EventListener* event_listener
                       ACE_ENV_ARG_DECL);

protected:
  // = Data Members
  /// The Event Manager
  TAO_Notify_Event_Manager* event_manager_;

  TAO_Notify_Worker_Task* lookup_task_;

  /// Factory for manager ojects
  TAO_Notify_EMO_Factory* emo_factory_;
};

#include "ace/post.h"
#endif /* TAO_NOTIFY_EVENT_PROCESSOR_H */
