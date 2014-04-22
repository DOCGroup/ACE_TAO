//
// $Id$
//

#ifndef CLIENT_LEAKS_CLIENT_TASK_H
#define CLIENT_LEAKS_CLIENT_TASK_H
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
  Client_Task (Test::Process_Factory_ptr process_Factory,
               int iterations);

  /// Return the total number of successful calls
  int successful_calls (void) const;

  /// Thread entry point
  int svc (void);

private:
  /// Run one iteration of the test
  int one_iteration (void);

  /// Make sure that the thread has a connection available
  void validate_connection (void);

private:
  /// Reference to the test interface
  Test::Process_Factory_var process_factory_;

  /// Total number of processes created by each thread
  int iterations_;

  /// At least some calls must be successful
  int successful_calls_;

  /// Implement the Monitor Object Pattern.
  TAO_SYNCH_MUTEX mutex_;
};

#include /**/ "ace/post.h"
#endif /* CLIENT_LEAKS_CLIENT_TASK_H */
