/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Notification
//
// = FILENAME
//   Notify_Default_EMO_Factory.h
//
// = DESCRIPTION
//   Default factory for event manager and its helper objects.
//
// = AUTHOR
//   Pradeep Gore <pradeep@cs.wustl.edu>
//
// ============================================================================

#ifndef NOTIFY_DEFAULT_EMO_FACTORY_H
#define NOTIFY_DEFAULT_EMO_FACTORY_H
#include "ace/pre.h"

#include "notify_export.h"
#include "Notify_Event_Manager_Objects_Factory.h"
#include "ace/Service_Config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// @@ Pradeep: I appretiate the separation of concerns from having
// multiple factories, but consider that some configurations of
// Collection, Locks, and dispatchers don't go well together.  The
// purpose of an Abstract Factory is to have a single object that
// returns consistently configured objects....

class TAO_Notify_Export TAO_Notify_Default_EMO_Factory : public TAO_Notify_EMO_Factory
{
 public:
  TAO_Notify_Default_EMO_Factory (void);
  ~TAO_Notify_Default_EMO_Factory ();

  static int init_svc (void);
  // Helper function to register the default factory into the service
  // configurator.

  // = The Service_Object entry points
  virtual int init (int argc, char* argv[]);
  virtual int fini (void);

  virtual TAO_Notify_Event_Manager* create_event_manager (TAO_Notify_EventChannel_i* channel, CORBA::Environment &ACE_TRY_ENV);
  // Create an event manager.

  virtual TAO_Notify_Event_Map* create_event_map (CORBA::Environment &ACE_TRY_ENV);
  // Create the event map.

  virtual TAO_Notify_Event_Processor* create_event_processor (TAO_Notify_Event_Manager* event_manager, CORBA::Environment &ACE_TRY_ENV);
  // Create event processor.

  // = Create processing tasks.
  virtual TAO_Notify_Worker_Task* create_source_eval_task (CORBA::Environment &ACE_TRY_ENV);
  virtual TAO_Notify_Worker_Task* create_lookup_task (CORBA::Environment &ACE_TRY_ENV);
  virtual TAO_Notify_Worker_Task* create_listener_eval_task ( CORBA::Environment &ACE_TRY_ENV);
  virtual TAO_Notify_Worker_Task* create_dispatching_task (CORBA::Environment &ACE_TRY_ENV);
  virtual void print_values (void);
protected:
  // = Params read via the svc.conf
  CORBA::Boolean mt_dispatching_;
  CORBA::Boolean mt_source_eval_;
  CORBA::Boolean mt_lookup_;
  CORBA::Boolean mt_listener_eval_;

  int dispatching_threads_;
  int source_threads_;
  int lookup_threads_;
  int listener_threads_;
};

ACE_STATIC_SVC_DECLARE (TAO_Notify_Default_EMO_Factory)
ACE_FACTORY_DECLARE (TAO_Notify,TAO_Notify_Default_EMO_Factory)

#include "ace/post.h"
#endif /* NOTIFY_DEFAULT_EMO_FACTORY_H */
