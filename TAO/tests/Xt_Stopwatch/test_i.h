// $Id$

// ============================================================================
//
// = LIBRARY
//   TAO/tests/Xt_Stopwatch
//
// = FILENAME
//   test_i.h
//
// = AUTHOR
//   Bala
//
// ============================================================================

#ifndef SERVER_TEST_I_H
#define SERVER_TEST_I_H

#include "testS.h"

#if defined(ACE_HAS_XT)

class Stopwatch_imp : public POA_Stopwatch
{
  // = TITLE
  //   Stopwatch implementation class
  //
  // = DESCRIPTION
  //   Implements the Stopwatch interface in test.idl
  //
public:
  Stopwatch_imp (CORBA::ORB_ptr);
  // ctor

  // = The Simple_Server methods.
  void start (CORBA::Environment&);
  void stop (CORBA::Environment&);
  void shutdown (CORBA::Environment&);

private:
  CORBA::ORB_var orb_;
  // The ORB
};

#if defined(__ACE_INLINE__)
#include "test_i.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_HAS_FL */

#endif /* TAO_FL_SERVER_TEST_I_H */
