//=============================================================================
/**
 *  @file   test_i.h
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
  Stopwatch_imp (CORBA::ORB_ptr orb, Timer_imp *timer);

  Stopwatch_imp (Stopwatch_imp const &) = delete;
  Stopwatch_imp &operator= (Stopwatch_imp const &) = delete;
  Stopwatch_imp (Stopwatch_imp &&) = delete;
  Stopwatch_imp &operator= (Stopwatch_imp &&) = delete;

  // = The Simple_Server methods.
  void start () override;

  void stop () override;

  void shutdown () override;

private:
  /// The ORB
  CORBA::ORB_var orb_ {};

  /// Hold the timer object...
  Timer_imp *timer_ {}; // Non-owning; the servant uses the server's timer.
};

#endif /* SERVER_TEST_I_H */
