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
//   Worker task handles command execution.
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

class TAO_Notify_AdminProperties;
class TAO_Notify_QoSAdmin_i;

class TAO_Notify_Export TAO_Notify_Worker_Task
{
  // = TITLE
  //   TAO_Notify_Worker_Task
  //
  // = DESCRIPTION
  //   A worker task that we use for various event processing jobs.
  //   Also see TAO_Notify_Command - This task executes Notify_Command objects.
  //   This implementation simply executes the command.
  //
public:
  // = Initialization and termination code
  TAO_Notify_Worker_Task (void);
  virtual ~TAO_Notify_Worker_Task ();
  // Constructor.

  virtual int init_task (TAO_Notify_AdminProperties* const admin_properties,
                         TAO_Notify_QoSAdmin_i* const qos_properties);
  // Init the task

  virtual void shutdown (TAO_ENV_SINGLE_ARG_DECL);
  // shutdown this task.

  virtual int process_event (TAO_Notify_Command *mb TAO_ENV_ARG_DECL, ACE_Time_Value *tv = 0);
  // Process the command.

  virtual void update_admin (TAO_Notify_AdminProperties& admin);
  virtual void update_qos (TAO_Notify_QoSAdmin_i& qos_admin);

};

#include "ace/post.h"
#endif /* TAO_NOTIFY_WORKER_TASK_H */
