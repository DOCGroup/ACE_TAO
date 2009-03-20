/* -*- C++ -*- */
/**
 *  @file Application_Command.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
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
 *
 */
class TAO_NOTIFY_TEST_Export TAO_Notify_Tests_Application_Command : public TAO_Notify_Tests_Command
{
public:
  /// Constructor
  TAO_Notify_Tests_Application_Command (void);

  /// Destructor
  ~TAO_Notify_Tests_Application_Command ();

  /// Parse args and populate options.
  virtual void init (ACE_Arg_Shifter& arg_shifter);

  /// Execute the command.
  virtual void execute_i (void);

  /// Get the command name.
  virtual const char* get_name (void);
  static const char* name (void);

protected:
#ifdef HPUX_11
   // Remove clash with /usr/include/machine/cpu.h and /usr/include/pa/cpu.h
#undef COMMAND
#endif
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
  virtual void handle_init (void);
  void handle_run (void);
  void handle_wait_for_completion (void);
  void handle_shutdown (void);
  void handle_dump_stats (void);
  void handle_signal_peer (void);
  void handle_wait_to_start (void);
};

#include /**/ "ace/post.h"
#endif /* TAO_Notify_Tests_APPLICATION_COMMAND_H */
