// -*- C++ -*-
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
#include "ace/Task.h"
#include "ace/Synch.h"
#include "ace/Condition_T.h"

class Caller : public ACE_Task_Base
{
public:
  Caller (Callback_ptr cb, CORBA::ORB_ptr orb);
  void go ();

  int svc ();

private:
  Callback_var cbobj_;
  CORBA::ORB_var orb_;
  TAO_SYNCH_MUTEX lock_;
  ACE_Condition<TAO_SYNCH_MUTEX> cond_;
  bool going_;
  CORBA::OctetSeq payload_;
};


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

  void shutdown ();
  // Safe way to shutdown

  void callback_method (const CORBA::OctetSeq& payload);
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
  //      for (int loop = 0; loop < 10; loop++)

public:
  Simple_Server_i (CORBA::ORB_ptr orb, int no_iterations);
  // ctor

  // = The Simple_Server methods.
  void ow_test ();

  CORBA::Long test_method (CORBA::Boolean do_callback);

  void callback_object (Callback_ptr callback);

  void shutdown ();

private:
  CORBA::ORB_var orb_;
  // The ORB

  int flag_;
  // Flag to indicate, whether we are ready for a remote call.

  Caller *caller_;
  // Callback Object

  int iterations_;
  // Number of times the callback needs to be called
  int ow_count_;
};

#if defined(__ACE_INLINE__)
#include "test_i.inl"
#endif /* __ACE_INLINE__ */

#endif /* TAO_BIDIRECTIONAL_TEST_I_H */
