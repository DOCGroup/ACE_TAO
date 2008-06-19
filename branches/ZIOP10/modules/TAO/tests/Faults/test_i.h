// $Id$

// ============================================================================
//
// = LIBRARY
//   TAO/tests/MT_Server
//
// = FILENAME
//   test_i.h
//
// = AUTHOR
//   Carlos O'Ryan
//
// ============================================================================

#ifndef TAO_FAILURE_TEST_I_H
#define TAO_FAILURE_TEST_I_H

#include "testS.h"

class Callback_i
  : public virtual POA_Callback
{
  // = TITLE
  //   A callback object to shutdown the "client"
  //
  // = DESCRIPTION
  //   To test that the server can tolerate client crashes the server
  //   receives a callback object.
  //
public:
  Callback_i (CORBA::ORB_ptr orb);
  // ctor

  void shutdown (CORBA::Boolean is_clean);
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
  Simple_Server_i (CORBA::ORB_ptr orb);
  // ctor

  // = The Simple_Server methods.
  CORBA::Long test_method (CORBA::Boolean do_callback,
                           CORBA::Boolean is_clean,
                           Callback_ptr callback);

  void shutdown_now (CORBA::Boolean is_clean);

  void shutdown (void);

private:
  CORBA::ORB_var orb_;
  // The ORB
};

class Middle_i : public virtual POA_Simple_Server
{
  // = TITLE
  //   Another implementation of the simple server...
  //
  // = DESCRIPTION
  //   Implements the Simple_Server interface in test.idl
  //
public:
  Middle_i (CORBA::ORB_ptr orb,
            Simple_Server_ptr server);
  // ctor

  // = The Simple_Server methods.
  CORBA::Long test_method (CORBA::Boolean do_callback,
                           CORBA::Boolean is_clean,
                           Callback_ptr callback);

  void shutdown_now (CORBA::Boolean is_clean);

  void shutdown (void);

private:
  CORBA::ORB_ptr orb_;
  // The ORB

  Simple_Server_var server_;
  // The real server
};

#if defined(__ACE_INLINE__)
#include "test_i.inl"
#endif /* __ACE_INLINE__ */

#endif /* TAO_FAILURE_TEST_I_H */
