
//=============================================================================
/**
 *  @file   test_i.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan
 */
//=============================================================================


#ifndef TAO_MT_SERVER_TEST_I_H
#define TAO_MT_SERVER_TEST_I_H

#include "testS.h"
#include "tao/RTScheduling/RTScheduler_Manager.h"
#include "Task_Stats.h"

/**
 * @class Simple_Server_i
 *
 * @brief Simpler Server implementation
 *
 * Implements the Simple_Server interface in test.idl
 */
class Simple_Server_i : public POA_Simple_Server
{
public:
  /// ctor
  Simple_Server_i (CORBA::ORB_ptr orb,
                   RTScheduling::Current_ptr current,
                   Task_Stats&,
                   int);

  // = The Simple_Server methods.
  CORBA::Long test_method (CORBA::Long x);

  void shutdown (void);

private:
  /// The ORB
  CORBA::ORB_var orb_;
  RTScheduling::Current_var current_;
  Task_Stats& task_stats_;
  int enable_yield_;
};

#if defined(__ACE_INLINE__)
#include "test_i.inl"
#endif /* __ACE_INLINE__ */

#endif /* TAO_MT_SERVER_TEST_I_H */
