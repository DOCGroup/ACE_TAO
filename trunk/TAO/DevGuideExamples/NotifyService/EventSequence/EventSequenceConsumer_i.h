#ifndef _EVENTCONSUMER_I_H_
#define _EVENTCONSUMER_I_H_

#include <orbsvcs/CosNotifyChannelAdminS.h>

class EventSequenceConsumer_i : public POA_CosNotifyComm::SequencePushConsumer
{
public:
  EventSequenceConsumer_i(CORBA::ORB_ptr orb);
  
  virtual void push_structured_events (
    const CosNotification::EventBatch & events 
    )
    throw (CORBA::SystemException, CosEventComm::Disconnected);
  
  virtual void offer_change (
    const CosNotification::EventTypeSeq & added,
    const CosNotification::EventTypeSeq & removed 
    )
    throw (CORBA::SystemException, CosNotifyComm::InvalidEventType);     
  
  virtual void disconnect_sequence_push_consumer ()
    throw(CORBA::SystemException);           
private:
  CORBA::ORB_var orb_;
};

#endif 
