/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Notification
//
// = FILENAME
//   Notify_Update_Dispatch_Command.h
//
// = DESCRIPTION
//
//
// = AUTHOR
//   Pradeep Gore <pradeep@cs.wustl.edu>
//
// ============================================================================

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

class TAO_Notify_Export TAO_Notify_Update_Dispatch_Command : public TAO_Notify_Command
{
  // = TITLE
  //   TAO_Notify_Update_Dispatch_Command
  //
  // = DESCRIPTION
  //
  //
 public:
  // = Initialization and termination code
  TAO_Notify_Update_Dispatch_Command (TAO_Notify_UpdateListener* listener, TAO_Notify_EventType_List& added, TAO_Notify_EventType_List& removed);

  ~TAO_Notify_Update_Dispatch_Command ();

  virtual int execute (TAO_ENV_SINGLE_ARG_DECL);
  // Command callback

protected:
  // = Data Members
  TAO_Notify_UpdateListener* update_listener_;
  TAO_Notify_EventType_List added_;
  TAO_Notify_EventType_List removed_;
};

/********************************************************************/

#include "ace/post.h"
#endif /* TAO_NOTIFY_UPDATE_DISPATCH_COMMAND_H */
