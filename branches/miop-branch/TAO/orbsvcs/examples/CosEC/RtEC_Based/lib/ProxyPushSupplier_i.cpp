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
  public POA_RtecEventComm::PushConsumer,
  public virtual PortableServer::RefCountServantBase
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

  virtual void push (const RtecEventComm::EventSet & data
                     TAO_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));
  // This method is called by the RTEvent Channel to supply data.

  virtual void disconnect_push_consumer (TAO_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));
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
TAO_CosEC_PushConsumerWrapper::push (const RtecEventComm::EventSet& set
                                     TAO_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException))
{
  for (CORBA::ULong i = 0;
       i < set.length ();
       ++i)
    {
      ACE_TRY
        {
          this->consumer_->push (set[i].data.any_value
                                 TAO_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      ACE_CATCHANY
        {
          // Ignore the exception...
        }
      ACE_ENDTRY;
    }
}

void
TAO_CosEC_PushConsumerWrapper::disconnect_push_consumer (TAO_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Deactivate the supplier proxy.
  this->consumer_->disconnect_push_consumer (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  PortableServer::POA_var poa =
    this->_default_POA (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  PortableServer::ObjectId_var id =
    poa->servant_to_id (this
                        TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  poa->deactivate_object (id.in ()
                          TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

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
TAO_CosEC_ProxyPushSupplier_i::disconnect_push_supplier (TAO_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->pps_->disconnect_push_supplier (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // Deactivate the supplier proxy
  PortableServer::POA_var poa =
    this->_default_POA (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  PortableServer::ObjectId_var id =
    poa->servant_to_id (this
                        TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  poa->deactivate_object (id.in ()
                          TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // @@ If we keep a list remember to remove this object from the
  // list.
}

void
TAO_CosEC_ProxyPushSupplier_i::connect_push_consumer (CosEventComm::PushConsumer_ptr push_consumer
                                                      TAO_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       CosEventChannelAdmin::AlreadyConnected,
                       CosEventChannelAdmin::TypeError))
{
  if (this->connected ())
    ACE_THROW (CosEventChannelAdmin::AlreadyConnected ());

  if (push_consumer == CosEventComm::PushConsumer::_nil())
    ACE_THROW (CORBA::BAD_PARAM ());

  TAO_CosEC_PushConsumerWrapper *wrapper;
  ACE_NEW_THROW_EX (wrapper,
                    TAO_CosEC_PushConsumerWrapper (push_consumer),
                    CORBA::NO_MEMORY ());
  ACE_CHECK;

  auto_ptr <TAO_CosEC_PushConsumerWrapper> auto_wrapper (wrapper);

  // @@ This code is not exception safe.
  RtecEventComm::PushConsumer_ptr  rtecpushconsumer =
    auto_wrapper.get ()->_this (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // give the ownership to the POA.
  auto_wrapper.get ()->_remove_ref (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->pps_->connect_push_consumer (rtecpushconsumer,
                                     this->qos_
                                     TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->wrapper_ = auto_wrapper.release ();
}

int
TAO_CosEC_ProxyPushSupplier_i::connected (void)
{
  return this->wrapper_ == 0 ? 0 : 1;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
  template class ACE_Auto_Basic_Ptr<TAO_CosEC_PushConsumerWrapper>;
  template class auto_ptr<TAO_CosEC_PushConsumerWrapper>;
#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
# pragma instantiate ACE_Auto_Basic_Ptr<TAO_CosEC_PushConsumerWrapper>
# pragma instantiate auto_ptr<TAO_CosEC_PushConsumerWrapper>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
