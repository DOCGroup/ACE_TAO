/* -*- C++ -*- */
/**
 *  @file Default_Application_Starter.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_NS_DEFAULT_APPLICATION_STARTER_H
#define TAO_NS_DEFAULT_APPLICATION_STARTER_H
#include "ace/pre.h"

#include "notify_test_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Application_Starter.h"
#include "ace/Service_Config.h"
#include "ace/Task.h"

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

/**
 * @class TAO_NS_Default_Application_Starter
 *
 * @brief A default Application Starter.
 *
 */
class TAO_NOTIFY_TEST_Export TAO_NS_Default_Application_Starter : public TAO_NS_Application_Starter
{
public:
  /// Constuctor
  TAO_NS_Default_Application_Starter (void);

  /// Destructor
  ~TAO_NS_Default_Application_Starter ();

  /// = Service_Object virtual method overloads.
  virtual int init (int argc, char *argv[]);
  virtual int fini (void);

  /// Bootstrap method should be called once by the application to start things off.
  void bootstrap (CORBA::ORB_var &orb ACE_ENV_ARG_DECL);

  /// Execute the commands.
  void run (ACE_ENV_SINGLE_ARG_DECL);

  /// Shutdown
  virtual void shutdown (void);

protected:
  /// The command builder
  TAO_NS_Command_Builder* cmd_builder_;

  /// Manage activation of periodic suppliers and consumers.
  TAO_NS_Activation_Manager* activation_manager_;

  /// Thread in which to run commands.
  TAO_NS_Worker worker_;

  /// The ORB we run.
  CORBA::ORB_var orb_;

  /// ORB Run Period.
  ACE_Time_Value run_period_;
};

ACE_STATIC_SVC_DECLARE_EXPORT (TAO_NOTIFY_TEST, TAO_NS_Default_Application_Starter);
ACE_FACTORY_DECLARE (TAO_NOTIFY_TEST, TAO_NS_Default_Application_Starter);

#if defined (__ACE_INLINE__)
#include "Default_Application_Starter.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NS_DEFAULT_APPLICATION_STARTER_H */
