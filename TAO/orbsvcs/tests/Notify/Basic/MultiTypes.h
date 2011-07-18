/* -*- C++ -*- */
// $Id$
// ==========================================================================
//
// = FILENAME
//   MultiTypes.h
//
// = DESCRIPTION
//   Test connect-disconnect methods of Notify.
//
// = AUTHOR
//    Pradeep Gore <pradeep@cs.wustl.edu>
//
// ==========================================================================

#ifndef NOTIFY_MULTI_TYPES_H
#define NOTIFY_MULTI_TYPES_H

#include "Notify_Test_Client.h"
#include "Notify_StructuredPushConsumer.h"
#include "Notify_StructuredPushSupplier.h"
#include "Notify_PushConsumer.h"
#include "Notify_PushSupplier.h"
#include "Notify_SequencePushConsumer.h"
#include "Notify_SequencePushSupplier.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class MultiTypes;

/***************************************************************************/

class MultiTypes_PushConsumer : public TAO_Notify_Tests_PushConsumer
{
public:
  MultiTypes_PushConsumer (MultiTypes* client);

  void push (
             const CORBA::Any & data
             );

protected:
  MultiTypes* client_;
};

/***************************************************************************/

class MultiTypes_StructuredPushConsumer : public TAO_Notify_Tests_StructuredPushConsumer
{
public:
  MultiTypes_StructuredPushConsumer (MultiTypes* client);

  // = StructuredPushSupplier methods
  virtual void push_structured_event (const CosNotification::StructuredEvent & notification
                                      );

protected:
  MultiTypes* client_;
};

class MultiTypes_SequencePushConsumer : public TAO_Notify_Tests_SequencePushConsumer
{
public:
  MultiTypes_SequencePushConsumer (MultiTypes* client);

  // = SequencePushConsumer methods
  virtual void push_structured_events (
        const CosNotification::EventBatch & notifications
      );

protected:
  MultiTypes* client_;
};


/***************************************************************************/

class MultiTypes : public Notify_Test_Client
{
public:
  // Initialization and termination code.
  MultiTypes (void);
  virtual ~MultiTypes ();

  void on_received_event (MultiTypes_PushConsumer* consumer);
  void on_received_event (MultiTypes_StructuredPushConsumer* consumer);
  void on_received_event (MultiTypes_SequencePushConsumer* consumer);

  int parse_args (int argc,
                  ACE_TCHAR *argv[]);

  int init (int argc,
            ACE_TCHAR *argv []);
  // initialization.

  void run_test (void);
  // Run the test.

  void end_test (void);
  // End the test.

  int check_results (void);
  // Check if we got the expected results.

protected:
  // Wait to receive events.
  void wait_for_all_consumers (int expected_count_per_consumer);

  void create_EC (void);
  // Create EC.

  CosNotifyChannelAdmin::EventChannel_var ec_;
  // The one channel that we create using the factory.

  CosNotifyChannelAdmin::ConsumerAdmin_var consumer_admin_;
  // The consumer admin used by consumers.

  CosNotifyChannelAdmin::SupplierAdmin_var supplier_admin_;
  // The supplier admin used by suppliers.

  TAO_Notify_Tests_PushConsumer* any_consumer_;
  TAO_Notify_Tests_StructuredPushConsumer* structured_consumer_;
  TAO_Notify_Tests_SequencePushConsumer* sequence_consumer_;
  // Consumers.

  TAO_Notify_Tests_PushSupplier* any_supplier_;
  TAO_Notify_Tests_StructuredPushSupplier* structured_supplier_;
  TAO_Notify_Tests_SequencePushSupplier* sequence_supplier_;
  // Suppliers

  // Count of Any events received;
  ACE_Atomic_Op <TAO_SYNCH_MUTEX, int> any_event_count_;

  // Count of Structured events received;
  ACE_Atomic_Op <TAO_SYNCH_MUTEX, int> struct_event_count_;

  // Count of Sequence events received;
  ACE_Atomic_Op <TAO_SYNCH_MUTEX, int> seq_event_count_;

  // disconnect_on_last_event_
  int disconnect_on_last_event_;
};

/***************************************************************************/

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* NOTIFY_MULTI_TYPES_H */
