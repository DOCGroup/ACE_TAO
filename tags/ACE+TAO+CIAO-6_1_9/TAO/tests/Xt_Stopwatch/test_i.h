
//=============================================================================
/**
 *  @file   test_i.h
 *
 *  $Id$
 *
 *  @author Bala
 */
//=============================================================================


#ifndef SERVER_TEST_I_H
#define SERVER_TEST_I_H

#include "testS.h"
#include "timer.h"


/**
 * @class Stopwatch_imp
 *
 * @brief Stopwatch implementation class
 *
 * Implements the Stopwatch interface in test.idl
 */
class Stopwatch_imp : public POA_Stopwatch
{
public:
  /// ctor
  Stopwatch_imp (CORBA::ORB_ptr, Timer_imp *);

  // = The Simple_Server methods.
  void start (void);

  void stop (void);

  void shutdown (void);

private:
  /// The ORB
  CORBA::ORB_var orb_;

  /// Hold the timer object...
  Timer_imp *timer_;
};

#endif /* SERVER_TEST_I_H */
