/* -*- C++ -*- */
/**
 *  @file Periodic_Consumer.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 */

#ifndef TAO_Notify_Tests_PERIODIC_CONSUMER_H
#define TAO_Notify_Tests_PERIODIC_CONSUMER_H
#include /**/ "ace/pre.h"

#include "notify_test_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Task_Stats.h"
#include "Notify_StructuredPushConsumer.h"
#include "ace/Arg_Shifter.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL
class ACE_Barrier;
ACE_END_VERSIONED_NAMESPACE_DECL

class TAO_Notify_Tests_Task_Callback;

/**
 * @class TAO_Notify_Tests_Periodic_Consumer
 *
 * @brief Receives Structured events periodically.
 *
 */
class TAO_NOTIFY_TEST_Export TAO_Notify_Tests_Periodic_Consumer : public TAO_Notify_Tests_StructuredPushConsumer
{
public:
  /// Constructor
  TAO_Notify_Tests_Periodic_Consumer (void);

  /// Destructor
  ~TAO_Notify_Tests_Periodic_Consumer ();

  /// set the helper callback
  void task_callback(TAO_Notify_Tests_Task_Callback* client);

  /// Init the state of this object.
  virtual int init_state (ACE_Arg_Shifter& arg_shifter);

  /// dump stats
  void dump_stats (ACE_TCHAR* msg, int dump_samples);

protected:

  virtual void push_structured_event (
        const CosNotification::StructuredEvent & notification
      );

  void handle_start_event (const CosNotification::PropertySeq& prop_seq);
  void check_priority (const CosNotification::PropertySeq& prop_seq);

  /// = Data members.
  /// Serialize state.
  TAO_SYNCH_MUTEX lock_;

  /// count of events received.
  int count_;

  /// We need to count down these initial events.
  int warmup_countdown_;

  /// max events expected.
  /// Sometimes max count may not be the number of events that the supplier will send.
  /// A filter might not allow some events to through. we can tell the consumer what to expect.
  int max_count_;

  /// The Load
  CORBA::ULong load_;

  /// Stats house keeping
  Task_Stats stats_;

  /// Client
  TAO_Notify_Tests_Task_Callback* client_;

  /// Flag to indicate if we want to check the expected priority.
  int check_priority_;

  /// This Flag is set if we received an event asking us to "Stop".
  int stop_received_;
};

#include /**/ "ace/post.h"
#endif /* TAO_Notify_Tests_PERIODIC_CONSUMER_H */
