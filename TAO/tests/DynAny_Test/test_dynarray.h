// -*- c++ -*-
// $Id$
// ============================================================================
//
// = LIBRARY
//    TAO/tests/DynAny_Test
//
// = FILENAME
//    test_dynarray.h
//
// = DESCRIPTION
//    Header file for class to test DynArray.
//
// = AUTHOR
//    Jeff Parsons <parsons@cs.wustl.edu>
//
// ============================================================================

#if !defined (TEST_DYNARRAY_H)
#define TEST_DYNARRAY_H

#include "tao/corba.h"

class Test_DynArray
{
public:
  Test_DynArray (CORBA::ORB_var orb);
  ~Test_DynArray (void);

  const char* test_name (void) const;

  int run_test (void);

private:
  CORBA::ORB_var orb_;
  char* test_name_;
  CORBA::ULong error_count_;
};

#endif /* TEST_DYNARRAY_H */
