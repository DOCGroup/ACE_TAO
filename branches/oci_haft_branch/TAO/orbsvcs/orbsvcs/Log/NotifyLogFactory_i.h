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
#include "NotifyLog_i.h"

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */
 
class TAO_NotifyLog_i;
class TAO_NotifyLogNotification;

/**
 * @class TAO_NotifyLogFactory_i
 *
 * @brief The NotifyLogFactory is a factory that is used to create NotifyLogs which are event-aware.
 */
class TAO_NotifyLog_Export TAO_NotifyLogFactory_i :
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

  /// Activate this servant with the POA passed in.
  DsNotifyLogAdmin::NotifyLogFactory_ptr
    activate (PortableServer::POA_ptr poa
              ACE_ENV_ARG_DECL);

  /// Used to create an NotifyLog.
  DsNotifyLogAdmin::NotifyLog_ptr create (
        DsLogAdmin::LogFullActionType full_action,
        CORBA::ULongLong max_size,
        const DsLogAdmin::CapacityAlarmThresholdList & thresholds,
        const CosNotification::QoSProperties & initial_qos,
        const CosNotification::AdminProperties & initial_admin,
        DsLogAdmin::LogId_out id
        ACE_ENV_ARG_DECL
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DsLogAdmin::InvalidLogFullAction,
        DsLogAdmin::InvalidThreshold,
        CosNotification::UnsupportedQoS,
        CosNotification::UnsupportedAdmin
      ));

  /// Same as create (), but allows clients to specify the id.
  DsNotifyLogAdmin::NotifyLog_ptr create_with_id (
        DsLogAdmin::LogId id,
        DsLogAdmin::LogFullActionType full_action,
        CORBA::ULongLong max_size,
        const DsLogAdmin::CapacityAlarmThresholdList & thresholds,
        const CosNotification::QoSProperties & initial_qos,
        const CosNotification::AdminProperties & initial_admin
        ACE_ENV_ARG_DECL
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DsLogAdmin::LogIdAlreadyExists,
        DsLogAdmin::InvalidLogFullAction,
        DsLogAdmin::InvalidThreshold,
        CosNotification::UnsupportedQoS,
        CosNotification::UnsupportedAdmin
      ));


  // = Methods from CosNotifyChannelAdmin::ConsumerAdmin.
  CosNotifyChannelAdmin::AdminID MyID (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));

  CosNotifyChannelAdmin::EventChannel_ptr MyChannel (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));

  CosNotifyChannelAdmin::InterFilterGroupOperator MyOperator (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));

  CosNotifyFilter::MappingFilter_ptr priority_filter (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  void priority_filter (CosNotifyFilter::MappingFilter_ptr ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));

  CosNotifyFilter::MappingFilter_ptr lifetime_filter (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  void lifetime_filter (CosNotifyFilter::MappingFilter_ptr ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));

  CosNotifyChannelAdmin::ProxyIDSeq* pull_suppliers (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  CosNotifyChannelAdmin::ProxyIDSeq* push_suppliers (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));

  CosNotifyChannelAdmin::ProxySupplier_ptr get_proxy_supplier (
    CosNotifyChannelAdmin::ProxyID proxy_id ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
      CosNotifyChannelAdmin::ProxyNotFound, 
      CORBA::SystemException
    ));

  CosNotifyChannelAdmin::ProxySupplier_ptr obtain_notification_pull_supplier (
    CosNotifyChannelAdmin::ClientType ctype,
    CosNotifyChannelAdmin::ProxyID& proxy_id ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
      CosNotifyChannelAdmin::AdminLimitExceeded, 
      CORBA::SystemException
    ));

  CosNotifyChannelAdmin::ProxySupplier_ptr obtain_notification_push_supplier (
    CosNotifyChannelAdmin::ClientType ctype,
    CosNotifyChannelAdmin::ProxyID& id ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
      CosNotifyChannelAdmin::AdminLimitExceeded, 
      CORBA::SystemException
    ));

  CosEventChannelAdmin::ProxyPushSupplier_ptr obtain_push_supplier (
          ACE_ENV_SINGLE_ARG_DECL 
        )
        ACE_THROW_SPEC ((
          CORBA::SystemException
        ));

  CosEventChannelAdmin::ProxyPullSupplier_ptr obtain_pull_supplier (
          ACE_ENV_SINGLE_ARG_DECL 
        )
        ACE_THROW_SPEC ((
          CORBA::SystemException
        ));

  void destroy (ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((
          CORBA::SystemException
        ));

  /// CosNotifyComm::NotifySubscribe.
  void subscription_change (const CosNotification::EventTypeSeq& added,
                            const CosNotification::EventTypeSeq& removed
                            ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
      CosNotifyComm::InvalidEventType, 
      CORBA::SystemException
    ));

  /// CosNotification::QoSAdmin.
  CosNotification::QoSProperties* get_qos (ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((
          CORBA::SystemException
        ));
  void set_qos (const CosNotification::QoSProperties& ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
      CosNotification::UnsupportedQoS, 
      CORBA::SystemException
    ));
  void validate_qos (const CosNotification::QoSProperties&,
                     CosNotification::NamedPropertyRangeSeq_out
                     ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
      CosNotification::UnsupportedQoS, 
      CORBA::SystemException
    ));
                   
  /// CosNotifyFilter::FilterAdmin.
  CosNotifyFilter::FilterID add_filter (CosNotifyFilter::Filter_ptr ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((
          CORBA::SystemException
        ));
  void remove_filter (CosNotifyFilter::FilterID ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((
          CORBA::SystemException
        ));
  CosNotifyFilter::Filter_ptr get_filter (CosNotifyFilter::FilterID ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
      CosNotifyFilter::FilterNotFound, 
      CORBA::SystemException
    ));
  CosNotifyFilter::FilterIDSeq* get_all_filters (ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((
          CORBA::SystemException
        ));
  void remove_all_filters (ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((
          CORBA::SystemException
        ));

protected:
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

  /// POA.
  PortableServer::POA_var poa_;
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /* TAO_TLS_NOTIFYLOGFACTORY_I_H */
