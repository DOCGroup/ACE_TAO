/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Notification
//
// = FILENAME
//   Notify_Command.h
//
// = DESCRIPTION
//   Command Object base class used by Notify's Worker Task objects.
//
// = AUTHOR
//   Pradeep Gore <pradeep@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_NOTIFY_COMMAND_H
#define TAO_NOTIFY_COMMAND_H

#include "ace/pre.h"
#include "ace/Message_Block.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/corba.h"
#include "notify_export.h"

class TAO_Notify_Worker_Task;

class TAO_Notify_Export TAO_Notify_Command : public ACE_Message_Block
{
  // = TITLE
  //   TAO_Notify_Command
  //
  // = DESCRIPTION
  //
  //
 public:
  virtual int execute (TAO_Notify_Worker_Task* parent_task, CORBA::Environment&) = 0;
  // Command callback
};

#include "ace/post.h"

#endif /* TAO_NOTIFY_COMMAND_H */
