// -*- c++ -*-
// $Id$
// ============================================================================
//
// = LIBRARY
//    TAO/tests/DynAny_Test
//
// = FILENAME
//    test_dynunion.h
//
// = DESCRIPTION
//    Header file for class to test DynUnion.
//
// = AUTHOR
//    Jeff Parsons <jp4@cs.wustl.edu>
//
// ============================================================================

#if !defined (TEST_DYNUNION_H)
#define TEST_DYNUNION_H

#include "tao/corba.h"

class Test_DynUnion
{
public:
  Test_DynUnion (CORBA::ORB_var orb);
  ~Test_DynUnion (void);

  const char* test_name (void) const;

  int run_test (void);

private:
  char* test_name_;
  CORBA::ORB_var orb_;
  CORBA::ULong error_count_;
};

#endif /* TEST_DYNUNION_H */

