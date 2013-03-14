
//=============================================================================
/**
 *  @file   test_i.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan
 */
//=============================================================================


#ifndef TAO_TPP_TEST_I_H
#define TAO_TPP_TEST_I_H

#include "testS.h"

/**
 * @class Test_i
 *
 * @brief An implementation for the Test interface
 *
 * Implements the Test interface in test.idl
 */
class RTCORBA_COMMON_Export Test_i : public POA_Test
{
public:
  /// ctor
  Test_i (void);

  // = The Test methods.
  void test_method (CORBA::Long id);

  void shutdown (const char *orb_id);
};

#if defined(__ACE_INLINE__)
#include "test_i.inl"
#endif /* __ACE_INLINE__ */

#endif /* TAO_LATENCY_TEST_I_H */
