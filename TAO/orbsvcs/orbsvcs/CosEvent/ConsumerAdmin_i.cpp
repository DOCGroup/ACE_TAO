// $Id$
//
#include "CosEvent/ConsumerAdmin_i.h"

ConsumerAdmin_i::ConsumerAdmin_i (const RtecEventChannelAdmin::ConsumerQOS &qos,
                                 RtecEventChannelAdmin::ConsumerAdmin_ptr
                                 rtec_consumeradmin)
  : qos_ (qos),
    rtec_consumeradmin_(RtecEventChannelAdmin::ConsumerAdmin::_duplicate (rtec_consumeradmin))
{
}

ConsumerAdmin_i::~ConsumerAdmin_i ()
{
  CORBA::release (this->rtec_consumeradmin_);
}

CosEventChannelAdmin::ProxyPushSupplier_ptr
ConsumerAdmin_i::obtain_push_supplier (CORBA::Environment &TAO_TRY_ENV)
{
  RtecEventChannelAdmin::ProxyPushSupplier_ptr rtecproxypushsupplier =
    this->rtec_consumeradmin_->obtain_push_supplier (TAO_TRY_ENV);

  TAO_CHECK_ENV_RETURN (TAO_TRY_ENV, 0);

  ProxyPushSupplier_i *pps =
    new ProxyPushSupplier_i (this->qos_, rtecproxypushsupplier);

  return pps->_this (TAO_TRY_ENV);
}

CosEventChannelAdmin::ProxyPullSupplier_ptr
ConsumerAdmin_i::obtain_pull_supplier (CORBA::Environment &TAO_TRY_ENV)
{
  // TODO: implement this.
  return 0;
}
