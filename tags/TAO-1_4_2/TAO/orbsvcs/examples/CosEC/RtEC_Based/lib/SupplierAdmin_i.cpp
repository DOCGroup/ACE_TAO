// $Id$

#include "SupplierAdmin_i.h"
#include "ace/Auto_Ptr.h"

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
TAO_CosEC_SupplierAdmin_i::obtain_push_consumer (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException))
{
  CosEventChannelAdmin::ProxyPushConsumer_ptr proxyconsumer_nil =
    CosEventChannelAdmin::ProxyPushConsumer::_nil ();

  RtecEventChannelAdmin::ProxyPushConsumer_var rtecproxypushconsumer =
    this->rtec_supplieradmin_->obtain_push_consumer (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (proxyconsumer_nil);

  TAO_CosEC_ProxyPushConsumer_i *proxypushconsumer;

  ACE_NEW_RETURN (proxypushconsumer,
                  TAO_CosEC_ProxyPushConsumer_i (this->qos_,
                                                 rtecproxypushconsumer.in ()),
                  proxyconsumer_nil);
  auto_ptr <TAO_CosEC_ProxyPushConsumer_i>
    auto_proxyconsumer (proxypushconsumer);

  CosEventChannelAdmin::ProxyPushConsumer_ptr proxy_obj =
    auto_proxyconsumer.get ()->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (proxyconsumer_nil);

   // give the ownership to the POA.
  auto_proxyconsumer.get ()->_remove_ref (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (proxyconsumer_nil);

  auto_proxyconsumer.release ();
  return proxy_obj;
}

CosEventChannelAdmin::ProxyPullConsumer_ptr
TAO_CosEC_SupplierAdmin_i::obtain_pull_consumer (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO: implement this.
  return CosEventChannelAdmin::ProxyPullConsumer::_nil ();
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
  template class ACE_Auto_Basic_Ptr<TAO_CosEC_ProxyPushConsumer_i>;
  template class auto_ptr<TAO_CosEC_ProxyPushConsumer_i>;
#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
# pragma instantiate ACE_Auto_Basic_Ptr<TAO_CosEC_ProxyPushConsumer_i>
# pragma instantiate auto_ptr<TAO_CosEC_ProxyPushConsumer_i>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
