/* -*- C++ -*- */
/**
 *  @file Command.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_Notify_Tests_COMMAND_H
#define TAO_Notify_Tests_COMMAND_H
#include "ace/pre.h"

#include "notify_test_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Arg_Shifter.h"
#include "tao/corba.h"
#include "ace/CORBA_macros.h"

/**
 * @class TAO_Notify_Tests_Command
 *
 * @brief Base Class for all command objects.
 *
 */
class TAO_NOTIFY_TEST_Export TAO_Notify_Tests_Command
{
  friend class TAO_Notify_Tests_Command_Builder;
public:
  /// Constuctor
  TAO_Notify_Tests_Command (void);

  /// Destructor
  virtual ~TAO_Notify_Tests_Command ();

  /// Parse args and populate options.
  virtual void init (ACE_Arg_Shifter& arg_shifter);

    /// Implement command execution.
  virtual void execute_i (ACE_ENV_SINGLE_ARG_DECL) = 0;

  /// Return the name of this command.
  virtual const char* get_name (void)= 0;

  ///= Each derived type must also implement the following signature:
  // static const char* name (void);

protected:
  /// Next command after this one.
  TAO_Notify_Tests_Command* next_;

  enum {INVALID = -1};

  int command_;

private:
  /// Execute the command.
  void execute (ACE_ENV_SINGLE_ARG_DECL);

  /// Save the next command to exec.
  void next (TAO_Notify_Tests_Command* command);
};

#include "ace/post.h"
#endif /* TAO_Notify_Tests_COMMAND_H */
