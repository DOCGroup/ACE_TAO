/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = DESCRIPTION
//   This test to measure how many events per minute can the EC
//   process, it also serves as an example how how to encode complex
//   data types in a octet sequence.
//
// ============================================================================

#ifndef ECT_CONSUMER_H
#define ECT_CONSUMER_H

#include "ace/Task.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/High_Res_Timer.h"
#include "orbsvcs/Channel_Clients_T.h"

#include "ECT_Driver.h"

class Test_Consumer : public POA_RtecEventComm::PushConsumer
{
  //
  // = TITLE
  //   Receive the events.
  //
  // = DESCRIPTION
public:
  Test_Consumer (ECT_Driver* driver,
                 void* cookie,
                 int n_suppliers);

  void connect (RtecScheduler::Scheduler_ptr scheduler,
                const char* name,
                int event_a,
                int event_b,
                RtecEventChannelAdmin::EventChannel_ptr ec,
                CORBA::Environment& _env);
  // This method connects the consumer to the EC.

  void disconnect (CORBA::Environment &_env);
  // Disconnect from the EC.

  void dump_results (const char* name);
  // Print out the results

  virtual void push (const RtecEventComm::EventSet& events,
                     CORBA::Environment &_env);
  virtual void disconnect_push_consumer (CORBA::Environment &);
  // The skeleton methods.

private:
  ECT_Driver* driver_;
  // The main driver for the test.

  void* cookie_;
  // A magic cookie passed by the driver that we pass back in our
  // callbacks.

  int n_suppliers_;
  // The number of suppliers that are feeding this consumer, we
  // terminate once we receive a shutdown event from each supplier.

  RtecEventChannelAdmin::ProxyPushSupplier_var supplier_proxy_;
  // We talk to the EC using this proxy.

  ACE_SYNCH_MUTEX lock_;
  int recv_count_;
  ACE_High_Res_Timer timer_;
  // How many events we have received.

  int shutdown_count_;
  // How many shutdown events we have received.
};

#endif /* ECT_CONSUMER_H */
