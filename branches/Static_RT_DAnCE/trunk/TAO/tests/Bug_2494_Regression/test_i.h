// $Id$

#ifndef TAO_BUG_2494_REGRESSION_TEST_I_H
#define TAO_BUG_2494_REGRESSION_TEST_I_H

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
  char *test_method (const char *x);

  void shutdown ();

private:
  CORBA::ORB_var orb_;
  // The ORB
};

#if defined(__ACE_INLINE__)
#include "test_i.inl"
#endif /* __ACE_INLINE__ */

#endif /* TAO_BUG_2494_REGRESSION_TEST_I_H */
