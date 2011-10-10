/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   Notify_Structured_Push_Consumer.h
 *
 *  $Id$
 *
 * A structured push consumer implementation.
 *
 *
 *  @author Chad Elliott <elliott_c@ociweb.com>
 */
//=============================================================================

#ifndef TAO_NOTIFY_STRUCTURED_PUSH_CONSUMER_H
#define TAO_NOTIFY_STRUCTURED_PUSH_CONSUMER_H

#include "Notify_StructuredPushConsumer.h"

class Notify_Test_Client;

class Notify_Structured_Push_Consumer : public TAO_Notify_Tests_StructuredPushConsumer
{
public:
  Notify_Structured_Push_Consumer (
                        const char* name,
                        unsigned int expected,
                        Notify_Test_Client& client);
  ~Notify_Structured_Push_Consumer ();

  void _connect (CosNotifyChannelAdmin::ConsumerAdmin_ptr consumer_admin);

private:
  void push_structured_event(const CosNotification::StructuredEvent&);

  ACE_CString name_;
  unsigned int expected_;
  unsigned int count_;
  Notify_Test_Client& client_;
  ACE_Time_Value first_;
};

#endif /* TAO_NOTIFY_STRUCTURED_PUSH_CONSUMER_H */
