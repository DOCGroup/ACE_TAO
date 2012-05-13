/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = DESCRIPTION
//   This is a helper class for the throughput tests of the Event
//   Channel.
//
// ============================================================================

#ifndef ECT_SUPPLIER_H
#define ECT_SUPPLIER_H

#include "ECT_Driver.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/Channel_Clients_T.h"
#include "orbsvcs/RtecEventChannelAdminC.h"
#include "orbsvcs/RtecSchedulerC.h"
#include "ace/Task.h"

class Test_Supplier : public ACE_Task<ACE_SYNCH>
{
  //
  // = TITLE
  //   Simplifies the supplier task startup.
  //
  // = DESCRIPTION
  //
public:
  Test_Supplier (ECT_Driver *driver);

  int svc (void);
  // Run the test, just forwards to the driver

  void connect (RtecScheduler::Scheduler_ptr scheduler,
                const char* name,
                int burst_count,
                int burst_size,
                int event_size,
                int burst_pause,
                int type_start,
                int type_count,
                RtecEventChannelAdmin::EventChannel_ptr ec);
  // This method connects the supplier to the EC.

  void disconnect (void);
  // Disconnect from the EC.

  virtual void disconnect_push_supplier (void);
  // The methods in the skeleton.

  RtecEventComm::EventSourceID supplier_id (void) const;
  // The supplier ID.

  RtecEventChannelAdmin::ProxyPushConsumer_ptr consumer_proxy (void);
  // We talk to the EC (as a supplier) using this proxy, no duplicates
  // are done here...

  void dump_results (const ACE_TCHAR* name,
                     ACE_Basic_Stats::scale_factor_type global_scale_factor);
  // Dump the results...

  void accumulate (ACE_Throughput_Stats& stats) const;
  // Add our statistics to <stats>

private:
  ECT_Driver *driver_;
  // Class we forward to.

  void *cookie_;
  // The test provide us a cookie so we can give back our identity.

  RtecEventComm::EventSourceID supplier_id_;
  // We generate an id based on the name....

  RtecEventChannelAdmin::ProxyPushConsumer_var consumer_proxy_;
  // We talk to the EC (as a supplier) using this proxy.

  ACE_PushSupplier_Adapter<Test_Supplier> supplier_;
  // We also connect to the EC as a consumer so we can receive the
  // timeout events.

  ACE_Throughput_Stats throughput_;
  // Measure the elapsed time spent while sending the events.

  int burst_count_;
  int burst_size_;
  int event_size_;
  int burst_pause_;
  int type_start_;
  int type_count_;
  // The test data.
};

#endif /* ECT_SUPPLIER_H */
