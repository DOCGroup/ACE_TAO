/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Notification
//
// = FILENAME
//   Notify_MT_Worker_Task.h
//
// = DESCRIPTION
//   MT Worker task
//
// = AUTHOR
//   Pradeep Gore <pradeep@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_MT_NOTIFY_WORKER_TASK_H
#define TAO_MT_NOTIFY_WORKER_TASK_H

#include "ace/pre.h"
#include "ace/Task.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Notify_Worker_Task.h"
#include "Notify_AdminProperties.h"

class TAO_Notify_Event_Processor;
class TAO_Notify_Buffering_Strategy;

class TAO_Notify_Export TAO_Notify_MT_Worker_Task : public TAO_Notify_Worker_Task, public ACE_Task<ACE_SYNCH>
{
  // = TITLE
  //   TAO_Notify_MT_Worker_Task
  //
  // = DESCRIPTION
  //   A MT worker task that we use for various event processing jobs.
  //   Also see TAO_Notify_Command - This task executes Notify_Command objects.
  //
public:
  // = Initialization and termination code
  TAO_Notify_MT_Worker_Task (void);
  // Constructor.

  ~TAO_Notify_MT_Worker_Task ();
  // Destructor.

  virtual int init_task (TAO_Notify_AdminProperties* const admin_properties);
  // Init the task

  virtual void shutdown (CORBA::Environment& ACE_TRY_ENV);
  // shutdown this task.

  virtual int process_event (TAO_Notify_Command *mb, CORBA::Environment& ACE_TRY_ENV, ACE_Time_Value *tv = 0);
  // Process the command.

 protected:
  virtual int svc (void);
  // svc command processes objects stored in the message queue.

  virtual int close (u_long);
  // Close hook.

  TAO_Notify_Buffering_Strategy* buffering_strategy_;
  // The buffering strategy to use.

  TAO_Notify_Property_Long* queue_length_;
  // We need to decrement the event_count_ everytime we dequeue a command
  // object.
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

  TAO_Notify_Shutdown_Command (void);

  virtual int execute (CORBA::Environment& ACE_TRY_ENV);
  // Returns -1. This signals worker threads to finish servicing requests.
};

//****************************************************************************************

#include "ace/post.h"
#endif /* TAO_MT_NOTIFY_WORKER_TASK_H */
