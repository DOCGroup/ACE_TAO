// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    DLL_Test_Impl.h
//
// = AUTHOR
//     Kirthika Parameswaran  <kirthika@cs.wustl.edu>
//
// ============================================================================

#ifndef ACE_TESTS_DLL_TEST_IMPL_H
#define ACE_TESTS_DLL_TEST_IMPL_H

#include "DLL_Test.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class Hello_Impl : public Hello
{
  // = TITLE
  //    The Hello class in the dynamically linkable library.
  //
  // = DESCRIPTION
  //    This class is used in this example to show how a library can
  //    be loaded on demand and its methods called on getting the
  //    symbols from the library.
public:
  Hello_Impl (void);
  // Constructor

  ~Hello_Impl (void);
  // Destructor

  void say_next (void);
  // See the documentation in the base class
};

#endif /* ACE_TESTS_DLL_TEST_IMPL_H */
