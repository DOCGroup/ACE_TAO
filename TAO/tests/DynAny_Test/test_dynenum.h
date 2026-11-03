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
  Test_DynEnum (CORBA::ORB_var orb, bool debug);
  ~Test_DynEnum () = default;
  Test_DynEnum (Test_DynEnum const&) = delete;
  Test_DynEnum (Test_DynEnum&&) = delete;
  Test_DynEnum& operator= (Test_DynEnum const&) = delete;
  Test_DynEnum& operator= (Test_DynEnum&&) = delete;

  char const* test_name () const;

  int run_test ();

private:
  CORBA::ORB_var orb_;
  CORBA::ULong error_count_ {};
  bool debug_ {};
};

#endif /* TEST_DYNENUM_H */

