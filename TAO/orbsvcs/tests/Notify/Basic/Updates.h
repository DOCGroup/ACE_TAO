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

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class Updates;

class Updates_StructuredPushConsumer : public TAO_Notify_StructuredPushConsumer
{
public:
  Updates_StructuredPushConsumer (Updates *test_client);
  // Contructor.

  virtual void offer_change (const CosNotification::EventTypeSeq & added,
                             const CosNotification::EventTypeSeq & removed
                             ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   CosNotifyComm::InvalidEventType));
  // Offer change is conveyed here.

protected:
  Updates * test_client_;
};

/***************************************************************************/

class Updates_StructuredPushSupplier : public TAO_Notify_StructuredPushSupplier
{
public:
  Updates_StructuredPushSupplier (Updates * test_client);
  // Constructor.

  virtual ~Updates_StructuredPushSupplier ();
  // Destructor.

  virtual void subscription_change (
      const CosNotification::EventTypeSeq & added,
      const CosNotification::EventTypeSeq & removed
      ACE_ENV_ARG_DECL
    )
    ACE_THROW_SPEC ((CORBA::SystemException,
                     CosNotifyComm::InvalidEventType));
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

  int init (int argc, char *argv [] ACE_ENV_ARG_DECL);
  // initialization.

  void run_test (ACE_ENV_SINGLE_ARG_DECL);
  // Run the test.

  /// print_event_types
  void print_event_types (const CosNotification::EventTypeSeq &types);

  /// Invoked by Supplier and Consumer when they receive updates
  void types_changed (const CosNotification::EventTypeSeq & added, const CosNotification::EventTypeSeq & removed);

  /// Wait for expected count of updates.
  void wait_for_updates (int expected_added, int expected_removed);

protected:
  void create_EC (ACE_ENV_SINGLE_ARG_DECL);
  // Create EC

  /// Helper to add types.
  void add_type (CosNotification::EventTypeSeq& type_seq, const char* type);

  // test subscription_change
  void test_subscription_change (ACE_ENV_SINGLE_ARG_DECL);

  // test offer_change
  void test_offer_change (ACE_ENV_SINGLE_ARG_DECL);

  /// Reset the counts.
  void reset_counts (void);

  /// Lock to serialize internal state.
  TAO_SYNCH_MUTEX lock_;

  /// Count of added updates received
  int added_count_;

  /// Count of removed updates received
  int removed_count_;
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
  friend class Updates_StructuredPushSupplier;
  friend class Updates_StructuredPushConsumer;
};

/***************************************************************************/

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* NOTIFY_TESTS_UPDATES_H */
