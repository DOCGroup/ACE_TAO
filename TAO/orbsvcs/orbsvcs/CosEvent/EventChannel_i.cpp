// $Id$
#include "EventChannel_i.h"

EventChannel_i::EventChannel_i(const RtecEventChannelAdmin::ConsumerQOS &consumerqos,
                               const RtecEventChannelAdmin::SupplierQOS &supplierqos,
                               RtecEventChannelAdmin::ConsumerAdmin_ptr rtec_consumeradmin,
                               RtecEventChannelAdmin::SupplierAdmin_ptr rtec_supplieradmin)
  : consumer_admin_ (consumerqos, rtec_consumeradmin),
    supplier_admin_ (supplierqos, rtec_supplieradmin),
    consumeradmin_ (0),
    supplieradmin_ (0)
{

}

EventChannel_i::~EventChannel_i ()
{
  //No-Op.
}

int
EventChannel_i::init (CORBA::Environment &TAO_TRY_ENV)
{
  this->consumeradmin_ = consumer_admin_._this (TAO_TRY_ENV);
  TAO_CHECK_ENV_RETURN (TAO_TRY_ENV, -1);

  this->supplieradmin_ = supplier_admin_._this (TAO_TRY_ENV);
  TAO_CHECK_ENV_RETURN (TAO_TRY_ENV, -1);

  return 0;
}

CosEventChannelAdmin::ConsumerAdmin_ptr
EventChannel_i::for_consumers (CORBA::Environment &TAO_TRY_ENV)
{
  return this->consumeradmin_;
}

CosEventChannelAdmin::SupplierAdmin_ptr
EventChannel_i::for_suppliers (CORBA::Environment &TAO_TRY_ENV)
{
  return this->supplieradmin_;
}

void
EventChannel_i::destroy (CORBA::Environment &TAO_TRY_ENV)
{
  // Deactivate the CosEventChannel
  PortableServer::POA_var poa = this->_default_POA (TAO_TRY_ENV);

  TAO_CHECK_ENV_RETURN_VOID (TAO_TRY_ENV);

  PortableServer::ObjectId_var id = poa->servant_to_id (this, TAO_TRY_ENV);

  TAO_CHECK_ENV_RETURN_VOID (TAO_TRY_ENV);

  poa->deactivate_object (id.in (), TAO_TRY_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_TRY_ENV);

  delete this;
}
