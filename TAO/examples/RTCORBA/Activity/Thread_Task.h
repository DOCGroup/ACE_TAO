/* -*- C++ -*- */
//=============================================================================
/**
 *  @file Thread_Task.h
 *
 *  A periodic task implementation that uses sleep.
 *
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 */
//=============================================================================
#ifndef THREAD_TASK_H
#define THREAD_TASK_H

#include "Periodic_Task.h"

/**
 * @class Thread_Task
 *
 * @brief A periodic task implementation that uses sleep.
 *
 * simplified Periodic Task -
 * if the <name_> activity, which is fired every <period_> time intervals,
 * exceeds its <exec_time_>, it is "late".
 * if <exec_time_> is greater than the <period_>, the activity(s) for the
 * overlapped period(s) are considered "missed".
 */
class Thread_Task : public Periodic_Task
{
 public:
  /// Constructor
  Thread_Task (void);

  /// Activate thread(s).
  virtual int activate_task (ACE_Barrier* barrier, RTCORBA::PriorityMapping *priority_mapping);

 protected:

  /// task svc
  virtual int svc (void);
};

#endif /* THREAD_TASK_H */
