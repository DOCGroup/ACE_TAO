/* -*- C++ -*- */
/**
 *  @file Proxy_T.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_NS_PROXY_T_H
#define TAO_NS_PROXY_T_H
#include "ace/pre.h"

#include "notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Proxy.h"

/**
 * @class TAO_NS_Proxy_T
 *
 * @brief The is a base class for all proxys , templatized by the servant
 * type. All the Filter Admin and QoS Admin interface methods are
 * implemented here by delegating to the admin implementations.
 *
 */
template <class SERVANT_TYPE>
class TAO_NS_Proxy_T : public SERVANT_TYPE, public virtual TAO_NS_Proxy
{
public:
  /// Constuctor
  TAO_NS_Proxy_T (void);

  /// Destructor
  ~TAO_NS_Proxy_T ();

  /// Implements TAO_NS_Object::servant method.
  virtual PortableServer::Servant servant (void);

  /// ServantBase refcount methods.
  virtual void _add_ref (ACE_ENV_SINGLE_ARG_DECL);
  virtual void _remove_ref (ACE_ENV_SINGLE_ARG_DECL);

  virtual void validate_event_qos (
    const CosNotification::QoSProperties & required_qos,
    CosNotification::NamedPropertyRangeSeq_out available_qos
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotification::UnsupportedQoS
  ));

 virtual CosNotification::QoSProperties * get_qos (
    ACE_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

  virtual void set_qos (
    const CosNotification::QoSProperties & qos
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotification::UnsupportedQoS
  ));

  virtual void validate_qos (
    const CosNotification::QoSProperties & required_qos,
    CosNotification::NamedPropertyRangeSeq_out available_qos
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotification::UnsupportedQoS
  ));

  virtual CosNotifyFilter::FilterID add_filter (
    CosNotifyFilter::Filter_ptr new_filter
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

  virtual void remove_filter (
    CosNotifyFilter::FilterID filter
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyFilter::FilterNotFound
  ));

  virtual CosNotifyFilter::Filter_ptr get_filter (
    CosNotifyFilter::FilterID filter
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyFilter::FilterNotFound
  ));

  virtual CosNotifyFilter::FilterIDSeq * get_all_filters (
    ACE_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

  virtual void remove_all_filters (
    ACE_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));
};

#if defined ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION_EXPORT
template class TAO_Notify_Export
TAO_NS_Proxy_T<POA_Event_Forwarder::StructuredProxyPushSupplier>;
template class TAO_Notify_Export
TAO_NS_Proxy_T<POA_CosNotifyChannelAdmin::SequenceProxyPushSupplier>;
template class TAO_Notify_Export
TAO_NS_Proxy_T<POA_Event_Forwarder::ProxyPushSupplier>;
template class TAO_Notify_Export
TAO_NS_Proxy_T<POA_CosEventChannelAdmin::ProxyPushSupplier>;
template class TAO_Notify_Export
TAO_NS_Proxy_T<POA_CosNotifyChannelAdmin::StructuredProxyPushConsumer>;
template class TAO_Notify_Export
TAO_NS_Proxy_T<POA_CosNotifyChannelAdmin::SequenceProxyPushConsumer>;
template class TAO_Notify_Export
TAO_NS_Proxy_T<POA_CosNotifyChannelAdmin::ProxyPushConsumer>;
template class TAO_Notify_Export
TAO_NS_Proxy_T<POA_CosEventChannelAdmin::ProxyPushConsumer>;
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION_EXPORT */

#if defined (__ACE_INLINE__)
#include "Proxy_T.inl"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Proxy_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Proxy_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include "ace/post.h"
#endif /* TAO_NS_PROXY_T_H */
