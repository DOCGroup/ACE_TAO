// $Id$
//
#include "orbsvcs/RtecEventChannelAdminC.h"
#include "orbsvcs/CosEventChannelAdminC.h"
#include "orbsvcs/CosEventChannelAdminS.h"
#include "orbsvcs/CosEventCommS.h"
#include "orbsvcs/RtecEventCommS.h"
#include "ProxyPushSupplier_i.h"

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
  // @@ Pradeep, please add comments.

  virtual void disconnect_push_consumer (CORBA::Environment &TAO_TRY_ENV);
  // @@ Pradeep, please add comments.

private:
  CosEventComm::PushConsumer_ptr consumer_;
  // The Cos PushConsumer that we're proxying for.
};

PushConsumerWrapper::PushConsumerWrapper
(CosEventComm::PushConsumer_ptr consumer)
    : consumer_ (CosEventComm::PushConsumer::_duplicate (consumer))
{
}

PushConsumerWrapper::~PushConsumerWrapper ()
{
  CORBA::release (this->consumer_);
}

void
PushConsumerWrapper::push (const RtecEventComm::EventSet& set,
                           CORBA::Environment &_env)
{
  for (CORBA::ULong i = 0; i < set.length (); ++i)
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
    pps_ (pps)
{
}

ProxyPushSupplier_i::~ProxyPushSupplier_i (void)
{
}

void
ProxyPushSupplier_i::disconnect_push_supplier (CORBA::Environment &TAO_TRY_ENV)
{
  // @@ Maybe we should do a "delete this" here? And consider POA
  // deactivation...
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
  // Implements the RtecEventConsumer interface
  PushConsumerWrapper *wrapper =
    // @@ Pradeep, please always use ACE_NEW_RETURN or ACE_NEW to
    // allocate memory dynamically.
    new PushConsumerWrapper (push_consumer);

  this->pps_->connect_push_consumer (wrapper->_this (TAO_TRY_ENV),
                                     this->qos_,
                                     TAO_TRY_ENV);
}
