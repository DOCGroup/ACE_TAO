/* -*- C++ -*- */
/**
 *  @file Filter_Command.h
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 */

#ifndef TAO_Notify_Tests_FILTER_COMMAND_H
#define TAO_Notify_Tests_FILTER_COMMAND_H
#include /**/ "ace/pre.h"

#include "notify_test_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/SString.h"
#include "orbsvcs/CosNotifyChannelAdminC.h"
#include "Command.h"

/**
 * @class TAO_Notify_Tests_Filter_Command
 *
 * @brief Command for Filter Objects.
 */
class TAO_NOTIFY_TEST_Export TAO_Notify_Tests_Filter_Command : public TAO_Notify_Tests_Command
{
public:
  /// Constructor
  TAO_Notify_Tests_Filter_Command ();

  /// Destructor
  ~TAO_Notify_Tests_Filter_Command ();

  /// Parse args and populate options.
  virtual void init (ACE_Arg_Shifter& arg_shifter);

  /// Execute the command.
  virtual void execute_i ();

  /// Name of object
  virtual const char* get_name ();
  static const char* name ();

protected:
  enum COMMAND
  {
    CREATE_FACTORY,
    CREATE_FILTER,
    ADD_CONSTRAINT,
    ADD_FILTER,
    REMOVE_CONSTRAINT,
    DESTROY,
    DUMP_STATE
  };

  ACE_CString name_;

  ACE_CString factory_;

  ACE_CString constraint_;

  ///= Handlers
  void handle_create_filter_factory ();
  void handle_create_filter ();
  void handle_add_constraint ();
  void handle_add_filter ();
  void handle_destroy_filter ();
};

#include /**/ "ace/post.h"
#endif /* TAO_Notify_Tests_FILTER_COMMAND_H */
