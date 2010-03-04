// $Id$
//
#include "orbsvcs/RtecEventChannelAdminC.h"
#include "orbsvcs/CosEventChannelAdminC.h"
#include "orbsvcs/CosEventChannelAdminS.h"
#include "orbsvcs/CosEventCommS.h"
#include "orbsvcs/RtecEventCommS.h"
#include "ProxyPushSupplier_i.h"
#include "ace/Auto_Ptr.h"

#if defined(_MSC_VER)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class TAO_CosEC_PushConsumerWrapper :
  public POA_RtecEventComm::PushConsumer
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

  virtual void push (const RtecEventComm::EventSet & data);
  // This method is called by the RTEvent Channel to supply data.

  virtual void disconnect_push_consumer (void);
  // Disconnects the consumer from the event channel.

private:
  CosEventComm::PushConsumer_var consumer_;
  // The Cos PushConsumer that we're proxying for.
};

#if defined(_MSC_VER)
#pragma warning(default:4250)
#endif /* _MSC_VER */

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
TAO_CosEC_PushConsumerWrapper::push (const RtecEventComm::EventSet& set)
{
  for (CORBA::ULong i = 0;
       i < set.length ();
       ++i)
    {
      try
        {
          this->consumer_->push (set[i].data.any_value);
        }
      catch (const CORBA::Exception&)
        {
          // Ignore the exception...
        }
    }
}

void
TAO_CosEC_PushConsumerWrapper::disconnect_push_consumer (void)
{
  // Deactivate the supplier proxy.
  this->consumer_->disconnect_push_consumer ();

  PortableServer::POA_var poa =
    this->_default_POA ();

  PortableServer::ObjectId_var id =
    poa->servant_to_id (this);

  poa->deactivate_object (id.in ());

  // @@ If we keep a list remember to remove this object from the
  // list.
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
TAO_CosEC_ProxyPushSupplier_i::disconnect_push_supplier (void)
{
  this->pps_->disconnect_push_supplier ();

  // Deactivate the supplier proxy
  PortableServer::POA_var poa =
    this->_default_POA ();

  PortableServer::ObjectId_var id =
    poa->servant_to_id (this);

  poa->deactivate_object (id.in ());

  // @@ If we keep a list remember to remove this object from the
  // list.
}

void
TAO_CosEC_ProxyPushSupplier_i::connect_push_consumer (CosEventComm::PushConsumer_ptr push_consumer)
{
  if (this->connected ())
    throw CosEventChannelAdmin::AlreadyConnected ();

  if (push_consumer == CosEventComm::PushConsumer::_nil())
    throw CORBA::BAD_PARAM ();

  TAO_CosEC_PushConsumerWrapper *wrapper;
  ACE_NEW_THROW_EX (wrapper,
                    TAO_CosEC_PushConsumerWrapper (push_consumer),
                    CORBA::NO_MEMORY ());

  auto_ptr <TAO_CosEC_PushConsumerWrapper> auto_wrapper (wrapper);

  // @@ This code is not exception safe.
  RtecEventComm::PushConsumer_ptr  rtecpushconsumer =
    auto_wrapper.get ()->_this ();

  // give the ownership to the POA.
  auto_wrapper.get ()->_remove_ref ();

  this->pps_->connect_push_consumer (rtecpushconsumer,
                                     this->qos_);

  this->wrapper_ = auto_wrapper.release ();
}

int
TAO_CosEC_ProxyPushSupplier_i::connected (void)
{
  return this->wrapper_ == 0 ? 0 : 1;
}

