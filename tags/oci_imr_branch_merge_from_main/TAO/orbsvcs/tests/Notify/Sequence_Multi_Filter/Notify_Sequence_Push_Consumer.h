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


class Notify_Sequence_Push_Consumer : public TAO_Notify_Tests_SequencePushConsumer
{
public:
  Notify_Sequence_Push_Consumer (const char* name,
                                 CORBA::Short policy,
                                 unsigned int low,
                                 unsigned int high,
                                 int& done);

  ~Notify_Sequence_Push_Consumer ();

  void _connect (CosNotifyChannelAdmin::ConsumerAdmin_ptr consumer_admin
                ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  void push_structured_events(const CosNotification::EventBatch&
                              ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  ACE_CString name_;
  CORBA::Short discard_policy_;
  unsigned int low_;
  unsigned int high_;
  unsigned int count_;
  int& done_;
};

#endif /* TAO_NOTIFY_SEQUENCE_PUSH_CONSUMER_H */
