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

class Notify_Sequence_Push_Consumer : public TAO_Notify_SequencePushConsumer
{
public:
  Notify_Sequence_Push_Consumer (const char* name,
                                 unsigned int expected,
                                 CORBA::Boolean& done);

  ~Notify_Sequence_Push_Consumer ();

  void connect (CosNotifyChannelAdmin::ConsumerAdmin_ptr consumer_admin
                TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  void push_structured_events(const CosNotification::EventBatch&
                              TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  ACE_CString name_;
  unsigned int expected_;
  unsigned int count_;
  CORBA::Boolean& done_;
};

#endif /* TAO_NOTIFY_SEQUENCE_PUSH_CONSUMER_H */
