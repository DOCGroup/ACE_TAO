/* -*- C++ -*- */
// $Id$
// ==========================================================================
//
// = FILENAME
//   AdminProperties.h
//
// = DESCRIPTION
//   Test for EC Admin QoS properties.
//
// = AUTHOR
//   Pradeep Gore <pradeep@cs.wustl.edu>
//
// ==========================================================================

#ifndef ADMINPROPERTIES
#define ADMINPROPERTIES

#include "ace/Task.h"
#include "Notify_Test_Client.h"
#include "Notify_StructuredPushConsumer.h"
#include "Notify_StructuredPushSupplier.h"

class AdminProperties;
/***************************************************************************/

class AdminProperties_StructuredPushConsumer : public TAO_Notify_Tests_StructuredPushConsumer
{
  friend class AdminProperties;
public:
  AdminProperties_StructuredPushConsumer (AdminProperties* client);

  // = StructuredPushSupplier methods
  virtual void push_structured_event (const CosNotification::StructuredEvent & notification
                                      ACE_ENV_ARG_DECL
                                      )
    ACE_THROW_SPEC ((
                     CORBA::SystemException,
                     CosEventComm::Disconnected
                     ));

protected:
  AdminProperties* client_;

  ACE_Atomic_Op <TAO_SYNCH_MUTEX, CORBA::Long> events_received_;
};


class AdminProperties_Task : public ACE_Task_Base
{
  // = TITLE
  //   Run a thread to dispatch events.
  //
  // = DESCRIPTION
  //   Use the ACE_Task_Base class.
  //

public:
  AdminProperties_Task (void);
  // Constructor.

  /// Init this object.
  void init (TAO_Notify_Tests_StructuredPushSupplier *supplier, AdminProperties* client);

  virtual int init (int argc, ACE_TCHAR *argv []);

  virtual int svc (void);
  // The thread entry point.

private:
  /// Supplier
  TAO_Notify_Tests_StructuredPushSupplier *supplier_;

  /// Client Object.
  AdminProperties* client_;
};

/***************************************************************************/

class AdminProperties : public Notify_Test_Client
{
  // = TITLE
  //   AdminProperties
  //
  // = DESCRIPTION
  //   Test for Notify EC properties -
  //   max_queue_length
  //   max_consumers
  //   max_suppliers
  //   reject_new_events
  //

  friend class AdminProperties_StructuredPushConsumer;
  friend class AdminProperties_Task;

public:
  AdminProperties (void);
  ~AdminProperties (void);

  int parse_args (int argc,
                  char *argv[]) ;

  int init (int argc,
            char *argv []
            ACE_ENV_ARG_DECL);
  // Initialization.

  void run_test (ACE_ENV_SINGLE_ARG_DECL);
  // Run the test.

protected:
  void create_suppliers (ACE_ENV_SINGLE_ARG_DECL);
  void create_consumers (ACE_ENV_SINGLE_ARG_DECL);

  /// Test MaxSuppliers and MaxConsumers
  void test_max_clients (ACE_ENV_SINGLE_ARG_DECL);

  /// Test MaxQueueLength properties
  void test_max_queue_length (ACE_ENV_SINGLE_ARG_DECL);

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

  /// Consumer Delay.
  ACE_Time_Value consumer_delay_;

  /// Inital delay.
  ACE_Time_Value initial_delay_;

  /// Number of consumers to connect to check MaxConsumers property.
  CORBA::Long consumers_;

  /// Number of suppliers to connect to check MaxSuppliers property.
  CORBA::Long suppliers_;

  CORBA::Long event_count_;
  // Number of events to send to the channel.

  /// Count of consumers successfully connect to the EC.
  int suppliers_connected_count_;

  /// Count of consumers successfully connect to the EC.
  int consumers_connected_count_;

  /// Boolean flag that is set if the IMPL_LIMIT exception is thrown when trying to
  /// exceed the Max_Queue_Length
  CORBA::Boolean was_rejected_;
};

#endif /* ADMINPROPERTIES */
