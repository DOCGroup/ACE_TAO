/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   Simple_Test.h
 *
 *  $Id$
 *
 * Simple test any supplier to any consumer.
 *
 *
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 */
//=============================================================================


#ifndef NOTIFY_TESTS_SIMPLE_H
#define NOTIFY_TESTS_SIMPLE_H

#include "Notify_Test_Client.h"
#include "Notify_PushConsumer.h"
#include "Notify_PushSupplier.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class Simple_Test;

/***************************************************************************/

class Event_AnyPushConsumer : public TAO_Notify_Tests_PushConsumer
{
public:
  /// Contructor.
  Event_AnyPushConsumer (Simple_Test *test_client);

  // = PushSupplier methods
  virtual void push (
      const CORBA::Any & data
    );

protected:
  Simple_Test * test_client_;
};

/***************************************************************************/

class Event_AnyPushSupplier : public TAO_Notify_Tests_PushSupplier
{
public:
  /// Constructor.
  Event_AnyPushSupplier (Simple_Test * test_client);

  /// Destructor.
  virtual ~Event_AnyPushSupplier (void);

protected:
  Simple_Test* test_client_;
};

/***************************************************************************/

class Simple_Test : public Notify_Test_Client
{
public:
  // Initialization and termination code
  Simple_Test (void);
  virtual ~Simple_Test (void);

  int parse_args (int argc,
                  ACE_TCHAR *argv[]) ;

  /// initialization.
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
