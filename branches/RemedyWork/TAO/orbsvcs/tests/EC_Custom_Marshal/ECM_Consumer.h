/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = DESCRIPTION
//   An example on custom marshaling in TAO.
//
// ============================================================================

#ifndef ECM_CONSUMER_H
#define ECM_CONSUMER_H

#include "ace/Task.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/Channel_Clients_T.h"
#include "orbsvcs/RtecEventChannelAdminC.h"
#include "ace/OS_NS_time.h"

class Driver;

class Test_Consumer : public POA_RtecEventComm::PushConsumer
{
  //
  // = TITLE
  //   Receive the events.
  //
  // = DESCRIPTION
public:
  Test_Consumer (Driver* driver, void* cookie);

  void connect (int event_a,
                int event_b,
                RtecEventChannelAdmin::EventChannel_ptr ec);
  // This method connects the consumer to the EC.

  void disconnect (void);
  // Disconnect from the EC.

  virtual void push (const RtecEventComm::EventSet& events);
  virtual void disconnect_push_consumer (void);
  // The skeleton methods.

private:
  Driver* driver_;
  // The main driver for the test.

  void* cookie_;
  // A magic cookie passed by the driver that we pass back in our
  // callbacks.

  RtecEventChannelAdmin::ProxyPushSupplier_var supplier_proxy_;
  // We talk to the EC using this proxy.
};

class Driver
{
  //
  // = TITLE
  //
  // = DESCRIPTION
  //
public:
  Driver (void);

  enum {
    MAX_CONSUMERS = 16
    // Maximum number of consumers.
  };

  int run (int argc, ACE_TCHAR* argv[]);
  // Execute the test.

  void push_consumer (void* consumer_cookie,
                      ACE_hrtime_t arrival,
                      const RtecEventComm::EventSet& events);
  // Callback method for consumers, if any of our consumers has
  // received events it will invoke this method.

private:
  int parse_args (int argc, ACE_TCHAR* argv[]);
  // parse the command line args

  void connect_consumers (RtecEventChannelAdmin::EventChannel_ptr local_ec);
  void disconnect_consumers (void);
  // Connect and disconnect the consumers.

private:
  Test_Consumer* consumers_[Driver::MAX_CONSUMERS];
  // The consumer array.

  int n_consumers_;
  // The number of consumers.

  int event_count_;
  // How many messages we will send in the suppliers

  int event_a_;
  int event_b_;
  // We send two types of events, with different contents.

  const ACE_TCHAR* pid_file_name_;
  // The name of a file where the process stores its pid

  TAO_SYNCH_MUTEX recv_count_mutex_;
  int recv_count_;
  // How many events we have received.
};

#endif /* ECM_CONSUMER_H */
