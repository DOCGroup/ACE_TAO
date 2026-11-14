
//=============================================================================
/**
 *  @file    CC_tests.h
 *
 *    This class implements a number of test objects to test the
 *    concurrency service.
 *
 *  @author Torben Worm <tworm@cs.wustl.edu>
 */
//=============================================================================

#include "orbsvcs/CosConcurrencyControlC.h"
#include "orbsvcs/CosNamingC.h"
#include "CC_naming_service.h"
#include <string>

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
  explicit CC_Test (CC_naming_service *ns);
  CC_Test (CC_Test const &) = delete;
  CC_Test (CC_Test &&) = delete;
  CC_Test &operator= (CC_Test const &) = delete;
  CC_Test &operator= (CC_Test &&) = delete;

  virtual ~CC_Test () = default;

  /// Run the test times_to_run number of times. Returns CC_SUCCESS on
  /// success CC_FAIL otherwise.
  virtual int run (int times_to_run = 1) = 0;

  /// Create a new lock set using the default global lock set factory
  /// from the naming service.
  CosConcurrencyControl::LockSet_var create_lock_set ();

protected:
  /// Borrowed naming service used to register and look up locks.
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

  ~Test_Single_Lock_With_Mode () override = default;
  Test_Single_Lock_With_Mode (Test_Single_Lock_With_Mode const &) = delete;
  Test_Single_Lock_With_Mode (Test_Single_Lock_With_Mode &&) = delete;
  Test_Single_Lock_With_Mode &operator= (Test_Single_Lock_With_Mode const &) = delete;
  Test_Single_Lock_With_Mode &operator= (Test_Single_Lock_With_Mode &&) = delete;

  /// Runs the test the specified number of times.
  int run (int times_to_run = 1) override;

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
  Test_Setup_LockSet (CC_naming_service *naming_service,
                      std::string const &name);

  ~Test_Setup_LockSet () override = default;
  Test_Setup_LockSet (Test_Setup_LockSet const &) = delete;
  Test_Setup_LockSet (Test_Setup_LockSet &&) = delete;
  Test_Setup_LockSet &operator= (Test_Setup_LockSet const &) = delete;
  Test_Setup_LockSet &operator= (Test_Setup_LockSet &&) = delete;

  /// Runs the test the specified number of times.
  int run (int times_to_run = 1) override;

private:
  /// The name of the lock
  std::string my_name_;
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
  Test_Use_Already_Created_LockSet (CC_naming_service *naming_service,
                                   std::string const &name);

  ~Test_Use_Already_Created_LockSet () override = default;
  Test_Use_Already_Created_LockSet (Test_Use_Already_Created_LockSet const &) = delete;
  Test_Use_Already_Created_LockSet (Test_Use_Already_Created_LockSet &&) = delete;
  Test_Use_Already_Created_LockSet &operator= (Test_Use_Already_Created_LockSet const &) = delete;
  Test_Use_Already_Created_LockSet &operator= (Test_Use_Already_Created_LockSet &&) = delete;

  /// Runs the test the specified number of times.
  int run (int times_to_run = 1) override;

private:
  /// The name of the lock
  std::string my_name_;
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
  Test_Unlock_Already_Created_LockSet (CC_naming_service *naming_service,
                                       std::string const &name);

  ~Test_Unlock_Already_Created_LockSet () override = default;
  Test_Unlock_Already_Created_LockSet (Test_Unlock_Already_Created_LockSet const &) = delete;
  Test_Unlock_Already_Created_LockSet (Test_Unlock_Already_Created_LockSet &&) = delete;
  Test_Unlock_Already_Created_LockSet &operator= (Test_Unlock_Already_Created_LockSet const &) = delete;
  Test_Unlock_Already_Created_LockSet &operator= (Test_Unlock_Already_Created_LockSet &&) = delete;

  /// Runs the test the specified number of times.
  int run (int times_to_run = 1) override;

private:
  /// The name of the lock
  std::string my_name_;
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
  /// The naming service must be initialized before the test is created.
  Test_Release_Not_Held_Lock (CC_naming_service *naming_service,
                              CosConcurrencyControl::lock_mode mode);

  ~Test_Release_Not_Held_Lock () override = default;
  Test_Release_Not_Held_Lock (Test_Release_Not_Held_Lock const &) = delete;
  Test_Release_Not_Held_Lock (Test_Release_Not_Held_Lock &&) = delete;
  Test_Release_Not_Held_Lock &operator= (Test_Release_Not_Held_Lock const &) = delete;
  Test_Release_Not_Held_Lock &operator= (Test_Release_Not_Held_Lock &&) = delete;

  /// Runs the test the specified number of times.
  int run (int times_to_run = 1) override;

private:
  /// The lock mode of the lock being tested
  CosConcurrencyControl::lock_mode mode_;
};

#endif /* !defined (_CC_TESTS_H_) */
