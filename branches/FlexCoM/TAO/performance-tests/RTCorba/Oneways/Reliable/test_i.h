
//=============================================================================
/**
 *  @file   test_i.h
 *
 *  $Id$
 *
 *  @author Jeff Parsons
 */
//=============================================================================


#ifndef TAO_RELIABLE_TEST_I_H
#define TAO_RELIABLE_TEST_I_H

#include "testS.h"

/**
 * @class Test_i
 *
 * @brief An implementation for the Test interface
 *
 * Implements the Test interface in test.idl
 */
class Test_i : public POA_Test
{
public:
  /// ctor
  Test_i (CORBA::ORB_ptr orb);

  // = The Test methods.
  void oneway_payload_test (const Test::data &d);

  void oneway_work_test (CORBA::ULong work);

  void twoway_work_test (CORBA::ULong work);

  void shutdown (void);

private:
  /// The ORB
  CORBA::ORB_var orb_;
};

#if defined(__ACE_INLINE__)
#include "test_i.inl"
#endif /* __ACE_INLINE__ */

#endif /* TAO_RELIABLE_TEST_I_H */
