//
// $Id$
//

#ifndef TAO_TUTORIALS_QUOTER_EVENT_SERVICE_STOCK_CONSUMER_H
#define TAO_TUTORIALS_QUOTER_EVENT_SERVICE_STOCK_CONSUMER_H

#include <orbsvcs/CosEventCommS.h>
#include <orbsvcs/CosEventChannelAdminC.h>

class Stock_Consumer : public POA_CosEventComm::PushConsumer {
public:
  Stock_Consumer ();

  void connect (CosEventChannelAdmin::EventChannel_ptr event_channel);
  // Connect to the event channel, using <consumer_admin>

  void disconnect ();
  // Disconnect from the event channel.

  void push (const CORBA::Any& data ACE_ENV_ARG_DECL_NOT_USED)
    throw (CORBA::SystemException);
  void disconnect_push_consumer (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    throw (CORBA::SystemException);

private:
  CosEventChannelAdmin::ProxyPushSupplier_var supplier_proxy_;
  // The supplier proxy.
};

#endif /* TAO_TUTORIALS_QUOTER_EVENT_SERVICE_STOCK_CONSUMER_H */
