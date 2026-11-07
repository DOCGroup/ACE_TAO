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
  EC_Connect () = default;
  EC_Connect (EC_Connect const&) = delete;
  EC_Connect (EC_Connect&&) = delete;
  EC_Connect& operator= (EC_Connect const&) = delete;
  EC_Connect& operator= (EC_Connect&&) = delete;

  // = The EC_Driver methods
  int parse_args (int& argc, ACE_TCHAR* argv[]) override;
  void print_usage () override;
  void print_args () const override;

  /// Don't run the suppliers, just test connect and disconnect calls.
  void execute_test () override;

  void dump_results () override;

  void connect_consumer (
    RtecEventChannelAdmin::ConsumerAdmin_ptr consumer_admin,
    int i) override;
  void connect_supplier (
    RtecEventChannelAdmin::SupplierAdmin_ptr supplier_admin,
    int i) override;
  void disconnect_consumers () override;
  void disconnect_suppliers () override;

  EC_Consumer* allocate_consumer (int i) override;
  EC_Supplier* allocate_supplier (int i) override;

  void connect_clients () override;
  void disconnect_clients () override;

private:
  ACE_hrtime_t start_time_ {};
  ACE_Throughput_Stats consumer_connect_;
  ACE_Throughput_Stats supplier_connect_;
  ACE_Throughput_Stats consumer_disconnect_;
  ACE_Throughput_Stats supplier_disconnect_;

  /// What is connected first?
  enum class Order
  {
    consumers,
    suppliers,
    interleaved
  };

  Order order_ {Order::consumers};
};

// ****************************************************************

class ECC_Consumer : public EC_Consumer
{
public:
  ECC_Consumer (EC_Driver *driver, void* cookie);
  ECC_Consumer (ECC_Consumer const&) = delete;
  ECC_Consumer (ECC_Consumer&&) = delete;
  ECC_Consumer& operator= (ECC_Consumer const&) = delete;
  ECC_Consumer& operator= (ECC_Consumer&&) = delete;

  void connect (
      RtecEventChannelAdmin::ConsumerAdmin_ptr consumer_admin,
      const RtecEventChannelAdmin::ConsumerQOS& qos,
      int shutdown_event_type) override;
  void connect (
      const RtecEventChannelAdmin::ConsumerQOS& qos,
      int shutdown_event_type) override;
  void dump_results (ACE_TCHAR const* name,
                     ACE_High_Res_Timer::global_scale_factor_type global_scale_factor) override;

private:
  ACE_Throughput_Stats connect_time_;
};

// ****************************************************************

class ECC_Supplier : public EC_Supplier
{
public:
  ECC_Supplier (EC_Driver *driver, void* cookie);
  ECC_Supplier (ECC_Supplier const&) = delete;
  ECC_Supplier (ECC_Supplier&&) = delete;
  ECC_Supplier& operator= (ECC_Supplier const&) = delete;
  ECC_Supplier& operator= (ECC_Supplier&&) = delete;

  void connect (
      RtecEventChannelAdmin::SupplierAdmin_ptr supplier_admin,
      RtecEventChannelAdmin::SupplierQOS const& qos,
      int shutdown_event_type) override;
  void connect (
      const RtecEventChannelAdmin::SupplierQOS& qos,
      int shutdown_event_type) override;
  void dump_results (ACE_TCHAR const* name,
                     ACE_High_Res_Timer::global_scale_factor_type global_scale_factor) override;

private:
  ACE_Throughput_Stats connect_time_;
};

#endif /* EC_CONNECT_H */
