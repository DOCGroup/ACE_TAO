/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = DESCRIPTION
//   An example on custom marshaling in TAO.
//
// ============================================================================

#ifndef ECM_SUPPLIER_H
#define ECM_SUPPLIER_H

#include "ace/Task.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/Channel_Clients_T.h"

class ECMS_Driver;

class Test_Supplier : public ACE_Task<ACE_SYNCH>
{
  //
  // = TITLE
  //   Simplifies the supplier task startup.
  //
  // = DESCRIPTION
  //   Runs the ECMS_Driver::supplier_task() method in another thread.
  //
public:
  Test_Supplier (ECMS_Driver *driver);

  int svc (void);
  // Run the test, just forwards to the driver

  void connect (const char* name,
                int event_a,
                int event_b,
                int event_period,
                RtecEventChannelAdmin::EventChannel_ptr ec,
                CORBA::Environment& _env);
  // This method connects the supplier to the EC.

  void disconnect (CORBA::Environment &_env);
  // Disconnect from the EC.

  virtual void disconnect_push_supplier (CORBA::Environment &);
  // The methods in the skeleton.

  RtecEventComm::EventSourceID supplier_id (void) const;
  // The supplier ID.

  RtecEventChannelAdmin::ProxyPushConsumer_ptr consumer_proxy (void);
  // We talk to the EC (as a supplier) using this proxy, no duplicates
  // are done here...

private:
  ECMS_Driver *driver_;
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
};

class ECMS_Driver
{
  //
  // = TITLE
  //
  // = DESCRIPTION
  //
public:
  ECMS_Driver (void);

  enum {
    MAX_SUPPLIERS = 16
    // Maximum number of suppliers.
  };

  int run (int argc, char* argv[]);
  // Execute the test.

  int supplier_task (Test_Supplier *supplier,
                     void *supplier_cookie);
  // This method is run by the supplier task.

  void push_supplier (void* supplier_cookie,
                      RtecEventChannelAdmin::ProxyPushConsumer_ptr consumer,
                      const RtecEventComm::EventSet &events,
                      CORBA::Environment &);
  // Callback method for suppliers, we push for them to their
  // consumers and take statistics on the way.
  // It is possible that we ignore the <consumer> parameter when
  // testing the short-circuit case.

private:
  int parse_args (int argc, char* argv[]);
  // parse the command line args

  void connect_suppliers (RtecEventChannelAdmin::EventChannel_ptr local_ec,
                          CORBA::Environment &_env);
  void disconnect_suppliers (CORBA::Environment &_env);
  // Connect the suppliers.

  void activate_suppliers (CORBA::Environment &_env);
  // Activate the suppliers, i.e. they start generating events.

private:
  Test_Supplier* suppliers_[ECMS_Driver::MAX_SUPPLIERS];
  // The suppliers array.

  int n_suppliers_;
  // The number of suppliers.

  int event_count_;
  // How many messages we will send in each suppliers.

  int event_period_;
  // The time between each event generation, in microseconds.

  int event_size_;
  // The event size (number of elements in the payload).

  int event_a_;
  int event_b_;
  // We send two types of events, with different contents.

  const char* pid_file_name_;
  // The name of a file where the process stores its pid
};

#endif /* ECM_SUPPLIER_H */
