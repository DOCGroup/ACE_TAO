/* -*- C++ -*- */
// $Id$
// ==========================================================================
//
// = LIBRARY
//   TAO/orbsvcs/tests/Notify/Discarding
//
// = FILENAME
//   Notify_Sequence_Push_Consumer.h
//
// = DESCRIPTION
//   A sequence push consumer implementation.
//
// = AUTHOR
//    Chad Elliott <elliott_c@ociweb.com>
//
// ==========================================================================
#ifndef TAO_NOTIFY_SEQUENCE_PUSH_CONSUMER_H
#define TAO_NOTIFY_SEQUENCE_PUSH_CONSUMER_H

#include "Notify_SequencePushConsumer.h"

class Notify_Test_Client;

class Notify_Sequence_Push_Consumer : public TAO_Notify_Tests_SequencePushConsumer
{
public:
  Notify_Sequence_Push_Consumer (const char* name,
                                 unsigned int expected,
                                 Notify_Test_Client& client);

  ~Notify_Sequence_Push_Consumer ();

  void _connect (CosNotifyChannelAdmin::ConsumerAdmin_ptr consumer_admin);

private:
  void push_structured_events(const CosNotification::EventBatch&);

  ACE_CString name_;
  unsigned int expected_;
  unsigned int count_;
  Notify_Test_Client& client_;
  ACE_Time_Value first_;
};

#endif /* TAO_NOTIFY_SEQUENCE_PUSH_CONSUMER_H */
