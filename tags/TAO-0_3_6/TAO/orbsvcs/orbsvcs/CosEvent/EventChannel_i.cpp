// $Id$
#include "EventChannel_i.h"

TAO_CosEC_EventChannel_i::TAO_CosEC_EventChannel_i (void)
  : consumer_admin_ (),
    supplier_admin_ (),
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
                                RtecEventChannelAdmin::EventChannel_ptr rtec,
                                CORBA::Environment &TAO_IN_ENV)
{
  RtecEventChannelAdmin::ConsumerAdmin_ptr rtec_consumeradmin =
    rtec->for_consumers (TAO_IN_ENV);
  TAO_CHECK_ENV_RETURN (TAO_IN_ENV, -1);

  if (this->consumer_admin_.init (consumerqos,
                                  rtec_consumeradmin) == -1)
    return -1;

  this->consumeradmin_ = consumer_admin_._this (TAO_IN_ENV);
  TAO_CHECK_ENV_RETURN (TAO_IN_ENV, -1);

  RtecEventChannelAdmin::SupplierAdmin_ptr rtec_supplieradmin =
    rtec->for_suppliers (TAO_IN_ENV);
  TAO_CHECK_ENV_RETURN (TAO_IN_ENV, -1);

  if (this->supplier_admin_.init (supplierqos,
                                  rtec_supplieradmin) == -1)
    return -1;

  this->supplieradmin_ = supplier_admin_._this (TAO_IN_ENV);
  TAO_CHECK_ENV_RETURN (TAO_IN_ENV, -1);

  return 0;
}

CosEventChannelAdmin::ConsumerAdmin_ptr
TAO_CosEC_EventChannel_i::for_consumers (CORBA::Environment &)
{
  // @@ Pradeep: you must make a copy here, because the caller is
  // responsible of removing this object.
  return
    CosEventChannelAdmin::ConsumerAdmin::_duplicate (this->consumeradmin_.in());
}

CosEventChannelAdmin::SupplierAdmin_ptr
TAO_CosEC_EventChannel_i::for_suppliers (CORBA::Environment &)
{
  // @@ Pradeep: you must make a copy here, because the caller is
  // responsible of removing this object, same here..
  return
    CosEventChannelAdmin::SupplierAdmin::_duplicate (this->supplieradmin_.in ());
}

void
TAO_CosEC_EventChannel_i::destroy (CORBA::Environment &TAO_IN_ENV)
{
  // Deactivate the CosEventChannel
  PortableServer::POA_var poa =
    this->_default_POA (TAO_IN_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_IN_ENV);

  PortableServer::ObjectId_var id = poa->servant_to_id (this,
                                                        TAO_IN_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_IN_ENV);

  poa->deactivate_object (id.in (),
                          TAO_IN_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_IN_ENV);

  this->supplieradmin_ =  CosEventChannelAdmin::SupplierAdmin::_nil ();
  this->consumeradmin_ =  CosEventChannelAdmin::ConsumerAdmin::_nil ();
}

void
TAO_CosEC_EventChannel_i::shutdown (CORBA::Environment &TAO_IN_ENV)
{
  this->destroy (TAO_IN_ENV);
  delete this;
}
