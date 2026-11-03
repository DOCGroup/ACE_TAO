//=============================================================================
/**
 *  @file    test_dynsequence.h
 *
 *  Header file for class to test DynSequence.
 *
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================


#if !defined (TEST_DYNSEQUENCE_H)
#define TEST_DYNSEQUENCE_H

#include "tao/ORB.h"

class Test_DynSequence
{
public:
  Test_DynSequence (CORBA::ORB_var orb, bool debug);
  ~Test_DynSequence () = default;
  Test_DynSequence (Test_DynSequence const&) = delete;
  Test_DynSequence (Test_DynSequence&&) = delete;
  Test_DynSequence& operator= (Test_DynSequence const&) = delete;
  Test_DynSequence& operator= (Test_DynSequence&&) = delete;

  char const* test_name () const;

  int run_test ();

private:
  CORBA::ORB_var orb_;
  CORBA::ULong error_count_ {};
  bool debug_ {};
};

#endif /* TEST_DYNSEQUENCE_H */

