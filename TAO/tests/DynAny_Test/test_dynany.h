//=============================================================================
/**
 *  @file    test_dynany.h
 *
 *  Header file for class to test the simple dynany.
 *
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================


#if !defined (TEST_DYNANY_H)
#define TEST_DYNANY_H

#include "tao/ORB.h"
#include "tao/AnyTypeCode/AnyTypeCode_methods.h"

class Test_DynAny
{
public:
  Test_DynAny (CORBA::ORB_var orb, int debug);
  ~Test_DynAny ();

  const char* test_name () const;

  int run_test ();

private:
  CORBA::ORB_var orb_;
  char* test_name_;
  CORBA::ULong error_count_;
  int debug_;
};

#endif /* TEST_DYNANY_H */

