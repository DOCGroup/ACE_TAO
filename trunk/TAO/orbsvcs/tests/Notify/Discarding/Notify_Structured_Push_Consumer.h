/* -*- C++ -*- */
// $Id$
// ==========================================================================
//
// = LIBRARY
//   TAO/orbsvcs/tests/Notify/Discarding
//
// = FILENAME
//   Notify_Structured_Push_Consumer.h
//
// = DESCRIPTION
//   A structured push consumer implementation.
//
// = AUTHOR
//    Chad Elliott <elliott_c@ociweb.com>
//
// ==========================================================================
#ifndef TAO_NOTIFY_STRUCTURED_PUSH_CONSUMER_H
#define TAO_NOTIFY_STRUCTURED_PUSH_CONSUMER_H

#include "Notify_StructuredPushConsumer.h"


class Notify_Structured_Push_Consumer: public TAO_Notify_StructuredPushConsumer
{
public:
  Notify_Structured_Push_Consumer (
                        const char* name,
                        CORBA::Short policy,
                        CORBA::Long expected,
                        CORBA::Long max_events_per_consumer,
                        CORBA::Boolean& done);

  void connect (CosNotifyChannelAdmin::ConsumerAdmin_ptr consumer_admin
                ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

protected:
  void push_structured_event (const CosNotification::StructuredEvent&
                              ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  ACE_CString name_;
  CORBA::Short discard_policy_;
  CORBA::Long expected_;
  CORBA::Long max_events_per_consumer_;
  CORBA::Long count_;
  CORBA::Boolean& done_;
};

#endif /* TAO_NOTIFY_STRUCTURED_PUSH_CONSUMER_H */
