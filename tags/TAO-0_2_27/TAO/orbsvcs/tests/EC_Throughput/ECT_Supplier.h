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

#if !defined (ECT_SUPPLIER_H)
#define ECT_SUPPLIER_H

#include "ace/Task.h"
#include "ace/High_Res_Timer.h"
#include "orbsvcs/Channel_Clients_T.h"

class ECTS_Driver;

class Test_Supplier : public ACE_Task<ACE_SYNCH>
{
  //
  // = TITLE
  //   Simplifies the supplier task startup.
  //
  // = DESCRIPTION
  //   Runs the ECTS_Driver::supplier_task() method in another thread.
  //
public:
  Test_Supplier (ECTS_Driver *driver);

  int svc (void);
  // Run the test, just forwards to the driver

  void connect (const char* name,
                int burst_count,
                int burst_size,
                int event_size,
		int burst_pause,
		int event_a,
		int event_b,
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

  void dump_results (const char* name);
  // Dump the results...

private:
  ECTS_Driver *driver_;
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

  ACE_High_Res_Timer timer_;
  // Measure the elapsed time spent while sending the events.

  int burst_count_;
  int burst_size_;
  int event_size_;
  int burst_pause_;
  int event_a_;
  int event_b_;
  // The test data.
};

class ECTS_Driver
{
  //
  // = TITLE
  //
  // = DESCRIPTION
  //
public:
  ECTS_Driver (void);

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
  Test_Supplier* suppliers_[ECTS_Driver::MAX_SUPPLIERS];
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

#endif /* ECT_SUPPLIER_H */
