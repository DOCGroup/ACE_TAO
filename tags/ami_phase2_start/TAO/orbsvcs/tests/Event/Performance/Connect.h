/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Real-time Event Channel testsuite
//
// = FILENAME
//   Connect
//
// = AUTHOR
//   Carlos O'Ryan (coryan@cs.wustl.edu)
//
// ============================================================================

#ifndef EC_CONNECT_H
#define EC_CONNECT_H

#include "Consumer.h"
#include "Supplier.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class EC_Connect : public EC_Driver
{
  //
  // = TITLE
  //   Test the EC connection feature
  //
  // = DESCRIPTION
  //   The EC can be configured to allow re-connection of suppliers
  //   and consumers, this test verifies that:
  //   + The EC does *not* allow connections if the feature is
  //     disabled (the default)
  //   + The EC does allow connections if the feature is enabled
  //     and:
  //     - There are no memory leaks
  //     - Compares the time required for a connection vs a complete
  //       connect/disconnect cycle, specially as the number of
  //       suppliers and consumers increases.
  //
public:
  EC_Connect (void);
  // Constructor

  // = The EC_Driver methods
  virtual int parse_args (int& argc, char* argv[]);
  virtual void print_usage (void);
  virtual void print_args (void) const;

  void execute_test (CORBA::Environment& ACE_TRY_ENV);
  // Don't run the suppliers, just test connect and disconnect calls.

  virtual void dump_results (void);

  virtual void connect_consumer (
    RtecEventChannelAdmin::ConsumerAdmin_ptr consumer_admin,
    int i,
    CORBA::Environment &ACE_TRY_ENV);
  virtual void connect_supplier (
    RtecEventChannelAdmin::SupplierAdmin_ptr supplier_admin,
    int i,
    CORBA::Environment &ACE_TRY_ENV);
  virtual void disconnect_consumers (CORBA::Environment& env);
  virtual void disconnect_suppliers (CORBA::Environment& env);

  virtual EC_Consumer* allocate_consumer (int i);
  virtual EC_Supplier* allocate_supplier (int i);

  virtual void connect_clients (CORBA::Environment&);
  virtual void disconnect_clients (CORBA::Environment&);

private:
  EC_Driver::Latency_Stats consumer_connect_;
  EC_Driver::Latency_Stats supplier_connect_;
  EC_Driver::Latency_Stats consumer_disconnect_;
  EC_Driver::Latency_Stats supplier_disconnect_;

  int order_;
  // What is connected first?
};

// ****************************************************************

class ECC_Consumer : public EC_Consumer
{
public:
  ECC_Consumer (EC_Driver *driver, void* cookie);

  virtual void connect (
      RtecEventChannelAdmin::ConsumerAdmin_ptr consumer_admin,
      const RtecEventChannelAdmin::ConsumerQOS& qos,
      int shutdown_event_type,
      CORBA::Environment &ACE_TRY_ENV);
  virtual void connect (
      const RtecEventChannelAdmin::ConsumerQOS& qos,
      int shutdown_event_type,
      CORBA::Environment &ACE_TRY_ENV);
  virtual void dump_results (const char* name);

private:
  EC_Driver::Latency_Stats connect_time_;
};

// ****************************************************************

class ECC_Supplier : public EC_Supplier
{
public:
  ECC_Supplier (EC_Driver *driver, void* cookie);

  virtual void connect (
        RtecEventChannelAdmin::SupplierAdmin_ptr supplier_admin,
        const RtecEventChannelAdmin::SupplierQOS& qos,
        int shutdown_event_type,
        CORBA::Environment& ACE_TRY_ENV);
  virtual void connect (
      const RtecEventChannelAdmin::SupplierQOS& qos,
      int shutdown_event_type,
      CORBA::Environment &ACE_TRY_ENV);
  virtual void dump_results (const char* name);

private:
  EC_Driver::Latency_Stats connect_time_;
};

#endif /* EC_CONNECT_H */
