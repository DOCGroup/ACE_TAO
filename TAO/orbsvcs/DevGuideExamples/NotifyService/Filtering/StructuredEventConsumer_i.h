#ifndef _EVENTCONSUMER_I_H_
#define _EVENTCONSUMER_I_H_

#include "orbsvcs/CosNotifyChannelAdminS.h"

class StructuredEventConsumer_i :
  public virtual POA_CosNotifyComm::StructuredPushConsumer
{
public:
  StructuredEventConsumer_i(CORBA::ORB_ptr orb);

  StructuredEventConsumer_i (StructuredEventConsumer_i const &) = delete;
  StructuredEventConsumer_i (StructuredEventConsumer_i &&) = delete;
  StructuredEventConsumer_i &operator= (StructuredEventConsumer_i const &) = delete;
  StructuredEventConsumer_i &operator= (StructuredEventConsumer_i &&) = delete;

  void push_structured_event (
      CosNotification::StructuredEvent const &notification) override;

  void offer_change (CosNotification::EventTypeSeq const & added,
                     CosNotification::EventTypeSeq const & removed) override;

  void disconnect_structured_push_consumer () override;

private:
  CORBA::ORB_var orb_;
};

#endif
