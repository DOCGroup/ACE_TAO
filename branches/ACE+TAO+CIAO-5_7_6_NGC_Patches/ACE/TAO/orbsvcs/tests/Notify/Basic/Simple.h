/* -*- C++ -*- */
// $Id$
// ==========================================================================
//
// = FILENAME
//   Simple_Test.h
//
// = DESCRIPTION
//   Simple test any supplier to any consumer.
//
// = AUTHOR
//    Pradeep Gore <pradeep@cs.wustl.edu>
//
// ==========================================================================

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
  Event_AnyPushConsumer (Simple_Test *test_client);
  // Contructor.

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
  Event_AnyPushSupplier (Simple_Test * test_client);
  // Constructor.

  virtual ~Event_AnyPushSupplier (void);
  // Destructor.

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

  int init (int argc,
            ACE_TCHAR *argv []);
  // initialization.

  void on_event_received (void);
  // Called when an event is received.

  void run_test (void);
  // Run the test.

  void end_test (void);
  // End the test.

  int check_results (void);
  // check if we got the expected results.

protected:
  void create_EC (void);
  // Create EC

  ACE_Atomic_Op <TAO_SYNCH_MUTEX, int> result_count_;
  // Number of events received so far.

  int event_count_;
  // Number of events to send

  CosNotifyChannelAdmin::EventChannel_var ec_;
  // The one channel that we create using the factory.

  CosNotifyChannelAdmin::ConsumerAdmin_var consumer_admin_;
  // The consumer admin used by consumers.

  CosNotifyChannelAdmin::SupplierAdmin_var supplier_admin_;
  // The supplier admin used by suppliers.

  TAO_Notify_Tests_PushConsumer* consumer_;
  // Consumer.

  TAO_Notify_Tests_PushSupplier* supplier_;
  // Supplier.

private:
  friend class Event_AnyPushSupplier;
  friend class Event_AnyPushConsumer;
};

/***************************************************************************/

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* NOTIFY_TESTS_SIMPLE_H */
