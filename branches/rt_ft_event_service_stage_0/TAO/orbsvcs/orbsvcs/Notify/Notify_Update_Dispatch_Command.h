/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   Notify_Update_Dispatch_Command.h
 *
 *  $Id$
 *
 *
 *
 *
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_NOTIFY_UPDATE_DISPATCH_COMMAND_H
#define TAO_NOTIFY_UPDATE_DISPATCH_COMMAND_H
#include "ace/pre.h"
#include "Notify_Command.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "notify_export.h"
#include "tao/corba.h"
#include "Notify_Collection.h"

class TAO_Notify_UpdateListener;

 /**
  * @class TAO_Notify_Update_Dispatch_Command
  *
  * @brief TAO_Notify_Update_Dispatch_Command
  *
  */
class TAO_Notify_Export TAO_Notify_Update_Dispatch_Command : public TAO_Notify_Command
{
 public:
  // = Initialization and termination code
  TAO_Notify_Update_Dispatch_Command (TAO_Notify_UpdateListener* listener, TAO_Notify_EventType_List& added, TAO_Notify_EventType_List& removed);

  ~TAO_Notify_Update_Dispatch_Command ();

  /// Command callback
  virtual int execute (ACE_ENV_SINGLE_ARG_DECL);

protected:
  // = Data Members
  TAO_Notify_UpdateListener* update_listener_;
  TAO_Notify_EventType_List added_;
  TAO_Notify_EventType_List removed_;
};

/********************************************************************/

#include "ace/post.h"
#endif /* TAO_NOTIFY_UPDATE_DISPATCH_COMMAND_H */
