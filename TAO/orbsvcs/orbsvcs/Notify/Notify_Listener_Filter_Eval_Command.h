/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Notification
//
// = FILENAME
//   Notify_Listener_Filter_Eval_Command.h
//
// = DESCRIPTION
//   Defines the Command object for evaluating the listener's filter.
//
// = AUTHOR
//   Pradeep Gore <pradeep@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_NOTIFY_LISTENER_FILTER_EVAL_COMMAND_H
#define TAO_NOTIFY_LISTENER_FILTER_EVAL_COMMAND_H

#include "ace/pre.h"
#include "Notify_Command.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_Notify_Event;
class TAO_Notify_EventListener;
class TAO_Notify_Event_Processor;

class TAO_Notify_Export TAO_Notify_Listener_Filter_Eval_Command : public TAO_Notify_Command
{
  // = TITLE
  //   TAO_Notify_Listener_Filter_Eval_Command
  //
  // = DESCRIPTION
  //   Listener filter evaluation command.
  //
 public:
  TAO_Notify_Listener_Filter_Eval_Command (TAO_Notify_Event_Processor* event_processor, TAO_Notify_Event* event, TAO_Notify_EventListener* event_listener, CORBA::Boolean eval_parent);
  // The event, listener, and hint to pass (see the listener interface for details)

  ~TAO_Notify_Listener_Filter_Eval_Command();

  virtual int execute (ACE_ENV_SINGLE_ARG_DECL);
  // Command callback

 protected:
  // = Data Members
  TAO_Notify_EventListener* event_listener_;
  CORBA::Boolean eval_parent_;
};

#include "ace/post.h"
#endif /* TAO_NOTIFY_LISTENER_FILTER_EVAL_COMMAND_H */
