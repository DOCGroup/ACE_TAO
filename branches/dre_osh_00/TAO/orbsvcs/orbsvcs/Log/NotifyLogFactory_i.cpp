#include "orbsvcs/Log/NotifyLogFactory_i.h"
#include "orbsvcs/Log/LogNotification.h"
#include "orbsvcs/Log/NotifyLogNotification.h"

ACE_RCSID (Log,
           NotifyLogFactory_i,
           "$Id$")


TAO_NotifyLogFactory_i::TAO_NotifyLogFactory_i (void)
{
  // No-Op.
}

TAO_NotifyLogFactory_i::TAO_NotifyLogFactory_i (CosNotifyChannelAdmin::EventChannelFactory_ptr ecf)
: notify_factory_ (ecf)
{
  ACE_DECLARE_NEW_CORBA_ENV;

  CosNotifyChannelAdmin::ChannelID id;
  CosNotifyChannelAdmin::AdminID adminID = 0;
  CosNotification::QoSProperties initial_qos;
  CosNotification::AdminProperties initial_admin;

  this->event_channel_ =
          this->notify_factory_->create_channel (initial_qos,
                                                 initial_admin,
                                                 id
                      ACE_ENV_ARG_PARAMETER);

  this->consumer_admin_ = this->event_channel_->new_for_consumers(CosNotifyChannelAdmin::OR_OP, adminID);

  CosNotification::EventTypeSeq added(1);
  CosNotification::EventTypeSeq removed (0);
  added.length (1);
  removed.length (0);

  added[0].domain_name =  CORBA::string_dup ("*");
  added[0].type_name = CORBA::string_dup ("*");

  this->consumer_admin_->subscription_change (added, removed ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

    ACE_NEW_THROW_EX (this->notifier_,
                  TAO_NotifyLogNotification(this->event_channel_.in ()),
                   CORBA::NO_MEMORY ());
}

TAO_NotifyLogFactory_i::~TAO_NotifyLogFactory_i()
{
  // No-Op.
}

DsNotifyLogAdmin::NotifyLogFactory_ptr
TAO_NotifyLogFactory_i::activate (PortableServer::POA_ptr poa
                                  ACE_ENV_ARG_DECL)
{
  DsNotifyLogAdmin::NotifyLogFactory_var v_return;

  PortableServer::ObjectId_var oid =
    poa->activate_object (this
                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (v_return._retn ());

  CORBA::Object_var obj =
    poa->id_to_reference (oid.in ()
                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (v_return._retn ());

  // narrow and store the result..
  this->log_mgr_ =
    DsLogAdmin::LogMgr::_narrow (obj.in ()
                                 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (v_return._retn ());

  v_return =
    DsNotifyLogAdmin::NotifyLogFactory::_narrow (obj.in ()
                                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (DsNotifyLogAdmin::NotifyLogFactory::_nil ());

  return v_return._retn ();
}

DsNotifyLogAdmin::NotifyLog_ptr
TAO_NotifyLogFactory_i::create (
        DsLogAdmin::LogFullActionType full_action,
        CORBA::ULongLong max_rec_size,
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
      ))
{
  // Get an id for this Log.
  this->max_id_++;

  DsNotifyLogAdmin::NotifyLog_ptr notifylog =
    this->create_with_id (this->max_id_,
                          full_action,
                          max_rec_size,
                          thresholds,
                          initial_qos,
                          initial_admin
                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (DsNotifyLogAdmin::NotifyLog::_nil ());

  // Set the id to return..
  id = this->max_id_;

  // Store the id in the LogIdList.
  CORBA::ULong len = logid_list_.length();
  logid_list_.length(len+1);
  logid_list_[len] = id;

  return notifylog;
}

DsNotifyLogAdmin::NotifyLog_ptr
TAO_NotifyLogFactory_i::create_with_id (
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
      ))
{
  ACE_UNUSED_ARG (initial_qos);

  ACE_UNUSED_ARG (initial_admin);

  ACE_UNUSED_ARG (thresholds);

  // Make sure the id not used up.
  if (hash_map_.find (id) == 0)
    ACE_THROW_RETURN (DsLogAdmin::LogIdAlreadyExists (),
                      DsNotifyLogAdmin::NotifyLog::_nil ());

  DsNotifyLogAdmin::NotifyLog_var notify_log;
  // Object to return.

  TAO_NotifyLog_i* notify_log_i;

  ACE_NEW_THROW_EX (notify_log_i,
                    TAO_NotifyLog_i (*this,
                                     this->log_mgr_.in (),
                                     this,
                                     this->notify_factory_.in (),
                                     this->notifier_,
                                     id,
                                     full_action,
                                     max_size
                                     ),
                    CORBA::NO_MEMORY ());

  ACE_CHECK_RETURN (notify_log._retn ());

  PortableServer::ServantBase_var safe_notify_log_i = notify_log_i;
  // Transfer ownership to the POA.

  notify_log_i->init (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (notify_log._retn ());

  //initialise the LogConsumer object
  notify_log_i->activate (ACE_ENV_SINGLE_ARG_PARAMETER);

  // Register with the poa
  notify_log = notify_log_i->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (notify_log._retn ());

  // Add to the Hash table..
  if (hash_map_.bind (id, notify_log.in ()) == -1)
    ACE_THROW_RETURN (CORBA::INTERNAL (),
                      DsNotifyLogAdmin::NotifyLog::_nil ());

  notifier_->object_creation (notify_log.in (),
                              id ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (DsNotifyLogAdmin::NotifyLog::_nil ());

  return notify_log._retn ();
}

CosNotifyChannelAdmin::AdminID
TAO_NotifyLogFactory_i::MyID (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
  return consumer_admin_->MyID (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CosNotifyChannelAdmin::EventChannel_ptr
TAO_NotifyLogFactory_i::MyChannel (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT(), 0);
}

CosNotifyChannelAdmin::InterFilterGroupOperator
TAO_NotifyLogFactory_i::MyOperator (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
  return consumer_admin_->MyOperator (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CosNotifyFilter::MappingFilter_ptr
TAO_NotifyLogFactory_i::priority_filter (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
  return consumer_admin_->priority_filter (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_NotifyLogFactory_i::priority_filter (CosNotifyFilter::MappingFilter_ptr filter ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
  consumer_admin_->priority_filter(filter ACE_ENV_ARG_PARAMETER);
}

CosNotifyFilter::MappingFilter_ptr
TAO_NotifyLogFactory_i::lifetime_filter (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
  return consumer_admin_->lifetime_filter (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_NotifyLogFactory_i::lifetime_filter (CosNotifyFilter::MappingFilter_ptr filter ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
  consumer_admin_->lifetime_filter (filter ACE_ENV_ARG_PARAMETER);
}

CosNotifyChannelAdmin::ProxyIDSeq*
TAO_NotifyLogFactory_i::pull_suppliers (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
  return consumer_admin_->pull_suppliers (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CosNotifyChannelAdmin::ProxyIDSeq*
TAO_NotifyLogFactory_i::push_suppliers (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
  return consumer_admin_->push_suppliers (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CosNotifyChannelAdmin::ProxySupplier_ptr
TAO_NotifyLogFactory_i::get_proxy_supplier (
  CosNotifyChannelAdmin::ProxyID proxy_id ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
      CosNotifyChannelAdmin::ProxyNotFound,
      CORBA::SystemException
    ))
{
  return consumer_admin_->get_proxy_supplier (proxy_id ACE_ENV_ARG_PARAMETER);
}

CosNotifyChannelAdmin::ProxySupplier_ptr
TAO_NotifyLogFactory_i::obtain_notification_pull_supplier (
  CosNotifyChannelAdmin::ClientType ctype,
  CosNotifyChannelAdmin::ProxyID& proxy_id ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
      CosNotifyChannelAdmin::AdminLimitExceeded,
      CORBA::SystemException
    ))
{
  return consumer_admin_->obtain_notification_pull_supplier (ctype, proxy_id ACE_ENV_ARG_PARAMETER);
}

CosNotifyChannelAdmin::ProxySupplier_ptr
TAO_NotifyLogFactory_i::obtain_notification_push_supplier (
  CosNotifyChannelAdmin::ClientType ctype,
  CosNotifyChannelAdmin::ProxyID& proxy_id ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
      CosNotifyChannelAdmin::AdminLimitExceeded,
      CORBA::SystemException
    ))
{
  return consumer_admin_->obtain_notification_push_supplier (ctype, proxy_id ACE_ENV_ARG_PARAMETER);
}

CosEventChannelAdmin::ProxyPushSupplier_ptr
TAO_NotifyLogFactory_i::obtain_push_supplier (
        ACE_ENV_SINGLE_ARG_DECL
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ))
{
  return consumer_admin_->obtain_push_supplier (ACE_ENV_SINGLE_ARG_PARAMETER);
}



CosEventChannelAdmin::ProxyPullSupplier_ptr
TAO_NotifyLogFactory_i::obtain_pull_supplier (
        ACE_ENV_SINGLE_ARG_DECL
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ))

{
  return consumer_admin_->obtain_pull_supplier (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_NotifyLogFactory_i::destroy (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
        ACE_THROW_SPEC ((
          CORBA::SystemException
        ))
{
  // No-Op.
}

//CosNotifyComm::NotifySubscribe
void
TAO_NotifyLogFactory_i::subscription_change (const CosNotification::EventTypeSeq& added,
                          const CosNotification::EventTypeSeq& removed
                          ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
      CosNotifyComm::InvalidEventType,
      CORBA::SystemException
    ))
{
  consumer_admin_->subscription_change (added, removed ACE_ENV_ARG_PARAMETER);
}

CosNotification::QoSProperties*
TAO_NotifyLogFactory_i::get_qos (ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((
          CORBA::SystemException
        ))
{
  return consumer_admin_->get_qos (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_NotifyLogFactory_i::set_qos (const CosNotification::QoSProperties& qos ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
      CosNotification::UnsupportedQoS,
      CORBA::SystemException
    ))
{
  consumer_admin_->set_qos (qos ACE_ENV_ARG_PARAMETER);
}

void
TAO_NotifyLogFactory_i::validate_qos (const CosNotification::QoSProperties& required_qos,
                   CosNotification::NamedPropertyRangeSeq_out available_qos
                   ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
      CosNotification::UnsupportedQoS,
      CORBA::SystemException
    ))
{
  consumer_admin_->validate_qos (required_qos, available_qos ACE_ENV_ARG_PARAMETER);
}

CosNotifyFilter::FilterID
TAO_NotifyLogFactory_i::add_filter (CosNotifyFilter::Filter_ptr filter ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((
          CORBA::SystemException
        ))
{
  return consumer_admin_->add_filter (filter ACE_ENV_ARG_PARAMETER);
}

void
TAO_NotifyLogFactory_i::remove_filter (CosNotifyFilter::FilterID id ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((
          CORBA::SystemException
        ))
{
  consumer_admin_->remove_filter (id ACE_ENV_ARG_PARAMETER);
}

CosNotifyFilter::Filter_ptr
TAO_NotifyLogFactory_i::get_filter (CosNotifyFilter::FilterID id ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
      CosNotifyFilter::FilterNotFound,
      CORBA::SystemException
    ))
{
  return consumer_admin_->get_filter (id ACE_ENV_ARG_PARAMETER);
}

CosNotifyFilter::FilterIDSeq*
TAO_NotifyLogFactory_i::get_all_filters (ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((
          CORBA::SystemException
        ))
{
  return consumer_admin_->get_all_filters (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_NotifyLogFactory_i::remove_all_filters (ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((
          CORBA::SystemException
        ))
{
  consumer_admin_->remove_all_filters (ACE_ENV_SINGLE_ARG_PARAMETER);
}
