//
// $Id$
//

#ifndef CLIENT_TASK_H
#define CLIENT_TASK_H
#include "ace/pre.h"

#include "TestC.h"
#include "ace/Task.h"
#include "ace/Basic_Stats.h"

/// Implement the Test::Client_Task interface
class Client_Task : public ACE_Task_Base
{
public:
  /// Constructor
  Client_Task (Test::Roundtrip_ptr roundtrip,
               int niterations);

  /// Add this thread results to the global numbers and print the
  /// per-thread results.
  void accumulate_and_dump (ACE_Basic_Stats &totals,
                            const char *msg,
                            ACE_UINT32 gsf);

  /// The service method
  virtual int svc (void);

private:
  /// Make sure that the current thread has a connection available.
  void validate_connection (TAO_ENV_SINGLE_ARG_DECL);

private:
  /// The object reference used for this test
  Test::Roundtrip_var roundtrip_;

  /// The number of iterations
  int niterations_;

  /// Keep track of the latency (minimum, average, maximum and jitter)
  ACE_Basic_Stats latency_;
};

#include "ace/post.h"
#endif /* CLIENT_TASK_H */
