// -*- C++ -*-

//=============================================================================
/**
 * @file      Test_Macros.h
 *
 * $Id$
 *
 * @author    James H. Hill
 */
//=============================================================================

#ifndef _CUTS_TEST_MACROS_H_
#define _CUTS_TEST_MACROS_H_

#define CUTS_TEST_EXPORT(test) \
  extern "C" CUTS_TEST_Export CUTS_Test * create_cuts_test (void) \
  { \
    return new test (); \
  }

#define CUTS_ADD_UNIT_TEST(unit_test) \
  this->add_unit_test (new unit_test ());

#define CUTS_VERIFY_TEST(cond, errmsg) \
  if (cond) \
  { \
    std::ostringstream message; \
    message << errmsg << std::ends; \
    this->message_ = message.str (); \
    return 1; \
  }

#define CUTS_TEST_DECLARE(test) \
  class test : public CUTS_Test \
  { \
  public: \
    test (void); \
    virtual ~test (void); \
  }

#define CUTS_TEST_IMPL_BEGIN(test, name) \
  test::test (void) \
  : CUTS_Test (name) \
  {

#define CUTS_TEST_IMPL_END(test) \
  } \
  test::~test (void) { }

#define CUTS_UNIT_TEST_DECLARE(test) \
  class test : public CUTS_Unit_Test \
  { \
  public: \
    test (void); \
    virtual ~test (void); \
    int run (int argc, char * argv []); \
  }

#define CUTS_UNIT_TEST_IMPL(test, name) \
  test::test (void) \
  : CUTS_Unit_Test (name) { } \
  test::~test (void) { }

#define CUTS_UNIT_TEST_RUN_IMPL(test) \
  int test::run (int argc, char * argv [])

#endif  // !defined _CUTS_TEST_MACROS_H_
