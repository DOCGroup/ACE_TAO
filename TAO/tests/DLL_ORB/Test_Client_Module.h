// -*- C++ -*-

//=============================================================================
/**
 * @file Test_Client_Module.h
 *
 * $Id$
 *
 * @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================

#ifndef TEST_MODULE_H
#define TEST_MODULE_H

#include "ace/pre.h"

#include "Test_Client_Module_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Task.h"
#include "ace/Service_Config.h"

#include "TestC.h"

/**
 * @class Test_Client_Module
 *
 * @brief The shared object that is instantiated when the client-side
 *        test module/library is dynamically loaded.
 *
 * This class invokes an operation on a "remote" CORBA object.
 */

class Test_Client_Module_Export Test_Client_Module : public ACE_Task_Base
{
public:

  /// Initializes object when dynamic linking occurs.
  virtual int init (int argc, ACE_TCHAR *argv[]);

  /// Terminates object when dynamic unlinking occurs.
  virtual int fini (void);

  /// Run by a daemon thread to handle deferred processing.
  virtual int svc (void);

private:

  /// Reference to the ORB.
  CORBA::ORB_var orb_;

  /// Reference to the test object.
  Test_var test_;

};

ACE_FACTORY_DECLARE (Test_Client_Module, Test_Client_Module)

#include "ace/post.h"

#endif  /* TEST_MODULE_H */
