// $Id$

// ============================================================================
//
// = LIBRARY
//   TAO/tests/Portable_Interceptors/Dynamic
//
// = FILENAME
//   test_i.h
//
// = AUTHOR
//   Nanbor Wang
//
// ============================================================================

#ifndef TAO_INTERCEPTOR_TEST_I_H
#define TAO_INTERCEPTOR_TEST_I_H

#include "testS.h"

class Visual_i : public POA_Test_Interceptors::Visual
{
  // = DESCRIPTION
  //   Implements the Visual interface in test.idl

public:
  Visual_i (CORBA::ORB_ptr orb);
  // ctor

  void normal (CORBA::Long arg
               ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA::Long calculate (
                         CORBA::Long one,
                         CORBA::Long two
                         ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));

  void user (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException,Test_Interceptors::Silly));

  void system (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));

  void shutdown (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  CORBA::ORB_var orb_;
  // The ORB pointer (for shutdown.)
};

#endif /* TAO_INTERCEPTOR_TEST_I_H */
