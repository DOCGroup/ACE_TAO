/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   Notify_Source_Filter_Eval_Command.h
 *
 *  $Id$
 *
 * Command object for evaluating source filter.
 *
 *
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 */
//=============================================================================


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

/**
 * @class TAO_Notify_Source_Filter_Eval_Command
 *
 * @brief TAO_Notify_Source_Filter_Eval_Command
 *
 * Command object for evaluating event source's filter.
 */
class TAO_Notify_Export TAO_Notify_Source_Filter_Eval_Command : public TAO_Notify_Command
{
public:
  // = Initialization and termination code
  /// Constructor.
  TAO_Notify_Source_Filter_Eval_Command (TAO_Notify_Event_Processor* event_processor, TAO_Notify_Event* event, TAO_Notify_EventSource* event_source);

  /// Destructor.
  ~TAO_Notify_Source_Filter_Eval_Command ();

  /// Command callback
  virtual int execute (ACE_ENV_SINGLE_ARG_DECL);

protected:
  // = Data Members
  TAO_Notify_EventSource* event_source_;
};

#include "ace/post.h"
#endif  /* TAO_NOTIFY_SOURCE_FILTER_EVAL_COMMAND_H */
