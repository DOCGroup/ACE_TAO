/**
 * @file Send_Task.h
 *
 * $Id$
 *
 */

#ifndef ECCL_SEND_TASK_H
#define ECCL_SEND_TASK_H

#include "Servant_var.h"
#include "Supplier.h"
#include "ace/Task.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class ECCL_Send_Task
 *
 * @brief Implement a simple ACE_Task to send the events
 *
 */
class ECCL_Send_Task : public ACE_Task_Base
{
public:
  /// Constructor
  ECCL_Send_Task ();

  void init (int iterations,
             int period_in_usecs,
             int event_type,
             int event_source,
             ECCL_Supplier *supplier,
             ACE_Barrier *barrier);

  /// Run the experiment
  int svc (void);

private:
  int iterations_;

  int period_in_usecs_;

  int event_type_;

  int event_source_;

  Servant_var<ECCL_Supplier> supplier_;

  ACE_Barrier *barrier_;
};


#endif /* ECCL_SEND_TASK_H */
