//
// $Id$
//

#ifndef CLIENT_LEAKS_CLIENT_TASK_H
#define CLIENT_LEAKS_CLIENT_TASK_H
#include "ace/pre.h"

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

  /// Thread entry point
  int svc (void);

private:
  /// Run one iteration of the test 
  void one_iteration (CORBA::Environment &ACE_TRY_ENV);

private:
  /// Reference to the test interface
  Test::Process_Factory_var process_factory_;

  /// Total number of processes created by each thread
  int iterations_;
};

#include "ace/post.h"
#endif /* CLIENT_LEAKS_CLIENT_TASK_H */
