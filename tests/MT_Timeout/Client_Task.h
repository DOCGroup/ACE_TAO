//
// $Id$
//

#ifndef MT_TIMEOUT_CLIENT_TASK_H
#define MT_TIMEOUT_CLIENT_TASK_H
#include /**/ "ace/pre.h"

#include "TestC.h"
#include "ace/Task.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/// Implement a Task to run the experiments using multiple threads.
class Client_Task : public ACE_Task_Base
{
public:
  /// Constructor
  /**
   * Each Client_Task runs the test for a certain number of
   * iterations, using pre-configured timeouts and sleep times.
   * The test can be made to timeout on each request by using a
   * timeout value smaller than the sleep time.
   *
   * @param orb A reference to the ORB, used to get the PolicyCurrent
   *            interface
   * @param sleep_service The object reference used for this test.
   * @param iterations Number of times the go_to_sleep() method is
   *                   invoked, in microseconds
   * @param sleep_time The sleep time passed on each test, in
   *                   microseconds
   * @param timeout The timeout set for the thread
   *
   */
  Client_Task (CORBA::ORB_ptr orb,
               Test::Sleep_Service_ptr sleep_service,
               int iterations,
               CORBA::ULong sleep_time,
               CORBA::ULong timeout);

  /// Return the total number of successful calls
  int successful_calls (void) const;

  /// Return the total number of timed out calls
  int timed_out_calls (void) const;

  /// Return the number of calls with a too huge difference in
  /// timeout versus actual response arrival.
  int too_big_difference_calls (void) const;

  /// Thread entry point
  int svc (void);

private:
  /// Run one iteration of the test
  int one_iteration (void);

  /// Make sure that the thread has a connection available
  void validate_connection (void);

private:
  /// A reference to the ORB
  CORBA::ORB_var orb_;

  /// Reference to the test interface
  Test::Sleep_Service_var sleep_service_;

  /// Total number of processes created by each thread
  int iterations_;

  /// Sleep time for each request, in microseconds
  CORBA::ULong sleep_time_;

  /// Timeout period, in microseconds
  CORBA::ULong timeout_;

  /// Count how many calls were successful
  int successful_calls_;

  /// Count how many calls timed out
  int timed_out_calls_;

  int too_big_difference_;

  /// Implement the Monitor Object Pattern.
  TAO_SYNCH_MUTEX mutex_;
};

#include /**/ "ace/post.h"
#endif /* MT_TIMEOUT_CLIENT_TASK_H */
