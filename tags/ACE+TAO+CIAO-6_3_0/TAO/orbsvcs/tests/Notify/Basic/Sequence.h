/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   Sequence.h
 *
 *  $Id$
 *
 * Test Sequence support in Notification.
 *
 *
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 */
//=============================================================================


#ifndef NOTIFY_TESTS_SEQUENCE_H
#define NOTIFY_TESTS_SEQUENCE_H

#include "Notify_Test_Client.h"
#include "Notify_SequencePushConsumer.h"
#include "Notify_SequencePushSupplier.h"
#include "orbsvcs/TimeBaseC.h"

#if defined(_MSC_VER)
#pragma warning(push)
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
      );

protected:
  Sequence* test_client_;
};

/***************************************************************************/

class SequencePushSupplier : public TAO_Notify_Tests_SequencePushSupplier
{
public:
  /// Constructor.
  SequencePushSupplier (Sequence * test_client);

  /// Destructor.
  virtual ~SequencePushSupplier (void);

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

  /// Number of events to send
  int event_count_;

  /// Batch size send by supplier.
  CORBA::ULong supplier_batch_size_;

  /// Max Batch size expected by consumer.
  CORBA::ULong consumer_batch_size_;

  TimeBase::TimeT pacing_;

  CORBA::Short order_policy_;

  /// Count the number of events received by the consumer.
  ACE_Atomic_Op <TAO_SYNCH_MUTEX, int> events_received_;

  ACE_Time_Value consumer_delay_;

  /// The one channel that we create using the factory.
  CosNotifyChannelAdmin::EventChannel_var ec_;

  /// The consumer admin used by consumers.
  CosNotifyChannelAdmin::ConsumerAdmin_var consumer_admin_;

  /// The supplier admin used by suppliers.
  CosNotifyChannelAdmin::SupplierAdmin_var supplier_admin_;

  /// Consumer
  TAO_Notify_Tests_SequencePushConsumer* consumer_;

  /// Supplier
  TAO_Notify_Tests_SequencePushSupplier* supplier_;

private:
  friend class SequencePushSupplier;
  friend class SequencePushConsumer;
};

/***************************************************************************/

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* NOTIFY_TESTS_SEQUENCE_H */
