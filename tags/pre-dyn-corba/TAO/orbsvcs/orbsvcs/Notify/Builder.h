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

#ifndef TAO_NS_BUILDER_H
#define TAO_NS_BUILDER_H
#include "ace/pre.h"

#include "notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableServer/PortableServer.h"
#include "orbsvcs/CosNotifyChannelAdminC.h"
#include "orbsvcs/NotifyExtC.h"

class TAO_NS_EventChannelFactory;
class TAO_NS_EventChannel;
class TAO_NS_SupplierAdmin;
class TAO_NS_ConsumerAdmin;
class TAO_NS_FilterFactory;
class TAO_NS_Object;
class TAO_NS_AdminProperties;

/**
 * @class TAO_NS_Builder
 *
 * @brief Helper class to create and activate CORBA objects.
 *
 */
class TAO_Notify_Export TAO_NS_Builder
{
public:
  TAO_NS_Builder (void);

  virtual ~TAO_NS_Builder ();

  ///= Factory Method
  CosNotifyChannelAdmin::EventChannelFactory_ptr build_event_channel_factory (PortableServer::POA_ptr poa ACE_ENV_ARG_DECL);

  CosNotifyChannelAdmin::EventChannel_ptr build_event_channel (TAO_NS_EventChannelFactory* ecf, const CosNotification::QoSProperties & initial_qos, const CosNotification::AdminProperties & initial_admin, CosNotifyChannelAdmin::ChannelID_out id ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     , CosNotification::UnsupportedQoS
                     , CosNotification::UnsupportedAdmin
                     ));

  CosNotifyChannelAdmin::ConsumerAdmin_ptr build_consumer_admin (TAO_NS_EventChannel* ec, CosNotifyChannelAdmin::InterFilterGroupOperator op, CosNotifyChannelAdmin::AdminID_out id ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  CosNotifyChannelAdmin::SupplierAdmin_ptr build_supplier_admin (TAO_NS_EventChannel* ec, CosNotifyChannelAdmin::InterFilterGroupOperator op, CosNotifyChannelAdmin::AdminID_out id ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  CosNotifyChannelAdmin::ProxyConsumer_ptr build_notification_push_consumer (TAO_NS_SupplierAdmin* sa, CosNotifyChannelAdmin::ClientType ctype, CosNotifyChannelAdmin::ProxyID_out proxy_id  ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     , CosNotifyChannelAdmin::AdminLimitExceeded
                     ));

  CosNotifyChannelAdmin::ProxySupplier_ptr build_notification_push_supplier (TAO_NS_ConsumerAdmin* ca, CosNotifyChannelAdmin::ClientType ctype, CosNotifyChannelAdmin::ProxyID_out proxy_id  ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     , CosNotifyChannelAdmin::AdminLimitExceeded
                     ));

  CosNotifyFilter::FilterFactory_ptr build_filter_factory (ACE_ENV_SINGLE_ARG_DECL);

  CosEventChannelAdmin::ProxyPushSupplier_ptr build_push_supplier (TAO_NS_ConsumerAdmin* ca ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  CosEventChannelAdmin::ProxyPushConsumer_ptr build_push_consumer (TAO_NS_SupplierAdmin* sa ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));
  ///= Helper Methods

  /// Apply Threadpool QoS
  virtual void apply_threadpool_qos (TAO_NS_Object& object, const NotifyExt::ThreadPoolParams& tp_params, TAO_NS_AdminProperties& admin_properties ACE_ENV_ARG_DECL);

  /// Apply ThreadpoolLanes QoS
  virtual void apply_threadpool_lane_qos (TAO_NS_Object& object, const NotifyExt::ThreadPoolLanesParams& tpl_params, TAO_NS_AdminProperties& admin_properties ACE_ENV_ARG_DECL);
};

#if defined (__ACE_INLINE__)
#include "Builder.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NS_BUILDER_H */
