/* -*- C++ -*- */
// $Id$
// ==========================================================================
//
// = FILENAME
//   AdminProperties_Test.h
//
// = DESCRIPTION
//   Test for EC Admin QoS properties.
//
// = AUTHOR
//   Pradeep Gore <pradeep@cs.wustl.edu>
//
// ==========================================================================

#ifndef ADMINPROPERTIES_TEST
#define ADMINPROPERTIES_TEST

#include "Notify_Test_Client.h"

class AdminProperties_Test : public Notify_Test_Client
{
  // = TITLE
  //   AdminProperties_Test
  //
  // = DESCRIPTION
  //   Test for Notify EC properties -
  //   max_queue_length
  //   max_consumers
  //   max_suppliers
  //   reject_new_events
  //
public:
  AdminProperties_Test (void);
  ~AdminProperties_Test (void);

  int parse_args(int argc, char *argv[]) ;

  void init (int argc, char *argv [], CORBA::Environment &ACE_TRY_ENV);
  // Initialization.

  void run_test (CORBA::Environment &ACE_TRY_ENV);
  // Run the test.

protected:
  void create_suppliers (CORBA::Environment &ACE_TRY_ENV);
  void create_consumers (CORBA::Environment &ACE_TRY_ENV);
  void send_events (CORBA::Environment &ACE_TRY_ENV);

  // Data Members
  CosNotifyChannelAdmin::EventChannel_var ec_;
  // The one channel that we create using the factory.

  CosNotifyChannelAdmin::ConsumerAdmin_var consumer_admin_;
  // The consumer admin used by consumers.

  CosNotifyChannelAdmin::SupplierAdmin_var supplier_admin_;
  // The supplier admin used by suppliers.

  CORBA::Long max_queue_length_;
  CORBA::Long max_consumers_;
  CORBA::Long max_suppliers_;
  CORBA::Boolean reject_new_events_;
  // Values for Admin Properties supplied by user.

  CORBA::Long consumers_;
  CORBA::Long suppliers_;
  // Number of consumers and suppliers to connect.

  CORBA::Long event_count_;
  // Number of events to send to the channel.
};

#endif /* ADMINPROPERTIES_TEST */
