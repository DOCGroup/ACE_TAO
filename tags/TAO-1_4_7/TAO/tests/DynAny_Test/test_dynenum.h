//=============================================================================
/**
 *  @file    test_dynenum.h
 *
 *  $Id$
 *
 *  Header file for class to test DynEnum.
 *
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
  ~Test_DynEnum (void);

  const char* test_name (void) const;

  int run_test (void);

private:
  CORBA::ORB_var orb_;
  char* test_name_;
  CORBA::ULong error_count_;
  int debug_;
};

#endif /* TEST_DYNENUM_H */

