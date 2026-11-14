/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   Notify_Sequence_Push_Consumer.h
 *
 * A sequence push consumer implementation.
 *
 *  @author Chad Elliott <elliott_c@ociweb.com>
 */
//=============================================================================

#ifndef TAO_NOTIFY_SEQUENCE_PUSH_CONSUMER_H
#define TAO_NOTIFY_SEQUENCE_PUSH_CONSUMER_H

#include "Notify_SequencePushConsumer.h"

#include <atomic>

class Notify_Test_Client;

class Notify_Sequence_Push_Consumer: public TAO_Notify_Tests_SequencePushConsumer
{
public:
  Notify_Sequence_Push_Consumer (unsigned int low,
                                 unsigned int high,
                                 Notify_Test_Client& client);
  Notify_Sequence_Push_Consumer (Notify_Sequence_Push_Consumer const&) = delete;
  Notify_Sequence_Push_Consumer (Notify_Sequence_Push_Consumer&&) = delete;
  Notify_Sequence_Push_Consumer& operator= (Notify_Sequence_Push_Consumer const&) = delete;
  Notify_Sequence_Push_Consumer& operator= (Notify_Sequence_Push_Consumer&&) = delete;

  void _connect (CosNotifyChannelAdmin::ConsumerAdmin_ptr consumer_admin);

protected:
  void push_structured_events (CosNotification::EventBatch const&) override;

  unsigned int const low_;
  unsigned int const high_;
  std::atomic<unsigned int> count_ {};
  Notify_Test_Client& client_;
};

#endif /* TAO_NOTIFY_SEQUENCE_PUSH_CONSUMER_H */
