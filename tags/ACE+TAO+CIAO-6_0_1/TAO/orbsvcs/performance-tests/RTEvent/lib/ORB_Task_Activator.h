/**
 * @file ORB_Task_Activator.h
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */

#ifndef TAO_PERF_RTEC_ORB_TASK_ACTIVATOR_H
#define TAO_PERF_RTEC_ORB_TASK_ACTIVATOR_H

#include "ORB_Task.h"
#include "Task_Activator.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class ORB_Task_Activator
 *
 * @brief Implement a helper class to activate and stop Send_Tasks.
 *
 */
class TAO_RTEC_Perf_Export ORB_Task_Activator : public Task_Activator<ORB_Task>
{
public:
  /// Constructor
  ORB_Task_Activator (int priority,
                      int scheduling_class,
                      int nthreads,
                      ORB_Task *task);

  /// Destructor, stop the task and wait for it
  ~ORB_Task_Activator (void);
};

#if defined(__ACE_INLINE__)
#include "ORB_Task_Activator.inl"
#endif /* __ACE_INLINE__ */

#endif /* TAO_PERF_RTEC_ORB_TASK_ACTIVATOR_H */
