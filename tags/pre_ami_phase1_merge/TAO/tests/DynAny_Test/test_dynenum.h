// -*- c++ -*-
// $Id$
// ============================================================================
//
// = LIBRARY
//    TAO/tests/DynAny_Test
//
// = FILENAME
//    test_dynenum.h
//
// = DESCRIPTION
//    Header file for class to test DynEnum.
//
// = AUTHOR
//    Jeff Parsons <jp4@cs.wustl.edu>
//
// ============================================================================

#if !defined (TEST_DYNENUM_H)
#define TEST_DYNENUM_H

#include "tao/corba.h"

class Test_DynEnum
{
public:
  Test_DynEnum (CORBA::ORB_var orb);
  ~Test_DynEnum (void);

  const char* test_name (void) const;

  int run_test (void);

private:
  char* test_name_;
  CORBA::ORB_var orb_;
  CORBA::ULong error_count_;
};

#endif /* TEST_DYNENUM_H */

