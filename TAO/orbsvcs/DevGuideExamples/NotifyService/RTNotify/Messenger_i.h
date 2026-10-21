#ifndef MESSENGER_H_
#define MESSENGER_H_

#include "orbsvcs/CosNotifyChannelAdminC.h"
#include "MessengerS.h"
#include "tao/RTCORBA/RTCORBA.h"

class  Messenger_i : public POA_Messenger
{
 public:
  Messenger_i (CORBA::ORB_ptr orb);

  Messenger_i (Messenger_i const &) = delete;
  Messenger_i (Messenger_i &&) = delete;
  Messenger_i &operator= (Messenger_i const &) = delete;
  Messenger_i &operator= (Messenger_i &&) = delete;

  ~Messenger_i () override;

  CORBA::Boolean send_message (char const * user_name,
                               char const * subject,
                               char *& message) override;

 private:
  CORBA::ORB_var orb_;
  CosNotifyChannelAdmin::StructuredProxyPushConsumer_var consumer_proxy_;
  RTCORBA::Current_var current_;
};

#endif
