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

#ifndef TAO_MT_SERVER_TEST_I_H
#define TAO_MT_SERVER_TEST_I_H

#include "testS.h"
#include "tao/RTScheduling/RTScheduler_Manager.h"
#include "Task_Stats.h"

class Simple_Server_i : public POA_Simple_Server
{
  // = TITLE
  //   Simpler Server implementation
  //
  // = DESCRIPTION
  //   Implements the Simple_Server interface in test.idl
  //
public:
  Simple_Server_i (CORBA::ORB_ptr orb,
                   RTScheduling::Current_ptr current,
                   Task_Stats&,
                   int);
  // ctor

  // = The Simple_Server methods.
  CORBA::Long test_method (CORBA::Long x);

  void shutdown (void);

private:
  CORBA::ORB_var orb_;
  RTScheduling::Current_var current_;
  Task_Stats& task_stats_;
  int enable_yield_;
  // The ORB
};

#if defined(__ACE_INLINE__)
#include "test_i.inl"
#endif /* __ACE_INLINE__ */

#endif /* TAO_MT_SERVER_TEST_I_H */
