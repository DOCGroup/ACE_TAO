/* -*- C++ -*- */
/**
 *  @file Driver.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_Notify_Tests_DRIVER_H
#define TAO_Notify_Tests_DRIVER_H
#include /**/ "ace/pre.h"

#include "notify_test_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Service_Config.h"
#include "ace/Task.h"

#include "tao/ORB.h"

#include "Driver_Base.h"

class TAO_Notify_Tests_Command_Builder;
class TAO_Notify_Tests_Activation_Manager;

/**
 * @class TAO_Notify_Tests_Worker
 *
 * @brief A Task to execute commands asynchronously.
 *
 */
class TAO_Notify_Tests_Worker : public ACE_Task_Base
{
public:
  TAO_Notify_Tests_Worker (void);
  // ctor

  /// Set the command builder.
  void command_builder (TAO_Notify_Tests_Command_Builder* cmd_builder);

  virtual int svc (void);
  // The thread entry point.

  void shutdown (void);

private:
  /// The command builder
  TAO_Notify_Tests_Command_Builder* cmd_builder_;
};



/**
 * Run a server thread
 *
 * Use the ACE_Task_Base class to run server threads
 */
class TAO_Notify_Tests_ORB_Run_Worker : public ACE_Task_Base
{
public:
  TAO_Notify_Tests_ORB_Run_Worker (void);
  // ctor

  void orb (CORBA::ORB_ptr orb);

  /// Srt the run period.
  void run_period (ACE_Time_Value run_period);

  virtual int svc (void);
  // The thread entry point.

private:
  /// The orb
  CORBA::ORB_var orb_;

  /// ORB Run Period.
  ACE_Time_Value run_period_;
};


/**
 * @class TAO_Notify_Tests_Driver
 *
 * @brief A default Application Starter.
 *
 */
class TAO_NOTIFY_TEST_Export TAO_Notify_Tests_Driver : public TAO_Notify_Tests_Driver_Base
{
public:
  /// Constructor
  TAO_Notify_Tests_Driver (void);

  /// Destructor
  ~TAO_Notify_Tests_Driver ();

  /// Init
  int init (int argc, ACE_TCHAR *argv[]);

  /// Execute the commands.
  void run (void);

  /// Shutdown
  virtual void shutdown (void);

protected:
  /// Parse command line parameters.
  int parse_args (int argc, ACE_TCHAR *argv[]);

  /// The command builder
  TAO_Notify_Tests_Command_Builder* cmd_builder_;

  /// Manage activation of periodic suppliers and consumers.
  TAO_Notify_Tests_Activation_Manager* activation_manager_;

  /// Thread in which to run commands.
  TAO_Notify_Tests_Worker worker_;

  /// Thread in which to run the orb.
  TAO_Notify_Tests_ORB_Run_Worker orb_run_worker_;

  /// The ORB we run.
  CORBA::ORB_var orb_;

  /// ORB Run Period.
  ACE_Time_Value run_period_;

  /// The file for output
  ACE_CString ior_output_file_;

  /// The file for input
  ACE_CString ior_input_file_;

  /// Set to skip the check for multiple priority levels.
  int skip_priority_levels_check_;
};

#include /**/ "ace/post.h"
#endif /* TAO_Notify_Tests_DRIVER_H */
