// -*- c++ -*-
// $Id$
// ============================================================================
//
// = LIBRARY
//    TAO/tests/DynAny_Test
//
// = FILENAME
//    test_dynany.h
//
// = DESCRIPTION
//    Header file for class to test the simple dynany.
//
// = AUTHOR
//    Jeff Parsons <parsons@cs.wustl.edu>
//
// ============================================================================

#if !defined (TEST_DYNANY_H)
#define TEST_DYNANY_H

#include "tao/corba.h"

class Test_DynAny
{
public:
  Test_DynAny (CORBA::ORB_var orb);
  ~Test_DynAny (void);

  const char* test_name (void) const;

  int run_test (void);

private:
  CORBA::ORB_var orb_;
  char* test_name_;
  CORBA::ULong error_count_;
};

#endif /* TEST_DYNANY_H */

