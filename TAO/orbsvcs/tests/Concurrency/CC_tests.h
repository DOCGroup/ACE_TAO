
//=============================================================================
/**
 *  @file    CC_tests.h
 *
 *  $Id$
 *
 *    This class implements a number of test objects to test the
 *    concurrency service.
 *
 *
 *  @author Torben Worm <tworm@cs.wustl.edu>
 */
//=============================================================================


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

/**
 * @class CC_Test
 *
 * @brief Defines an abstract base class for a test
 *
 * This class declares an interface to run the test of the
 * concurrency service.
 */
class CC_Test
{
public:
  /// Default constructor
  CC_Test (CC_naming_service *ns);

  /// Destructor
  virtual ~CC_Test (void);

  /// Run the test times_to_run number of times. Returns CC_SUCCESS on
  /// success CC_FAIL otherwise.
  virtual int run (int times_to_run = 1) = 0;

  /// Create a new lock set using the default global lock set factory
  /// from the naming service.
  CosConcurrencyControl::LockSet_ptr create_lock_set (void);

  /// Returns a human readable string from the lock mode enum.
  char *get_lock_mode_name (CosConcurrencyControl::lock_mode mode);

protected:
  /// The result of the test being performed.
  int result;

  /// The naming service beeing used to register and look up locks
  CC_naming_service *naming_service_;
};

/**
 * @class Test_Single_Lock_With_Mode
 *
 * @brief This is a simple test that checks that it is possible to set
 * the lock in the desired mode, try it, and release it.
 */
class Test_Single_Lock_With_Mode : public CC_Test
{
public:
  /**
   * Default constructor. The naming service must be initialized
   * before calling this method. The mode is the mode of the lock to
   * be tested.
   */
  Test_Single_Lock_With_Mode (CC_naming_service *naming_service,
                              CosConcurrencyControl::lock_mode mode);

  /// Destructor
  virtual ~Test_Single_Lock_With_Mode (void);

  /// Runs the test the specified number of times.
  virtual int run (int times_to_run = 1);

private:
  /// The lock mode of the lock being tested
  CosConcurrencyControl::lock_mode mode_;
};

/**
 * @class Test_Setup_LockSet
 *
 * @brief This class creates a read lock, registeres it with the naming
 * service and locks it.
 */
class Test_Setup_LockSet : public CC_Test
{
public:
  /**
   * Default constructor. The naming service must be initialized
   * before calling this method. The name is the name the lock will be
   * registered under in the naming service.
   */
  Test_Setup_LockSet (CC_naming_service *naming_service_,
                      char *name);

  /// Destructor
  virtual ~Test_Setup_LockSet (void);

  /// Runs the test the specified number of times.
  virtual int run (int times_to_run = 1);

private:
  /// The name of the lock
  char *my_name_;
};

/**
 * @class Test_Use_Already_Created_LockSet
 *
 * @brief This class looks up the lock in the naming service and locks
 * it.
 */
class Test_Use_Already_Created_LockSet : public CC_Test
{
public:
  /**
   * Default constructor. The naming service must be initialized
   * before calling this method. The name is the name the lock will be
   * looked up under in the naming service.
   */
  Test_Use_Already_Created_LockSet (CC_naming_service *naming_service_,
                                   char *name);

  /// Destructor
  virtual ~Test_Use_Already_Created_LockSet (void);

  /// Runs the test the specified number of times.
  virtual int run (int times_to_run = 1);

private:
  /// The name of the lock
  char *my_name_;
};

/**
 * @class Test_Unlock_Already_Created_LockSet
 *
 * @brief This class looks up the lock in the naming service and unlocks
 * it.
 */
class Test_Unlock_Already_Created_LockSet : public CC_Test
{
public:
  /**
   * Default constructor. The naming service must be initialized
   * before calling this method. The name is the name the lock will be
   * looked up under in the naming service.
   */
  Test_Unlock_Already_Created_LockSet (CC_naming_service *naming_service_,
                                       char *name);

  /// Destructor
  virtual ~Test_Unlock_Already_Created_LockSet (void);

  /// Runs the test the specified number of times.
  virtual int run (int times_to_run = 1);

private:
  /// The name of the lock
  char *my_name_;
};

/**
 * @class Test_Release_Not_Held_Lock
 *
 * @brief This class tests that the LockNotHeld exception is thrown if a
 * not held lock is released.
 */
class Test_Release_Not_Held_Lock : public CC_Test
{
public:
  /// Default constructor. The naming service must be initialized
  /// before calling this method. The mode is the mode of the lock
  Test_Release_Not_Held_Lock (CC_naming_service *naming_service_,
                                CosConcurrencyControl::lock_mode mode_);

  /// Destructor
  virtual ~Test_Release_Not_Held_Lock (void);

  /// Runs the test the specified number of times.
  virtual int run (int times_to_run = 1);

private:
  /// The lock mode of the lock being tested
  CosConcurrencyControl::lock_mode mode_;
};

#endif /* !defined (_CC_TESTS_H_) */
