// $Id$

#include "SupplierAdmin.h"
#include "Builder.h"
#include "Proxy.h"
#include "EventChannel.h"
#include "orbsvcs/ESF/ESF_Proxy_Collection.h"

#if ! defined (__ACE_INLINE__)
#include "SupplierAdmin.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(RT_Notify, TAO_NS_SupplierAdmin, "$Id$")

TAO_NS_SupplierAdmin::TAO_NS_SupplierAdmin (void)
{
}

TAO_NS_SupplierAdmin::~TAO_NS_SupplierAdmin ()
{
}


PortableServer::Servant
TAO_NS_SupplierAdmin::servant (void)
{
  return this;
}

void
TAO_NS_SupplierAdmin::_add_ref (ACE_ENV_SINGLE_ARG_DECL)
{
  this->_incr_refcnt ();
}

void
TAO_NS_SupplierAdmin::_remove_ref (ACE_ENV_SINGLE_ARG_DECL)
{
  this->_decr_refcnt ();
}

void
TAO_NS_SupplierAdmin::release (void)
{
  delete this;
  //@@ inform factory
}

void
TAO_NS_SupplierAdmin::set (CosNotifyChannelAdmin::InterFilterGroupOperator op ACE_ENV_ARG_DECL)
{
}

void 
TAO_NS_SupplierAdmin::destroy (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  this->inherited::destroy (this ACE_ENV_ARG_PARAMETER);
}

CosNotifyChannelAdmin::ProxyConsumer_ptr
TAO_NS_SupplierAdmin::obtain_notification_push_consumer (CosNotifyChannelAdmin::ClientType ctype, CosNotifyChannelAdmin::ProxyID_out proxy_id  ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , CosNotifyChannelAdmin::AdminLimitExceeded
                   ))
  
{
  return TAO_NS_PROPERTIES::instance()->builder ()->build_notification_push_consumer (this, 
										      ctype, 
										      proxy_id ACE_ENV_ARG_PARAMETER);
}

CosNotifyChannelAdmin::AdminID TAO_NS_SupplierAdmin::MyID (
    
  )
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))

{
  return this->id ();
}
  
::CosNotifyChannelAdmin::EventChannel_ptr TAO_NS_SupplierAdmin::MyChannel (
    
  )
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))

{
  //Add your implementation here
	return 0;
}
  
::CosNotifyChannelAdmin::InterFilterGroupOperator TAO_NS_SupplierAdmin::MyOperator (
    
  )
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))

{
	return ::CosNotifyChannelAdmin::OR_OP;

  //Add your implementation here
}
  
::CosNotifyChannelAdmin::ProxyIDSeq * TAO_NS_SupplierAdmin::pull_consumers (
    
  )
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))

{
  //Add your implementation here
	return 0;
}
  
::CosNotifyChannelAdmin::ProxyIDSeq * TAO_NS_SupplierAdmin::push_consumers (
    
  )
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))

{
  //Add your implementation here
	return 0;
}
  
::CosNotifyChannelAdmin::ProxyConsumer_ptr TAO_NS_SupplierAdmin::get_proxy_consumer (
                                                                                                      CosNotifyChannelAdmin::ProxyID proxy_id
                                                                                                      )
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , CosNotifyChannelAdmin::ProxyNotFound
                   ))

{
  //Add your implementation here
	return 0;
}
  
::CosNotifyChannelAdmin::ProxyConsumer_ptr TAO_NS_SupplierAdmin::obtain_notification_pull_consumer (
                                                                                                                     CosNotifyChannelAdmin::ClientType ctype,
                                                                                                                     CosNotifyChannelAdmin::ProxyID_out proxy_id
                                                                                                                     )
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , CosNotifyChannelAdmin::AdminLimitExceeded
                   ))

{
  //Add your implementation here
	return 0;
}
  
::CosNotification::QoSProperties * TAO_NS_SupplierAdmin::get_qos (
    
  )
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))

{
  //Add your implementation here
	return 0;
}
  
void TAO_NS_SupplierAdmin::set_qos (
                                                     const CosNotification::QoSProperties & qos
                                                     )
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , CosNotification::UnsupportedQoS
                   ))

{
  //Add your implementation here
}
  
void TAO_NS_SupplierAdmin::validate_qos (
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
  
void TAO_NS_SupplierAdmin::offer_change (
                                                          const CosNotification::EventTypeSeq & added,
                                                          const CosNotification::EventTypeSeq & removed
                                                          )
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , CosNotifyComm::InvalidEventType
                   ))

{
  //Add your implementation here
}
  
CosNotifyFilter::FilterID TAO_NS_SupplierAdmin::add_filter (
                                                                             CosNotifyFilter::Filter_ptr new_filter
                                                                             )
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))

{
  //Add your implementation here
	return 0;
}
  
void TAO_NS_SupplierAdmin::remove_filter (
                                                           CosNotifyFilter::FilterID filter
                                                           )
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , CosNotifyFilter::FilterNotFound
                   ))

{
  //Add your implementation here
}
  
::CosNotifyFilter::Filter_ptr TAO_NS_SupplierAdmin::get_filter (
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
  
::CosNotifyFilter::FilterIDSeq * TAO_NS_SupplierAdmin::get_all_filters (
    
  )
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))

{
  //Add your implementation here
	return 0;
}
  
void TAO_NS_SupplierAdmin::remove_all_filters (
    
  )
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))

{
  //Add your implementation here
}
  
::CosEventChannelAdmin::ProxyPushConsumer_ptr TAO_NS_SupplierAdmin::obtain_push_consumer (
    
  )
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))

{
  //Add your implementation here
	return 0;
}
  
::CosEventChannelAdmin::ProxyPullConsumer_ptr TAO_NS_SupplierAdmin::obtain_pull_consumer (
    
  )
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))

{
  //Add your implementation here
	return 0;
}

