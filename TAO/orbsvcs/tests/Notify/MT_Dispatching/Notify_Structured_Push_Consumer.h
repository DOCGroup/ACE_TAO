/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   Notify_Structured_Push_Consumer.h
 *
 * A structured push consumer implementation.
 *
 *  @author Chad Elliott <elliott_c@ociweb.com>
 */
//=============================================================================

#ifndef TAO_NOTIFY_STRUCTURED_PUSH_CONSUMER_H
#define TAO_NOTIFY_STRUCTURED_PUSH_CONSUMER_H

#include "Notify_StructuredPushConsumer.h"

#include <atomic>

class Notify_Test_Client;

class Notify_Structured_Push_Consumer : public TAO_Notify_Tests_StructuredPushConsumer
{
public:
  Notify_Structured_Push_Consumer (unsigned int expected,
                                   Notify_Test_Client& client);
  Notify_Structured_Push_Consumer (Notify_Structured_Push_Consumer const&) = delete;
  Notify_Structured_Push_Consumer (Notify_Structured_Push_Consumer&&) = delete;
  Notify_Structured_Push_Consumer& operator= (Notify_Structured_Push_Consumer const&) = delete;
  Notify_Structured_Push_Consumer& operator= (Notify_Structured_Push_Consumer&&) = delete;

protected:
  void push_structured_event (CosNotification::StructuredEvent const&) override;

  unsigned int const expected_;
  std::atomic<unsigned int> count_ {};

  Notify_Test_Client& client_;
};

#endif /* TAO_NOTIFY_STRUCTURED_PUSH_CONSUMER_H */
