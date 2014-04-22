/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   Consumer.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 */
//=============================================================================


#ifndef EC_CONSUMER_H
#define EC_CONSUMER_H

#include "Driver.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/RtecEventCommS.h"
#include "orbsvcs/RtecEventChannelAdminC.h"
#include "ace/OS_NS_time.h"
#include "ace/High_Res_Timer.h"

/**
 * @class EC_Consumer
 *
 * @brief Simple consumer object to implement EC tests.
 *
 * This class is a consumer of events.
 * The class is just a helper to simplify common tasks in EC
 * tests, such as subscribing for a range of events, disconnecting
 * from the EC, informing the driver of shutdown messages, etc.
 * There are several ways to connect and disconnect this class,
 * and it is up to the driver program to use the right one.
 */
class EC_Test_Export EC_Consumer : public POA_RtecEventComm::PushConsumer
{
public:
  /// Constructor
  EC_Consumer (EC_Driver* driver, void* cookie);

  /// The driver program can build the QoS attributes and obtain the
  /// ConsumerAdmin, we do the rest.
  virtual void connect (
      RtecEventChannelAdmin::ConsumerAdmin_ptr consumer_admin,
      const RtecEventChannelAdmin::ConsumerQOS& qos,
      int shutdown_event_type);

  /**
   * The driver program can build the QoS attributes and we use
   * whatevet supplier_proxy we already have (useful for reconnection
   * tests).
   */
  virtual void connect (
      const RtecEventChannelAdmin::ConsumerQOS& qos,
      int shutdown_event_type);

  /// returns 0 if it is not connected
  virtual int connected (void) const;

  /// The application can invoke this method to disconnect from the EC
  /// and deactivate this class.
  void disconnect (void);

  /// The application is shutting down, deactivate the consumer.
  void shutdown (void);

  /// Accumulate our statistics to the totals.
  void accumulate (ACE_Throughput_Stats& throughput) const;

  /// Printout the statistics
  virtual void dump_results (
    const ACE_TCHAR* name,
    ACE_High_Res_Timer::global_scale_factor_type global_scale_factor);

  // = The RtecEventComm::PushConsumer methods

  /// The skeleton methods.
  virtual void push (const RtecEventComm::EventSet& events);
  virtual void disconnect_push_consumer (void);

private:
  /// The main driver for the test.
  EC_Driver* driver_;

  /// A magic cookie passed by the driver that we pass back in our
  /// callbacks.
  void* cookie_;

  /// We talk to the EC using this proxy.
  RtecEventChannelAdmin::ProxyPushSupplier_var supplier_proxy_;

  /// Protect internal state
  TAO_SYNCH_MUTEX lock_;

  /// The timestamp for the first message received
  ACE_hrtime_t throughput_start_;

  /// Used for reporting stats
  ACE_Throughput_Stats throughput_;

  /// The number of push() calls
  int push_count_;

  /// The type used to indicate shutdown
  int shutdown_event_type_;

  /// Is the consumer active in the POA?
  int is_active_;

  /// Cache the object reference to speed up connect/disconnect calls.
  RtecEventComm::PushConsumer_var myself_;
};

#endif /* ECT_CONSUMER_H */
