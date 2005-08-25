//
// $Id$
//

#ifndef CLIENT_TASK_H
#define CLIENT_TASK_H
#include /**/ "ace/pre.h"

#include "RoundTripClientC.h"
#include "ace/Task.h"

/// Implement the Test::Client_Task interface
class Client_Task : public ACE_Task_Base
{
public:
  /// Constructor
  Client_Task (Benchmark::RoundTripClient_ptr roundtrip);

  /// The service method
  virtual int svc (void);

private:

  /// The object reference used for this test
  Benchmark::RoundTripClient_var roundtrip_;
};

#include /**/ "ace/post.h"
#endif /* CLIENT_TASK_H */
