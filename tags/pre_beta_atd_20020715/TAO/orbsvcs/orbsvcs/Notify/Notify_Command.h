/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   Notify_Command.h
 *
 *  $Id$
 *
 * Command Object base class used by Notify's Worker Task objects.
 *
 *
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_NOTIFY_COMMAND_H
#define TAO_NOTIFY_COMMAND_H

#include "ace/pre.h"
#include "ace/Message_Block.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/corba.h"
#include "notify_export.h"

#if !defined (TAO_NOTIFY_COMMAND_PRIORITY_BASE)
#define TAO_NOTIFY_COMMAND_PRIORITY_BASE 32768
#endif /* TAO_NOTIFY_COMMAND_PRIORITY_BASE */

class TAO_Notify_Event_Processor;
class TAO_Notify_Event;

 /**
  * @class TAO_Notify_Command
  *
  * @brief TAO_Notify_Command
  *
  * Base class for Command Objects.
  * Command objects are executed by the Notify_Worker_Task.
  */
class TAO_Notify_Export TAO_Notify_Command : public ACE_Message_Block
{
 public:
  TAO_Notify_Command (TAO_Notify_Event_Processor* event_processor, TAO_Notify_Event* event);
  ~TAO_Notify_Command ();

  /// Command callback
  virtual int execute (ACE_ENV_SINGLE_ARG_DECL) = 0;

  virtual CORBA::Short priority (void) const;

protected:
  // = Data Members
  /// The command objects should notify the event processor once they have successfully
  /// executed commands.
  TAO_Notify_Event_Processor* event_processor_;

  /// The event that we "execute" this command on.
  TAO_Notify_Event* event_;
};


#if defined (__ACE_INLINE__)
#include "Notify_Command.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"

#endif /* TAO_NOTIFY_COMMAND_H */
