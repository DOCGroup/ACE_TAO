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
#include "timer.h"

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
  Stopwatch_imp (CORBA::ORB_ptr, Timer_imp *);
  // ctor

  // = The Simple_Server methods.
  void start (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));

  void stop (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));

  void shutdown (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  CORBA::ORB_var orb_;
  // The ORB

  Timer_imp *timer_;
  // Hold the timer object...
};

#if defined(__ACE_INLINE__)
#include "test_i.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_HAS_XT */

#endif /* SERVER_TEST_I_H */
