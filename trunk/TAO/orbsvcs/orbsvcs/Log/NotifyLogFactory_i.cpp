// $Id$

#include "orbsvcs/Log/NotifyLogFactory_i.h"
#include "orbsvcs/Log/LogNotification.h"
#include "orbsvcs/Log/NotifyLogNotification.h"
#include "ace/OS_NS_stdio.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_NotifyLogFactory_i::TAO_NotifyLogFactory_i (void)
{
}

TAO_NotifyLogFactory_i::TAO_NotifyLogFactory_i (CosNotifyChannelAdmin::EventChannelFactory_ptr ecf)
: notify_factory_ (ecf)
{

  CosNotifyChannelAdmin::ChannelID id;
  CosNotifyChannelAdmin::AdminID adminID = 0;
  CosNotification::QoSProperties initial_qos;
  CosNotification::AdminProperties initial_admin;

  this->event_channel_ =
          this->notify_factory_->create_channel (initial_qos,
                                                 initial_admin,
                                                 id);

  this->consumer_admin_ = this->event_channel_->new_for_consumers(CosNotifyChannelAdmin::OR_OP, adminID);

  CosNotification::EventTypeSeq added(1);
  CosNotification::EventTypeSeq removed (0);
  added.length (1);
  removed.length (0);

  added[0].domain_name =  CORBA::string_dup ("*");
  added[0].type_name = CORBA::string_dup ("*");

  this->consumer_admin_->subscription_change (added, removed);

    ACE_NEW_THROW_EX (this->notifier_,
                  TAO_NotifyLogNotification(this->event_channel_.in ()),
                   CORBA::NO_MEMORY ());
}

TAO_NotifyLogFactory_i::~TAO_NotifyLogFactory_i()
{
  // No-Op.
}

DsNotifyLogAdmin::NotifyLogFactory_ptr
TAO_NotifyLogFactory_i::activate (CORBA::ORB_ptr orb,
                                  PortableServer::POA_ptr poa)
{
  TAO_LogMgr_i::init (orb, poa);


  PortableServer::ObjectId_var oid =
    this->factory_poa_->activate_object (this);

  CORBA::Object_var obj =
    this->factory_poa_->id_to_reference (oid.in ());

  // narrow and store the result..
  this->log_mgr_ =
    DsLogAdmin::LogMgr::_narrow (obj.in ());

  DsNotifyLogAdmin::NotifyLogFactory_var v_return =
    DsNotifyLogAdmin::NotifyLogFactory::_narrow (obj.in ());

  return v_return._retn ();
}

DsNotifyLogAdmin::NotifyLog_ptr
TAO_NotifyLogFactory_i::create (
        DsLogAdmin::LogFullActionType full_action,
        CORBA::ULongLong max_size,
        const DsLogAdmin::CapacityAlarmThresholdList & thresholds,
        const CosNotification::QoSProperties & initial_qos,
        const CosNotification::AdminProperties & initial_admin,
        DsLogAdmin::LogId_out id_out
      )
{
  ACE_UNUSED_ARG (initial_qos);
  ACE_UNUSED_ARG (initial_admin);

  this->create_i (full_action,
                  max_size,
                  & thresholds,
                  id_out);
  DsLogAdmin::LogId id = id_out;

#if (TAO_HAS_MINIMUM_POA == 0)
  DsLogAdmin::Log_var log =
    this->create_log_reference (id);
#else
  DsLogAdmin::Log_var log =
    this->create_log_object (id);
#endif

  // narrow to NotifyLog
  DsNotifyLogAdmin::NotifyLog_var notify_log =
    DsNotifyLogAdmin::NotifyLog::_narrow (log.in ());

  // @@ JTC - squelch exception?
  notifier_->object_creation (id);

  return notify_log._retn ();
}

DsNotifyLogAdmin::NotifyLog_ptr
TAO_NotifyLogFactory_i::create_with_id (
        DsLogAdmin::LogId id,
        DsLogAdmin::LogFullActionType full_action,
        CORBA::ULongLong max_size,
        const DsLogAdmin::CapacityAlarmThresholdList & thresholds,
        const CosNotification::QoSProperties & initial_qos,
        const CosNotification::AdminProperties & initial_admin
      )
{
  ACE_UNUSED_ARG (initial_qos);
  ACE_UNUSED_ARG (initial_admin);

  this->create_with_id_i (id,
                          full_action,
                          max_size,
                          & thresholds);

#if (TAO_HAS_MINIMUM_POA == 0)
  DsLogAdmin::Log_var log =
    this->create_log_reference (id);
#else
  DsLogAdmin::Log_var log =
    this->create_log_object (id);
#endif

  // narrow to NotifyLog
  DsNotifyLogAdmin::NotifyLog_var notify_log =
    DsNotifyLogAdmin::NotifyLog::_narrow (log.in ());

  // @@ JTC - squelch exception?
  notifier_->object_creation (id);

  return notify_log._retn();
}

CORBA::RepositoryId
TAO_NotifyLogFactory_i::create_repositoryid ()
{
  return CORBA::string_dup (DsNotifyLogAdmin::_tc_NotifyLog->id ());
}

PortableServer::ServantBase*
TAO_NotifyLogFactory_i::create_log_servant (DsLogAdmin::LogId id)
{
  TAO_NotifyLog_i* notify_log_i;

  ACE_NEW_THROW_EX (notify_log_i,
                    TAO_NotifyLog_i (this->orb_.in (),
                                     this->log_poa_.in (),
                                     *this,
                                     this->log_mgr_.in (),
                                     this->notify_factory_.in (),
                                     this->notifier_,
                                     id
                                     ),
                    CORBA::NO_MEMORY ());

  notify_log_i->init ();

  //initialise the LogConsumer object
  notify_log_i->activate ();

  return notify_log_i;
}

CosNotifyChannelAdmin::AdminID
TAO_NotifyLogFactory_i::MyID (void)
{
  return consumer_admin_->MyID ();
}

CosNotifyChannelAdmin::EventChannel_ptr
TAO_NotifyLogFactory_i::MyChannel (void)
{
  throw CORBA::NO_IMPLEMENT();
}

CosNotifyChannelAdmin::InterFilterGroupOperator
TAO_NotifyLogFactory_i::MyOperator (void)
{
  return consumer_admin_->MyOperator ();
}

CosNotifyFilter::MappingFilter_ptr
TAO_NotifyLogFactory_i::priority_filter (void)
{
  return consumer_admin_->priority_filter ();
}

void
TAO_NotifyLogFactory_i::priority_filter (CosNotifyFilter::MappingFilter_ptr filter)
{
  consumer_admin_->priority_filter(filter);
}

CosNotifyFilter::MappingFilter_ptr
TAO_NotifyLogFactory_i::lifetime_filter (void)
{
  return consumer_admin_->lifetime_filter ();
}

void
TAO_NotifyLogFactory_i::lifetime_filter (CosNotifyFilter::MappingFilter_ptr filter)
{
  consumer_admin_->lifetime_filter (filter);
}

CosNotifyChannelAdmin::ProxyIDSeq*
TAO_NotifyLogFactory_i::pull_suppliers (void)
{
  return consumer_admin_->pull_suppliers ();
}

CosNotifyChannelAdmin::ProxyIDSeq*
TAO_NotifyLogFactory_i::push_suppliers (void)
{
  return consumer_admin_->push_suppliers ();
}

CosNotifyChannelAdmin::ProxySupplier_ptr
TAO_NotifyLogFactory_i::get_proxy_supplier (
  CosNotifyChannelAdmin::ProxyID proxy_id)
{
  return consumer_admin_->get_proxy_supplier (proxy_id);
}

CosNotifyChannelAdmin::ProxySupplier_ptr
TAO_NotifyLogFactory_i::obtain_notification_pull_supplier (
  CosNotifyChannelAdmin::ClientType ctype,
  CosNotifyChannelAdmin::ProxyID& proxy_id)
{
  return consumer_admin_->obtain_notification_pull_supplier (ctype, proxy_id);
}

CosNotifyChannelAdmin::ProxySupplier_ptr
TAO_NotifyLogFactory_i::obtain_notification_push_supplier (
  CosNotifyChannelAdmin::ClientType ctype,
  CosNotifyChannelAdmin::ProxyID& proxy_id)
{
  return consumer_admin_->obtain_notification_push_supplier (ctype, proxy_id);
}

CosEventChannelAdmin::ProxyPushSupplier_ptr
TAO_NotifyLogFactory_i::obtain_push_supplier (
      )
{
  return consumer_admin_->obtain_push_supplier ();
}



CosEventChannelAdmin::ProxyPullSupplier_ptr
TAO_NotifyLogFactory_i::obtain_pull_supplier (
      )

{
  return consumer_admin_->obtain_pull_supplier ();
}

void
TAO_NotifyLogFactory_i::destroy (void)
{
  // No-Op.
}

//CosNotifyComm::NotifySubscribe
void
TAO_NotifyLogFactory_i::subscription_change (const CosNotification::EventTypeSeq& added,
                          const CosNotification::EventTypeSeq& removed)
{
  consumer_admin_->subscription_change (added, removed);
}

CosNotification::QoSProperties*
TAO_NotifyLogFactory_i::get_qos (void)
{
  return consumer_admin_->get_qos ();
}

void
TAO_NotifyLogFactory_i::set_qos (const CosNotification::QoSProperties& qos)
{
  consumer_admin_->set_qos (qos);
}

void
TAO_NotifyLogFactory_i::validate_qos (const CosNotification::QoSProperties& required_qos,
                   CosNotification::NamedPropertyRangeSeq_out available_qos)
{
  consumer_admin_->validate_qos (required_qos, available_qos);
}

CosNotifyFilter::FilterID
TAO_NotifyLogFactory_i::add_filter (CosNotifyFilter::Filter_ptr filter)
{
  return consumer_admin_->add_filter (filter);
}

void
TAO_NotifyLogFactory_i::remove_filter (CosNotifyFilter::FilterID id)
{
  consumer_admin_->remove_filter (id);
}

CosNotifyFilter::Filter_ptr
TAO_NotifyLogFactory_i::get_filter (CosNotifyFilter::FilterID id)
{
  return consumer_admin_->get_filter (id);
}

CosNotifyFilter::FilterIDSeq*
TAO_NotifyLogFactory_i::get_all_filters (void)
{
  return consumer_admin_->get_all_filters ();
}

void
TAO_NotifyLogFactory_i::remove_all_filters (void)
{
  consumer_admin_->remove_all_filters ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
