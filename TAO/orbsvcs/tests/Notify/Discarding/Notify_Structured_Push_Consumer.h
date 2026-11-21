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

class Notify_Test_Client;

class Notify_Structured_Push_Consumer: public TAO_Notify_Tests_StructuredPushConsumer
{
public:
  Notify_Structured_Push_Consumer (Notify_Structured_Push_Consumer const&) = delete;
  Notify_Structured_Push_Consumer (Notify_Structured_Push_Consumer&&) = delete;
  Notify_Structured_Push_Consumer& operator= (Notify_Structured_Push_Consumer const&) = delete;
  Notify_Structured_Push_Consumer& operator= (Notify_Structured_Push_Consumer&&) = delete;

  Notify_Structured_Push_Consumer (
                        char const* name,
                        CORBA::Short policy,
                        CORBA::Long max_events_per_consumer,
                        Notify_Test_Client& client);

  void _connect (CosNotifyChannelAdmin::ConsumerAdmin_ptr consumer_admin);

protected:
  void push_structured_event (CosNotification::StructuredEvent const&) override;

  ACE_CString name_;
  CORBA::Short discard_policy_;
  CORBA::Long max_events_per_consumer_;
  CORBA::Long count_ {};
  CORBA::Long first_ {};
  Notify_Test_Client& client_;
  CORBA::Long sent_ { 40 };
};

#endif /* TAO_NOTIFY_STRUCTURED_PUSH_CONSUMER_H */
