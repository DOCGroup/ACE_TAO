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

#if !defined(_CC_TESTS_H_)
#define _CC_TESTS_H_

enum {CC_FAIL, CC_SUCCESS = 1};
// Return codes for the tests

class CC_Test
{
  // = TITLE
  //    Defines an abstract base class for a test
  //
  // = DESCRIPTION
  //    This class declares an interface to run the test of the
  //    concurrency service.
  //
public:
  CC_Test(CC_naming_service *ns);
  // Default constructor

  ~CC_Test(void);
  // Destructor

  virtual int run(int times_to_run = 1) = 0;
  // Run the test times_to_run number of times. Returns CC_SUCCESS on success
  // CC_FAIL otherwise

  CosConcurrencyControl::LockSet_ptr
  create_lock_set(void);
  // Create a new lock set using the default global lock set factory from 
  // the naming service

  char *get_lock_mode_name(CosConcurrencyControl::lock_mode mode);
  // Returns a human readable string from the lock mode enum

protected:
  int result;
  CC_naming_service *naming_service_;
private:
};

// This is a simple test that checks that it is possible to set the lock
// in the desired mode, try it, and release it.
class Test_Single_Lock_With_Mode : public CC_Test
{
public:
  Test_Single_Lock_With_Mode(CC_naming_service *naming_service,
                             CosConcurrencyControl::lock_mode mode);

  ~Test_Single_Lock_With_Mode();

  virtual int run(int times_to_run = 1);
private:
  CosConcurrencyControl::lock_mode mode_;
};

// This class creates a read lock, registeres it with the naming
// service and locks it.
class Test_Setup_LockSet : public CC_Test
{
public:
  Test_Setup_LockSet(CC_naming_service *naming_service_, char *name);
  
  ~Test_Setup_LockSet();

  virtual int run(int times_to_run = 1);
private:
  char *my_name_;
};

// This class looks up the lock in the naming service and locks it
class Test_Use_Already_Created_LockSet : public CC_Test
{
public:
  Test_Use_Already_Created_LockSet(CC_naming_service *naming_service_,
                                   char *name);
  
  ~Test_Use_Already_Created_LockSet();
  
  virtual int run(int times_to_run = 1);
private:
  char *my_name_;
};

// This class looks up the lock in the naming service and unlocks it
class Test_Unlock_Already_Created_LockSet : public CC_Test
{
public:
  Test_Unlock_Already_Created_LockSet(CC_naming_service *naming_service_,
                                   char *name);
  
  ~Test_Unlock_Already_Created_LockSet();
  
  virtual int run(int times_to_run = 1);
private:
  char *my_name_;
};

#endif /* !defined(_CC_TESTS_H_) */
