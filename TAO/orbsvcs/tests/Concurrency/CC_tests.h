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

protected:
  int result;
  CC_naming_service *naming_service_;
private:
};

class Test_Single_Lock_With_Mode : public CC_Test
{
public:
  Test_Single_Lock_With_Mode(CC_naming_service *naming_service,
                             CosConcurrencyControl::lock_mode mode);

  ~Test_Single_Lock_With_Mode();

  virtual int run(int times_to_run = 1);
private:
};

class Test_Against_Other_LockSet : public CC_Test
{
public:
  Test_Against_Other_LockSet(CC_naming_service *naming_service,
                             char *lock_set);

  ~Test_Against_Other_LockSet();

  virtual int run(int times_to_run = 1);
private:
};

#endif /* !defined(_CC_TESTS_H_) */
