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
//   The Event Processor holds the strategies for processing the event as it goes from
//   source to destination(s).
//   It is composed of 4 ACE_Task's that can be either passive or active:
//   a) Source filter evaluation task - checks if the event passes the source's filter.
//   b) Lookup Task - Looksup which listeners are subscribed to recieve this events.
//   c) Listener filter evaluation task - checks if the event passes the listeners filter.
//   d) Dispatching Task - dispatches the event to listeners.
//
//   Breaking up the event processing into these stages allows us to configure the Notify's EC
//   according to the requirements that suit a client's use case.
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

  void init (CORBA::Environment& ACE_TRY_ENV);
  // Init

  void shutdown (CORBA::Environment &ACE_TRY_ENV);
  // Shutdown operations.

  void process_event (TAO_Notify_Event* event, TAO_Notify_EventSource* event_source, CORBA::Environment& ACE_TRY_ENV);
  // Event processing entry point.

  // = Accessors
  TAO_Notify_Worker_Task* get_listener_filter_eval_task (void);
  TAO_Notify_Worker_Task* get_dispatching_task (void);

 protected:
  typedef ACE_Module<ACE_MT_SYNCH> TAO_Notify_Module;
  typedef ACE_Stream<ACE_MT_SYNCH> TAO_Notify_Stream;

  // = Data Members
  TAO_Notify_Event_Manager* event_manager_;
  // The Event Manager

  TAO_Notify_Stream processing_stream_;
  // The processing stream.

  TAO_Notify_Worker_Task* first_task_;
  TAO_Notify_Worker_Task* listener_filter_eval_task_;
  TAO_Notify_Worker_Task* dispatching_task_;
  // Tasks required for direct access.

  TAO_Notify_EMO_Factory* emo_factory_;
  // Factory for manager ojects
};

#include "ace/post.h"
#endif /* TAO_NOTIFY_EVENT_PROCESSOR_H */
