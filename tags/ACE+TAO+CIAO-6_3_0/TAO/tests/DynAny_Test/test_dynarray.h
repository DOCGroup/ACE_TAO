//=============================================================================
/**
 *  @file    test_dynarray.h
 *
 *  $Id$
 *
 *  Header file for class to test DynArray.
 *
 *
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================


#if !defined (TEST_DYNARRAY_H)
#define TEST_DYNARRAY_H

#include "tao/ORB.h"

class Test_DynArray
{
public:
  Test_DynArray (CORBA::ORB_var orb, int debug);
  ~Test_DynArray (void);

  const char* test_name (void) const;

  int run_test (void);

private:
  CORBA::ORB_var orb_;
  char* test_name_;
  CORBA::ULong error_count_;
  int debug_;
};

#endif /* TEST_DYNARRAY_H */
