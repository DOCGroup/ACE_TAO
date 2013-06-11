// -*- C++ -*-
// $Id$

#ifndef MESSENGER_H_
#define MESSENGER_H_

#include "orbsvcs/CosNotifyChannelAdminC.h"
#include "MessengerS.h"

class  Messenger_i : public POA_Messenger
{
 public:
  Messenger_i (CORBA::ORB_ptr orb);

  virtual ~Messenger_i (void);

  CORBA::Boolean send_message (const char * user_name,
                               const char * subject,
                               char *& message);

 private:
  CORBA::ORB_var orb_;
  CosNotifyChannelAdmin::StructuredProxyPushConsumer_var consumer_proxy_;
};

#endif





