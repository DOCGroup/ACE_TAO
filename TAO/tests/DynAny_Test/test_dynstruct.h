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
  Test_DynStruct (CORBA::ORB_var orb, bool debug);
  ~Test_DynStruct () = default;
  Test_DynStruct (Test_DynStruct const&) = delete;
  Test_DynStruct (Test_DynStruct&&) = delete;
  Test_DynStruct& operator= (Test_DynStruct const&) = delete;
  Test_DynStruct& operator= (Test_DynStruct&&) = delete;

  char const* test_name () const;

  int run_test ();

private:
  CORBA::ORB_var orb_;
  CORBA::ULong error_count_ {};
  bool debug_ {};
};

#endif /* TEST_DYNSTRUCT_H */
