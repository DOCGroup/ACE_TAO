/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   Notify_Lookup_Command.h
 *
 *  $Id$
 *
 * Command object for looking up subscriptions for a given event.
 *
 *
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_NOTIFY_LOOKUP_COMMAND_H
#define TAO_NOTIFY_LOOKUP_COMMAND_H

#include "ace/pre.h"
#include "orbsvcs/ESF/ESF_Worker.h"
#include "Notify_Command.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_Notify_Event;
class TAO_Notify_EventListener;
class TAO_Notify_Event_Map;

 /**
  * @class TAO_Notify_Lookup_Command
  *
  * @brief TAO_Notify_Lookup_Command
  *
  * Command object for event subscription lookup in the event map.
  */
class TAO_Notify_Export TAO_Notify_Lookup_Command : public TAO_Notify_Command, public TAO_ESF_Worker<TAO_Notify_EventListener>
{
 public:
  // = Initialization and termination code
  TAO_Notify_Lookup_Command (TAO_Notify_Event_Processor* event_processor, TAO_Notify_Event* event, TAO_Notify_Event_Map* event_map);

  ~TAO_Notify_Lookup_Command ();

  /// Command callback
  virtual int execute (ACE_ENV_SINGLE_ARG_DECL_NOT_USED);

  // = TAO_ESF_Worker method
  void work (TAO_Notify_EventListener* listener ACE_ENV_ARG_DECL);
 protected:
  // = Data member
  /// The event map to lookup in.
  TAO_Notify_Event_Map* event_map_;
};

#include "ace/post.h"
#endif /* TAO_NOTIFY_LOOKUP_COMMAND_H */
