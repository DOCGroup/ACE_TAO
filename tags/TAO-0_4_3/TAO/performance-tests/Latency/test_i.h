// $Id$

// ============================================================================
//
// = LIBRARY
//   TAO/tests/Latency
//
// = FILENAME
//   test_i.h
//
// = AUTHOR
//   Carlos O'Ryan
//
// ============================================================================

#ifndef TAO_LATENCY_TEST_I_H
#define TAO_LATENCY_TEST_I_H

#include "testS.h"

class Test_i : public POA_Test
{
  // = TITLE
  //   An implementation for the Test interface
  //
  // = DESCRIPTION
  //   Implements the Test interface in test.idl
  //
public:
  Test_i (CORBA::ORB_ptr orb, int workload);
  // ctor

  // = The Test methods.
  void test_method (CORBA::Environment&)
    ACE_THROW_SPEC (());

  void shutdown (CORBA::Environment&)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  CORBA::ORB_var orb_;
  // The ORB

  int workload_;
  // How much "work" do we do on each iteration
};

#if defined(__ACE_INLINE__)
#include "test_i.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_LATENCY_TEST_I_H */
