/* -*- C++ -*- */
// $Id$
// ==========================================================================
//
// = FILENAME
//   Events_Test.h
//
// = DESCRIPTION
//   Test to check if events are received by all 3 types of consumers.
//   This is intended to be a simple test without any filters with default subscription (all events).
//
// = AUTHOR
//    Pradeep Gore <pradeep@cs.wustl.edu>
//
// ==========================================================================

#ifndef NOTIFY_TESTS_EventS_H
#define NOTIFY_TESTS_EventS_H

#include "Notify_Test_Client.h"
#include "Notify_StructuredPushConsumer.h"
#include "Notify_StructuredPushSupplier.h"

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class Events_Test;

class Event_StructuredPushConsumer : public TAO_Notify_StructuredPushConsumer
{
public:
  Event_StructuredPushConsumer (Events_Test *test_client);
  // Contructor.

  // = StructuredPushSupplier methods
  virtual void push_structured_event (
        const CosNotification::StructuredEvent & notification,
        CORBA::Environment &ACE_TRY_ENV
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        CosEventComm::Disconnected
       ));

protected:
  Events_Test * test_client_;
};

/***************************************************************************/

class Event_StructuredPushSupplier : public TAO_Notify_StructuredPushSupplier
{
public:
  Event_StructuredPushSupplier (Events_Test * test_client);
  // Constructor.

  virtual ~Event_StructuredPushSupplier ();
  // Destructor.

protected:
  Events_Test* test_client_;
};

/***************************************************************************/

class Events_Test : public Notify_Test_Client
{
public:
  // Initialization and termination code
  Events_Test (void);
  virtual ~Events_Test ();

  int parse_args(int argc, char *argv[]) ;

  void init (int argc, char *argv [], CORBA::Environment &ACE_TRY_ENV);
  // initialization.

  void on_event_received (void);
  // Called when an event is received.

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
  // Number of events received so far.

  int event_count_;
  // Number of events to send

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
  friend class Event_StructuredPushSupplier;
  friend class Event_StructuredPushConsumer;
};

/***************************************************************************/

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* NOTIFY_TESTS_EventS_H */
