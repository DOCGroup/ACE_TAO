// $Id$

// ============================================================================
//
// = LIBRARY
//   TAO/tests/Leader_Followers/
//
// = FILENAME
//   test_i.h
//
// = AUTHOR
//   Irfan Pyarali
//
// ============================================================================

#include "testS.h"

class test_i : public POA_test
{
  // = TITLE
  //   Simple test implementation.
  //
public:
  test_i (CORBA::ORB_ptr orb);
  // ctor.

  // = The test interface methods.
  CORBA::ULong method (CORBA::ULong work);

  void shutdown (void);

private:
  CORBA::ORB_var orb_;
  // The ORB.
};
