#ifndef _EVENTCONSUMER_I_H_
#define _EVENTCONSUMER_I_H_

#include <orbsvcs/CosNotifyCommS.h>

class StructuredEventConsumer_i : 
  public virtual POA_CosNotifyComm::StructuredPushConsumer
{
public:
    StructuredEventConsumer_i(CORBA::ORB_ptr orb);

    virtual void push_structured_event(
      const CosNotification::StructuredEvent &notification
      )
      throw (CORBA::SystemException, CosEventComm::Disconnected);  

   virtual void offer_change (
        const CosNotification::EventTypeSeq & added,
        const CosNotification::EventTypeSeq & removed
      )
      
      throw (CORBA::SystemException, CosNotifyComm::InvalidEventType);     

   virtual void disconnect_structured_push_consumer()
      throw (CORBA::SystemException);           

   int nummsgs();

private:
    CORBA::ORB_var orb_;
    int count_;
};

#endif 
