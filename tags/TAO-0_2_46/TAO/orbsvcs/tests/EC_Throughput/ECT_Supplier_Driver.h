/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = DESCRIPTION
//   This test to measure how many events per minute can the EC
//   process, it also serves as an example how how to encode complex
//   data types in a octet sequence.
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

class ECT_Supplier_Driver : public ECT_Driver
{
  //
  // = TITLE
  //
  // = DESCRIPTION
  //
public:
  ECT_Supplier_Driver (void);
  virtual ~ECT_Supplier_Driver (void);

  virtual void shutdown_consumer (void* consumer_cookie,
                                  CORBA::Environment&);
  // Not used....

  enum {
    MAX_SUPPLIERS = 16
    // Maximum number of suppliers.
  };

  int run (int argc, char* argv[]);
  // Execute the test.

private:
  int parse_args (int argc, char* argv[]);
  // parse the command line args

  void connect_suppliers (RtecEventChannelAdmin::EventChannel_ptr local_ec,
                          CORBA::Environment &_env);
  void disconnect_suppliers (CORBA::Environment &_env);
  // Connect the suppliers.

  void activate_suppliers (CORBA::Environment &_env);
  // Activate the suppliers, i.e. they start generating events.

  void dump_results (void);
  // Dump the results for each supplier.

private:
  Test_Supplier* suppliers_[ECT_Supplier_Driver::MAX_SUPPLIERS];
  // The suppliers array.

  int n_suppliers_;
  // The number of suppliers.

  int burst_count_;
  // How many bursts we will send from each supplier.

  int burst_size_;
  // The number of events

  int event_size_;
  // The size of the payload on each event.

  int burst_pause_;
  // The time between each event burst, in microseconds.

  int event_a_;
  int event_b_;
  // We send two types of events, with different contents.

  const char* pid_file_name_;
  // The name of a file where the process stores its pid
};

#endif /* ECT_SUPPLIER_DRIVER_H */
