/**
 * @file Send_Task.h
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */

#ifndef TAO_PERF_RTEC_SEND_TASK_H
#define TAO_PERF_RTEC_SEND_TASK_H

#include "Servant_var.h"
#include "Supplier.h"
#include "ace/Task.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class Send_Task
 *
 * @brief Implement a simple ACE_Task to send the events
 *
 */
class Send_Task : public ACE_Task_Base
{
public:
  /// Constructor
  Send_Task (void);

  void init (int iterations,
             int period_in_usecs,
             int event_type,
             int event_source,
             Supplier *supplier,
             ACE_Barrier *barrier);

  /// Run the experiment
  int svc (void);

  /// Stop the experiment
  void stop (void);

private:
  int iterations_;

  int period_in_usecs_;

  int event_type_;

  int event_source_;

  Servant_var<Supplier> supplier_;

  ACE_Barrier *barrier_;

  TAO_SYNCH_MUTEX mutex_;
  int stop_;
};


#endif /* TAO_PERF_RTEC_SEND_TASK_H */
