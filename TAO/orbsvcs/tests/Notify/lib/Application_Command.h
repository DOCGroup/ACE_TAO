/* -*- C++ -*- */
/**
 *  @file Application_Command.h
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 */

#ifndef TAO_Notify_Tests_APPLICATION_COMMAND_H
#define TAO_Notify_Tests_APPLICATION_COMMAND_H
#include /**/ "ace/pre.h"

#include "notify_test_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/ORB.h"
#include "Command.h"

class TAO_Notify_Tests_Application_Command;

/**
 * @class TAO_Notify_Tests_Application_Command
 *
 * @brief Command to control the Application.
 */
class TAO_NOTIFY_TEST_Export TAO_Notify_Tests_Application_Command : public TAO_Notify_Tests_Command
{
public:
  /// Constructor
  TAO_Notify_Tests_Application_Command ();

  /// Destructor
  ~TAO_Notify_Tests_Application_Command ();

  /// Parse args and populate options.
  virtual void init (ACE_Arg_Shifter& arg_shifter);

  /// Execute the command.
  virtual void execute_i ();

  /// Get the command name.
  virtual const char* get_name ();
  static const char* name ();

protected:
  enum COMMAND
  {
    INIT,
    RUN,
    WAIT_FOR_EVENTS,
    SHUTDOWN,
    DUMP_STATE,
    SIGNAL_PEER,
    WAIT_TO_START
  };

  /// Flag used with the DUMP_STATE command that asks for all samples to be put into the
  /// output data file.
  int dump_samples_;

  ///= Handlers
  virtual void handle_init ();
  void handle_run ();
  void handle_wait_for_completion ();
  void handle_shutdown ();
  void handle_dump_stats ();
  void handle_signal_peer ();
  void handle_wait_to_start ();
};

#include /**/ "ace/post.h"
#endif /* TAO_Notify_Tests_APPLICATION_COMMAND_H */
