// -*- C++ -*-

/**
 *  @file Proxy_T.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_Notify_PROXY_T_H
#define TAO_Notify_PROXY_T_H

#include /**/ "ace/pre.h"

#include "orbsvcs/Notify/notify_serv_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/Notify/Proxy.h"


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_Notify_Proxy_T
 *
 * @brief The is a base class for all proxys , templatized by the servant
 * type. All the Filter Admin and QoS Admin interface methods are
 * implemented here by delegating to the admin implementations.
 *
 */
template <class SERVANT_TYPE>
class TAO_Notify_Proxy_T : public SERVANT_TYPE, public virtual TAO_Notify_Proxy
{
public:
  /// Constuctor
  TAO_Notify_Proxy_T (void);

  /// Destructor
  ~TAO_Notify_Proxy_T ();

  /// Implements TAO_Notify_Object::servant method.
  virtual PortableServer::Servant servant (void);

  /// ServantBase refcount methods.
  virtual void _add_ref (void);
  virtual void _remove_ref (void);

  virtual void validate_event_qos (
    const CosNotification::QoSProperties & required_qos,
    CosNotification::NamedPropertyRangeSeq_out available_qos
  );

 virtual CosNotification::QoSProperties * get_qos (
  );

  virtual void set_qos (
    const CosNotification::QoSProperties & qos
  );

  virtual void validate_qos (
    const CosNotification::QoSProperties & required_qos,
    CosNotification::NamedPropertyRangeSeq_out available_qos
  );

  virtual CosNotifyFilter::FilterID add_filter (
    CosNotifyFilter::Filter_ptr new_filter
  );

  virtual void remove_filter (
    CosNotifyFilter::FilterID filter
  );

  virtual CosNotifyFilter::Filter_ptr get_filter (
    CosNotifyFilter::FilterID filter
  );

  virtual CosNotifyFilter::FilterIDSeq * get_all_filters (
  );

  virtual void remove_all_filters (
  );
};

#if defined ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION_EXPORT
template class TAO_Notify_Serv_Export
TAO_Notify_Proxy_T<POA_Event_Forwarder::StructuredProxyPushSupplier>;
template class TAO_Notify_Serv_Export
TAO_Notify_Proxy_T<POA_CosNotifyChannelAdmin::SequenceProxyPushSupplier>;
template class TAO_Notify_Serv_Export
TAO_Notify_Proxy_T<POA_Event_Forwarder::ProxyPushSupplier>;
template class TAO_Notify_Serv_Export
TAO_Notify_Proxy_T<POA_CosEventChannelAdmin::ProxyPushSupplier>;
template class TAO_Notify_Serv_Export
TAO_Notify_Proxy_T<POA_CosNotifyChannelAdmin::StructuredProxyPushConsumer>;
template class TAO_Notify_Serv_Export
TAO_Notify_Proxy_T<POA_CosNotifyChannelAdmin::SequenceProxyPushConsumer>;
template class TAO_Notify_Serv_Export
TAO_Notify_Proxy_T<POA_CosNotifyChannelAdmin::ProxyPushConsumer>;
template class TAO_Notify_Serv_Export
TAO_Notify_Proxy_T<POA_CosEventChannelAdmin::ProxyPushConsumer>;
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION_EXPORT */

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "orbsvcs/Notify/Proxy_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Proxy_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"
#endif /* TAO_Notify_PROXY_T_H */
