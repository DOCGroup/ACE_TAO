// $Id$
//
#include "orbsvcs/RtecEventChannelAdminC.h"
#include "orbsvcs/CosEventChannelAdminC.h"
#include "orbsvcs/CosEventChannelAdminS.h"
#include "orbsvcs/CosEventCommS.h"
#include "orbsvcs/RtecEventCommS.h"
#include "ProxyPushSupplier_i.h"

// @@ Pradeep: In general the same comments that I did for the
// Consumer side apply. 

class PushConsumerWrapper : public POA_RtecEventComm::PushConsumer
{
  // = TITLE
  //   Wrapper class for the Rtec PushConsumer.
  //
  // = DESCRIPTION
  //   The Rtec ProxyPushSupplier uses a Rtec PushConsumer.  This
  //   class wraps the Cos PushConsumer to make it look like a Rtec
  //   PushConsumer.
public:
  // = Initialization and termination methods.
  PushConsumerWrapper (CosEventComm::PushConsumer_ptr consumer);
  // Constructor.

  ~PushConsumerWrapper (void);
  // Destructor.

  virtual void push (const RtecEventComm::EventSet & data,
                     CORBA::Environment &TAO_TRY_ENV);
  // This method is called by the RTEvent Channel to supply data.

  virtual void disconnect_push_consumer (CORBA::Environment &TAO_TRY_ENV);
  // Disconnects the consumer from the event channel.

private:
  CosEventComm::PushConsumer_ptr consumer_;
  // The Cos PushConsumer that we're proxying for.
};

PushConsumerWrapper::PushConsumerWrapper
(CosEventComm::PushConsumer_ptr consumer)
    : consumer_ (CosEventComm::PushConsumer::_duplicate (consumer))
{
  // No-Op.
}

PushConsumerWrapper::~PushConsumerWrapper ()
{
  CORBA::release (this->consumer_);
}

void
PushConsumerWrapper::push (const RtecEventComm::EventSet& set,
                           CORBA::Environment &_env)
{
  for (CORBA::ULong i = 0;
       i < set.length ();
       ++i)
    this->consumer_->push (set[i].data.any_value,
                           _env);
}

void
PushConsumerWrapper::disconnect_push_consumer (CORBA::Environment &TAO_TRY_ENV)
{
  // Deactivate the supplier proxy.
  this->consumer_->disconnect_push_consumer (TAO_TRY_ENV);

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
}

ProxyPushSupplier_i::ProxyPushSupplier_i
        (const RtecEventChannelAdmin::ConsumerQOS &qos,
	 RtecEventChannelAdmin::ProxyPushSupplier_ptr pps)
  : qos_ (qos),
    pps_ (RtecEventChannelAdmin::ProxyPushSupplier::_duplicate (pps))
{
  // No-Op.
}

ProxyPushSupplier_i::~ProxyPushSupplier_i (void)
{
  CORBA::release (this->pps_);
}

void
ProxyPushSupplier_i::disconnect_push_supplier (CORBA::Environment &TAO_TRY_ENV)
{
  this->pps_->disconnect_push_supplier (TAO_TRY_ENV);

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
}

void ProxyPushSupplier_i::connect_push_consumer (CosEventComm::PushConsumer_ptr push_consumer,
                                                 CORBA::Environment &TAO_TRY_ENV)
{
  if (this->connected ())
    TAO_THROW_ENV (CosEventChannelAdmin::AlreadyConnected (),
                   TAO_TRY_ENV);
  if (push_consumer == CosEventComm::PushConsumer::_nil())
    TAO_THROW_ENV (CORBA::BAD_PARAM (CORBA::COMPLETED_NO),
                   TAO_TRY_ENV);
  ACE_NEW (this->wrapper_,
           PushConsumerWrapper (push_consumer));
  this->pps_->connect_push_consumer (this->wrapper_->_this (TAO_TRY_ENV),
                                     this->qos_,
                                     TAO_TRY_ENV);
}

int
ProxyPushSupplier_i::connected (void)
{
  return this->wrapper_ == 0 ? 0 : 1;
}
