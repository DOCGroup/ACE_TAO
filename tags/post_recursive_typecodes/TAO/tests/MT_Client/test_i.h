// $Id$

// ============================================================================
//
// = LIBRARY
//   TAO/tests/MT_Client
//
// = FILENAME
//   test_i.h
//
// = AUTHOR
//   Carlos O'Ryan
//
// ============================================================================

#ifndef TAO_MT_CLIENT_TEST_I_H
#define TAO_MT_CLIENT_TEST_I_H

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
  void test_method (CORBA::Environment&);
  void shutdown (CORBA::Environment&);

private:
  CORBA::ORB_var orb_;
};

#if defined(__ACE_INLINE__)
#include "test_i.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_MT_CLIENT_TEST_I_H */
