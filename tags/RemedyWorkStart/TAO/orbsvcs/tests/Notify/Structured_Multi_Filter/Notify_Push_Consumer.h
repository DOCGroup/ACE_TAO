/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   Notify_Push_Consumer.h
 *
 *  $Id$
 *
 * A structured push consumer implementation.
 *
 *
 *  @author Chip Jones <jones_c@ociweb.com>
 */
//=============================================================================

#ifndef NOTIFY_PUSH_CONSUMER_H
#define NOTIFY_PUSH_CONSUMER_H

#include "Notify_StructuredPushConsumer.h"

class Notify_Test_Client;

enum NS_FilterType {None, AndOp, OrOp};

class Notify_Push_Consumer : public TAO_Notify_Tests_StructuredPushConsumer
{
public:
  Notify_Push_Consumer (const char* name, int sent, NS_FilterType consumerFilter,
    NS_FilterType supplierFilter, Notify_Test_Client& client);

  void _connect (CosNotifyChannelAdmin::ConsumerAdmin_ptr consumer_admin,
                CosNotifyChannelAdmin::EventChannel_ptr ec);

private:
  void push_structured_event (const CosNotification::StructuredEvent&);
private:
  ACE_CString name_;
  CORBA::Long sent_;
  CORBA::Long received_;
  CORBA::Long expected_;
  Notify_Test_Client& client_;
  NS_FilterType consumerFilter_;
  NS_FilterType supplierFilter_;
};

#endif /* NOTIFY_PUSH_SUPPLIER_H */
