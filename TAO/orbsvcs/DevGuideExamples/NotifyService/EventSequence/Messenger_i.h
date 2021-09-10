#ifndef MESSENGER_H_
#define MESSENGER_H_

#include "MessengerS.h"

#include "orbsvcs/CosNotifyChannelAdminC.h"
#include <memory>

class EventSequenceSupplier_i;

class  Messenger_i : public POA_Messenger
{
public:
  Messenger_i (CORBA::ORB_ptr orb);

  virtual ~Messenger_i ();

  CORBA::Boolean send_message (
      const char * user_name,
      const char * subject,
      char *& message);

private:
  CORBA::ORB_var orb_;
  CosNotifyChannelAdmin::SequenceProxyPushConsumer_var consumer_proxy_;
  std::unique_ptr<EventSequenceSupplier_i> supplier_;
};

#endif





