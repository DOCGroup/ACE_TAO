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
  void oneway_op (CORBA::ULong request_number,
                  CORBA::Environment &ACE_TRY_ENV =
                    TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  void twoway_op (CORBA::ULong request_number,
                  CORBA::Environment &ACE_TRY_ENV =
                    TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  void shutdown (CORBA::Environment &ACE_TRY_ENV =
                   TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  CORBA::ORB_var orb_;
  // The ORB
};

#if defined(__ACE_INLINE__)
#include "test_i.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_RELIABLE_TEST_I_H */
