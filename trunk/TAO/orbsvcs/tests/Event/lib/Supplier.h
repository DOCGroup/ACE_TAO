/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   Supplier.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 */
//=============================================================================


#ifndef EC_SUPPLIER_H
#define EC_SUPPLIER_H

#include "Driver.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/RtecEventCommS.h"
#include "orbsvcs/RtecEventChannelAdminC.h"
#include "ace/Task.h"
#include "ace/OS_NS_time.h"
#include "ace/High_Res_Timer.h"

/**
 * @class EC_Supplier
 *
 * @brief Simple supplier object to implement EC tests.
 *
 * This class is a supplier of events.
 * The class is just a helper to simplify common tasks in EC
 * tests, such as subscribing for a range of events, disconnecting
 * from the EC, informing the driver of shutdown messages, etc.
 * There are several ways to connect and disconnect this class,
 * and it is up to the driver program to use the right one.
 * The driver can request that to this class to send a new event,
 * a new shutdown event or to become "active" and send a number of
 * events at a certain rate.
 */
class EC_Test_Export EC_Supplier : public POA_RtecEventComm::PushSupplier
{
public:
  /**
   * Constructor, specifies the types of events to send.
   * Notice that the user can connect to the EC using other
   * publications, this is useful for filtering tests.
   */
  EC_Supplier (EC_Driver *driver,
               void* supplier_cookie);

  /// The types of the event is chosen by the driver, based on the
  /// cookie and the <event_number>
  void send_event (int event_number);

  /// Send <event> to the EC.
  void send_event (const RtecEventComm::EventSet& event);

  /// Set the event type and source in <event>
  void send_event (int event_number,
                   const RtecEventComm::Event& event);

  /// Send a shutdown event.
  void send_shutdown (void);

  /// Connect using a <supplier_admin> and publications (<qos>)
  /// computed by the user
  virtual void connect (
        RtecEventChannelAdmin::SupplierAdmin_ptr supplier_admin,
        const RtecEventChannelAdmin::SupplierQOS& qos,
        int shutdown_event_type);

  /// Connect using the current consumer_proxy (useful for reconnect test)
  virtual void connect (
        const RtecEventChannelAdmin::SupplierQOS& qos,
        int shutdown_event_type);

  /// Disconnect from the EC, also deactivates the object
  void disconnect (void);

  /// Disconnect from the EC, also deactivates the object
  void shutdown (void);

  /// Dump the results...
  virtual void dump_results (
    const ACE_TCHAR* name,
    ACE_High_Res_Timer::global_scale_factor_type global_scale_factor);

  /// Add our statistics to <stats>
  void accumulate (ACE_Throughput_Stats& stats) const;

  /// Return an event type to push....
  void event_type (int event_number,
                   RtecEventComm::Event& event);

  // = The PushSupplier methods
  virtual void disconnect_push_supplier (void);

private:
  /// Class we forward to.
  EC_Driver *driver_;

  /// Magic cookie provided by the supplier to identify ourselves
  void* cookie_;

  /// Protect the internal state
  TAO_SYNCH_MUTEX lock_;

  /// Count the number of push() calls
  int push_count_;

  /// We talk to the EC (as a supplier) using this proxy.
  RtecEventChannelAdmin::ProxyPushConsumer_var consumer_proxy_;

  /// The time for the first event sent
  ACE_hrtime_t throughput_start_;

  /// Measure the elapsed time spent while sending the events.
  ACE_Throughput_Stats throughput_;

  int burst_count_;
  int burst_size_;
  int payload_size_;
  int burst_pause_;

  /// The test data.
  int shutdown_event_type_;

  /// The publications, used to select the events.
  RtecEventChannelAdmin::SupplierQOS qos_;

  /// Is the supplier active in the POA?
  int is_active_;

  /// Cache the object reference to speed up connect/disconnect calls
  RtecEventComm::PushSupplier_var myself_;
};

// ****************************************************************

/**
 * @class EC_Supplier_Task
 */
class EC_Supplier_Task : public ACE_Task_Base
{
public:
  /// Constructor
  EC_Supplier_Task (EC_Supplier* supplier,
                    EC_Driver* driver,
                    void* cookie,
                    int burst_count,
                    int burst_size,
                    int burst_pause,
                    int payload_size,
                    int shutdown_event_type,
                    ACE_Thread_Manager* thr_mgr = 0);

  /// The svc call
  virtual int svc (void);

private:
  /// The supplier
  EC_Supplier* supplier_;

  /// The driver program
  EC_Driver* driver_;

  /// The magic cookie assigned to the supplier
  void* cookie_;

  /// Number of events "bursts"
  int burst_count_;

  /// The number of events in a "burst", i.e. between two calls to
  /// sleep.
  int burst_size_;

  /// The sleep time (in usecs) between each burst
  int burst_pause_;

  /// The size of the payload in each event.
  int payload_size_;

  /// Define the shutdown event, invoked at the end of the loop.
  int shutdown_event_type_;
};

#endif /* EC_SUPPLIER_H */
