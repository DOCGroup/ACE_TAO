#include "EventChannel_i.h"
#include <memory>

TAO_CosEC_EventChannel_i::TAO_CosEC_EventChannel_i ()
  : consumer_admin_ (0),
    supplier_admin_ (0),
    consumeradmin_ (CosEventChannelAdmin::ConsumerAdmin::_nil ()),
    supplieradmin_ (CosEventChannelAdmin::SupplierAdmin::_nil ())
{
}

int
TAO_CosEC_EventChannel_i::init (const RtecEventChannelAdmin::ConsumerQOS &consumerqos,
                                const RtecEventChannelAdmin::SupplierQOS &supplierqos,
                                RtecEventChannelAdmin::EventChannel_ptr rtec)
{
  // Allocate the admins..
  TAO_CosEC_ConsumerAdmin_i *consumer_;
  ACE_NEW_RETURN (consumer_,
                  TAO_CosEC_ConsumerAdmin_i (),
                  -1);

  std::unique_ptr <TAO_CosEC_ConsumerAdmin_i> auto_consumer_ (consumer_);

  TAO_CosEC_SupplierAdmin_i *supplier_;
  ACE_NEW_RETURN (supplier_,
                  TAO_CosEC_SupplierAdmin_i (),
                  -1);

  std::unique_ptr <TAO_CosEC_SupplierAdmin_i> auto_supplier_ (supplier_);

  RtecEventChannelAdmin::ConsumerAdmin_ptr rtec_consumeradmin =
    rtec->for_consumers ();

  if (auto_consumer_.get ()->init (consumerqos,
                                   rtec_consumeradmin) == -1)
    return -1;

  this->consumeradmin_ =
    auto_consumer_.get ()->_this ();

  // give the ownership to the POA.
  auto_consumer_.get ()->_remove_ref ();

  RtecEventChannelAdmin::SupplierAdmin_ptr rtec_supplieradmin =
    rtec->for_suppliers ();

  if (auto_supplier_.get ()->init (supplierqos,
                                   rtec_supplieradmin) == -1)
    return -1;

  this->supplieradmin_ =
    auto_supplier_.get ()->_this ();

  // give the ownership to the POA.
  auto_supplier_.get ()->_remove_ref ();

  this->consumer_admin_ = auto_consumer_.release ();
  this->supplier_admin_ = auto_supplier_.release ();

  return 0;
}

CosEventChannelAdmin::ConsumerAdmin_ptr
TAO_CosEC_EventChannel_i::for_consumers ()
{
  // @@ Pradeep: you must make a copy here, because the caller is
  // responsible of removing this object.
  return CosEventChannelAdmin::ConsumerAdmin::_duplicate (this->consumeradmin_.in());
}

CosEventChannelAdmin::SupplierAdmin_ptr
TAO_CosEC_EventChannel_i::for_suppliers ()
{
  // @@ Pradeep: you must make a copy here, because the caller is
  // responsible of removing this object, same here..
  return CosEventChannelAdmin::SupplierAdmin::_duplicate (this->supplieradmin_.in ());
}

void
TAO_CosEC_EventChannel_i::destroy ()
{
  // Deactivate the CosEventChannel
  PortableServer::POA_var poa =
    this->_default_POA ();

  PortableServer::ObjectId_var id = poa->servant_to_id (this);

  poa->deactivate_object (id.in ());

  this->supplieradmin_ =  CosEventChannelAdmin::SupplierAdmin::_nil ();
  this->consumeradmin_ =  CosEventChannelAdmin::ConsumerAdmin::_nil ();
}

void
TAO_CosEC_EventChannel_i::shutdown ()
{
  this->destroy ();
}
