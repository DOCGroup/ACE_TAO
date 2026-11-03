//=============================================================================
/**
 *  @file    test_dynarray.h
 *
 *  Header file for class to test DynArray.
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
  Test_DynArray (CORBA::ORB_var orb, bool debug);
  ~Test_DynArray () = default;
  Test_DynArray (Test_DynArray const&) = delete;
  Test_DynArray (Test_DynArray&&) = delete;
  Test_DynArray& operator= (Test_DynArray const&) = delete;
  Test_DynArray& operator= (Test_DynArray&&) = delete;

  char const* test_name () const;

  int run_test ();

private:
  CORBA::ORB_var orb_;
  CORBA::ULong error_count_ {};
  bool debug_ {};
};

#endif /* TEST_DYNARRAY_H */
