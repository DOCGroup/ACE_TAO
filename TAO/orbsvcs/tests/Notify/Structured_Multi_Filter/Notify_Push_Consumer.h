/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   Notify_Push_Consumer.h
 *
 * A structured push consumer implementation.
 *
 *  @author Chip Jones <jones_c@ociweb.com>
 */
//=============================================================================

#ifndef NOTIFY_PUSH_CONSUMER_H
#define NOTIFY_PUSH_CONSUMER_H

#include "Notify_StructuredPushConsumer.h"

#include <atomic>

class Notify_Test_Client;

enum class NS_FilterType
{
  none,
  and_op,
  or_op
};

class Notify_Push_Consumer : public TAO_Notify_Tests_StructuredPushConsumer
{
public:
  Notify_Push_Consumer (int sent, NS_FilterType consumer_filter,
                        NS_FilterType supplier_filter, Notify_Test_Client& client);
  Notify_Push_Consumer (Notify_Push_Consumer const&) = delete;
  Notify_Push_Consumer (Notify_Push_Consumer&&) = delete;
  Notify_Push_Consumer& operator= (Notify_Push_Consumer const&) = delete;
  Notify_Push_Consumer& operator= (Notify_Push_Consumer&&) = delete;

  void _connect (CosNotifyChannelAdmin::ConsumerAdmin_ptr consumer_admin,
                 CosNotifyChannelAdmin::EventChannel_ptr ec);

  void push_structured_event (CosNotification::StructuredEvent const&) override;

private:
  std::atomic<CORBA::Long> received_ {};
  CORBA::Long const expected_;
  Notify_Test_Client& client_;
  NS_FilterType const consumer_filter_;
  NS_FilterType const supplier_filter_;
};

#endif /* NOTIFY_PUSH_SUPPLIER_H */
