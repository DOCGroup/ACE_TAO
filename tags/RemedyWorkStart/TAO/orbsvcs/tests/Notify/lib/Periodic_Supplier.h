/* -*- C++ -*- */
/**
 *  @file Periodic_Supplier.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 */

#ifndef TAO_Notify_Tests_PERIODIC_SUPPLIER_H
#define TAO_Notify_Tests_PERIODIC_SUPPLIER_H
#include /**/ "ace/pre.h"

#include "notify_test_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Notify_StructuredPushSupplier.h"
#include "Task_Stats.h"
#include "StructuredEvent.h"
#include "ace/Task.h"
#include "ace/Barrier.h"
#include "ace/Arg_Shifter.h"

class TAO_Notify_Tests_Task_Callback;

/**
 * @class TAO_Notify_Tests_Periodic_Supplier
 *
 * @brief A Periodic Supplier.
 *
 */
class TAO_NOTIFY_TEST_Export TAO_Notify_Tests_Periodic_Supplier : public TAO_Notify_Tests_StructuredPushSupplier, public ACE_Task <ACE_SYNCH>
{
public:
  /// Constructor
  TAO_Notify_Tests_Periodic_Supplier (void);

  /// Destructor
  ~TAO_Notify_Tests_Periodic_Supplier ();

  /// set the helper callback
  void task_callback(TAO_Notify_Tests_Task_Callback* client);

  /// Init the state of this object.
  int init_state (ACE_Arg_Shifter& arg_shifter);

  /// Activate this task, synch on the given barrier.
  virtual int activate_task (ACE_Barrier* barrier);

  /// task svc
  virtual int svc (void);

  /// Dump stats.
  void dump_stats (ACE_TCHAR* msg, int dump_samples);

  /// Get the name of the proxy
  const char* proxy_name (void);

protected:

  /// svc method.
  void handle_svc (void);

  /// Send a few events before the actual measurements.
  void send_warmup_events (void);

  /// Send Prologue
  void send_prologue (void);

  /// All tasks synch at this barrier.
  ACE_Barrier* barrier_;

  /// The event to send.
  TAO_Notify_Tests_StructuredEvent event_;

  TAO_Notify_Tests_StructuredEvent zeroth_event;

  /// The priority of this task.
  CORBA::Short priority_;

  /// Period
  long period_;

  /// Counts the total number of deadlines missed.
  long total_deadlines_missed_;

  /// RunTime - The Max. time to run the supplier.
  long run_time_;

  /// Worst case exec. time.
  unsigned long exec_time_;

  /// Phase
  long phase_;

  /// Number of times to exec. Job
  int iter_;

  /// A load factor supplied to each Job.
  CORBA::ULong load_;

  /// Stats house keeping
  Task_Stats stats_;

  /// Client
  TAO_Notify_Tests_Task_Callback* client_;
};

#include /**/ "ace/post.h"
#endif /* TAO_Notify_Tests_PERIODIC_SUPPLIER_H */
