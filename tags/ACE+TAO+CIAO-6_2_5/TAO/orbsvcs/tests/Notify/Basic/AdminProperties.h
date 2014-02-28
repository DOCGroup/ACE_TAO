/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   AdminProperties.h
 *
 *  $Id$
 *
 * Test for EC Admin QoS properties.
 *
 *
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 */
//=============================================================================


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
                                      );

protected:
  AdminProperties* client_;

  ACE_Atomic_Op <TAO_SYNCH_MUTEX, CORBA::Long> events_received_;
};


/**
 * @class AdminProperties_Task
 *
 * @brief Run a thread to dispatch events.
 *
 * Use the ACE_Task_Base class.
 */
class AdminProperties_Task : public ACE_Task_Base
{

public:
  /// Constructor.
  AdminProperties_Task (void);

  /// Init this object.
  void init (TAO_Notify_Tests_StructuredPushSupplier *supplier, AdminProperties* client);

  virtual int init (int argc, ACE_TCHAR *argv []);

  /// The thread entry point.
  virtual int svc (void);

private:
  /// Supplier
  TAO_Notify_Tests_StructuredPushSupplier *supplier_;

  /// Client Object.
  AdminProperties* client_;
};

/***************************************************************************/

/**
 * @class AdminProperties
 *
 * @brief AdminProperties
 *
 * Test for Notify EC properties -
 * max_queue_length
 * max_consumers
 * max_suppliers
 * reject_new_events
 */
class AdminProperties : public Notify_Test_Client
{

  friend class AdminProperties_StructuredPushConsumer;
  friend class AdminProperties_Task;

public:
  AdminProperties (void);
  ~AdminProperties (void);

  int parse_args (int argc, ACE_TCHAR *argv[]) ;

  // Initialization.

  /// Run the test.
  void run_test (void);

private:
  void create_suppliers (void);
  void create_consumers (void);
  void create_channel(bool reject);

  /// Test MaxSuppliers and MaxConsumers
  void test_max_clients (void);

  /// Test MaxQueueLength properties
  void test_max_queue_length (bool reject);

  // Data Members
  /// The one channel that we create using the factory.
  CosNotifyChannelAdmin::EventChannel_var ec_;

  /// The consumer admin used by consumers.
  CosNotifyChannelAdmin::ConsumerAdmin_var consumer_admin_;

  /// The supplier admin used by suppliers.
  CosNotifyChannelAdmin::SupplierAdmin_var supplier_admin_;

  /// Values for Admin Properties supplied by user.
  CORBA::Long max_queue_length_;
  CORBA::Long max_consumers_;
  CORBA::Long max_suppliers_;
  CORBA::Boolean reject_new_events_;

  /// Number of consumers to connect to check MaxConsumers property.
  CORBA::Long consumers_;

  /// Number of suppliers to connect to check MaxSuppliers property.
  CORBA::Long suppliers_;

  /// Number of events to send to the channel.
  CORBA::Long event_count_;

  /// Count of consumers successfully connect to the EC.
  int suppliers_connected_count_;

  /// Count of consumers successfully connect to the EC.
  int consumers_connected_count_;

  // Number of supplier pushes that resulted in an IMP_LIMIT exception
  int rejections_;
};

#endif /* ADMINPROPERTIES */
