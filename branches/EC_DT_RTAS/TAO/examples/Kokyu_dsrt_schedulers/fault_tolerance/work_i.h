//$Id$

// ============================================================================
//
// = LIBRARY
//   TAO/tests/MT_Server
//
// = FILENAME
//   work_i.h
//
// = AUTHOR
//   Carlos O'Ryan
//
// ============================================================================

#ifndef TAO_MT_SERVER_TEST_I_H
#define TAO_MT_SERVER_TEST_I_H

#include "workS.h"
#include "tao/RTScheduling/RTScheduler_Manager.h"
#include "Task_Stats.h"

class Complex_Server_i : public POA_Complex_Server
{
  // = TITLE
  //   Simpler Server implementation
  //
  // = DESCRIPTION
  //   Implements the Complex_Server interface in work.idl
  //
public:
  Complex_Server_i (CORBA::ORB_ptr orb, 
                   RTScheduling::Current_ptr current,
                   Task_Stats&,
                   int);
  // ctor

  // = The Complex_Server methods.
  void test_method2 (CORBA::Long x ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));

  void shutdown (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  CORBA::ORB_var orb_;
  RTScheduling::Current_var current_;
  Task_Stats& task_stats_;
  int enable_yield_;
  // The ORB
};

#if defined(__ACE_INLINE__)
#include "work_i.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_MT_SERVER_TEST_I_H */
