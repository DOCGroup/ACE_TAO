/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Real-time Event Channel testsuite
//
// = FILENAME
//   Supplier
//
// = AUTHOR
//   Carlos O'Ryan (coryan@cs.wustl.edu)
//
// ============================================================================

#ifndef EC_SUPPLIER_H
#define EC_SUPPLIER_H

#include "Driver.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/RtecEventCommS.h"
#include "orbsvcs/RtecEventChannelAdminC.h"
#include "ace/Task.h"

class EC_Test_Export EC_Supplier : public POA_RtecEventComm::PushSupplier
{
  //
  // = TITLE
  //   Simple supplier object to implement EC tests.
  //
  // = DESCRIPTION
  //   This class is a supplier of events.
  //   The class is just a helper to simplify common tasks in EC
  //   tests, such as subscribing for a range of events, disconnecting
  //   from the EC, informing the driver of shutdown messages, etc.
  //
  //   There are several ways to connect and disconnect this class,
  //   and it is up to the driver program to use the right one.
  //
  //   The driver can request that to this class to send a new event,
  //   a new shutdown event or to become "active" and send a number of
  //   events at a certain rate.
  //
public:
  EC_Supplier (EC_Driver *driver,
               void* supplier_cookie);
  // Constructor, specifies the types of events to send.
  // Notice that the user can connect to the EC using other
  // publications, this is useful for filtering tests.

  void send_event (int event_number
                   ACE_ENV_ARG_DECL);
  // The types of the event is chosen by the driver, based on the
  // cookie and the <event_number>

  void send_event (const RtecEventComm::EventSet& event
                   ACE_ENV_ARG_DECL);
  // Send <event> to the EC.

  void send_event (int event_number,
                   const RtecEventComm::Event& event);
  // Set the event type and source in <event>

  void send_shutdown (ACE_ENV_SINGLE_ARG_DECL);
  // Send a shutdown event.

  virtual void connect (
        RtecEventChannelAdmin::SupplierAdmin_ptr supplier_admin,
        const RtecEventChannelAdmin::SupplierQOS& qos,
        int shutdown_event_type
        ACE_ENV_ARG_DECL);
  // Connect using a <supplier_admin> and publications (<qos>)
  // computed by the user

  virtual void connect (
        const RtecEventChannelAdmin::SupplierQOS& qos,
        int shutdown_event_type
        ACE_ENV_ARG_DECL);
  // Connect using the current consumer_proxy (useful for reconnect test)

  void disconnect (ACE_ENV_SINGLE_ARG_DECL);
  // Disconnect from the EC, also deactivates the object

  void shutdown (ACE_ENV_SINGLE_ARG_DECL);
  // Disconnect from the EC, also deactivates the object

  virtual void dump_results (const char* name,
                             ACE_UINT32 global_scale_factor);
  // Dump the results...

  void accumulate (ACE_Throughput_Stats& stats) const;
  // Add our statistics to <stats>

  void event_type (int event_number,
                   RtecEventComm::Event& event);
  // Return an event type to push....

  // = The PushSupplier methods
  virtual void disconnect_push_supplier (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  EC_Driver *driver_;
  // Class we forward to.

  void* cookie_;
  // Magic cookie provided by the supplier to identify ourselves

  TAO_SYNCH_MUTEX lock_;
  // Protect the internal state

  int push_count_;
  // Count the number of push() calls

  RtecEventChannelAdmin::ProxyPushConsumer_var consumer_proxy_;
  // We talk to the EC (as a supplier) using this proxy.

  ACE_hrtime_t throughput_start_;
  // The time for the first event sent

  ACE_Throughput_Stats throughput_;
  // Measure the elapsed time spent while sending the events.

  int burst_count_;
  int burst_size_;
  int payload_size_;
  int burst_pause_;

  int shutdown_event_type_;
  // The test data.

  RtecEventChannelAdmin::SupplierQOS qos_;
  // The publications, used to select the events.

  int is_active_;
  // Is the supplier active in the POA?

  RtecEventComm::PushSupplier_var myself_;
  // Cache the object reference to speed up connect/disconnect calls
};

// ****************************************************************

class EC_Supplier_Task : public ACE_Task_Base
{
  //
public:
  EC_Supplier_Task (EC_Supplier* supplier,
                    EC_Driver* driver,
                    void* cookie,
                    int burst_count,
                    int burst_size,
                    int burst_pause,
                    int payload_size,
                    int shutdown_event_type,
                    ACE_Thread_Manager* thr_mgr = 0);
  // Constructor

  virtual int svc (void);
  // The svc call

private:
  EC_Supplier* supplier_;
  // The supplier

  EC_Driver* driver_;
  // The driver program

  void* cookie_;
  // The magic cookie assigned to the supplier

  int burst_count_;
  // Number of events "bursts"

  int burst_size_;
  // The number of events in a "burst", i.e. between two calls to
  // sleep.

  int burst_pause_;
  // The sleep time (in usecs) between each burst

  int payload_size_;
  // The size of the payload in each event.

  int shutdown_event_type_;
  // Define the shutdown event, invoked at the end of the loop.
};

#endif /* EC_SUPPLIER_H */
