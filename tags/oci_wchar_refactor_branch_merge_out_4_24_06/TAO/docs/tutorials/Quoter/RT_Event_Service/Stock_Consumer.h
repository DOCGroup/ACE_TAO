//
// $Id$
//

#ifndef TAO_TUTORIALS_QUOTER_RT_EVENT_SERVICE_STOCK_CONSUMER_H
#define TAO_TUTORIALS_QUOTER_RT_EVENT_SERVICE_STOCK_CONSUMER_H

#include <orbsvcs/RtecEventCommS.h>
#include <orbsvcs/RtecEventChannelAdminC.h>

class Stock_Consumer : public POA_RtecEventComm::PushConsumer {
public:
  Stock_Consumer ();

  void connect (RtecEventChannelAdmin::EventChannel_ptr event_channel,
                const RtecEventChannelAdmin::ConsumerQOS &subscriptions);
  // Connect to the event channel, using <consumer_admin> and
  // <subscriptions>

  void disconnect ();
  // Disconnect from the event channel.

  void push (const RtecEventComm::EventSet& data ACE_ENV_ARG_DECL_NOT_USED)
    throw (CORBA::SystemException);
  void disconnect_push_consumer (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    throw (CORBA::SystemException);

private:
  RtecEventChannelAdmin::ProxyPushSupplier_var supplier_proxy_;
  // The supplier proxy.
};

#endif /* TAO_TUTORIALS_QUOTER_RT_EVENT_SERVICE_STOCK_CONSUMER_H */
