// -*- c++ -*-
// $Id$
// ============================================================================
//
// = LIBRARY
//    TAO/tests/DynAny_Test
//
// = FILENAME
//    test_dynsequence.h
//
// = DESCRIPTION
//    Header file for class to test DynSequence.
//
// = AUTHOR
//    Jeff Parsons <jp4@cs.wustl.edu>
//
// ============================================================================

#if !defined (TEST_DYNSEQUENCE_H)
#define TEST_DYNSEQUENCE_H

#include "tao/corba.h"

class Test_DynSequence
{
public:
  Test_DynSequence (CORBA::ORB_var orb);
  ~Test_DynSequence (void);

  const char* test_name (void) const;

  int run_test (void);

private:
  char* test_name_;
  CORBA::ORB_var orb_;
  CORBA::ULong error_count_;
};

#endif /* TEST_DYNSEQUENCE_H */

