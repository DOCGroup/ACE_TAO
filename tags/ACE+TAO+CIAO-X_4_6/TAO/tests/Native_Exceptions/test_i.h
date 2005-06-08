// $Id$

// ============================================================================
//
// = LIBRARY
//   TAO/tests/Native_Exceptions
//
// = FILENAME
//   test_i.h
//
// = AUTHOR
//   Carlos O'Ryan
//
// ============================================================================

#ifndef TAO_NATIVE_EXCEPTIONS_TEST_I_H
#define TAO_NATIVE_EXCEPTIONS_TEST_I_H

#include "testS.h"

class Simple_Server_i : public POA_Simple_Server
{
  // = TITLE
  //   Simpler Server implementation
  //
  // = DESCRIPTION
  //   Implements the Simple_Server interface in test.idl
  //
public:
  Simple_Server_i (CORBA::ORB_ptr orb);
  // ctor

  // = The Simple_Server methods.
  CORBA::Long test_method (CORBA::Long x)
      ACE_THROW_SPEC((CORBA::SystemException));

  CORBA::Long test_raise (CORBA::Long x)
    ACE_THROW_SPEC ((Simple_Server::Failure));

  void shutdown (void) ACE_THROW_SPEC ((CORBA::SystemException));

private:
  CORBA::ORB_var orb_;
};

#if defined(__ACE_INLINE__)
#include "test_i.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_NATIVE_EXCEPTIONS_TEST_I_H */
