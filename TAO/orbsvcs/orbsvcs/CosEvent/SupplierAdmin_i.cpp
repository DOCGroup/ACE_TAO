// $Id$

#include "SupplierAdmin_i.h"

SupplierAdmin_i::SupplierAdmin_i (void)
  : qos_ (),
    rtec_supplieradmin_ ()
{
  // No-Op.
}

SupplierAdmin_i::~SupplierAdmin_i (void)
{
  // No-Op.
}

int
SupplierAdmin_i::init (const RtecEventChannelAdmin::SupplierQOS &supplierqos,
                       RtecEventChannelAdmin::SupplierAdmin_ptr rtec_supplieradmin)
{
  this->qos_ = supplierqos;
  this->rtec_supplieradmin_ =
    RtecEventChannelAdmin::SupplierAdmin::_duplicate (rtec_supplieradmin);
  return 0;
}

CosEventChannelAdmin::ProxyPushConsumer_ptr
SupplierAdmin_i::obtain_push_consumer (CORBA::Environment &TAO_TRY_ENV)
{
  RtecEventChannelAdmin::ProxyPushConsumer_ptr rtecproxypushconsumer =
    this->rtec_supplieradmin_->obtain_push_consumer (TAO_TRY_ENV);
  TAO_CHECK_ENV_RETURN (TAO_TRY_ENV, 0);

  ProxyPushConsumer_i *ppc =
    // @@ Pradeep, make sure you ALWAYS use the ACE_NEW_RETURN or
    // ACE_NEW macros when you allocate memory in order to avoid
    // problems if the dynamically allocation fails.
    new ProxyPushConsumer_i (this->qos_,
                             rtecproxypushconsumer);

  return ppc->_this (TAO_TRY_ENV);
}

CosEventChannelAdmin::ProxyPullConsumer_ptr
SupplierAdmin_i::obtain_pull_consumer (CORBA::Environment &TAO_TRY_ENV)
{
  // TODO: implement this.
  return CosEventChannelAdmin::ProxyPullConsumer::_nil ();
}
