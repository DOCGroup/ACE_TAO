/* -*- C++ -*- */
/**
 *  @file StructuredProxyPushSupplier.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_NS_STRUCTUREDPROXYPUSHSUPPLIER_H
#define TAO_NS_STRUCTUREDPROXYPUSHSUPPLIER_H
#include "ace/pre.h"

#include "../notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableServer/PortableServerC.h"
#include "orbsvcs/CosNotifyChannelAdminS.h"
#include "../ProxySupplier.h"

/**
 * @class TAO_NS_StructuredProxyPushSupplier
 *
 * @brief Implements the CosNotifyChannelAdmin::StructuredProxyPushSupplier methods. However does not derive from it. This class is to be used by specific Tie implementations of the Notify ProxySupplier interface.
 *
 */
class TAO_Notify_Export TAO_NS_StructuredProxyPushSupplier : public virtual TAO_NS_ProxySupplier
{
  friend class TAO_NS_Builder;

public:
  /// Constuctor
  TAO_NS_StructuredProxyPushSupplier (void);

  /// Destructor
  ~TAO_NS_StructuredProxyPushSupplier ();  

  /// Set the servant.
  void servant (PortableServer::Servant servant);

  /// Return servant
  virtual PortableServer::Servant servant (void);

  /// = Servant methods
  virtual void connect_structured_push_consumer (
    CosNotifyComm::StructuredPushConsumer_ptr push_consumer ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
    , CosEventChannelAdmin::AlreadyConnected
    , CosEventChannelAdmin::TypeError
  ));

virtual void suspend_connection (
    
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
    , CosNotifyChannelAdmin::ConnectionAlreadyInactive
    , CosNotifyChannelAdmin::NotConnected
  ));

virtual void resume_connection (
    
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
    , CosNotifyChannelAdmin::ConnectionAlreadyActive
    , CosNotifyChannelAdmin::NotConnected
  ));

virtual ::CosNotifyChannelAdmin::ProxyType MyType (
    
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual ::CosNotifyChannelAdmin::ConsumerAdmin_ptr MyAdmin (
    
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual ::CosNotifyFilter::MappingFilter_ptr priority_filter (
    
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual void priority_filter (
    CosNotifyFilter::MappingFilter_ptr priority_filter
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual ::CosNotifyFilter::MappingFilter_ptr lifetime_filter (
    
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual void lifetime_filter (
    CosNotifyFilter::MappingFilter_ptr lifetime_filter
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual ::CosNotification::EventTypeSeq * obtain_offered_types (
    CosNotifyChannelAdmin::ObtainInfoMode mode
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual void validate_event_qos (
    const CosNotification::QoSProperties & required_qos,
    CosNotification::NamedPropertyRangeSeq_out available_qos
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
    , CosNotification::UnsupportedQoS
  ));

virtual void disconnect_structured_push_supplier (
    
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual ::CosNotification::QoSProperties * get_qos (
    
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual void set_qos (
    const CosNotification::QoSProperties & qos
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
    , CosNotification::UnsupportedQoS
  ));

virtual void validate_qos (
    const CosNotification::QoSProperties & required_qos,
    CosNotification::NamedPropertyRangeSeq_out available_qos
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
    , CosNotification::UnsupportedQoS
  ));

virtual CosNotifyFilter::FilterID add_filter (
    CosNotifyFilter::Filter_ptr new_filter
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual void remove_filter (
    CosNotifyFilter::FilterID filter
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
    , CosNotifyFilter::FilterNotFound
  ));

virtual ::CosNotifyFilter::Filter_ptr get_filter (
    CosNotifyFilter::FilterID filter
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
    , CosNotifyFilter::FilterNotFound
  ));

virtual ::CosNotifyFilter::FilterIDSeq * get_all_filters (
    
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual void remove_all_filters (
    
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual void subscription_change (
    const CosNotification::EventTypeSeq & added,
    const CosNotification::EventTypeSeq & removed
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
    , CosNotifyComm::InvalidEventType
  ));

protected:
  PortableServer::Servant servant_;
};

#if defined (__ACE_INLINE__)
#include "StructuredProxyPushSupplier.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NS_STRUCTUREDPROXYPUSHSUPPLIER_H */
