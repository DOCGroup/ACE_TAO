/* -*- C++ -*- */
/**
 *  @file Command_Builder.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_Notify_Tests_COMMAND_BUILDER_H
#define TAO_Notify_Tests_COMMAND_BUILDER_H
#include "ace/pre.h"

#include "notify_test_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Service_Config.h"
#include "ace/Service_Object.h"
#include "ace/Unbounded_Queue.h"
#include "ace/SString.h"
#include "ace/CORBA_macros.h"
#include "ace/Hash_Map_Manager.h"
#include "tao/ORB.h"

class TAO_Notify_Tests_Command_Factory;
class TAO_Notify_Tests_Command;

/**
 * @class TAO_Notify_Tests_Command_Builder
 *
 * @brief Class for collating and serially executing command objects.
 *
 */
class TAO_NOTIFY_TEST_Export TAO_Notify_Tests_Command_Builder : public ACE_Service_Object
{
public:
  /// Constuctor
  TAO_Notify_Tests_Command_Builder (void);

  /// Destructor
  ~TAO_Notify_Tests_Command_Builder ();

  /// = Service_Object virtual method overloads.
  virtual int init (int argc, char *argv[]);
  virtual int fini (void);

  /// Register the factory against the name of the factory.
  void _register (ACE_CString command_factory_name, TAO_Notify_Tests_Command_Factory* command_factory);

  /// Execute the <start_command_>.
  void execute (ACE_ENV_SINGLE_ARG_DECL);

protected:
  /// The starting command. each command executes the next one when done.
  TAO_Notify_Tests_Command* start_command_;

  /// Last command initialized.
  TAO_Notify_Tests_Command* last_command_;

  /// The factory map.
  ACE_Hash_Map_Manager <ACE_CString, TAO_Notify_Tests_Command_Factory*, TAO_SYNCH_MUTEX> factory_map_;
};

ACE_STATIC_SVC_DECLARE_EXPORT (TAO_NOTIFY_TEST, TAO_Notify_Tests_Command_Builder)

ACE_FACTORY_DECLARE (TAO_NOTIFY_TEST, TAO_Notify_Tests_Command_Builder)

#include "ace/post.h"
#endif /* TAO_Notify_Tests_COMMAND_BUILDER_H */
