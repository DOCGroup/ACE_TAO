/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Notification
//
// = FILENAME
//   Notify_Event_Manager_Objects_Factory.h
//
// = DESCRIPTION
//   Interface definition of Factory for event manager objects.
//
// = AUTHOR
//   Pradeep Gore <pradeep@cs.wustl.edu>
//
// ============================================================================

#ifndef NOTIFY_EMO_FACTORY_H
#define NOTIFY_EMO_FACTORY_H

#include "ace/pre.h"
#include "ace/Service_Object.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/corba.h"
#include "notify_export.h"

class TAO_Notify_Event_Manager;
class TAO_Notify_Event_Map;
class TAO_Notify_Event_Processor;
class TAO_Notify_Worker_Task;
class TAO_Notify_EventChannel_i;

class TAO_Notify_Export TAO_Notify_EMO_Factory : public ACE_Service_Object
{
  // = TITLE
  //   TAO_Notify_EMO_Factory
  //
  // = DESCRIPTION
  //   Factory interface for event manager and friends.
  //
 public:
  virtual ~TAO_Notify_EMO_Factory (){};

  virtual TAO_Notify_Event_Manager* create_event_manager (TAO_Notify_EventChannel_i* channel ACE_ENV_ARG_DECL)= 0;
  // Create an event manager.

  virtual TAO_Notify_Event_Map* create_event_map (ACE_ENV_SINGLE_ARG_DECL)= 0;
  // Create the event map.

  virtual TAO_Notify_Event_Processor* create_event_processor (TAO_Notify_Event_Manager* event_manager ACE_ENV_ARG_DECL)= 0;
  // Create event processor.

  // = Create processing tasks.
  virtual TAO_Notify_Worker_Task* create_source_eval_task (ACE_ENV_SINGLE_ARG_DECL)= 0;
  virtual TAO_Notify_Worker_Task* create_lookup_task (ACE_ENV_SINGLE_ARG_DECL)= 0;
  virtual TAO_Notify_Worker_Task* create_listener_eval_task (ACE_ENV_SINGLE_ARG_DECL)= 0;

  virtual TAO_Notify_Worker_Task* create_dispatching_task (ACE_ENV_SINGLE_ARG_DECL)= 0;
  // Task that dispatches events.
  virtual TAO_Notify_Worker_Task* create_updates_task (ACE_ENV_SINGLE_ARG_DECL)= 0;
  // Task that dispatches updates.

  // Destroy processing tasks.
  virtual void destroy_source_eval_task (TAO_Notify_Worker_Task* task) = 0;
  virtual void destroy_lookup_task (TAO_Notify_Worker_Task* task) = 0;
  virtual void destroy_listener_eval_task (TAO_Notify_Worker_Task* task) = 0;
  virtual void destroy_dispatching_task (TAO_Notify_Worker_Task* task) = 0;
  virtual void destroy_updates_task (TAO_Notify_Worker_Task* task) = 0;

  virtual void print_values (void) = 0;
  // bad hack. why is this here?!
};

#include "ace/post.h"
#endif /* NOTIFY_EMO_FACTORY_H */
