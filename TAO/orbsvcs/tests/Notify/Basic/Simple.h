/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   Simple.h
 *
 * Simple test any supplier to any consumer.
 *
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 */
//=============================================================================


#ifndef NOTIFY_TESTS_SIMPLE_H
#define NOTIFY_TESTS_SIMPLE_H

#include "Notify_Test_Client.h"
#include "Notify_PushConsumer.h"
#include "Notify_PushSupplier.h"

#include <atomic>

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class Simple_Test;

/***************************************************************************/

class Event_AnyPushConsumer : public TAO_Notify_Tests_PushConsumer
{
public:
  /// Constructor.
  explicit Event_AnyPushConsumer (Simple_Test *test_client);

  // = PushSupplier methods
  void push (CORBA::Any const &data) override;

protected:
  Simple_Test * test_client_;
};

/***************************************************************************/

class Event_AnyPushSupplier : public TAO_Notify_Tests_PushSupplier
{
public:
  /// Constructor.
  explicit Event_AnyPushSupplier (Simple_Test *test_client);

  /// Destructor.
  ~Event_AnyPushSupplier () override;

protected:
  Simple_Test* test_client_;
};

/***************************************************************************/

class Simple_Test : public Notify_Test_Client
{
public:
  // Initialization and termination code
  Simple_Test ();
  ~Simple_Test () override;

  int parse_args (int argc,
                  ACE_TCHAR *argv[]) override;

  /// initialization.
  int init (int argc,
            ACE_TCHAR *argv []) override;

  /// Called when an event is received.
  void on_event_received ();

  /// Run the test.
  void run_test ();

  /// End the test.
  void end_test ();

  /// check if we got the expected results.
  int check_results ();

protected:
  /// Create EC
  void create_EC ();

  /// Number of events received so far.
  std::atomic<int> result_count_ {0};

  /// Number of events to send
  int event_count_;

  /// The one channel that we create using the factory.
  CosNotifyChannelAdmin::EventChannel_var ec_;

  /// The consumer admin used by consumers.
  CosNotifyChannelAdmin::ConsumerAdmin_var consumer_admin_;

  /// The supplier admin used by suppliers.
  CosNotifyChannelAdmin::SupplierAdmin_var supplier_admin_;

  /// Consumer.
  TAO_Notify_Tests_PushConsumer* consumer_;

  /// Supplier.
  TAO_Notify_Tests_PushSupplier* supplier_;

private:
  friend class Event_AnyPushSupplier;
  friend class Event_AnyPushConsumer;
};

/***************************************************************************/

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* NOTIFY_TESTS_SIMPLE_H */
