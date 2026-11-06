/* -*- C++ -*- */
//
// ============================================================================
//
// = DESCRIPTION
//   This is a helper class for the throughput tests of the Event
//   Channel.
//
// ============================================================================

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

class ECT_Supplier_Driver : public ECT_Driver
{
  //
  // = TITLE
  //
  // = DESCRIPTION
  //
public:
  ECT_Supplier_Driver () = default;
  ~ECT_Supplier_Driver () override = default;

  ECT_Supplier_Driver (ECT_Supplier_Driver const&) = delete;
  ECT_Supplier_Driver (ECT_Supplier_Driver&&) = delete;
  ECT_Supplier_Driver& operator= (ECT_Supplier_Driver const&) = delete;
  ECT_Supplier_Driver& operator= (ECT_Supplier_Driver&&) = delete;

  void shutdown_consumer () override;
  // Not used....

  static constexpr int MAX_SUPPLIERS {16};

  int run (int argc, ACE_TCHAR* argv[]);
  // Execute the test.

private:
  int parse_args (int argc, ACE_TCHAR* argv[]);
  // parse the command line args

  void connect_suppliers (RtecScheduler::Scheduler_ptr scheduler,
                          RtecEventChannelAdmin::EventChannel_ptr local_ec);
  void disconnect_suppliers ();
  // Connect the suppliers.

  void activate_suppliers ();
  // Activate the suppliers, i.e. they start generating events.

  void dump_results ();
  // Dump the results for each supplier.

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

  /// These pointers own suppliers; POA references are released by disconnect().
  std::array<std::unique_ptr<Test_Supplier>, MAX_SUPPLIERS> suppliers_ {};
};

#endif /* ECT_SUPPLIER_DRIVER_H */
