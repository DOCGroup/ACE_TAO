/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   Notify_Push_Consumer.h
 *
 * A push consumer implementation.
 *
 *  @author Chad Elliott <elliott_c@ociweb.com>
 */
//=============================================================================

#ifndef TAO_NOTIFY_PUSH_CONSUMER_H
#define TAO_NOTIFY_PUSH_CONSUMER_H
#include "Notify_StructuredPushConsumer.h"

class Notify_Test_Client;

class Notify_Push_Consumer: public TAO_Notify_Tests_StructuredPushConsumer
{
public:
  Notify_Push_Consumer (Notify_Push_Consumer const&) = delete;
  Notify_Push_Consumer (Notify_Push_Consumer&&) = delete;
  Notify_Push_Consumer& operator= (Notify_Push_Consumer const&) = delete;
  Notify_Push_Consumer& operator= (Notify_Push_Consumer&&) = delete;

  explicit Notify_Push_Consumer (Notify_Test_Client& client);
  void _connect (CosNotifyChannelAdmin::ConsumerAdmin_ptr consumer_admin,
    CosNotifyFilter::Filter_ptr filter);

  int messages_received () const;

protected:
  void push_structured_event (CosNotification::StructuredEvent const&) override;

  Notify_Test_Client& client_;
  int messages_received_ {};
};

#endif /* TAO_NOTIFY_PUSH_CONSUMER_H */
