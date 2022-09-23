//=============================================================================
/**
 *  @file    test_dynstruct.h
 *
 *  Header file for class to test DynStruct.
 *
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================


#if !defined (TEST_DYNSTRUCT_H)
#define TEST_DYNSTRUCT_H

#include "tao/ORB.h"

class Test_DynStruct
{
public:
  Test_DynStruct (CORBA::ORB_var orb, int debug);
  ~Test_DynStruct ();

  const char* test_name () const;

  int run_test ();

private:
  CORBA::ORB_var orb_;
  char* test_name_;
  CORBA::ULong error_count_;
  int debug_;
};

#endif /* TEST_DYNSTRUCT_H */
