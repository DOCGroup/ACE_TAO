// $Id$

#include "EventChannelFactory.h"

#if ! defined (__ACE_INLINE__)
#include "EventChannelFactory.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Notify, TAO_NS_EventChannelFactory, "$Id$")

#include "ace/Dynamic_Service.h"
#include "Properties.h"
#include "Factory.h"
#include "Builder.h"
#include "EventChannel.h"
#include "Container_T.h"
#include "Find_Worker_T.h"
#include "Seq_Worker_T.h"

typedef TAO_NS_Find_Worker_T<TAO_NS_EventChannel
                             , CosNotifyChannelAdmin::EventChannel
                             , CosNotifyChannelAdmin::EventChannel_ptr
                             , CosNotifyChannelAdmin::ChannelNotFound>
TAO_NS_EventChannel_Find_Worker;

typedef TAO_NS_Seq_Worker_T<TAO_NS_EventChannel> TAO_NS_EventChannel_Seq_Worker;

TAO_NS_EventChannelFactory::TAO_NS_EventChannelFactory (void)
  :ec_container_ (0)
{
}

TAO_NS_EventChannelFactory::~TAO_NS_EventChannelFactory ()
{
}

void
TAO_NS_EventChannelFactory::destroy (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  if (this->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER) == 1)
    return;

  TAO_NS_Properties* properties = TAO_NS_PROPERTIES::instance();

  delete this->ec_container_;

  // Shutdown the ORB.
  CORBA::ORB_var orb = properties->orb ();
  orb->shutdown ();

  // Reset references to CORBA objects.
  properties->orb (CORBA::ORB::_nil ());
  properties->default_poa (PortableServer::POA::_nil ());
}

void
TAO_NS_EventChannelFactory::init (PortableServer::POA_ptr poa ACE_ENV_ARG_DECL)
{
  this->default_filter_factory_ =
    TAO_NS_PROPERTIES::instance()->builder()->build_filter_factory (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // Init ec_container_
  ACE_NEW_THROW_EX (this->ec_container_,
                    TAO_NS_EventChannel_Container (),
                    CORBA::INTERNAL ());
  ACE_CHECK;

  this->ec_container_->init (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  TAO_NS_POA_Helper* object_poa = 0;

  // Bootstrap initial Object POA
  ACE_NEW_THROW_EX (object_poa,
                    TAO_NS_POA_Helper (),
                    CORBA::NO_MEMORY ());
  ACE_CHECK;

  auto_ptr<TAO_NS_POA_Helper> auto_object_poa (object_poa);

  object_poa->init (poa ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // release auto_ref.
  auto_object_poa.release ();

  this->object_poa_own (object_poa);

  // We are also activated in the same Object POA.
  this->poa_ = this->object_poa_;

  // Make the Proxys acivate in this same POA.
  this->proxy_poa_ = this->object_poa_;
}

void
TAO_NS_EventChannelFactory::_add_ref (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  this->_incr_refcnt ();
}

void
TAO_NS_EventChannelFactory::_remove_ref (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  this->_decr_refcnt ();
}

void
TAO_NS_EventChannelFactory::release (void)
{
  delete this;
  //@@ inform factory
}

void
TAO_NS_EventChannelFactory::remove (TAO_NS_EventChannel* event_channel ACE_ENV_ARG_DECL)
{
  this->ec_container_->remove (event_channel ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

int
TAO_NS_EventChannelFactory::shutdown (ACE_ENV_SINGLE_ARG_DECL)
{
  if (TAO_NS_Object::shutdown (ACE_ENV_SINGLE_ARG_PARAMETER) == 1)
    return 1;

  this->ec_container_->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (1);

  return 0;
}

CosNotifyFilter::FilterFactory_ptr
TAO_NS_EventChannelFactory::get_default_filter_factory (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  return CosNotifyFilter::FilterFactory::_duplicate (this->default_filter_factory_.in ());
}

::CosNotifyChannelAdmin::EventChannel_ptr TAO_NS_EventChannelFactory::create_channel (
    const CosNotification::QoSProperties & initial_qos,
    const CosNotification::AdminProperties & initial_admin,
    CosNotifyChannelAdmin::ChannelID_out id ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , CosNotification::UnsupportedQoS
                   , CosNotification::UnsupportedAdmin
                   ))
{
  return TAO_NS_PROPERTIES::instance()->builder()->build_event_channel (this
                                                                        , initial_qos
                                                                        , initial_admin
                                                                        , id
                                                                        ACE_ENV_ARG_PARAMETER);
}

CosNotifyChannelAdmin::ChannelIDSeq*
TAO_NS_EventChannelFactory::get_all_channels (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
  TAO_NS_EventChannel_Seq_Worker seq_worker;

  return seq_worker.create (*this->ec_container_ ACE_ENV_ARG_PARAMETER);
}

CosNotifyChannelAdmin::EventChannel_ptr
TAO_NS_EventChannelFactory::get_event_channel (CosNotifyChannelAdmin::ChannelID id ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , CosNotifyChannelAdmin::ChannelNotFound
                   ))
{
  TAO_NS_EventChannel_Find_Worker find_worker;

  return find_worker.resolve (id, *this->ec_container_ ACE_ENV_ARG_PARAMETER);
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class TAO_NS_Find_Worker_T<TAO_NS_EventChannel
                              , CosNotifyChannelAdmin::EventChannel
                              , CosNotifyChannelAdmin::EventChannel_ptr
                              , CosNotifyChannelAdmin::ChannelNotFound>;
template class TAO_NS_Seq_Worker_T<TAO_NS_EventChannel>;

template class TAO_NS_Container_T <TAO_NS_EventChannel>;

template class TAO_ESF_Shutdown_Proxy<TAO_NS_EventChannel>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate TAO_NS_Find_Worker_T<TAO_NS_EventChannel
                              , CosNotifyChannelAdmin::EventChannel
                              , CosNotifyChannelAdmin::EventChannel_ptr
                              , CosNotifyChannelAdmin::ChannelNotFound>
#pragma instantiate TAO_NS_Seq_Worker_T<TAO_NS_EventChannel>


#pragma instantiate TAO_NS_Container_T <TAO_NS_EventChannel>

#pragma instantiate TAO_ESF_Shutdown_Proxy<TAO_NS_EventChannel>;

#endif /*ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
