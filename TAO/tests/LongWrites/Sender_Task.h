//
// $Id$
//

#ifndef LONGWRITES_SENDER_TASK_H
#define LONGWRITES_SENDER_TASK_H
#include "ace/pre.h"

#include "tao/corbafwd.h"
#include "ace/Task.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class Sender;

/// Implement a Task to run the experiments using multiple threads.
class Sender_Task : public ACE_Task_Base
{
public:
  /// Constructor
  Sender_Task (Sender *sender);

  /// Run the experiment using the configuration below
  int run_test (int thread_count,
                CORBA::Long event_count,
                CORBA::ULong event_size);

  /// Thread entry point
  int svc (void);

private:
  /// Reference to the test interface
  Sender *sender_;

  /// Total number of events
  CORBA::Long event_count_;

  /// Size of each message
  CORBA::ULong event_size_;
};

#include "ace/post.h"
#endif /* LONGWRITES_SENDER_TASK_H */
