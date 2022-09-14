//=============================================================================
/**
 *  @file    test_dynenum.h
 *
 *  Header file for class to test DynEnum.
 *
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================


#if !defined (TEST_DYNENUM_H)
#define TEST_DYNENUM_H

#include "tao/ORB.h"

class Test_DynEnum
{
public:
  Test_DynEnum (CORBA::ORB_var orb, int debug);
  ~Test_DynEnum ();

  const char* test_name () const;

  int run_test ();

private:
  CORBA::ORB_var orb_;
  char* test_name_;
  CORBA::ULong error_count_;
  int debug_;
};

#endif /* TEST_DYNENUM_H */

