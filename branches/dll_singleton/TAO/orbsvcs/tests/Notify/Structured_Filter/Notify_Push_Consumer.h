/* -*- C++ -*- */
// $Id$
// ==========================================================================
//
// = LIBRARY
//   TAO/orbsvcs/tests/Notify/Structured_Filter
//
// = FILENAME
//   Notify_Push_Consumer.h
//
// = DESCRIPTION
//   A structured push consumer implementation.
//
// = AUTHOR
//    Chip Jones <jones_c@ociweb.com>
//
// ==========================================================================
#ifndef NOTIFY_PUSH_CONSUMER_H
#define NOTIFY_PUSH_CONSUMER_H

#include "Notify_StructuredPushConsumer.h"


class Notify_Push_Consumer: public TAO_Notify_StructuredPushConsumer
{
public:
  Notify_Push_Consumer (const char* name);

  static CORBA::Short get_count ();

private:
  void push_structured_event (const CosNotification::StructuredEvent&
                              ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  static CORBA::Short event_count;
  ACE_CString name_;
};

#endif /* NOTIFY_PUSH_SUPPLIER_H */
