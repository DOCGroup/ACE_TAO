//
#include "orbsvcs/RtecEventChannelAdminC.h"
#include "orbsvcs/CosEventChannelAdminC.h"
#include "orbsvcs/CosEventChannelAdminS.h"
#include "orbsvcs/CosEventCommS.h"
#include "orbsvcs/RtecEventCommS.h"
#include "ProxyPushSupplier_i.h"
#include <memory>

#if defined(_MSC_VER)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

/**
 * @brief Adapts a Cos PushConsumer for use by the Rtec ProxyPushSupplier.
 *
 * The wrapper presents the Cos PushConsumer as an Rtec PushConsumer.
 */
class TAO_CosEC_PushConsumerWrapper :
  public POA_RtecEventComm::PushConsumer
{
public:
  explicit TAO_CosEC_PushConsumerWrapper (CosEventComm::PushConsumer_ptr consumer);

  ~TAO_CosEC_PushConsumerWrapper () override = default;
  TAO_CosEC_PushConsumerWrapper (TAO_CosEC_PushConsumerWrapper const &) = delete;
  TAO_CosEC_PushConsumerWrapper (TAO_CosEC_PushConsumerWrapper &&) = delete;
  TAO_CosEC_PushConsumerWrapper &operator= (TAO_CosEC_PushConsumerWrapper const &) = delete;
  TAO_CosEC_PushConsumerWrapper &operator= (TAO_CosEC_PushConsumerWrapper &&) = delete;
  /// Receives data supplied by the RT Event Channel.
  void push (RtecEventComm::EventSet const &data) override;

  /// Disconnects the consumer from the event channel.
  void disconnect_push_consumer () override;

private:
  /// The Cos PushConsumer being proxied.
  CosEventComm::PushConsumer_var consumer_;
};

#if defined(_MSC_VER)
#pragma warning(default:4250)
#endif /* _MSC_VER */

TAO_CosEC_PushConsumerWrapper::TAO_CosEC_PushConsumerWrapper
(CosEventComm::PushConsumer_ptr consumer)
  : consumer_ (CosEventComm::PushConsumer::_duplicate (consumer))
{
}

void
TAO_CosEC_PushConsumerWrapper::push (RtecEventComm::EventSet const &set)
{
  for (CORBA::ULong i = 0;
       i < set.length ();
       ++i)
    {
      try
        {
          this->consumer_->push (set[i].data.any_value);
        }
      catch (CORBA::Exception const &)
        {
          // Ignore the exception...
        }
    }
}

void
TAO_CosEC_PushConsumerWrapper::disconnect_push_consumer ()
{
  // Deactivate the supplier proxy.
  this->consumer_->disconnect_push_consumer ();

  PortableServer::POA_var const poa =
    this->_default_POA ();

  PortableServer::ObjectId_var const id =
    poa->servant_to_id (this);

  poa->deactivate_object (id.in ());

  // @@ If we keep a list remember to remove this object from the
  // list.
}

TAO_CosEC_ProxyPushSupplier_i::TAO_CosEC_ProxyPushSupplier_i
  (RtecEventChannelAdmin::ConsumerQOS const &qos,
   RtecEventChannelAdmin::ProxyPushSupplier_ptr pps)
  : qos_ (qos),
    pps_ (RtecEventChannelAdmin::ProxyPushSupplier::_duplicate (pps))
{
}

void
TAO_CosEC_ProxyPushSupplier_i::disconnect_push_supplier ()
{
  this->pps_->disconnect_push_supplier ();

  // Deactivate the supplier proxy
  PortableServer::POA_var const poa =
    this->_default_POA ();

  PortableServer::ObjectId_var const id =
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

  if (push_consumer == CosEventComm::PushConsumer::_nil ())
    throw CORBA::BAD_PARAM ();

  TAO_CosEC_PushConsumerWrapper *wrapper {};
  ACE_NEW_THROW_EX (wrapper,
                    TAO_CosEC_PushConsumerWrapper (push_consumer),
                    CORBA::NO_MEMORY ());

  std::unique_ptr<TAO_CosEC_PushConsumerWrapper> auto_wrapper (wrapper);

  // @@ This code is not exception safe.
  RtecEventComm::PushConsumer_ptr const rtecpushconsumer =
    auto_wrapper.get ()->_this ();

  // give the ownership to the POA.
  auto_wrapper.get ()->_remove_ref ();

  this->pps_->connect_push_consumer (rtecpushconsumer,
                                     this->qos_);

  this->wrapper_ = auto_wrapper.release ();
}

bool
TAO_CosEC_ProxyPushSupplier_i::connected () const
{
  return this->wrapper_ != nullptr;
}

