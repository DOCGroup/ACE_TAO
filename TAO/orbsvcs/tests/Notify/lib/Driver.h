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

#ifndef TAO_NS_DRIVER_H
#define TAO_NS_DRIVER_H
#include "ace/pre.h"

#include "notify_test_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Service_Config.h"
#include "ace/Task.h"
#include "tao/corba.h"
#include "Driver_Base.h"

class TAO_NS_Command_Builder;
class TAO_NS_Activation_Manager;

/**
 * @class TAO_NS_Worker
 *
 * @brief A Task to execute commands asynchronously.
 *
 */
class TAO_NS_Worker : public ACE_Task_Base
{
  // = TITLE
  //   Run a server thread
  //
  // = DESCRIPTION
  //   Use the ACE_Task_Base class to run server threads
  //
public:
  TAO_NS_Worker (void);
  // ctor

  /// Set the command builder.
  void command_builder (TAO_NS_Command_Builder* cmd_builder);

  virtual int svc (void);
  // The thread entry point.

  void shutdown (void);

private:
  /// The command builder
  TAO_NS_Command_Builder* cmd_builder_;
};



class TAO_NS_ORB_Run_Worker : public ACE_Task_Base
{
  // = TITLE
  //   Run a server thread
  //
  // = DESCRIPTION
  //   Use the ACE_Task_Base class to run server threads
  //
public:
  TAO_NS_ORB_Run_Worker (void);
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
 * @class TAO_NS_Driver
 *
 * @brief A default Application Starter.
 *
 */
class TAO_NOTIFY_TEST_Export TAO_NS_Driver : public TAO_NS_Driver_Base
{
public:
  /// Constuctor
  TAO_NS_Driver (void);

  /// Destructor
  ~TAO_NS_Driver ();

  /// Init
  int init (int argc, ACE_TCHAR *argv[] ACE_ENV_ARG_DECL);

  /// Execute the commands.
  void run (ACE_ENV_SINGLE_ARG_DECL);

  /// Shutdown
  virtual void shutdown (void);

protected:
  /// Parse command line parameters.
  int parse_args (int argc, char *argv[]);

  /// The command builder
  TAO_NS_Command_Builder* cmd_builder_;

  /// Manage activation of periodic suppliers and consumers.
  TAO_NS_Activation_Manager* activation_manager_;

  /// Thread in which to run commands.
  TAO_NS_Worker worker_;

  /// Thread in which to run the orb.
  TAO_NS_ORB_Run_Worker orb_run_worker_;

  /// The ORB we run.
  CORBA::ORB_var orb_;

  /// ORB Run Period.
  ACE_Time_Value run_period_;

  /// The file for output
  ACE_CString ior_output_file_;

  /// The file for input
  ACE_CString ior_input_file_;
};

#include "ace/post.h"
#endif /* TAO_NS_DRIVER_H */
