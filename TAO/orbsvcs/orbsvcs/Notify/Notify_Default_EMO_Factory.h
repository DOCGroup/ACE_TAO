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
#include "ace/Singleton.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// @@ Pradeep: I appretiate the separation of concerns from having
// multiple factories, but consider that some configurations of
// Collection, Locks, and dispatchers don't go well together.  The
// purpose of an Abstract Factory is to have a single object that
// returns consistently configured objects....

class TAO_EMO_Options // Options read by service conf.
{
public:
  TAO_EMO_Options (void);

  // Params read via the svc.conf
  CORBA::Boolean mt_dispatching_;
  CORBA::Boolean mt_source_eval_;
  CORBA::Boolean mt_lookup_;
  CORBA::Boolean mt_listener_eval_;
  CORBA::Boolean asynch_updates_;
  CORBA::Boolean alloc_task_per_proxy_;

  int dispatching_threads_;
  int source_threads_;
  int lookup_threads_;
  int listener_threads_;
};

class TAO_Notify_Export TAO_Notify_Default_EMO_Factory : public TAO_Notify_EMO_Factory
{
 public:
  TAO_Notify_Default_EMO_Factory (void);
  ~TAO_Notify_Default_EMO_Factory ();

  static int init_svc (void);
  // Helper function to register the default factory into the service
  // configurator.

  // = The Service_Object entry points
  virtual int init (int argc, ACE_TCHAR* argv[]);
  virtual int fini (void);

  virtual TAO_Notify_Event_Manager* create_event_manager (TAO_Notify_EventChannel_i* channel ACE_ENV_ARG_DECL);
  // Create an event manager.

  virtual TAO_Notify_Event_Map* create_event_map (ACE_ENV_SINGLE_ARG_DECL);
  // Create the event map.

  virtual TAO_Notify_Event_Processor* create_event_processor (TAO_Notify_Event_Manager* event_manager ACE_ENV_ARG_DECL);
  // Create event processor.

  // = Create processing tasks.
  virtual TAO_Notify_Worker_Task* create_source_eval_task (ACE_ENV_SINGLE_ARG_DECL);
  virtual TAO_Notify_Worker_Task* create_lookup_task (ACE_ENV_SINGLE_ARG_DECL);
  virtual TAO_Notify_Worker_Task* create_listener_eval_task ( ACE_ENV_SINGLE_ARG_DECL);
  virtual TAO_Notify_Worker_Task* create_dispatching_task (ACE_ENV_SINGLE_ARG_DECL);
  virtual TAO_Notify_Worker_Task* create_updates_task (ACE_ENV_SINGLE_ARG_DECL);

  virtual void destroy_source_eval_task (TAO_Notify_Worker_Task* task);
  virtual void destroy_lookup_task (TAO_Notify_Worker_Task* task);
  virtual void destroy_listener_eval_task (TAO_Notify_Worker_Task* task);
  virtual void destroy_dispatching_task (TAO_Notify_Worker_Task* task);
  virtual void destroy_updates_task (TAO_Notify_Worker_Task* task);

  virtual void print_values (void);
  int init_instance ();

protected:
  //= Protected Methods
  TAO_Notify_Worker_Task* create_task (int mt, int tp_size ACE_ENV_ARG_DECL);
  // Create a worker task, mt => is this a MT task, if so, tp_size is thread pool size.

  int preallocate_tasks (void);

  //= Data Members
  TAO_Notify_Worker_Task* prealloc_source_eval_task_;
  TAO_Notify_Worker_Task* prealloc_listener_eval_task_;
  TAO_Notify_Worker_Task* prealloc_dispatching_task_;
};

// Typedef an Options Singleton.
typedef ACE_Singleton <TAO_EMO_Options, ACE_Null_Mutex> EMO_OPTIONS;

ACE_STATIC_SVC_DECLARE (TAO_Notify_Default_EMO_Factory)
ACE_FACTORY_DECLARE (TAO_Notify,TAO_Notify_Default_EMO_Factory)

#include "ace/post.h"
#endif /* NOTIFY_DEFAULT_EMO_FACTORY_H */
