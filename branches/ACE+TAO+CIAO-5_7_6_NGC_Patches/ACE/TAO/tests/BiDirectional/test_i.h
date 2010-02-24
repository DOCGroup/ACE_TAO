// $Id$

// ============================================================================
//
// = LIBRARY
//   TAO/tests/BiDirectional
//
// = FILENAME
//   test_i.h
//
// = AUTHOR
//   Balachandran Natarajan <bala@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_BIDIRECTIONAL_TEST_I_H
#define TAO_BIDIRECTIONAL_TEST_I_H

#include "testS.h"

class Callback_i : public POA_Callback
{
  // = TITLE
  //   A callback object to the "client"
  //
  // = DESCRIPTION
  //   To test that the server can call the client on the same
  //   connection that was established by the client
  //
public:
  Callback_i (CORBA::ORB_ptr orb);
  // ctor

  void shutdown (void);
  // Safe way to shutdown

  void callback_method (void);
  // The callback method

private:
  CORBA::ORB_var orb_;
  // The orb

};

class Simple_Server_i : public POA_Simple_Server
{
  // = TITLE
  //   Simpler Server implementation
  //
  // = DESCRIPTION
  //   Implements the Simple_Server interface in test.idl
  //
public:
  Simple_Server_i (CORBA::ORB_ptr orb, int no_iterations);
  // ctor

  // = The Simple_Server methods.
  CORBA::Long test_method (CORBA::Boolean do_callback);

  void callback_object (Callback_ptr callback);

  void shutdown (void);

  int call_client (void);

private:
  CORBA::ORB_var orb_;
  // The ORB

  int flag_;
  // Flag to indicate, whether we are ready for a remote call.

  Callback_var callback_;
  // Callback Object

  int no_iterations_;
  // Number of times the callback needs to be called
};

#if defined(__ACE_INLINE__)
#include "test_i.inl"
#endif /* __ACE_INLINE__ */

#endif /* TAO_BIDIRECTIONAL_TEST_I_H */
