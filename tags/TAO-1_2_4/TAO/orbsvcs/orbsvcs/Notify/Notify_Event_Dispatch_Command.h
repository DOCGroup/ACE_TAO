/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   Notify_Event_Dispatch_Command.h
 *
 *  $Id$
 *
 * Definition for the Command object responsible for dispatching events to consumers.
 *
 *
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_NOTIFY_EVENT_DISPATCH_COMMAND_H
#define TAO_NOTIFY_EVENT_DISPATCH_COMMAND_H

#include "ace/pre.h"
#include "Notify_Command.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_Notify_Event;
class TAO_Notify_EventListener;
class TAO_Notify_Worker_Task;

 /**
  * @class TAO_Notify_Event_Dispatch_Command
  *
  * @brief TAO_Notify_Event_Dispatch_Command
  *
  * The Command for dispatching to a consumer.
  */
class TAO_Notify_Export TAO_Notify_Event_Dispatch_Command : public TAO_Notify_Command
{
 public:
  // = Initialization and termination code.
  TAO_Notify_Event_Dispatch_Command (TAO_Notify_Event_Processor* event_processor, TAO_Notify_Event* event, TAO_Notify_EventListener* event_listener);

  ~TAO_Notify_Event_Dispatch_Command ();

  /// Command callback
  virtual int execute (ACE_ENV_SINGLE_ARG_DECL);

 protected:
  // = Data Members
  TAO_Notify_EventListener* event_listener_;
};

#include "ace/post.h"
#endif /* TAO_NOTIFY_EVENT_DISPATCH_COMMAND_H */
