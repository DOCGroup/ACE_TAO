/**
 * @file Send_Task_Stopper.h
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */

#ifndef TAO_PERF_RTEC_SEND_TASK_STOPPER_H
#define TAO_PERF_RTEC_SEND_TASK_STOPPER_H

#include "Send_Task.h"
#include "Task_Activator.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class Send_Task_Stopper
 *
 * @brief Implement a helper class to activate and stop Send_Tasks.
 *
 */
class TAO_RTEC_Perf_Export Send_Task_Stopper : public Task_Activator<Send_Task>
{
public:
  /// Constructor
  Send_Task_Stopper (int priority,
                     int scheduling_class,
                     Send_Task *task);

  /// Destructor, stop the task and wait for it
  ~Send_Task_Stopper (void);
};

#if defined(__ACE_INLINE__)
#include "Send_Task_Stopper.inl"
#endif /* __ACE_INLINE__ */

#endif /* TAO_PERF_RTEC_SEND_TASK_STOPPER_H */
