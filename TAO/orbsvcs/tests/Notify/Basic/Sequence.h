/* -*- C++ -*- */
// $Id$
// ==========================================================================
//
// = FILENAME
//   Events.h
//
// = DESCRIPTION
//   Test Sequence support in Notification.
//
// = AUTHOR
//    Pradeep Gore <pradeep@cs.wustl.edu>
//
// ==========================================================================

#ifndef NOTIFY_TESTS_SEQUENCE_H
#define NOTIFY_TESTS_SEQUENCE_H

#include "Notify_Test_Client.h"
#include "Notify_SequencePushConsumer.h"
#include "Notify_SequencePushSupplier.h"
#include "orbsvcs/TimeBaseC.h"

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class Sequence;

class SequencePushConsumer : public TAO_Notify_Tests_SequencePushConsumer
{
public:
  SequencePushConsumer (Sequence* client);

  // = SequencePushConsumer methods
  virtual void push_structured_events (
        const CosNotification::EventBatch & notifications
        ACE_ENV_ARG_DECL
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        CosEventComm::Disconnected
       ));

protected:
  Sequence* test_client_;
};

/***************************************************************************/

class SequencePushSupplier : public TAO_Notify_Tests_SequencePushSupplier
{
public:
  SequencePushSupplier (Sequence * test_client);
  // Constructor.

  virtual ~SequencePushSupplier (void);
  // Destructor.

protected:
  Sequence* test_client_;
};

/***************************************************************************/

class Sequence : public Notify_Test_Client
{
public:
  // Initialization and termination code.
  Sequence (void);
  virtual ~Sequence (void);

  int parse_args (int argc,
                  char *argv[]) ;

  int init (int argc,
            char *argv []
            ACE_ENV_ARG_DECL);
  // Initialization.

  void on_event_received (void);
  // Called when an event is received.

  void run_test (ACE_ENV_SINGLE_ARG_DECL);
  // Run the test.

  void end_test (ACE_ENV_SINGLE_ARG_DECL);
  // End the test.

  int check_results (void);
  // check if we got the expected results.

protected:
  void create_EC (ACE_ENV_SINGLE_ARG_DECL);
  // Create EC

  int event_count_;
  // Number of events to send

  /// Batch size send by supplier.
  CORBA::ULong supplier_batch_size_;

  /// Max Batch size expected by consumer.
  CORBA::ULong consumer_batch_size_;

  TimeBase::TimeT pacing_;

  CORBA::Short order_policy_;

  /// Count the number of events received by the consumer.
  ACE_Atomic_Op <TAO_SYNCH_MUTEX, int> events_received_;

  ACE_Time_Value consumer_delay_;

  CosNotifyChannelAdmin::EventChannel_var ec_;
  // The one channel that we create using the factory.

  CosNotifyChannelAdmin::ConsumerAdmin_var consumer_admin_;
  // The consumer admin used by consumers.

  CosNotifyChannelAdmin::SupplierAdmin_var supplier_admin_;
  // The supplier admin used by suppliers.

  TAO_Notify_Tests_SequencePushConsumer* consumer_;
  // Consumer

  TAO_Notify_Tests_SequencePushSupplier* supplier_;
  // Supplier

private:
  friend class SequencePushSupplier;
  friend class SequencePushConsumer;
};

/***************************************************************************/

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* NOTIFY_TESTS_SEQUENCE_H */
