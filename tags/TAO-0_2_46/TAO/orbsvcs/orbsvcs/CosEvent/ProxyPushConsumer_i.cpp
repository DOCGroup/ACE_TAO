/* -*- C++ -*- */
// $Id$

#include "orbsvcs/Event_Utilities.h"
#include "orbsvcs/CosEventChannelAdminC.h"
#include "orbsvcs/CosEventChannelAdminS.h"
#include "orbsvcs/RtecEventCommS.h"
#include "orbsvcs/CosEventCommS.h"
#include "CosEvent/ProxyPushConsumer_i.h"

class PushSupplierWrapper : public POA_RtecEventComm::PushSupplier
{
  // = TITLE
  //   A Wrapper for the Rtec PushSupplier.
  // = DESCRIPTION
  //   The Rtec ProxyPushConsumer uses a Rtec PushSupplier.
  //   This class wraps the Cos PushSupplier to make it look like a Rtec PushSupplier.
public:
  // = Initialization and termination methods.
  PushSupplierWrapper (CosEventComm::PushSupplier_ptr supplier);

  ~PushSupplierWrapper ();

  virtual void disconnect_push_supplier (CORBA::Environment &TAO_TRY_ENV);
  // disconnects the push supplier.
private:
  CosEventComm::PushSupplier_ptr supplier_;
  // The Cos PushSupplier that we're proxying for.
};

PushSupplierWrapper::PushSupplierWrapper
(CosEventComm::PushSupplier_ptr supplier)
  : supplier_ (CosEventComm::PushSupplier::_duplicate (supplier))
{
}

PushSupplierWrapper::~PushSupplierWrapper ()
{
  CORBA::release (supplier_);
}

void
PushSupplierWrapper::disconnect_push_supplier (CORBA::Environment &TAO_TRY_ENV)
{
  this->supplier_->disconnect_push_supplier (TAO_TRY_ENV);

 // Deactivate the supplier proxy
  PortableServer::POA_var poa =
    this->_default_POA (TAO_TRY_ENV);

  TAO_CHECK_ENV_RETURN_VOID (TAO_TRY_ENV);

  PortableServer::ObjectId_var id =
    poa->servant_to_id (this, TAO_TRY_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_TRY_ENV);

  poa->deactivate_object (id.in (), TAO_TRY_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_TRY_ENV);

  // @@ If we keep a list remember to remove this object from the list.
  delete this;
};

ProxyPushConsumer_i::ProxyPushConsumer_i (const RtecEventChannelAdmin::SupplierQOS &qos, RtecEventChannelAdmin::ProxyPushConsumer_ptr ppc)
  : qos_ (qos),
    ppc_ (ppc)
{
}

ProxyPushConsumer_i::~ProxyPushConsumer_i ()
{
}

void
ProxyPushConsumer_i::push (const CORBA::Any &data,
                     CORBA::Environment &TAO_TRY_ENV)
{
  RtecEventComm::EventSet events (1);
  events.length (1);

  RtecEventComm::Event& e = events[0];
  // TODO: fill this..
  //e.header.source = ECB_SupplierID_Test::SUPPLIER_ID;
  e.header.source = 1;
  e.header.ttl = 1;
  // TODO: fill this..
  e.header.type = ACE_ES_EVENT_ANY;

  ACE_hrtime_t t = ACE_OS::gethrtime ();
  ORBSVCS_Time::hrtime_to_TimeT (e.header.creation_time, t);
  e.header.ec_recv_time = ORBSVCS_Time::zero;
  e.header.ec_send_time = ORBSVCS_Time::zero;

  e.data.any_value = data;

  this->ppc_->push (events, TAO_TRY_ENV);
}

void
ProxyPushConsumer_i::disconnect_push_consumer (CORBA::Environment &TAO_TRY_ENV)
{
  this->ppc_->disconnect_push_consumer (TAO_TRY_ENV);

  // Deactivate the ProxyPushConsumer
  PortableServer::POA_var poa =
    this->_default_POA (TAO_TRY_ENV);

  TAO_CHECK_ENV_RETURN_VOID (TAO_TRY_ENV);

  PortableServer::ObjectId_var id =
    poa->servant_to_id (this, TAO_TRY_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_TRY_ENV);

  poa->deactivate_object (id.in (), TAO_TRY_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_TRY_ENV);

  delete this;
}

void
ProxyPushConsumer_i::connect_push_supplier (CosEventComm::PushSupplier_ptr push_supplier, CORBA::Environment &TAO_TRY_ENV)
{
  //if (this->connected ())
  //TAO_THROW (EventChannelAdmin::AlreadyConnected());

  // Implements the RtecEventSupplier interface
  PushSupplierWrapper *wrapper = new PushSupplierWrapper(push_supplier);

  this->ppc_->connect_push_supplier (wrapper->_this (TAO_TRY_ENV),
                                     this->qos_,
                                     TAO_TRY_ENV);
}
