// $Id$
//
#include "CosEvent/SupplierAdmin_i.h"

SupplierAdmin_i::SupplierAdmin_i(const RtecEventChannelAdmin::SupplierQOS &qos,
                                 RtecEventChannelAdmin::SupplierAdmin_ptr
                                 rtec_supplieradmin)
  :qos_ (qos),
   rtec_supplieradmin_(RtecEventChannelAdmin::SupplierAdmin::_duplicate (rtec_supplieradmin))
{
}

SupplierAdmin_i::~SupplierAdmin_i ()
{
  CORBA::release (this->rtec_supplieradmin_);
}

CosEventChannelAdmin::ProxyPushConsumer_ptr
SupplierAdmin_i::obtain_push_consumer (CORBA::Environment &TAO_TRY_ENV)
{
  RtecEventChannelAdmin::ProxyPushConsumer_ptr rtecproxypushconsumer =
    this->rtec_supplieradmin_->obtain_push_consumer (TAO_TRY_ENV);

  TAO_CHECK_ENV_RETURN (TAO_TRY_ENV, 0);

  ProxyPushConsumer_i *ppc =
    new ProxyPushConsumer_i (this->qos_, rtecproxypushconsumer);

  return ppc->_this (TAO_TRY_ENV);
}


CosEventChannelAdmin::ProxyPullConsumer_ptr
SupplierAdmin_i::obtain_pull_consumer (CORBA::Environment &TAO_TRY_ENV)
{
  // TODO: implement this.
  return 0;
}
