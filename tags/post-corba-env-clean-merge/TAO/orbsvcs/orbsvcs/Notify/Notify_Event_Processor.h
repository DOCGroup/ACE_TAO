/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Notification
//
// = FILENAME
//   Notify_Event_Processor.h
//
// = DESCRIPTION
//        The QoS properties of "Order Policy" and "Discard Policy" can be set
//        per proxy.To accomodate this we now have an ACE_Task per proxy.
//        The TAO_Notify_Event_Processor asks each event listener and source
//        for the task object to send commands to.
//
// = AUTHOR
//   Pradeep Gore <pradeep@cs.wustl.edu>
//
// ============================================================================
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

class TAO_Notify_Export TAO_Notify_Event_Processor
{
  // = TITLE
  //   TAO_Notify_Event_Processor
  //
  // = DESCRIPTION
  //   The Event Processor.
  //
 public:
  // = Initialialization and termination code.
  TAO_Notify_Event_Processor (TAO_Notify_Event_Manager* event_manager);
  virtual ~TAO_Notify_Event_Processor ();

  void init (TAO_ENV_SINGLE_ARG_DECL);
  // Init

  void shutdown (TAO_ENV_SINGLE_ARG_DECL);
  // Shutdown operations.

  void evaluate_source_filter (TAO_Notify_Event* event,
                               TAO_Notify_EventSource* event_source
                               TAO_ENV_ARG_DECL);
  // Event processing entry point.

  // = Callbacks for Source/Event Listeners.
  void lookup_subscriptions (TAO_Notify_Event* event,
                             TAO_Notify_EventSource* event_source
                             TAO_ENV_ARG_DECL);
  // This method is called by an Event_Source after it has
  // successfully evaluated its filter.

  void evaluate_listener_filter (TAO_Notify_Event* event,
                                 TAO_Notify_EventListener* event_listener,
                                 CORBA::Boolean eval_parent
                                 TAO_ENV_ARG_DECL);
  // This method is called by the subscription lookup command asking
  // that <event> be delivered to <event_listener>.

  void dispatch_event (TAO_Notify_Event* event,
                       TAO_Notify_EventListener* event_listener
                       TAO_ENV_ARG_DECL);
  // This method is called by an Event_Listener after it has
  // successfully evaluated its filter.

protected:
  // = Data Members
  TAO_Notify_Event_Manager* event_manager_;
  // The Event Manager

  TAO_Notify_Worker_Task* lookup_task_;

  TAO_Notify_EMO_Factory* emo_factory_;
  // Factory for manager ojects
};

#include "ace/post.h"
#endif /* TAO_NOTIFY_EVENT_PROCESSOR_H */
