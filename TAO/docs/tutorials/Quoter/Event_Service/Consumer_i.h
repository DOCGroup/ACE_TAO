//
// $Id$
//

#ifndef TAO_TUTORIALS_QUOTER_EVENT_SERVICE_CONSUMER_I_H
#define TAO_TUTORIALS_QUOTER_EVENT_SERVICE_CONSUMER_I_H

#include <orbsvcs/orbsvcs/CosEventCommS.h>
#include <orbsvcs/orbsvcs/CosEventChanneAdminC.h>

class Stock_Consumer : public POA_CosEventComm::PushConsumer {
public:
  Stock_Consumer ();

  void connect (CosEventChanneAdmin::ConsumerAdmin_ptr consumer_admin);

  void push (const CORBA::Any& data ACE_ENV_ARG_DECL_NOT_USED);
  void disconnect_push_consumer (ACE_ENV_SINGLE_ARG_DECL_NOT_USED);

private:
};

#endif /* TAO_TUTORIALS_QUOTER_EVENT_SERVICE_CONSUMER_I_H */
