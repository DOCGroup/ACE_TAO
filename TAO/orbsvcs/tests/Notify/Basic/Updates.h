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

class Notify_Test_Updates_Client;

class Update_StructuredPushConsumer : public TAO_Notify_StructuredPushConsumer
{
public:
  virtual ~Update_StructuredPushConsumer ();
  // Destructor.

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
};

/***************************************************************************/

class Update_StructuredPushSupplier : public TAO_Notify_StructuredPushSupplier
{
public:
  Update_StructuredPushSupplier (Notify_Test_Updates_Client* test_client);
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
  Notify_Test_Updates_Client* test_client_;
};

/***************************************************************************/

class Notify_Test_Updates_Client : public Notify_Test_Client
{
 public:
  // Initialization and termination code
  Notify_Test_Updates_Client (void);
  virtual ~Notify_Test_Updates_Client ();

  virtual void init_concrete (int argc, char *argv [], CORBA::Environment &ACE_TRY_ENV);
  // initialization.

  virtual void run_test (CORBA::Environment &ACE_TRY_ENV);
  // Run the test.

protected:
  int count_;
  // Number of updates to test.

  CosNotifyChannelAdmin::ConsumerAdmin_var consumer_admin_;
  // The consumer admin used by consumers.

  CosNotifyChannelAdmin::SupplierAdmin_var supplier_admin_;
  // The supplier admin used by suppliers.

  TAO_Notify_StructuredPushConsumer* consumer_;
  // Consumer

  TAO_Notify_StructuredPushSupplier* supplier_;
  // Supplier
};

/***************************************************************************/
#endif /* NOTIFY_TESTS_UPDATES_H */
