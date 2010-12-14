// $Id$

#ifndef MESSENGERI_H_
#define MESSENGERI_H_

#include "orbsvcs/CosNotifyChannelAdminC.h"
#include "orbsvcs/CosNotifyCommC.h"
#include "orbsvcs/CosNamingC.h"

#include "MessengerS.h"

//Class Messenger_i
class  Messenger_i : public virtual POA_Messenger
{
public:
  //Constructor
  Messenger_i (CORBA::ORB_ptr orb);

  //Destructor
  virtual ~Messenger_i (void);

 CORBA::Boolean send_message (
    const char * user_name,
    const char * subject,
    char *& message
  );

private:
   CORBA::ORB_var orb_;
   CosNotifyChannelAdmin::StructuredProxyPushConsumer_var consumer_proxy_;
};

#endif /* MESSENGERI_H_  */
