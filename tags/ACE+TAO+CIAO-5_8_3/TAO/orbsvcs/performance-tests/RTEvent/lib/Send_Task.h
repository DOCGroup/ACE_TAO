/**
 * @file Send_Task.h
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */

#ifndef TAO_PERF_RTEC_SEND_TASK_H
#define TAO_PERF_RTEC_SEND_TASK_H

#include "tao/Utils/Servant_Var.h"
#include "Supplier.h"
#include "ace/Task.h"
#include "ace/Synch_Traits.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

ACE_BEGIN_VERSIONED_NAMESPACE_DECL
class ACE_Barrier;
ACE_END_VERSIONED_NAMESPACE_DECL

/**
 * @class Send_Task
 *
 * @brief Implement a simple ACE_Task to send the events
 *
 */
class TAO_RTEC_Perf_Export Send_Task : public ACE_Task_Base
{
public:
  /// Constructor
  Send_Task (void);

  /// This is for compilers that are a PITA complaining that we are
  /// hiding something.
  virtual int init (int argc, ACE_TCHAR *argv[])
  {
    return ACE_Task_Base::init (argc, argv);
  }

  void init (int iterations,
             int period_in_usecs,
             int startup_sleep,
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

  int startup_sleep_;

  int event_type_;

  int event_source_;

  TAO::Utils::Servant_Var<Supplier> supplier_;

  ACE_Barrier *barrier_;

  TAO_SYNCH_MUTEX mutex_;
  int stop_;
};


#endif /* TAO_PERF_RTEC_SEND_TASK_H */
