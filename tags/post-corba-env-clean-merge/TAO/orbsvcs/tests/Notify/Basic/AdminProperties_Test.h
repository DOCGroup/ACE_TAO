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

  void init (int argc, char *argv [] TAO_ENV_ARG_DECL);
  // Initialization.

  void run_test (TAO_ENV_SINGLE_ARG_DECL);
  // Run the test.

protected:
  void create_suppliers (TAO_ENV_SINGLE_ARG_DECL);
  void create_consumers (TAO_ENV_SINGLE_ARG_DECL);
  void send_events (TAO_ENV_SINGLE_ARG_DECL);

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
