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

class Callback_i : public POA_Callback
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

  void shutdown (CORBA::Boolean is_clean,
                 CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException));
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
                           Callback_ptr callback,
                           CORBA::Environment&)
    ACE_THROW_SPEC ((CORBA::SystemException));

  void shutdown_now (CORBA::Boolean is_clean,
                     CORBA::Environment&)
    ACE_THROW_SPEC ((CORBA::SystemException));

  void shutdown (CORBA::Environment&)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  CORBA::ORB_var orb_;
  // The ORB
};

#if defined(__ACE_INLINE__)
#include "test_i.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_FAILURE_TEST_I_H */
