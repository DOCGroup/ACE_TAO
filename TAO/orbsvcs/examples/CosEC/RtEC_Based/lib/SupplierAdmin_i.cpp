#include "SupplierAdmin_i.h"
#include <memory>

TAO_CosEC_SupplierAdmin_i::TAO_CosEC_SupplierAdmin_i ()
  : qos_ (),
    rtec_supplieradmin_ (RtecEventChannelAdmin::SupplierAdmin::_nil ())
{
}

int
TAO_CosEC_SupplierAdmin_i::init (const RtecEventChannelAdmin::SupplierQOS &supplierqos,
                       RtecEventChannelAdmin::SupplierAdmin_ptr rtec_supplieradmin)
{
  this->qos_ = supplierqos;
  this->rtec_supplieradmin_ =
    RtecEventChannelAdmin::SupplierAdmin::_duplicate (rtec_supplieradmin);
  return 0;
}

CosEventChannelAdmin::ProxyPushConsumer_ptr
TAO_CosEC_SupplierAdmin_i::obtain_push_consumer ()
{
  CosEventChannelAdmin::ProxyPushConsumer_ptr proxyconsumer_nil =
    CosEventChannelAdmin::ProxyPushConsumer::_nil ();

  RtecEventChannelAdmin::ProxyPushConsumer_var rtecproxypushconsumer =
    this->rtec_supplieradmin_->obtain_push_consumer ();

  TAO_CosEC_ProxyPushConsumer_i *proxypushconsumer;

  ACE_NEW_RETURN (proxypushconsumer,
                  TAO_CosEC_ProxyPushConsumer_i (this->qos_,
                                                 rtecproxypushconsumer.in ()),
                  proxyconsumer_nil);
  std::unique_ptr <TAO_CosEC_ProxyPushConsumer_i>
    auto_proxyconsumer (proxypushconsumer);

  CosEventChannelAdmin::ProxyPushConsumer_ptr proxy_obj =
    auto_proxyconsumer.get ()->_this ();

   // give the ownership to the POA.
  auto_proxyconsumer.get ()->_remove_ref ();

  auto_proxyconsumer.release ();
  return proxy_obj;
}

CosEventChannelAdmin::ProxyPullConsumer_ptr
TAO_CosEC_SupplierAdmin_i::obtain_pull_consumer ()
{
  // TODO: implement this.
  return CosEventChannelAdmin::ProxyPullConsumer::_nil ();
}
