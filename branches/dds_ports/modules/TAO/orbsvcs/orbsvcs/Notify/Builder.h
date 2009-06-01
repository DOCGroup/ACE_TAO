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

#include "orbsvcs/Notify/notify_serv_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/CosNotifyChannelAdminC.h"
#include "orbsvcs/NotifyExtC.h"

#include "orbsvcs/Notify/AdminProperties.h"
#include "orbsvcs/Notify/Topology_Object.h"

#include "tao/PortableServer/PortableServer.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Notify_EventChannelFactory;
class TAO_Notify_EventChannel;
class TAO_Notify_SupplierAdmin;
class TAO_Notify_ConsumerAdmin;
class TAO_Notify_ProxyConsumer;
class TAO_Notify_ProxySupplier;
class TAO_Notify_FilterFactory;
class TAO_Notify_Object;

/**
 * @class TAO_Notify_Builder
 *
 * @brief Helper class to create and activate CORBA objects.
 *
 */
class TAO_Notify_Serv_Export TAO_Notify_Builder
{
public:
  TAO_Notify_Builder (void);

  virtual ~TAO_Notify_Builder ();

  ///= Factory Methods

  /// Build EventChannel Factory.
  virtual CosNotifyChannelAdmin::EventChannelFactory_ptr
  build_event_channel_factory (PortableServer::POA_ptr poa
                               , const char* factory_name = 0);

  /// Build the Filter Factory.
  virtual CosNotifyFilter::FilterFactory_ptr
  build_filter_factory (PortableServer::POA_ptr poa, TAO_Notify_FilterFactory*& ff);

  /// Build EventChannel.
  virtual CosNotifyChannelAdmin::EventChannel_ptr
  build_event_channel (TAO_Notify_EventChannelFactory* ecf
                       , const CosNotification::QoSProperties & initial_qos
                       , const CosNotification::AdminProperties & initial_admin
                       , CosNotifyChannelAdmin::ChannelID_out id
                       , const char* ec_name = 0);

  virtual TAO_Notify_EventChannel *
  build_event_channel (TAO_Notify_EventChannelFactory* ecf
                       , const CosNotifyChannelAdmin::ChannelID id
                       , const char* ec_name = 0);


  /// Build ConsumerAdmin
  virtual CosNotifyChannelAdmin::ConsumerAdmin_ptr
  build_consumer_admin (TAO_Notify_EventChannel* ec
                        , CosNotifyChannelAdmin::InterFilterGroupOperator op
                        , CosNotifyChannelAdmin::AdminID_out id);

  /// Build ConsumerAdmin during topology restore
  /// TODO: this returns a reference to the actual type
  /// to accomodate loadable builder/factory there should
  /// be an appropriate base class for it to return.
  virtual TAO_Notify_ConsumerAdmin *
  build_consumer_admin (TAO_Notify_EventChannel* ec
                        , const CosNotifyChannelAdmin::AdminID id //CORBA::Long id          // note: an in parameter!
                        );

  /// Build SupplierAdmin
  virtual CosNotifyChannelAdmin::SupplierAdmin_ptr
  build_supplier_admin (TAO_Notify_EventChannel * ec
                        , CosNotifyChannelAdmin::InterFilterGroupOperator op
                        , CosNotifyChannelAdmin::AdminID_out id);

  /// Build ConsumerAdmin during topology restore
  /// TODO: this returns a reference to the actual type
  /// to accomodate loadable builder/factory there should
  /// be an appropriate base class for it to return.
  virtual TAO_Notify_SupplierAdmin *
  build_supplier_admin (TAO_Notify_EventChannel * ec
                        , const CosNotifyChannelAdmin::AdminID id //CORBA::Long id          // note: an in parameter!
                        );

  /// Build ProxyConsumer
  virtual CosNotifyChannelAdmin::ProxyConsumer_ptr
  build_proxy (TAO_Notify_SupplierAdmin* sa
               , CosNotifyChannelAdmin::ClientType ctype
               , CosNotifyChannelAdmin::ProxyID_out proxy_id
               , const CosNotification::QoSProperties & initial_qos);

  /// Reload ProxyConsumer
  virtual TAO_Notify_ProxyConsumer *
  build_proxy (TAO_Notify_SupplierAdmin* sa
               , CosNotifyChannelAdmin::ClientType ctype
               , const CosNotifyChannelAdmin::ProxyID proxy_id);

  /// Build ProxySupplier.
  virtual CosNotifyChannelAdmin::ProxySupplier_ptr
  build_proxy (TAO_Notify_ConsumerAdmin* ca
               , CosNotifyChannelAdmin::ClientType ctype
               , CosNotifyChannelAdmin::ProxyID_out proxy_id
               , const CosNotification::QoSProperties & initial_qos);

  /// Reload ProxySupplier.
  virtual TAO_Notify_ProxySupplier *
  build_proxy (TAO_Notify_ConsumerAdmin* ca
               , CosNotifyChannelAdmin::ClientType ctype
               , const CosNotifyChannelAdmin::ProxyID proxy_id);

  /// Build CosEC style ProxySupplier.
  virtual CosEventChannelAdmin::ProxyPushSupplier_ptr
  build_proxy (TAO_Notify_ConsumerAdmin* ca);

  /// Build CosEC style ProxyConsumer.
  virtual CosEventChannelAdmin::ProxyPushConsumer_ptr
  build_proxy (TAO_Notify_SupplierAdmin* sa);

  /// Apply Reactive concurrency.
  virtual void apply_reactive_concurrency (TAO_Notify_Object& object);

  /// Apply Thread Pools.
  virtual void apply_thread_pool_concurrency (
      TAO_Notify_Object& object,
      const NotifyExt::ThreadPoolParams& tp_params);

  /// Apply Thread Pools with Lanes.
  virtual void apply_lane_concurrency (
      TAO_Notify_Object& object,
      const NotifyExt::ThreadPoolLanesParams& tpl_params);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_Notify_BUILDER_H */
