// $Id$

#include "StructuredProxyPushConsumer.h"
#include "ace/Refcounted_Auto_Ptr.h"
#include "StructuredPushSupplier.h"
#include "StructuredEvent.h"
#include "../Admin.h"

#if ! defined (__ACE_INLINE__)
#include "StructuredProxyPushConsumer.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(RT_Notify, TAO_NS_StructuredProxyPushConsumer, "$Id$")

TAO_NS_StructuredProxyPushConsumer::TAO_NS_StructuredProxyPushConsumer (void)
{
}

TAO_NS_StructuredProxyPushConsumer::~TAO_NS_StructuredProxyPushConsumer ()
{

}

PortableServer::Servant
TAO_NS_StructuredProxyPushConsumer::servant (void)
{
  return this;
}

void
TAO_NS_StructuredProxyPushConsumer::_add_ref (ACE_ENV_SINGLE_ARG_DECL)
{
  this->_incr_refcnt ();
}

void
TAO_NS_StructuredProxyPushConsumer::_remove_ref (ACE_ENV_SINGLE_ARG_DECL)
{
  this->_decr_refcnt ();
}

void
TAO_NS_StructuredProxyPushConsumer::release (void)
{
  delete this;
  //@@ inform factory
}

void 
TAO_NS_StructuredProxyPushConsumer::connect_structured_push_supplier (CosNotifyComm::StructuredPushSupplier_ptr push_supplier ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , CosEventChannelAdmin::AlreadyConnected
                   ))
{
  // Convert Supplier to Base Type
  TAO_NS_StructuredPushSupplier * supplier;
  ACE_NEW_THROW_EX (supplier,
                    TAO_NS_StructuredPushSupplier (push_supplier),
                    CORBA::NO_MEMORY ());
 
  supplier->destroy_callback (supplier);

  this->connect (supplier ACE_ENV_ARG_PARAMETER);
}

  
void
TAO_NS_StructuredProxyPushConsumer::push_structured_event (const CosNotification::StructuredEvent & notification ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , CosEventComm::Disconnected
                   ))
{
  // Convert 
  TAO_NS_Event_var event (new TAO_NS_StructuredEvent (notification)); 
  //this->proxy ()->push (event);
  this->push (event);
}
  
  
::CosNotifyChannelAdmin::ProxyType TAO_NS_StructuredProxyPushConsumer::MyType (
    
  )
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))

{
  //Add your implementation here
  return ::CosNotifyChannelAdmin::PUSH_STRUCTURED;
}
  
::CosNotifyChannelAdmin::SupplierAdmin_ptr TAO_NS_StructuredProxyPushConsumer::MyAdmin (
    
  )
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))

{
  //Add your implementation here
	return 0;
}
  
::CosNotification::EventTypeSeq * TAO_NS_StructuredProxyPushConsumer::obtain_subscription_types (
                                                                                                                  CosNotifyChannelAdmin::ObtainInfoMode mode
                                                                                                                  )
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))

{
  //Add your implementation here
	return 0;
}
  
void TAO_NS_StructuredProxyPushConsumer::validate_event_qos (
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

void
TAO_NS_StructuredProxyPushConsumer::disconnect_structured_push_consumer (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
	this->inherited::destroy (this ACE_ENV_ARG_PARAMETER);
}
  
::CosNotification::QoSProperties * TAO_NS_StructuredProxyPushConsumer::get_qos (
    
  )
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))

{
  //Add your implementation here
	return 0;
}
  
void TAO_NS_StructuredProxyPushConsumer::set_qos (
                                                                   const CosNotification::QoSProperties & qos
                                                                   )
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , CosNotification::UnsupportedQoS
                   ))

{
  //Add your implementation here
}
  
void TAO_NS_StructuredProxyPushConsumer::validate_qos (
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
  
CosNotifyFilter::FilterID TAO_NS_StructuredProxyPushConsumer::add_filter (
                                                                                           CosNotifyFilter::Filter_ptr new_filter
                                                                                           )
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))

{
  //Add your implementation here
	return 0;
}
  
void TAO_NS_StructuredProxyPushConsumer::remove_filter (
                                                                         CosNotifyFilter::FilterID filter
                                                                         )
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , CosNotifyFilter::FilterNotFound
                   ))

{
  //Add your implementation here
}
  
::CosNotifyFilter::Filter_ptr TAO_NS_StructuredProxyPushConsumer::get_filter (
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
  
::CosNotifyFilter::FilterIDSeq * TAO_NS_StructuredProxyPushConsumer::get_all_filters (
    
  )
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))

{
  //Add your implementation here
  return 0;
}
  
void TAO_NS_StructuredProxyPushConsumer::remove_all_filters (
    
  )
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))

{
  //Add your implementation here
}
  
void TAO_NS_StructuredProxyPushConsumer::offer_change (
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

