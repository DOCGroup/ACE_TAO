/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   Events.h
 *
 *  $Id$
 *
 * Test to check if events are received by all 3 types of consumers.
 * This is intended to be a simple test without any filters with default subscription (all events).
 *
 *
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 */
//=============================================================================


#ifndef NOTIFY_TESTS_EventS_H
#define NOTIFY_TESTS_EventS_H

#include "Notify_Test_Client.h"
#include "Notify_StructuredPushConsumer.h"
#include "Notify_StructuredPushSupplier.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class Events;

class Event_StructuredPushConsumer : public TAO_Notify_Tests_StructuredPushConsumer
{
public:
  /// Contructor.
  Event_StructuredPushConsumer (Events *test_client);

  // = StructuredPushSupplier methods.
  virtual void push_structured_event (
      const CosNotification::StructuredEvent & notification
    );

protected:
  Events * test_client_;
};

/***************************************************************************/

class Event_StructuredPushSupplier : public TAO_Notify_Tests_StructuredPushSupplier
{
public:
  /// Constructor.
  Event_StructuredPushSupplier (Events * test_client);

  /// Destructor.
  virtual ~Event_StructuredPushSupplier (void);

protected:
  Events* test_client_;
};

/***************************************************************************/

class Events : public Notify_Test_Client
{
public:
  // Initialization and termination code.
  Events (void);
  virtual ~Events (void);

  int parse_args (int argc,
                  ACE_TCHAR *argv[]) ;

  /// Initialization.
  int init (int argc,
            ACE_TCHAR *argv []);

  /// Called when an event is received.
  void on_event_received (void);

  /// Run the test.
  void run_test (void);

  /// End the test.
  void end_test (void);

  /// check if we got the expected results.
  int check_results (void);

protected:
  /// Create EC
  void create_EC (void);

  /// Number of events received so far.
  ACE_Atomic_Op <TAO_SYNCH_MUTEX, int> result_count_;

  /// Use the default admins.
  int use_default_admin_;

  /// Number of events to send
  int event_count_;

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
  friend class Event_StructuredPushSupplier;
  friend class Event_StructuredPushConsumer;
};

/***************************************************************************/

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* NOTIFY_TESTS_EventS_H */
