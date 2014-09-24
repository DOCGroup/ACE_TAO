// -*- C++ -*-
// $Id$

#ifndef _EVENTCONSUMER_I_H_
#define _EVENTCONSUMER_I_H_

#include "orbsvcs/CosNotifyCommS.h"

class StructuredEventConsumer_i :
  public virtual POA_CosNotifyComm::StructuredPushConsumer
{
public:
    StructuredEventConsumer_i(CORBA::ORB_ptr orb);

    virtual void push_structured_event(
      const CosNotification::StructuredEvent &notification
                                       );

   virtual void offer_change (
        const CosNotification::EventTypeSeq & added,
        const CosNotification::EventTypeSeq & removed
                              );

   virtual void disconnect_structured_push_consumer();

   int nummsgs();

private:
    CORBA::ORB_var orb_;
    int count_;
};

#endif
