
#ifndef TAO_TUTORIALS_QUOTER_EVENT_SERVICE_STOCK_CONSUMER_H
#define TAO_TUTORIALS_QUOTER_EVENT_SERVICE_STOCK_CONSUMER_H

#include <orbsvcs/CosEventCommS.h>
#include <orbsvcs/CosEventChannelAdminC.h>

class Stock_Consumer : public POA_CosEventComm::PushConsumer {
public:
  Stock_Consumer () = default;

  void connect (CosEventChannelAdmin::EventChannel_ptr event_channel);
  // Connect to the event channel, using <consumer_admin>

  void disconnect ();
  // Disconnect from the event channel.

  void push (CORBA::Any const &data);
  void disconnect_push_consumer ();

  Stock_Consumer (Stock_Consumer const &) = delete;
  Stock_Consumer (Stock_Consumer &&) = delete;
  Stock_Consumer &operator= (Stock_Consumer const &) = delete;
  Stock_Consumer &operator= (Stock_Consumer &&) = delete;

private:
  CosEventChannelAdmin::ProxyPushSupplier_var supplier_proxy_;
  // The supplier proxy.
};

#endif /* TAO_TUTORIALS_QUOTER_EVENT_SERVICE_STOCK_CONSUMER_H */
