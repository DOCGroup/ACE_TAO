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
  Test_DynUnion (CORBA::ORB_var orb, bool debug);
  ~Test_DynUnion () = default;
  Test_DynUnion (Test_DynUnion const&) = delete;
  Test_DynUnion (Test_DynUnion&&) = delete;
  Test_DynUnion& operator= (Test_DynUnion const&) = delete;
  Test_DynUnion& operator= (Test_DynUnion&&) = delete;

  char const* test_name () const;

  int run_test ();

private:
  CORBA::ORB_var orb_;
  CORBA::ULong error_count_ {};
  bool debug_ {};
};

#endif /* TEST_DYNUNION_H */

