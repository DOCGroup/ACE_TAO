/* -*- C++ -*- */
/**
 *  @file Builder.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_Notify_BUILDER_H
#define TAO_Notify_BUILDER_H
#include /**/ "ace/pre.h"

#include "notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableServer/PortableServer.h"
#include "orbsvcs/CosNotifyChannelAdminC.h"
#include "orbsvcs/NotifyExtC.h"
#include "AdminProperties.h"

class TAO_Notify_EventChannelFactory;
class TAO_Notify_EventChannel;
class TAO_Notify_SupplierAdmin;
class TAO_Notify_ConsumerAdmin;
class TAO_Notify_FilterFactory;
class TAO_Notify_Object;

/**
 * @class TAO_Notify_Builder
 *
 * @brief Helper class to create and activate CORBA objects.
 *
 */
class TAO_Notify_Export TAO_Notify_Builder
{
public:
  TAO_Notify_Builder (void);

  virtual ~TAO_Notify_Builder ();

  ///= Factory Methods

  /// Build EventChannel Factory.
  virtual CosNotifyChannelAdmin::EventChannelFactory_ptr
  build_event_channel_factory (PortableServer::POA_ptr poa ACE_ENV_ARG_DECL);

  /// Build the Filter Factory.
  virtual CosNotifyFilter::FilterFactory_ptr build_filter_factory (ACE_ENV_SINGLE_ARG_DECL);

  /// Build EventChannel.
  virtual CosNotifyChannelAdmin::EventChannel_ptr
  build_event_channel (TAO_Notify_EventChannelFactory* ecf
                       , const CosNotification::QoSProperties & initial_qos
                       , const CosNotification::AdminProperties & initial_admin
                       , CosNotifyChannelAdmin::ChannelID_out id
                       ACE_ENV_ARG_DECL);

  /// Build ConsumerAdmin
  virtual CosNotifyChannelAdmin::ConsumerAdmin_ptr
  build_consumer_admin (TAO_Notify_EventChannel* ec
                        , CosNotifyChannelAdmin::InterFilterGroupOperator op
                        , CosNotifyChannelAdmin::AdminID_out id
                        ACE_ENV_ARG_DECL);

  /// Build SupplierAdmin
  virtual CosNotifyChannelAdmin::SupplierAdmin_ptr
  build_supplier_admin (TAO_Notify_EventChannel* ec
                        , CosNotifyChannelAdmin::InterFilterGroupOperator op
                        , CosNotifyChannelAdmin::AdminID_out id
                        ACE_ENV_ARG_DECL);

  /// Build ProxyConsumer
  virtual CosNotifyChannelAdmin::ProxyConsumer_ptr
  build_proxy (TAO_Notify_SupplierAdmin* sa
               , CosNotifyChannelAdmin::ClientType ctype
               , CosNotifyChannelAdmin::ProxyID_out proxy_id
               , const CosNotification::QoSProperties & initial_qos
               ACE_ENV_ARG_DECL);

  /// Build ProxySupplier.
  virtual CosNotifyChannelAdmin::ProxySupplier_ptr
  build_proxy (TAO_Notify_ConsumerAdmin* ca
               , CosNotifyChannelAdmin::ClientType ctype
               , CosNotifyChannelAdmin::ProxyID_out proxy_id
               , const CosNotification::QoSProperties & initial_qos
               ACE_ENV_ARG_DECL);

  /// Build CosEC style ProxySupplier.
  virtual CosEventChannelAdmin::ProxyPushSupplier_ptr
  build_proxy (TAO_Notify_ConsumerAdmin* ca ACE_ENV_ARG_DECL);

  /// Build CosEC style ProxyConsumer.
  virtual CosEventChannelAdmin::ProxyPushConsumer_ptr
  build_proxy (TAO_Notify_SupplierAdmin* sa ACE_ENV_ARG_DECL);

  /// Apply Reactive concurrency.
  virtual void apply_reactive_concurrency (TAO_Notify_Object& object ACE_ENV_ARG_DECL);

  /// Apply Thread Pools.
  virtual void apply_thread_pool_concurrency (TAO_Notify_Object& object, const NotifyExt::ThreadPoolParams& tp_params ACE_ENV_ARG_DECL);

  /// Apply Thread Pools with Lanes.
  virtual void apply_lane_concurrency (TAO_Notify_Object& object, const NotifyExt::ThreadPoolLanesParams& tpl_params ACE_ENV_ARG_DECL);
};

#if defined (__ACE_INLINE__)
#include "Builder.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* TAO_Notify_BUILDER_H */
