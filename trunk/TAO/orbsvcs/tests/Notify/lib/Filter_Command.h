/* -*- C++ -*- */
/**
 *  @file Filter_Command.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_Notify_Tests_FILTER_COMMAND_H
#define TAO_Notify_Tests_FILTER_COMMAND_H
#include "ace/pre.h"

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
 *
 */
class TAO_NOTIFY_TEST_Export TAO_Notify_Tests_Filter_Command : public TAO_Notify_Tests_Command
{
public:
  /// Constuctor
  TAO_Notify_Tests_Filter_Command (void);

  /// Destructor
  ~TAO_Notify_Tests_Filter_Command ();

  /// Parse args and populate options.
  virtual void init (ACE_Arg_Shifter& arg_shifter);

  /// Execute the command.
  virtual void execute_i (ACE_ENV_SINGLE_ARG_DECL);

  /// Name of object
  virtual const char* get_name (void);
  static const char* name (void);

protected:

#ifdef HPUX_11
   // Remove clash with /usr/include/machine/cpu.h and /usr/include/pa/cpu.h
#undef COMMAND
#endif
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
  void handle_create_filter_factory (ACE_ENV_SINGLE_ARG_DECL);
  void handle_create_filter (ACE_ENV_SINGLE_ARG_DECL);
  void handle_add_constraint (ACE_ENV_SINGLE_ARG_DECL);
  void handle_add_filter (ACE_ENV_SINGLE_ARG_DECL);
  void handle_destroy_filter (ACE_ENV_SINGLE_ARG_DECL);
};

#include "ace/post.h"
#endif /* TAO_Notify_Tests_FILTER_COMMAND_H */
