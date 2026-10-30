
#ifndef BIG_ONEWAYS_SESSION_TASK_H
#define BIG_ONEWAYS_SESSION_TASK_H
#include /**/ "ace/pre.h"

#include "ace/Task.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class Session;

/// Implement a Task to run the experiments using multiple threads.
class Session_Task : public ACE_Task_Base
{
public:
  /// Constructor
  explicit Session_Task (Session *session);
  Session_Task (Session_Task const &) = delete;
  Session_Task &operator= (Session_Task const &) = delete;
  Session_Task (Session_Task &&) = delete;
  Session_Task &operator= (Session_Task &&) = delete;

  /// Thread entry point
  int svc () override;

private:
  /// Reference to the test interface
  Session *session_;
};

#include /**/ "ace/post.h"
#endif /* BIG_ONEWAYS_SESSION_TASK_H */
