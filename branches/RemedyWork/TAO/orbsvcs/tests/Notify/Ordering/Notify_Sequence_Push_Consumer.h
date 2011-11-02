/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   Notify_Sequence_Push_Consumer.h
 *
 *  $Id$
 *
 * A sequence push consumer implementation.
 *
 *
 *  @author Chad Elliott <elliott_c@ociweb.com>
 */
//=============================================================================

#ifndef TAO_NOTIFY_SEQUENCE_PUSH_CONSUMER_H
#define TAO_NOTIFY_SEQUENCE_PUSH_CONSUMER_H

#include "Notify_SequencePushConsumer.h"

class Notify_Test_Client;

class Notify_Sequence_Push_Consumer: public TAO_Notify_Tests_SequencePushConsumer
{
public:
  Notify_Sequence_Push_Consumer (const char* name,
                                 CORBA::Short policy,
                                 bool use_ordering,
                                 int expected_count,
                                 Notify_Test_Client& client);

  void _connect (CosNotifyChannelAdmin::ConsumerAdmin_ptr consumer_admin);

protected:
  void push_structured_events (const CosNotification::EventBatch&);

  ACE_CString name_;
  CORBA::Short order_policy_;
  bool use_ordering_;
  int expected_;
  int count_;
  int previous_first_;
  Notify_Test_Client& client_;
};

#endif /* TAO_NOTIFY_SEQUENCE_PUSH_CONSUMER_H */
