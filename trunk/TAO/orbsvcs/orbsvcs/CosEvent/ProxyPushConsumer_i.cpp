/* -*- C++ -*- */
// $Id$

#include "orbsvcs/Event_Utilities.h"
#include "orbsvcs/CosEventChannelAdminC.h"
#include "orbsvcs/CosEventChannelAdminS.h"
#include "orbsvcs/RtecEventCommS.h"
#include "orbsvcs/CosEventCommS.h"
#include "ProxyPushConsumer_i.h"

class PushSupplierWrapper : public POA_RtecEventComm::PushSupplier
{
  // = TITLE
  //   A Wrapper for the Rtec PushSupplier.
  //
  // = DESCRIPTION
  //   The Rtec ProxyPushConsumer uses a Rtec PushSupplier.  This
  //   class wraps the Cos PushSupplier to make it look like a Rtec
  //   PushSupplier.
public:
  // = Initialization and termination methods.
  PushSupplierWrapper (CosEventComm::PushSupplier_ptr supplier);
  // Constructor.

  ~PushSupplierWrapper (void);
  // Destructor.

  virtual void disconnect_push_supplier (CORBA::Environment &TAO_TRY_ENV);
  // Disconnects the push supplier.

private:
  CosEventComm::PushSupplier_ptr supplier_;
  // The Cos PushSupplier that we're proxying for.
};

PushSupplierWrapper::PushSupplierWrapper
(CosEventComm::PushSupplier_ptr supplier)
  : supplier_ (CosEventComm::PushSupplier::_duplicate (supplier))
{
  // No-Op.
}

PushSupplierWrapper::~PushSupplierWrapper ()
{
  CORBA::release (this->supplier_);
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
    poa->servant_to_id (this,
                        TAO_TRY_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_TRY_ENV);

  poa->deactivate_object (id.in (),
                          TAO_TRY_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_TRY_ENV);

  // @@ If we keep a list remember to remove this object from the
  // list.
  delete this;
};

ProxyPushConsumer_i::ProxyPushConsumer_i (const RtecEventChannelAdmin::SupplierQOS &qos,
                                          RtecEventChannelAdmin::ProxyPushConsumer_ptr ppc)
  : qos_ (qos),
    ppc_ (RtecEventChannelAdmin::ProxyPushConsumer::_duplicate (ppc)),
    wrapper_ (0)
{
  // No-Op.
}

ProxyPushConsumer_i::~ProxyPushConsumer_i (void)
{
  CORBA::release (this->ppc_);
}

void
ProxyPushConsumer_i::push (const CORBA::Any &data,
                     CORBA::Environment &TAO_TRY_ENV)
{
  RtecEventComm::EventSet events (1);
  events.length (1);

  RtecEventComm::Event &e = events[0];
  RtecEventComm::Event eqos =
    qos_.publications[0].event;

  // NOTE: we initialize the <EventHeader> field using the 1st
  // <publications> from the <SupplierQOS>.so we assume that
  // publications[0] is initialized.
  e.header.source = eqos.header.source;
  e.header.ttl = eqos.header.ttl;
  e.header.type = eqos.header.type;

  ACE_hrtime_t t = ACE_OS::gethrtime ();

  ORBSVCS_Time::hrtime_to_TimeT (e.header.creation_time,
                                 t);
  e.header.ec_recv_time = ORBSVCS_Time::zero;
  e.header.ec_send_time = ORBSVCS_Time::zero;

  e.data.any_value = data;

  this->ppc_->push (events,
                    TAO_TRY_ENV);
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
    poa->servant_to_id (this,
                        TAO_TRY_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_TRY_ENV);

  poa->deactivate_object (id.in (),
                          TAO_TRY_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_TRY_ENV);

  delete this;
}

void
ProxyPushConsumer_i::connect_push_supplier (CosEventComm::PushSupplier_ptr push_supplier,
                                            CORBA::Environment &TAO_TRY_ENV)
{
  if (this->connected ())
    TAO_THROW_ENV (CosEventChannelAdmin::AlreadyConnected (),
                   TAO_TRY_ENV);

  ACE_NEW (this->wrapper_, PushSupplierWrapper (push_supplier));

  this->ppc_->connect_push_supplier (this->wrapper_->_this (TAO_TRY_ENV),
                                     this->qos_,
                                     TAO_TRY_ENV);
}

int
ProxyPushConsumer_i::connected (void)
{
  return this->wrapper_ == 0 ? 0 : 1;
}
