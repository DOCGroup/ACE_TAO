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


class Notify_Structured_Push_Consumer : public TAO_Notify_StructuredPushConsumer
{
public:
  Notify_Structured_Push_Consumer (
                        const char* name,
                        unsigned int expected,
                        CORBA::Boolean& done);
protected:
  void push_structured_event (const CosNotification::StructuredEvent&
                              ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  ACE_CString name_;
  unsigned int expected_;
  unsigned int count_;
  CORBA::Boolean& done_;
};

#endif /* TAO_NOTIFY_STRUCTURED_PUSH_CONSUMER_H */
