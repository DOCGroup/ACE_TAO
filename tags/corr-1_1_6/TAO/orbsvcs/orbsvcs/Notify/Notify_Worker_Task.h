/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Notification
//
// = FILENAME
//   Notify_Worker_Task.h
//
// = DESCRIPTION
//   Worker task used by the event processor.
//
// = AUTHOR
//   Pradeep Gore <pradeep@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_NOTIFY_WORKER_TASK_H
#define TAO_NOTIFY_WORKER_TASK_H

#include "ace/pre.h"
#include "ace/Task.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/corba.h"
#include "Notify_Command.h"
#include "notify_export.h"

class TAO_Notify_Event_Manager;

class TAO_Notify_Export TAO_Notify_Worker_Task : public ACE_Task<ACE_SYNCH>
{
  // = TITLE
  //   TAO_Notify_Worker_Task
  //
  // = DESCRIPTION
  //   A worker task that we use for various event processing jobs.
  //   Also see TAO_Notify_Command - This task executes Notify_Command objects.
  //
public:
  // = Initialization and termination code
  TAO_Notify_Worker_Task (TAO_Notify_Event_Manager* event_manager, CORBA::Boolean activate_object);
  // Constructor.

  virtual int open (void *args);
  // Activate the task if <active_object_> is set.

  virtual int svc (void);
  // svc command objects stored in the message queue.

  void shutdown (CORBA::Environment& ACE_TRY_ENV);
  // shutdown this task.

  virtual int process_event (TAO_Notify_Command *mb, CORBA::Environment& ACE_TRY_ENV, ACE_Time_Value *tv = 0);
  // Allows the producer to pass messages to the <Message_Block>.

  TAO_Notify_Event_Manager* event_manager (void);
  // access manager.

 protected:
  virtual int close (u_long);
  // Close hook.

  TAO_Notify_Event_Manager* event_manager_;
  // The Event Manager using us.

  CORBA::Boolean activate_object_;
  // Is this an active object.
};

//****************************************************************************************

class TAO_Notify_Export TAO_Notify_Shutdown_Command : public TAO_Notify_Command
{
  // = TITLE
  //   TAO_Notify_Shutdown_Command
  //
  // = DESCRIPTION
  //   Shutdown command to shutdown the task.
  //
 public:
  virtual int execute (TAO_Notify_Worker_Task* parent_task, CORBA::Environment&);
  // Returns -1. This signals worker threads to finish servicing requests.
};

//****************************************************************************************

#include "ace/pre.h"
#endif /* TAO_NOTIFY_WORKER_TASK_H */
