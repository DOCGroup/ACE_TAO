// -*- C++ -*-

//=============================================================================
/**
 * @file Test_Server_Module.h
 *
 * @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================

#ifndef TEST_SERVER_MODULE_H
#define TEST_SERVER_MODULE_H

#include /**/ "ace/pre.h"

#include "Test_Server_Module_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Task.h"
#include "Test_i.h"


/**
 * @class Test_Server_Module
 *
 * @brief The shared object that is instantiated when the server-side
 *        test module/library is dynamically loaded.
 *
 * This class handles all of the server-side ORB tasks, such as
 * activating the test CORBA object, and running the ORB.
 */

class Test_Server_Module_Export Test_Server_Module : public ACE_Task_Base
{
public:
  Test_Server_Module () = default;

  /// Initializes object when dynamic linking occurs.
  int init (int argc, ACE_TCHAR *argv[]) override;

  /// Terminates object when dynamic unlinking occurs.
  int fini () override;

  /// Run by a daemon thread to handle deferred processing.
  int svc () override;

  ~Test_Server_Module () override;
  Test_Server_Module (Test_Server_Module const &) = delete;
  Test_Server_Module (Test_Server_Module &&) = delete;
  Test_Server_Module &operator= (Test_Server_Module const &) = delete;
  Test_Server_Module &operator= (Test_Server_Module &&) = delete;

private:
  /// Reference to the ORB.
  CORBA::ORB_var orb_ {};

  /// Reference to the POA.
  PortableServer::POA_var poa_ {};

  /// Reference to the POA Manager.
  PortableServer::POAManager_var poa_manager_ {};

  /// The servant that implements the "Test" object.
  Test_i servant_;
};

ACE_FACTORY_DECLARE (Test_Server_Module, Test_Server_Module)

#include /**/ "ace/post.h"

#endif  /* TEST_SERVER_MODULE_H */
