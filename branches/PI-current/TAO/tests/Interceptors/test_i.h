// $Id$

// ============================================================================
//
// = LIBRARY
//   TAO/tests/Interceptors
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

  void normal (CORBA::Long arg,
               CORBA::Environment&);

  void nothing (CORBA::Environment&);

  void user (CORBA::Environment&)
    ACE_THROW_SPEC ((CORBA::SystemException,Test_Interceptors::Silly));

  void system (CORBA::Environment&);

  void shutdown (CORBA::Environment&)
    ACE_THROW_SPEC (());

private:
  CORBA::ORB_var orb_;
  // The ORB pointer (for shutdown.)
};

#endif /* TAO_INTERCEPTOR_TEST_I_H */
