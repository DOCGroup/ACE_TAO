/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   Connect.h
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 */
//=============================================================================


#ifndef EC_CONNECT_H
#define EC_CONNECT_H

#include "Consumer.h"
#include "Supplier.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class EC_Connect
 *
 * @brief Test the EC connection feature
 *
 * The EC can be configured to allow re-connection of suppliers
 * and consumers, this test verifies that:
 * + The EC does *not* allow connections if the feature is
 * disabled (the default)
 * + The EC does allow connections if the feature is enabled
 * and:
 * - There are no memory leaks
 * - Compares the time required for a connection vs a complete
 * connect/disconnect cycle, specially as the number of
 * suppliers and consumers increases.
 */
class EC_Connect : public EC_Driver
{
public:
  /// Constructor
  EC_Connect ();

  // = The EC_Driver methods
  virtual int parse_args (int& argc, ACE_TCHAR* argv[]);
  virtual void print_usage ();
  virtual void print_args () const;

  /// Don't run the suppliers, just test connect and disconnect calls.
  void execute_test ();

  virtual void dump_results ();

  virtual void connect_consumer (
    RtecEventChannelAdmin::ConsumerAdmin_ptr consumer_admin,
    int i);
  virtual void connect_supplier (
    RtecEventChannelAdmin::SupplierAdmin_ptr supplier_admin,
    int i);
  virtual void disconnect_consumers ();
  virtual void disconnect_suppliers ();

  virtual EC_Consumer* allocate_consumer (int i);
  virtual EC_Supplier* allocate_supplier (int i);

  virtual void connect_clients ();
  virtual void disconnect_clients ();

private:
  ACE_hrtime_t start_time_;
  ACE_Throughput_Stats consumer_connect_;
  ACE_Throughput_Stats supplier_connect_;
  ACE_Throughput_Stats consumer_disconnect_;
  ACE_Throughput_Stats supplier_disconnect_;

  /// What is connected first?
  int order_;
};

// ****************************************************************

class ECC_Consumer : public EC_Consumer
{
public:
  ECC_Consumer (EC_Driver *driver, void* cookie);

  virtual void connect (
      RtecEventChannelAdmin::ConsumerAdmin_ptr consumer_admin,
      const RtecEventChannelAdmin::ConsumerQOS& qos,
      int shutdown_event_type);
  virtual void connect (
      const RtecEventChannelAdmin::ConsumerQOS& qos,
      int shutdown_event_type);
  virtual void dump_results (const ACE_TCHAR* name,
                             ACE_High_Res_Timer::global_scale_factor_type global_scale_factor);

private:
  ACE_Throughput_Stats connect_time_;
};

// ****************************************************************

class ECC_Supplier : public EC_Supplier
{
public:
  ECC_Supplier (EC_Driver *driver, void* cookie);

  virtual void connect (
        RtecEventChannelAdmin::SupplierAdmin_ptr supplier_admin,
        const RtecEventChannelAdmin::SupplierQOS& qos,
        int shutdown_event_type);
  virtual void connect (
      const RtecEventChannelAdmin::SupplierQOS& qos,
      int shutdown_event_type);
  virtual void dump_results (const ACE_TCHAR* name,
                             ACE_High_Res_Timer::global_scale_factor_type global_scale_factor);

private:
  ACE_Throughput_Stats connect_time_;
};

#endif /* EC_CONNECT_H */
