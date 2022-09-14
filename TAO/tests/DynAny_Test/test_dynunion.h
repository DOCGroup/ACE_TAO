//=============================================================================
/**
 *  @file    test_dynunion.h
 *
 *  Header file for class to test DynUnion.
 *
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================


#if !defined (TEST_DYNUNION_H)
#define TEST_DYNUNION_H

#include "tao/ORB.h"

class Test_DynUnion
{
public:
  Test_DynUnion (CORBA::ORB_var orb, int debug);
  ~Test_DynUnion ();

  const char* test_name () const;

  int run_test ();

private:
  CORBA::ORB_var orb_;
  char* test_name_;
  CORBA::ULong error_count_;
  int debug_;
};

#endif /* TEST_DYNUNION_H */

