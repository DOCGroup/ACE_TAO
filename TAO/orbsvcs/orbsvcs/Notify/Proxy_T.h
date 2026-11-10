// -*- C++ -*-

/**
 *  @file Proxy_T.h
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
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
 */
template <class SERVANT_TYPE>
class TAO_Notify_Proxy_T : public SERVANT_TYPE, public virtual TAO_Notify_Proxy
{
public:
  TAO_Notify_Proxy_T ();

  ~TAO_Notify_Proxy_T ();

  /// Implements TAO_Notify_Object::servant method.
  virtual PortableServer::Servant servant ();

  /// ServantBase refcount methods.
  void _add_ref () override;
  void _remove_ref () override;

  void validate_event_qos (
    CosNotification::QoSProperties const &required_qos,
    CosNotification::NamedPropertyRangeSeq_out available_qos) override;

  CosNotification::QoSProperties *get_qos () override;

  void set_qos (CosNotification::QoSProperties const &qos) override;

  void validate_qos (
    CosNotification::QoSProperties const &required_qos,
    CosNotification::NamedPropertyRangeSeq_out available_qos) override;

  CosNotifyFilter::FilterID add_filter (CosNotifyFilter::Filter_ptr new_filter) override;

  void remove_filter (CosNotifyFilter::FilterID filter) override;

  CosNotifyFilter::Filter_ptr get_filter (CosNotifyFilter::FilterID filter) override;

  CosNotifyFilter::FilterIDSeq *get_all_filters () override;

  void remove_all_filters () override;
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

#include "orbsvcs/Notify/Proxy_T.cpp"

#include /**/ "ace/post.h"
#endif /* TAO_Notify_PROXY_T_H */
