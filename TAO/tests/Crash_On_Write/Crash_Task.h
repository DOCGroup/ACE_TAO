
#ifndef CRASH_TASK_H
#define CRASH_TASK_H
#include /**/ "ace/pre.h"

#include "ace/Task.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/// Implement a Task to run the experiments using multiple threads.
class Crash_Task : public ACE_Task_Base
{
public:
  /// Constructor
  Crash_Task (ACE_Thread_Manager *thr_mgr,
              ACE_Time_Value const &running_time);

  Crash_Task (Crash_Task const &) = delete;
  Crash_Task (Crash_Task &&) = delete;
  Crash_Task & operator= (Crash_Task const &) = delete;
  Crash_Task & operator= (Crash_Task &&) = delete;

  /// Thread entry point
  int svc () override;

private:
  /// Total running time for each thread
  ACE_Time_Value const running_time_;
};

#include /**/ "ace/post.h"
#endif /* CRASH_TASK_H */
