 // $Id$

#include "StructuredProxyPushSupplier.h"
#include "tao/PortableServer/Servant_Base.h"

#include "StructuredPushConsumer.h"
#include "../Event_Manager.h"
#include "../Admin.h"

#if ! defined (__ACE_INLINE__)
#include "StructuredProxyPushSupplier.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(RT_Notify, TAO_NS_StructuredProxyPushSupplier, "$Id$")

TAO_NS_StructuredProxyPushSupplier::TAO_NS_StructuredProxyPushSupplier (void)
{
}

TAO_NS_StructuredProxyPushSupplier::~TAO_NS_StructuredProxyPushSupplier ()
{
}

void
TAO_NS_StructuredProxyPushSupplier::servant (PortableServer::Servant servant)
{
  servant_ = servant;
}

PortableServer::Servant
TAO_NS_StructuredProxyPushSupplier::servant (void)
{
  return servant_;
}

void TAO_NS_StructuredProxyPushSupplier::connect_structured_push_consumer (
    CosNotifyComm::StructuredPushConsumer_ptr push_consumer ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
    , CosEventChannelAdmin::AlreadyConnected
    , CosEventChannelAdmin::TypeError
  ))
{
    // Convert Consumer to Base Type
  TAO_NS_StructuredPushConsumer* consumer;
  ACE_NEW_THROW_EX (consumer,
                    TAO_NS_StructuredPushConsumer (push_consumer),
                    CORBA::NO_MEMORY ());

  consumer->destroy_callback (consumer);

  this->connect (consumer ACE_ENV_ARG_PARAMETER);
}

  
void TAO_NS_StructuredProxyPushSupplier::subscription_change (
							      const CosNotification::EventTypeSeq & added,
							      const CosNotification::EventTypeSeq & removed
							      ACE_ENV_ARG_DECL
							      )
  ACE_THROW_SPEC ((
		   CORBA::SystemException
		   , CosNotifyComm::InvalidEventType
		   ))

{
  TAO_NS_EventTypeSeq seq_added (added);
  TAO_NS_EventTypeSeq seq_removed (removed);

  ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, ace_mon, this->lock_,
		      CORBA::INTERNAL ());
  ACE_CHECK;
  
  this->preprocess (seq_added, seq_removed);
  
  if (this->is_connected () == 1)
    {
      event_manager_->subscribe (this, seq_added);
      event_manager_->un_subscribe (this, seq_removed);
    }
}

void TAO_NS_StructuredProxyPushSupplier::disconnect_structured_push_supplier (
    
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))

  {
    this->inherited::destroy (this ACE_ENV_ARG_PARAMETER);
  }
  
void TAO_NS_StructuredProxyPushSupplier::suspend_connection (
    
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
    , CosNotifyChannelAdmin::ConnectionAlreadyInactive
    , CosNotifyChannelAdmin::NotConnected
  ))

  {
    //Add your implementation here
  }
  
void TAO_NS_StructuredProxyPushSupplier::resume_connection (
    
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
    , CosNotifyChannelAdmin::ConnectionAlreadyActive
    , CosNotifyChannelAdmin::NotConnected
  ))

  {
    //Add your implementation here
  }
  
::CosNotifyChannelAdmin::ProxyType TAO_NS_StructuredProxyPushSupplier::MyType (
    
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))

  {
    //Add your implementation here
	return ::CosNotifyChannelAdmin::PUSH_STRUCTURED;
  }
  
::CosNotifyChannelAdmin::ConsumerAdmin_ptr TAO_NS_StructuredProxyPushSupplier::MyAdmin (
    
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))

  {
    //Add your implementation here
	return 0;
  }
  
::CosNotifyFilter::MappingFilter_ptr TAO_NS_StructuredProxyPushSupplier::priority_filter (
    
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))

  {
    //Add your implementation here
	return 0;
  }
  
void TAO_NS_StructuredProxyPushSupplier::priority_filter (
    CosNotifyFilter::MappingFilter_ptr priority_filter
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))

  {
    //Add your implementation here
  }
  
::CosNotifyFilter::MappingFilter_ptr TAO_NS_StructuredProxyPushSupplier::lifetime_filter (
    
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))

  {
    //Add your implementation here
	return 0;
  }
  
void TAO_NS_StructuredProxyPushSupplier::lifetime_filter (
    CosNotifyFilter::MappingFilter_ptr lifetime_filter
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))

  {
    //Add your implementation here
  }
  
::CosNotification::EventTypeSeq * TAO_NS_StructuredProxyPushSupplier::obtain_offered_types (
    CosNotifyChannelAdmin::ObtainInfoMode mode
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))

  {
    //Add your implementation here
	return 0;
  }
  
void TAO_NS_StructuredProxyPushSupplier::validate_event_qos (
    const CosNotification::QoSProperties & required_qos,
    CosNotification::NamedPropertyRangeSeq_out available_qos
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
    , CosNotification::UnsupportedQoS
  ))

  {
    //Add your implementation here
  }
    
::CosNotification::QoSProperties * TAO_NS_StructuredProxyPushSupplier::get_qos (
    
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))

  {
    //Add your implementation here
	return 0;
  }
  
void TAO_NS_StructuredProxyPushSupplier::set_qos (
    const CosNotification::QoSProperties & qos
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
    , CosNotification::UnsupportedQoS
  ))

  {
    //Add your implementation here
  }
  
void TAO_NS_StructuredProxyPushSupplier::validate_qos (
    const CosNotification::QoSProperties & required_qos,
    CosNotification::NamedPropertyRangeSeq_out available_qos
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
    , CosNotification::UnsupportedQoS
  ))

  {
    //Add your implementation here
  }
  
CosNotifyFilter::FilterID TAO_NS_StructuredProxyPushSupplier::add_filter (
    CosNotifyFilter::Filter_ptr new_filter
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))

  {
    //Add your implementation here
	return 0;
  }
  
void TAO_NS_StructuredProxyPushSupplier::remove_filter (
    CosNotifyFilter::FilterID filter
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
    , CosNotifyFilter::FilterNotFound
  ))

  {
    //Add your implementation here
  }
  
::CosNotifyFilter::Filter_ptr TAO_NS_StructuredProxyPushSupplier::get_filter (
    CosNotifyFilter::FilterID filter
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
    , CosNotifyFilter::FilterNotFound
  ))

  {
    //Add your implementation here
	return 0;
  }
  
::CosNotifyFilter::FilterIDSeq * TAO_NS_StructuredProxyPushSupplier::get_all_filters (
    
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))

  {
    //Add your implementation here
	return 0;
  }
  
void TAO_NS_StructuredProxyPushSupplier::remove_all_filters (
    
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))

  {
    //Add your implementation here
  }
