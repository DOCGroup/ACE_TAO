// $Id$
//
#include "orbsvcs/RtecEventChannelAdminC.h"
#include "orbsvcs/CosEventChannelAdminC.h"
#include "orbsvcs/CosEventChannelAdminS.h"
#include "orbsvcs/CosEventCommS.h"
#include "orbsvcs/RtecEventCommS.h"
#include "ProxyPushSupplier_i.h"

class TAO_CosEC_PushConsumerWrapper : public POA_RtecEventComm::PushConsumer
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
  TAO_CosEC_PushConsumerWrapper (CosEventComm::PushConsumer_ptr consumer);
  // Constructor.

  ~TAO_CosEC_PushConsumerWrapper (void);
  // Destructor.

  virtual void push (const RtecEventComm::EventSet & data,
                     CORBA::Environment &TAO_IN_ENV);
  // This method is called by the RTEvent Channel to supply data.

  virtual void disconnect_push_consumer (CORBA::Environment &TAO_IN_ENV);
  // Disconnects the consumer from the event channel.

private:
  CosEventComm::PushConsumer_var consumer_;
  // The Cos PushConsumer that we're proxying for.
};

TAO_CosEC_PushConsumerWrapper::TAO_CosEC_PushConsumerWrapper
(CosEventComm::PushConsumer_ptr consumer)
  : consumer_ (CosEventComm::PushConsumer::_duplicate (consumer))
{
  // No-Op.
}

TAO_CosEC_PushConsumerWrapper::~TAO_CosEC_PushConsumerWrapper ()
{
  // No-Op.
}

void
TAO_CosEC_PushConsumerWrapper::push (const RtecEventComm::EventSet& set,
                                     CORBA::Environment &TAO_IN_ENV)
{
  for (CORBA::ULong i = 0;
       i < set.length ();
       ++i)
    this->consumer_->push (set[i].data.any_value,
                           TAO_IN_ENV);
}

void
TAO_CosEC_PushConsumerWrapper::disconnect_push_consumer (CORBA::Environment &TAO_IN_ENV)
{
  // Deactivate the supplier proxy.
  this->consumer_->disconnect_push_consumer (TAO_IN_ENV);

  PortableServer::POA_var poa =
    this->_default_POA (TAO_IN_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_IN_ENV);

  PortableServer::ObjectId_var id =
    poa->servant_to_id (this,
                        TAO_IN_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_IN_ENV);

  poa->deactivate_object (id.in (),
                          TAO_IN_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_IN_ENV);

  // @@ If we keep a list remember to remove this object from the
  // list.
  delete this;
}

TAO_CosEC_ProxyPushSupplier_i::TAO_CosEC_ProxyPushSupplier_i
(const RtecEventChannelAdmin::ConsumerQOS &qos,
 RtecEventChannelAdmin::ProxyPushSupplier_ptr pps)
  : qos_ (qos),
    pps_ (RtecEventChannelAdmin::ProxyPushSupplier::_duplicate (pps)),
    wrapper_ (0)
{
  // No-Op.
}

TAO_CosEC_ProxyPushSupplier_i::~TAO_CosEC_ProxyPushSupplier_i (void)
{
  // No-Op.
}

void
TAO_CosEC_ProxyPushSupplier_i::disconnect_push_supplier (CORBA::Environment &TAO_IN_ENV)
{
  this->pps_->disconnect_push_supplier (TAO_IN_ENV);

  // Deactivate the supplier proxy
  PortableServer::POA_var poa =
    this->_default_POA (TAO_IN_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_IN_ENV);

  PortableServer::ObjectId_var id =
    poa->servant_to_id (this,
                        TAO_IN_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_IN_ENV);

  poa->deactivate_object (id.in (),
                          TAO_IN_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_IN_ENV);

  // @@ If we keep a list remember to remove this object from the
  // list.
  delete this;
}

void TAO_CosEC_ProxyPushSupplier_i::connect_push_consumer (CosEventComm::PushConsumer_ptr push_consumer,
                                                           CORBA::Environment &TAO_IN_ENV)
{
  if (this->connected ())
    TAO_THROW_ENV (CosEventChannelAdmin::AlreadyConnected (),
                   TAO_IN_ENV);

  if (push_consumer == CosEventComm::PushConsumer::_nil())
    TAO_THROW_ENV (CORBA::BAD_PARAM (CORBA::COMPLETED_NO),
                   TAO_IN_ENV);

  ACE_NEW_THROW (this->wrapper_,
                 TAO_CosEC_PushConsumerWrapper (push_consumer),
                 CORBA::NO_MEMORY (CORBA::COMPLETED_NO));

  this->pps_->connect_push_consumer (this->wrapper_->_this (TAO_IN_ENV),
                                       this->qos_,
                                       TAO_IN_ENV);
}

int
TAO_CosEC_ProxyPushSupplier_i::connected (void)
{
  return this->wrapper_ == 0 ? 0 : 1;
}
