// -*- c++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Concurrency
//
// = FILENAME
//    CC_tests.h
//
// = DESCRIPTION
//      This class implements a number of test objects to test the
//      concurrency service.
//
// = AUTHORS
//    Torben Worm <tworm@cs.wustl.edu>
//
// ============================================================================

#include "tao/corba.h"
#include "orbsvcs/CosConcurrencyControlC.h"
#include "orbsvcs/CosNamingC.h"
#include "CC_naming_service.h"

#ifndef _CC_TESTS_H_
#define _CC_TESTS_H_

// Return codes for the tests
enum
{
  CC_FAIL,
  CC_SUCCESS = 1
};

class CC_Test
{
  // = TITLE
  //    Defines an abstract base class for a test
  //
  // = DESCRIPTION
  //    This class declares an interface to run the test of the
  //    concurrency service.
public:
  CC_Test (CC_naming_service *ns);
  // Default constructor

  virtual ~CC_Test (void);
  // Destructor

  virtual int run (int times_to_run = 1) = 0;
  // Run the test times_to_run number of times. Returns CC_SUCCESS on
  // success CC_FAIL otherwise.

  CosConcurrencyControl::LockSet_ptr create_lock_set (void);
  // Create a new lock set using the default global lock set factory
  // from the naming service.

  char *get_lock_mode_name (CosConcurrencyControl::lock_mode mode);
  // Returns a human readable string from the lock mode enum.

protected:
  int result;
  // The result of the test being performed.

  CC_naming_service *naming_service_;
  // The naming service beeing used to register and look up locks
};

class Test_Single_Lock_With_Mode : public CC_Test
{
  // = TITLE
  //   This is a simple test that checks that it is possible to set
  //   the lock in the desired mode, try it, and release it.
public:
  Test_Single_Lock_With_Mode (CC_naming_service *naming_service,
                              CosConcurrencyControl::lock_mode mode);
  // Default constructor. The naming service must be initialized
  // before calling this method. The mode is the mode of the lock to
  // be tested.

  virtual ~Test_Single_Lock_With_Mode (void);
  // Destructor

  virtual int run (int times_to_run = 1);
  // Runs the test the specified number of times.

private:
  CosConcurrencyControl::lock_mode mode_;
  // The lock mode of the lock being tested
};

class Test_Setup_LockSet : public CC_Test
{
  // = TITLE
  //   This class creates a read lock, registeres it with the naming
  //   service and locks it.
public:
  Test_Setup_LockSet (CC_naming_service *naming_service_,
                      char *name);
  // Default constructor. The naming service must be initialized
  // before calling this method. The name is the name the lock will be
  // registered under in the naming service.

  virtual ~Test_Setup_LockSet (void);
  // Destructor

  virtual int run (int times_to_run = 1);
  // Runs the test the specified number of times.

private:
  char *my_name_;
  // The name of the lock
};

class Test_Use_Already_Created_LockSet : public CC_Test
{
  // = TITLE
  //   This class looks up the lock in the naming service and locks
  //   it.
public:
  Test_Use_Already_Created_LockSet (CC_naming_service *naming_service_,
                                   char *name);
  // Default constructor. The naming service must be initialized
  // before calling this method. The name is the name the lock will be
  // looked up under in the naming service.

  virtual ~Test_Use_Already_Created_LockSet (void);
  // Destructor

  virtual int run (int times_to_run = 1);
  // Runs the test the specified number of times.

private:
  char *my_name_;
  // The name of the lock
};

class Test_Unlock_Already_Created_LockSet : public CC_Test
{
  // = TITLE
  //   This class looks up the lock in the naming service and unlocks
  //   it.
public:
  Test_Unlock_Already_Created_LockSet (CC_naming_service *naming_service_,
                                       char *name);
  // Default constructor. The naming service must be initialized
  // before calling this method. The name is the name the lock will be
  // looked up under in the naming service.

  virtual ~Test_Unlock_Already_Created_LockSet (void);
  // Destructor

  virtual int run (int times_to_run = 1);
  // Runs the test the specified number of times.

private:
  char *my_name_;
  // The name of the lock
};

class Test_Release_Not_Held_Lock : public CC_Test
{
  // = TITLE
  //   This class tests that the LockNotHeld exception is thrown if a
  //   not held lock is released.
public:
  Test_Release_Not_Held_Lock (CC_naming_service *naming_service_,
                                CosConcurrencyControl::lock_mode mode_);
  // Default constructor. The naming service must be initialized
  // before calling this method. The mode is the mode of the lock

  virtual ~Test_Release_Not_Held_Lock (void);
  // Destructor

  virtual int run (int times_to_run = 1);
  // Runs the test the specified number of times.

private:
  CosConcurrencyControl::lock_mode mode_;
  // The lock mode of the lock being tested
};

#endif /* !defined (_CC_TESTS_H_) */
