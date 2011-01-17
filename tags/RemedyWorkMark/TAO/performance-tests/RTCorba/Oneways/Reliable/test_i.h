// $Id$

// ============================================================================
//
// = LIBRARY
//   TAO/performance-tests/RTCorba/Oneways/Reliable
//
// = FILENAME
//   test_i.h
//
// = AUTHOR
//   Jeff Parsons
//
// ============================================================================

#ifndef TAO_RELIABLE_TEST_I_H
#define TAO_RELIABLE_TEST_I_H

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
  Test_i (CORBA::ORB_ptr orb);
  // ctor

  // = The Test methods.
  void oneway_payload_test (const Test::data &d);

  void oneway_work_test (CORBA::ULong work);

  void twoway_work_test (CORBA::ULong work);

  void shutdown (void);

private:
  CORBA::ORB_var orb_;
  // The ORB
};

#if defined(__ACE_INLINE__)
#include "test_i.inl"
#endif /* __ACE_INLINE__ */

#endif /* TAO_RELIABLE_TEST_I_H */
