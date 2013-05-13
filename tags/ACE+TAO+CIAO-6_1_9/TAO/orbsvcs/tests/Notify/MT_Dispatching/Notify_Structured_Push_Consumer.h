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
  void set_delay_parameters (unsigned int delay_count, unsigned long delay_period);

protected:
  void push_structured_event (const CosNotification::StructuredEvent&);

  ACE_CString name_;
  unsigned int expected_;
  unsigned int count_;
  unsigned int delay_count_;
  unsigned long delay_period_;

  Notify_Test_Client& client_;
};

#endif /* TAO_NOTIFY_STRUCTURED_PUSH_CONSUMER_H */
