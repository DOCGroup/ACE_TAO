// $Id$

// ============================================================================
//
// = LIBRARY
//   TAO/tests/TPP
//
// = FILENAME
//   test_i.h
//
// = AUTHOR
//   Carlos O'Ryan
//
// ============================================================================

#ifndef TAO_TPP_TEST_I_H
#define TAO_TPP_TEST_I_H

#include "testS.h"

class RTCORBA_COMMON_Export Test_i : public POA_Test
{
  // = TITLE
  //   An implementation for the Test interface
  //
  // = DESCRIPTION
  //   Implements the Test interface in test.idl
  //
public:
  Test_i (void);
  // ctor

  // = The Test methods.
  void test_method (CORBA::Long id);

  void shutdown (const char *orb_id);
};

#if defined(__ACE_INLINE__)
#include "test_i.inl"
#endif /* __ACE_INLINE__ */

#endif /* TAO_LATENCY_TEST_I_H */
