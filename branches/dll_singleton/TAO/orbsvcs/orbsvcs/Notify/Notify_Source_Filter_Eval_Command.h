/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Notification
//
// = FILENAME
//   Notify_Source_Filter_Eval_Command.h
//
// = DESCRIPTION
//   Command object for evaluating source filter.
//
// = AUTHOR
//   Pradeep Gore <pradeep@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_NOTIFY_SOURCE_FILTER_EVAL_COMMAND_H
#define TAO_NOTIFY_SOURCE_FILTER_EVAL_COMMAND_H

#include "ace/pre.h"
#include "Notify_Command.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "notify_export.h"

class TAO_Notify_Event;
class TAO_Notify_EventSource;

class TAO_Notify_Export TAO_Notify_Source_Filter_Eval_Command : public TAO_Notify_Command
{
  // = TITLE
  //   TAO_Notify_Source_Filter_Eval_Command
  //
  // = DESCRIPTION
  //   Command object for evaluating event source's filter.
  //
public:
  // = Initialization and termination code
  TAO_Notify_Source_Filter_Eval_Command (TAO_Notify_Event_Processor* event_processor, TAO_Notify_Event* event, TAO_Notify_EventSource* event_source);
  // Constructor.

  ~TAO_Notify_Source_Filter_Eval_Command ();
  // Destructor.

  virtual int execute (ACE_ENV_SINGLE_ARG_DECL);
  // Command callback

protected:
  // = Data Members
  TAO_Notify_EventSource* event_source_;
};

#include "ace/post.h"
#endif  /* TAO_NOTIFY_SOURCE_FILTER_EVAL_COMMAND_H */
