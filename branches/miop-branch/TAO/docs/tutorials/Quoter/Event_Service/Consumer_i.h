//
// $Id$
//

#ifndef TAO_TUTORIALS_QUOTER_EVENT_SERVICE_CONSUMER_I_H
#define TAO_TUTORIALS_QUOTER_EVENT_SERVICE_CONSUMER_I_H

#include <orbsvcs/CosEventCommS.h>
#include <orbsvcs/CosEventChanneAdminC.h>

class Stock_Consumer : public POA_CosEventComm::PushConsumer {
public:
  Stock_Consumer ();

  void connect (CosEventChanneAdmin::ConsumerAdmin_ptr consumer_admin);

  void push (const CORBA::Any& data TAO_ENV_ARG_DECL_NOT_USED);
  void disconnect_push_consumer (TAO_ENV_SINGLE_ARG_DECL_NOT_USED);

private:
};

#endif /* TAO_TUTORIALS_QUOTER_EVENT_SERVICE_CONSUMER_I_H */
