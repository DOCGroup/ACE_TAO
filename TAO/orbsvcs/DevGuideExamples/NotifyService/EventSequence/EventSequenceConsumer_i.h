#ifndef _EVENTCONSUMER_I_H_
#define _EVENTCONSUMER_I_H_

#include "orbsvcs/CosNotifyChannelAdminS.h"

class EventSequenceConsumer_i : public POA_CosNotifyComm::SequencePushConsumer
{
public:
  EventSequenceConsumer_i(CORBA::ORB_ptr orb);

  EventSequenceConsumer_i (EventSequenceConsumer_i const &) = delete;
  EventSequenceConsumer_i (EventSequenceConsumer_i &&) = delete;
  EventSequenceConsumer_i &operator= (EventSequenceConsumer_i const &) = delete;
  EventSequenceConsumer_i &operator= (EventSequenceConsumer_i &&) = delete;

  void push_structured_events (CosNotification::EventBatch const & events) override;

  void offer_change (CosNotification::EventTypeSeq const & added,
                     CosNotification::EventTypeSeq const & removed) override;

  void disconnect_sequence_push_consumer () override;
private:
  CORBA::ORB_var orb_;
};

#endif
