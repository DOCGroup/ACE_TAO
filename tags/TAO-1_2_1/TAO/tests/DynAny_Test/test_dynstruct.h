// -*- c++ -*-
// $Id$
// ============================================================================
//
// = LIBRARY
//    TAO/tests/DynAny_Test
//
// = FILENAME
//    test_dynstruct.h
//
// = DESCRIPTION
//    Header file for class to test DynStruct.
//
// = AUTHOR
//    Jeff Parsons <parsons@cs.wustl.edu>
//
// ============================================================================

#if !defined (TEST_DYNSTRUCT_H)
#define TEST_DYNSTRUCT_H

#include "tao/corba.h"

class Test_DynStruct
{
public:
  Test_DynStruct (CORBA::ORB_var orb);
  ~Test_DynStruct (void);

  const char* test_name (void) const;

  int run_test (void);

private:
  CORBA::ORB_var orb_;
  char* test_name_;
  CORBA::ULong error_count_;
};

#endif /* TEST_DYNSTRUCT_H */
