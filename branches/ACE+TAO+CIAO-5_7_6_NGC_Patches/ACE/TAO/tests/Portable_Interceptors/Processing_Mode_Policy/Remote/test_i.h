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

  void normal (CORBA::Long arg);

  CORBA::Long calculate (
                         CORBA::Long one,
                         CORBA::Long two);

  void user (void);

  void system (void);

  void shutdown (void);

private:
  CORBA::ORB_var orb_;
  // The ORB pointer (for shutdown.)
};

#endif /* TAO_INTERCEPTOR_TEST_I_H */
