// -*- c++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/DynAny_Test
//
// = FILENAME
//    test_wrapper.h
//
// = DESCRIPTION
//    Header file for the DynAny basic test manager
//
// = AUTHOR
//    Jeff Parsons <parsons@cs.wustl.edu>
//
// ============================================================================

#if !defined (TEST_WRAPPER_H)
#define TEST_WRAPPER_H

template <class T>
class Test_Wrapper
{
public:
  typedef T TEST_OBJECT;

  // constructor and destructor
  Test_Wrapper (T *);

  ~Test_Wrapper (void);

  int run_test (void);

private:
  // This does all the work.
  TEST_OBJECT* test_object_;
};

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "test_wrapper.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("test_wrapper.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* TEST_WRAPPER_H */
