/* -*- C++ -*- */
/**
 * @file ECT_Supplier_Driver.h
 * @brief Driver for the remote Event Channel throughput supplier.
 */

#ifndef ECT_SUPPLIER_DRIVER_H
#define ECT_SUPPLIER_DRIVER_H

#include "ace/Task.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/High_Res_Timer.h"
#include "orbsvcs/Channel_Clients_T.h"
#include "ECT_Driver.h"
#include "ECT_Supplier.h"
#include "orbsvcs/Event_Service_Constants.h"
#include <array>
#include <memory>

/// Connects suppliers to a remote event channel and generates events.
class ECT_Supplier_Driver : public ECT_Driver
{
public:
  ECT_Supplier_Driver () = default;

  ECT_Supplier_Driver (ECT_Supplier_Driver const&) = delete;
  ECT_Supplier_Driver (ECT_Supplier_Driver&&) = delete;
  ECT_Supplier_Driver& operator= (ECT_Supplier_Driver const&) = delete;
  ECT_Supplier_Driver& operator= (ECT_Supplier_Driver&&) = delete;

  /// Required base callback; a supplier-only driver has no consumers.
  void shutdown_consumer () override;

  static constexpr int MAX_SUPPLIERS {16};

  /// Connect suppliers, generate events, and report throughput.
  int run (int argc, ACE_TCHAR* argv[]);

private:
  /// Parse test-specific command-line options.
  int parse_args (int argc, ACE_TCHAR* argv[]);

  /// Connect and disconnect all configured suppliers.
  void connect_suppliers (RtecScheduler::Scheduler_ptr scheduler,
                          RtecEventChannelAdmin::EventChannel_ptr local_ec);
  void disconnect_suppliers ();

  /// Start all supplier tasks.
  void activate_suppliers ();

  /// Print per-supplier and aggregate throughput results.
  void dump_results ();

private:
  struct Options
  {
    int n_suppliers {1};
    int burst_count {10};
    int burst_size {100};
    int event_size {128};
    int burst_pause {100};
    int type_start {ACE_ES_EVENT_UNDEFINED};
    int type_count {1};
    ACE_TCHAR const* pid_file_name {};
  } options_;

  /// Own suppliers; disconnect() releases their POA references.
  std::array<std::unique_ptr<Test_Supplier>, MAX_SUPPLIERS> suppliers_ {};
};

#endif /* ECT_SUPPLIER_DRIVER_H */
