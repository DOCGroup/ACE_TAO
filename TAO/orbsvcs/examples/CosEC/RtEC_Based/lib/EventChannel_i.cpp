// $Id$
#include "EventChannel_i.h"
#include "ace/Auto_Ptr.h"

TAO_CosEC_EventChannel_i::TAO_CosEC_EventChannel_i (void)
  : consumer_admin_ (0),
    supplier_admin_ (0),
    consumeradmin_ (CosEventChannelAdmin::ConsumerAdmin::_nil ()),
    supplieradmin_ (CosEventChannelAdmin::SupplierAdmin::_nil ())
{
  // No-Op.
}

TAO_CosEC_EventChannel_i::~TAO_CosEC_EventChannel_i (void)
{
  //No-Op.
}

int
TAO_CosEC_EventChannel_i::init (const RtecEventChannelAdmin::ConsumerQOS &consumerqos,
                                const RtecEventChannelAdmin::SupplierQOS &supplierqos,
                                RtecEventChannelAdmin::EventChannel_ptr rtec
                                TAO_ENV_ARG_DECL)
{
  // Allocate the admins..
  TAO_CosEC_ConsumerAdmin_i *consumer_;
  ACE_NEW_RETURN (consumer_,
                  TAO_CosEC_ConsumerAdmin_i (),
                  -1);

  auto_ptr <TAO_CosEC_ConsumerAdmin_i> auto_consumer_ (consumer_);

  TAO_CosEC_SupplierAdmin_i *supplier_;
  ACE_NEW_RETURN (supplier_,
                  TAO_CosEC_SupplierAdmin_i (),
                  -1);

  auto_ptr <TAO_CosEC_SupplierAdmin_i> auto_supplier_ (supplier_);

  RtecEventChannelAdmin::ConsumerAdmin_ptr rtec_consumeradmin =
    rtec->for_consumers (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (auto_consumer_.get ()->init (consumerqos,
                                   rtec_consumeradmin) == -1)
    return -1;

  this->consumeradmin_ =
    auto_consumer_.get ()->_this (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // give the ownership to the POA.
  auto_consumer_.get ()->_remove_ref (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  RtecEventChannelAdmin::SupplierAdmin_ptr rtec_supplieradmin =
    rtec->for_suppliers (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (auto_supplier_.get ()->init (supplierqos,
                                   rtec_supplieradmin) == -1)
    return -1;

  this->supplieradmin_ =
    auto_supplier_.get ()->_this (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // give the ownership to the POA.
  auto_supplier_.get ()->_remove_ref (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  this->consumer_admin_ = auto_consumer_.release ();
  this->supplier_admin_ = auto_supplier_.release ();

  return 0;
}

CosEventChannelAdmin::ConsumerAdmin_ptr
TAO_CosEC_EventChannel_i::for_consumers (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException))
{
  // @@ Pradeep: you must make a copy here, because the caller is
  // responsible of removing this object.
  return
    CosEventChannelAdmin::ConsumerAdmin::_duplicate (this->consumeradmin_.in());
}

CosEventChannelAdmin::SupplierAdmin_ptr
TAO_CosEC_EventChannel_i::for_suppliers (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException))
{
  // @@ Pradeep: you must make a copy here, because the caller is
  // responsible of removing this object, same here..
  return
    CosEventChannelAdmin::SupplierAdmin::_duplicate (this->supplieradmin_.in ());
}

void
TAO_CosEC_EventChannel_i::destroy (TAO_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Deactivate the CosEventChannel
  PortableServer::POA_var poa =
    this->_default_POA (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  PortableServer::ObjectId_var id = poa->servant_to_id (this
                                                        TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  poa->deactivate_object (id.in ()
                          TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->supplieradmin_ =  CosEventChannelAdmin::SupplierAdmin::_nil ();
  this->consumeradmin_ =  CosEventChannelAdmin::ConsumerAdmin::_nil ();
}

void
TAO_CosEC_EventChannel_i::shutdown (TAO_ENV_SINGLE_ARG_DECL)
{
  this->destroy (TAO_ENV_SINGLE_ARG_PARAMETER);
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
  template class ACE_Auto_Basic_Ptr<TAO_CosEC_ConsumerAdmin_i>;
  template class auto_ptr<TAO_CosEC_ConsumerAdmin_i>;
  template class ACE_Auto_Basic_Ptr<TAO_CosEC_SupplierAdmin_i>;
  template class auto_ptr<TAO_CosEC_SupplierAdmin_i>;
#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
# pragma instantiate ACE_Auto_Basic_Ptr<TAO_CosEC_ConsumerAdmin_i>
# pragma instantiate auto_ptr<TAO_CosEC_ConsumerAdmin_i>
# pragma instantiate ACE_Auto_Basic_Ptr<TAO_CosEC_SupplierAdmin_i>
# pragma instantiate auto_ptr<TAO_CosEC_SupplierAdmin_i>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
