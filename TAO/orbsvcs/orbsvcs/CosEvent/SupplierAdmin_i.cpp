// $Id$

#include "SupplierAdmin_i.h"

TAO_CosEC_SupplierAdmin_i::TAO_CosEC_SupplierAdmin_i (void)
  : qos_ (),
    rtec_supplieradmin_ (RtecEventChannelAdmin::SupplierAdmin::_nil ())
{
  // No-Op.
}

TAO_CosEC_SupplierAdmin_i::~TAO_CosEC_SupplierAdmin_i (void)
{
  // No-Op.
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
TAO_CosEC_SupplierAdmin_i::obtain_push_consumer (CORBA::Environment &TAO_IN_ENV)
{
  RtecEventChannelAdmin::ProxyPushConsumer_var rtecproxypushconsumer =
    this->rtec_supplieradmin_->obtain_push_consumer (TAO_IN_ENV);
  TAO_CHECK_ENV_RETURN (TAO_IN_ENV, 0);

  TAO_CosEC_ProxyPushConsumer_i *proxypushconsumer;

  ACE_NEW_RETURN (proxypushconsumer,
                  TAO_CosEC_ProxyPushConsumer_i (this->qos_,
                                                 rtecproxypushconsumer.in ()),
                  CosEventChannelAdmin::ProxyPushConsumer::_nil ());
  return proxypushconsumer->_this (TAO_IN_ENV);
}

CosEventChannelAdmin::ProxyPullConsumer_ptr
TAO_CosEC_SupplierAdmin_i::obtain_pull_consumer (CORBA::Environment &)
{
  // TODO: implement this.
  return CosEventChannelAdmin::ProxyPullConsumer::_nil ();
}
