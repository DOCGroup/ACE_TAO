
#ifndef TAO_TUTORIALS_QUOTER_RT_EVENT_SERVICE_STOCK_CONSUMER_H
#define TAO_TUTORIALS_QUOTER_RT_EVENT_SERVICE_STOCK_CONSUMER_H

#include <orbsvcs/RtecEventCommS.h>
#include <orbsvcs/RtecEventChannelAdminC.h>

class Stock_Consumer : public POA_RtecEventComm::PushConsumer {
public:
  Stock_Consumer () = default;

  Stock_Consumer (Stock_Consumer const &) = delete;
  Stock_Consumer (Stock_Consumer &&) = delete;
  Stock_Consumer &operator= (Stock_Consumer const &) = delete;
  Stock_Consumer &operator= (Stock_Consumer &&) = delete;

  void connect (RtecEventChannelAdmin::EventChannel_ptr event_channel,
                RtecEventChannelAdmin::ConsumerQOS const &subscriptions);
  // Connect to the event channel, using <consumer_admin> and
  // <subscriptions>

  void disconnect ();
  // Disconnect from the event channel.

  void push (RtecEventComm::EventSet const &data) override;
  void disconnect_push_consumer () override;

private:
  RtecEventChannelAdmin::ProxyPushSupplier_var supplier_proxy_ {};
  // The supplier proxy.
};

#endif /* TAO_TUTORIALS_QUOTER_RT_EVENT_SERVICE_STOCK_CONSUMER_H */
