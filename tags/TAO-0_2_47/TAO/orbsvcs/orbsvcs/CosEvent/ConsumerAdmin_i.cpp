// $Id$

#include "ConsumerAdmin_i.h"

ConsumerAdmin_i::ConsumerAdmin_i (void)
  : qos_ (),
    rtec_consumeradmin_ (RtecEventChannelAdmin::ConsumerAdmin::_nil ())
{
  // No-Op.
}

ConsumerAdmin_i::~ConsumerAdmin_i (void)
{
  // @@ It should be easier to use an _var for holding this object
  // reference... 
  CORBA::release (this->rtec_consumeradmin_);
}

int
ConsumerAdmin_i::init (const RtecEventChannelAdmin::ConsumerQOS &consumerqos,
                       RtecEventChannelAdmin::ConsumerAdmin_ptr rtec_consumeradmin)
{
  this->qos_ = consumerqos;
  this->rtec_consumeradmin_ =
    RtecEventChannelAdmin::ConsumerAdmin::_duplicate (rtec_consumeradmin);
  return 0;
}

CosEventChannelAdmin::ProxyPushSupplier_ptr
ConsumerAdmin_i::obtain_push_supplier (CORBA::Environment &TAO_TRY_ENV)
{
  RtecEventChannelAdmin::ProxyPushSupplier_var rtecproxypushsupplier =
    this->rtec_consumeradmin_->obtain_push_supplier (TAO_TRY_ENV);
  TAO_CHECK_ENV_RETURN (TAO_TRY_ENV, 0);

  ProxyPushSupplier_i *pps;

  ACE_NEW_RETURN (pps,
                  ProxyPushSupplier_i (this->qos_,
                                       rtecproxypushsupplier.in ()),
                  CosEventChannelAdmin::ProxyPushSupplier::_nil ());

  return pps->_this (TAO_TRY_ENV);
}

CosEventChannelAdmin::ProxyPullSupplier_ptr
ConsumerAdmin_i::obtain_pull_supplier (CORBA::Environment &TAO_TRY_ENV)
{
  // TODO: implement this.
  return CosEventChannelAdmin::ProxyPullSupplier::_nil ();
}
