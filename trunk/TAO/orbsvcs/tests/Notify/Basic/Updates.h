/* -*- C++ -*- */
// $Id$
// ==========================================================================
//
// = FILENAME
//   Updates.h
//
// = DESCRIPTION
//   Test to check if <offer_change> and <subscription_change> messages are
//   received correctly.
//
// = AUTHOR
//    Pradeep Gore <pradeep@cs.wustl.edu>
//
// ==========================================================================

#ifndef NOTIFY_TESTS_UPDATES_H
#define NOTIFY_TESTS_UPDATES_H

#include "Notify_Test_Client.h"
#include "Notify_StructuredPushConsumer.h"
#include "Notify_StructuredPushSupplier.h"

class Updates;

class Update_StructuredPushConsumer : public TAO_Notify_StructuredPushConsumer
{
public:
  Update_StructuredPushConsumer (Updates *test_client);
  // Contructor.

  virtual void offer_change (
        const CosNotification::EventTypeSeq & added,
        const CosNotification::EventTypeSeq & removed,
        CORBA::Environment &ACE_TRY_ENV
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        CosNotifyComm::InvalidEventType
      ));
  // Offer change is conveyed here.

protected:
  Updates * test_client_;
};

/***************************************************************************/

class Update_StructuredPushSupplier : public TAO_Notify_StructuredPushSupplier
{
public:
  Update_StructuredPushSupplier (Updates * test_client);
  // Constructor.

  virtual ~Update_StructuredPushSupplier ();
  // Destructor.

  virtual void subscription_change (
        const CosNotification::EventTypeSeq & added,
        const CosNotification::EventTypeSeq & removed,
        CORBA::Environment &ACE_TRY_ENV
        )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        CosNotifyComm::InvalidEventType
      ));
  // Subscription change is conveyed here.

protected:
  Updates* test_client_;
};

/***************************************************************************/

class Updates : public Notify_Test_Client
{
public:
  // Initialization and termination code
  Updates (void);
  virtual ~Updates ();

  int parse_args(int argc, char *argv[]) ;

  void init (int argc, char *argv [], CORBA::Environment &ACE_TRY_ENV);
  // initialization.

  void run_test (CORBA::Environment &ACE_TRY_ENV);
  // Run the test.

  void end_test (CORBA::Environment &ACE_TRY_ENV);
  // End the test.

  int check_results (void);
  // check if we got the expected results.

protected:
  void create_EC (CORBA::Environment &ACE_TRY_ENV);
  // Create EC

  ACE_Atomic_Op <TAO_SYNCH_MUTEX, int> result_count_;
  // we are waiting for 4 events to happen.

  const char* domain_name_;
  const char* type_name_;

  int update_count_;
  // Number of updates to test.

  int offers_added_, offers_removed_;
  // Check if these are equal to <update_count_> at the end of the test.

  int subscriptions_added_, subscriptions_removed_;
  // Check if these are equal to <update_count_> at the end of the test.

  CosNotifyChannelAdmin::EventChannel_var ec_;
  // The one channel that we create using the factory.

  CosNotifyChannelAdmin::ConsumerAdmin_var consumer_admin_;
  // The consumer admin used by consumers.

  CosNotifyChannelAdmin::SupplierAdmin_var supplier_admin_;
  // The supplier admin used by suppliers.

  TAO_Notify_StructuredPushConsumer* consumer_;
  // Consumer

  TAO_Notify_StructuredPushSupplier* supplier_;
  // Supplier

private:
  friend class Update_StructuredPushSupplier;
  friend class Update_StructuredPushConsumer;
};

/***************************************************************************/
#endif /* NOTIFY_TESTS_UPDATES_H */
