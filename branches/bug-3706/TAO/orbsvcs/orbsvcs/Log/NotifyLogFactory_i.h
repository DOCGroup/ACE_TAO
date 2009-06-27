// -*- C++ -*-

//=============================================================================
/**
 *  @file   NotifyLogFactory_i.h
 *
 *  $Id$
 *
 *  Implements the Factory for NotifyLog Objects.
 *
 *
 *  @author David A. Hanvey <d.hanvey@qub.ac.uk>
 */
//=============================================================================

#ifndef TAO_TLS_NOTIFYLOGFACTORY_I_H
#define TAO_TLS_NOTIFYLOGFACTORY_I_H

#include /**/ "ace/pre.h"

#include "orbsvcs/DsNotifyLogAdminS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/DsLogAdminS.h"
#include "orbsvcs/Log/LogMgr_i.h"
#include "orbsvcs/Log/NotifyLog_i.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_NotifyLog_i;
class TAO_NotifyLogNotification;

/**
 * @class TAO_NotifyLogFactory_i
 *
 * @brief The NotifyLogFactory is a factory that is used to create NotifyLogs which are event-aware.
 */
class TAO_NotifyLog_Serv_Export TAO_NotifyLogFactory_i :
  public POA_DsNotifyLogAdmin::NotifyLogFactory,
  public TAO_LogMgr_i
{
public:
  //= Initialization and termination code.

  /// Constructor - default.
  TAO_NotifyLogFactory_i (void);

  /// Constructor.
  TAO_NotifyLogFactory_i (CosNotifyChannelAdmin::EventChannelFactory_ptr ecf);

  /// Destructor.
  ~TAO_NotifyLogFactory_i ();

  /// Activate this servant with the ORB and POA passed in.
  DsNotifyLogAdmin::NotifyLogFactory_ptr
    activate (CORBA::ORB_ptr orb, PortableServer::POA_ptr poa);

  /// Used to create an NotifyLog.
  DsNotifyLogAdmin::NotifyLog_ptr create (
        DsLogAdmin::LogFullActionType full_action,
        CORBA::ULongLong max_size,
        const DsLogAdmin::CapacityAlarmThresholdList & thresholds,
        const CosNotification::QoSProperties & initial_qos,
        const CosNotification::AdminProperties & initial_admin,
        DsLogAdmin::LogId_out id);

  /// Same as create (), but allows clients to specify the id.
  DsNotifyLogAdmin::NotifyLog_ptr create_with_id (
        DsLogAdmin::LogId id,
        DsLogAdmin::LogFullActionType full_action,
        CORBA::ULongLong max_size,
        const DsLogAdmin::CapacityAlarmThresholdList & thresholds,
        const CosNotification::QoSProperties & initial_qos,
        const CosNotification::AdminProperties & initial_admin
      );


  // = Methods from CosNotifyChannelAdmin::ConsumerAdmin.
  CosNotifyChannelAdmin::AdminID MyID (void);

  CosNotifyChannelAdmin::EventChannel_ptr MyChannel (void);

  CosNotifyChannelAdmin::InterFilterGroupOperator MyOperator (void);

  CosNotifyFilter::MappingFilter_ptr priority_filter (void);
  void priority_filter (CosNotifyFilter::MappingFilter_ptr);

  CosNotifyFilter::MappingFilter_ptr lifetime_filter (void);
  void lifetime_filter (CosNotifyFilter::MappingFilter_ptr);

  CosNotifyChannelAdmin::ProxyIDSeq* pull_suppliers (void);
  CosNotifyChannelAdmin::ProxyIDSeq* push_suppliers (void);

  CosNotifyChannelAdmin::ProxySupplier_ptr get_proxy_supplier (
    CosNotifyChannelAdmin::ProxyID proxy_id);

  CosNotifyChannelAdmin::ProxySupplier_ptr obtain_notification_pull_supplier (
    CosNotifyChannelAdmin::ClientType ctype,
    CosNotifyChannelAdmin::ProxyID& proxy_id);

  CosNotifyChannelAdmin::ProxySupplier_ptr obtain_notification_push_supplier (
    CosNotifyChannelAdmin::ClientType ctype,
    CosNotifyChannelAdmin::ProxyID& id);

  CosEventChannelAdmin::ProxyPushSupplier_ptr obtain_push_supplier (void);

  CosEventChannelAdmin::ProxyPullSupplier_ptr obtain_pull_supplier (void);

  void destroy (void);

  /// CosNotifyComm::NotifySubscribe.
  void subscription_change (const CosNotification::EventTypeSeq& added,
                            const CosNotification::EventTypeSeq& removed);

  /// CosNotification::QoSAdmin.
  CosNotification::QoSProperties* get_qos (void);
  void set_qos (const CosNotification::QoSProperties&);
  void validate_qos (const CosNotification::QoSProperties&,
                     CosNotification::NamedPropertyRangeSeq_out);

  /// CosNotifyFilter::FilterAdmin.
  CosNotifyFilter::FilterID add_filter (CosNotifyFilter::Filter_ptr);
  void remove_filter (CosNotifyFilter::FilterID);
  CosNotifyFilter::Filter_ptr get_filter (CosNotifyFilter::FilterID);
  CosNotifyFilter::FilterIDSeq* get_all_filters (void);
  void remove_all_filters (void);

protected:
  virtual CORBA::RepositoryId
    create_repositoryid ();

  virtual PortableServer::ServantBase*
    create_log_servant (DsLogAdmin::LogId id);

  /// Our object ref. after <active>ation.
  DsLogAdmin::LogMgr_var log_mgr_;

  /// The Channel ID.
  CosNotifyChannelAdmin::ChannelID channel_id_;

  /// The EventChannel the factory uses to enable the
  /// NotifyLogNotification object to connect to and send
  /// log-generated events.
  CosNotifyChannelAdmin::EventChannel_var event_channel_;

  /// The ConsumerAdmin that the NotifyLogFactory supports.
  CosNotifyChannelAdmin::ConsumerAdmin_var consumer_admin_;

  //CosEventChannelAdmin::EventChannel_var es_event_channel_;

  //CosEventChannelAdmin::ConsumerAdmin_var es_consumer_admin_;

  /// Used for log-generated events.
  TAO_NotifyLogNotification* notifier_;

  /// The EventChannelFactory used to create an EventChannel.
  CosNotifyChannelAdmin::EventChannelFactory_var notify_factory_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /* TAO_TLS_NOTIFYLOGFACTORY_I_H */
