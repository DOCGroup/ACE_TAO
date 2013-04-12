/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   Updates.h
 *
 *  $Id$
 *
 * Test to check if <offer_change> and <subscription_change> messages are
 * received correctly.
 *
 *
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 */
//=============================================================================


#ifndef NOTIFY_TESTS_UPDATES_H
#define NOTIFY_TESTS_UPDATES_H

#include "Notify_Test_Client.h"
#include "Notify_StructuredPushConsumer.h"
#include "Notify_StructuredPushSupplier.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class Updates;

class Updates_StructuredPushConsumer : public TAO_Notify_Tests_StructuredPushConsumer
{
public:
  /// Contructor.
  Updates_StructuredPushConsumer (Updates *test_client);

  /// Offer change is conveyed here.
  virtual void offer_change (const CosNotification::EventTypeSeq & added,
                             const CosNotification::EventTypeSeq & removed);

protected:
  Updates * test_client_;
};

/***************************************************************************/

class Updates_StructuredPushSupplier : public TAO_Notify_Tests_StructuredPushSupplier
{
public:
  /// Constructor.
  Updates_StructuredPushSupplier (Updates * test_client);

  /// Destructor.
  virtual ~Updates_StructuredPushSupplier ();

  /// Subscription change is conveyed here.
  virtual void subscription_change (
      const CosNotification::EventTypeSeq & added,
      const CosNotification::EventTypeSeq & removed
    );

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

  /// initialization.
  int init (int argc, ACE_TCHAR *argv []);

  /// Run the test.
  void run_test (void);

  /// print_event_types
  void print_event_types (const CosNotification::EventTypeSeq &types);

  /// Invoked by Supplier and Consumer when they receive updates
  void types_changed (const CosNotification::EventTypeSeq & added, const CosNotification::EventTypeSeq & removed);

  /// Wait for expected count of updates.
  void wait_for_updates (int expected_added, int expected_removed);

protected:
  /// Create EC
  void create_EC (void);

  /// Helper to add types.
  void add_type (CosNotification::EventTypeSeq& type_seq, const char* type);

  // test subscription_change
  void test_subscription_change (void);

  // test offer_change
  void test_offer_change (void);

  /// Reset the counts.
  void reset_counts (void);

  /// Lock to serialize internal state.
  TAO_SYNCH_MUTEX lock_;

  /// Count of added updates received
  int added_count_;

  /// Count of removed updates received
  /// Check if these are equal to <update_count_> at the end of the test.
  int removed_count_;

  /// The one channel that we create using the factory.
  CosNotifyChannelAdmin::EventChannel_var ec_;

  /// The consumer admin used by consumers.
  CosNotifyChannelAdmin::ConsumerAdmin_var consumer_admin_;

  /// The supplier admin used by suppliers.
  CosNotifyChannelAdmin::SupplierAdmin_var supplier_admin_;

  /// Consumer
  TAO_Notify_Tests_StructuredPushConsumer* consumer_;

  /// Supplier
  TAO_Notify_Tests_StructuredPushSupplier* supplier_;

private:
  friend class Updates_StructuredPushSupplier;
  friend class Updates_StructuredPushConsumer;
};

/***************************************************************************/

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* NOTIFY_TESTS_UPDATES_H */
