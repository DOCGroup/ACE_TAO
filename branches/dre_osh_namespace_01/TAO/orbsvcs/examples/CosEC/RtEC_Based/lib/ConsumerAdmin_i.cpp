// $Id$

#include "ConsumerAdmin_i.h"
#include "ace/Auto_Ptr.h"

TAO_CosEC_ConsumerAdmin_i::TAO_CosEC_ConsumerAdmin_i (void)
  : qos_ (),
    rtec_consumeradmin_ (RtecEventChannelAdmin::ConsumerAdmin::_nil ())
{
  // No-Op.
}

TAO_CosEC_ConsumerAdmin_i::~TAO_CosEC_ConsumerAdmin_i (void)
{
  // No-Op.
}

int
TAO_CosEC_ConsumerAdmin_i::init (const RtecEventChannelAdmin::ConsumerQOS &consumerqos,
                                 RtecEventChannelAdmin::ConsumerAdmin_ptr rtec_consumeradmin)
{
  this->qos_ = consumerqos;
  this->rtec_consumeradmin_ =
    RtecEventChannelAdmin::ConsumerAdmin::_duplicate (rtec_consumeradmin);
  return 0;
}

CosEventChannelAdmin::ProxyPushSupplier_ptr
TAO_CosEC_ConsumerAdmin_i::obtain_push_supplier (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException))
{
  CosEventChannelAdmin::ProxyPushSupplier_ptr proxysupplier_nil =
    CosEventChannelAdmin::ProxyPushSupplier::_nil ();

  RtecEventChannelAdmin::ProxyPushSupplier_var rtecproxypushsupplier =
    this->rtec_consumeradmin_->obtain_push_supplier (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (proxysupplier_nil);

  TAO_CosEC_ProxyPushSupplier_i *proxypushsupplier;

  ACE_NEW_RETURN (proxypushsupplier,
                  TAO_CosEC_ProxyPushSupplier_i (this->qos_,
                                                 rtecproxypushsupplier.in ()),
                  proxysupplier_nil);

  auto_ptr<TAO_CosEC_ProxyPushSupplier_i>
    auto_proxysupplier (proxypushsupplier);

  CosEventChannelAdmin::ProxyPushSupplier_ptr proxy_obj =
    auto_proxysupplier.get ()->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (proxysupplier_nil);

   // give the ownership to the POA.
  auto_proxysupplier.get ()->_remove_ref (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (proxysupplier_nil);

  auto_proxysupplier.release ();
  return proxy_obj;
}

CosEventChannelAdmin::ProxyPullSupplier_ptr
TAO_CosEC_ConsumerAdmin_i::obtain_pull_supplier (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO: implement this.
  return CosEventChannelAdmin::ProxyPullSupplier::_nil ();
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
  template class ACE_Auto_Basic_Ptr<TAO_CosEC_ProxyPushSupplier_i>;
  template class auto_ptr<TAO_CosEC_ProxyPushSupplier_i>;
#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
# pragma instantiate ACE_Auto_Basic_Ptr<TAO_CosEC_ProxyPushSupplier_i>
# pragma instantiate auto_ptr<TAO_CosEC_ProxyPushSupplier_i>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
