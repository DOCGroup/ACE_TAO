// $Id$

#include "orbsvcs/Log/NotifyLog_i.h"
#include "orbsvcs/Log/LogMgr_i.h"
#include "orbsvcs/Log/LogNotification.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_NotifyLog_i::TAO_NotifyLog_i (CORBA::ORB_ptr orb,
                                  PortableServer::POA_ptr poa,
                                  TAO_LogMgr_i &logmgr_i,
                                  DsLogAdmin::LogMgr_ptr factory,
                                  CosNotifyChannelAdmin::EventChannelFactory_ptr ecf,
                                  TAO_LogNotification *log_notifier,
                                  DsLogAdmin::LogId id)
  : TAO_Log_i (orb, logmgr_i, factory, id, log_notifier),
    notify_factory_ (CosNotifyChannelAdmin::EventChannelFactory::_duplicate (ecf)),
    poa_ (PortableServer::POA::_duplicate (poa))
{

  CosNotifyChannelAdmin::ChannelID channel_id;

  CosNotification::QoSProperties initial_qos;
  CosNotification::AdminProperties initial_admin;


  ACE_ASSERT (!CORBA::is_nil (this->notify_factory_.in ()));

  this->event_channel_ =
          this->notify_factory_->create_channel (initial_qos,
                                                 initial_admin,
                                                 channel_id);
}

TAO_NotifyLog_i::~TAO_NotifyLog_i ()
{
  this->event_channel_->destroy ();
}

DsLogAdmin::Log_ptr
TAO_NotifyLog_i::copy (DsLogAdmin::LogId &id)
{
  DsNotifyLogAdmin::NotifyLogFactory_var notifyLogFactory =
    DsNotifyLogAdmin::NotifyLogFactory::_narrow (factory_.in ());

  CosNotification::QoSProperties* qos = get_qos ();

  CosNotification::AdminProperties* admin = get_admin ();

  DsNotifyLogAdmin::NotifyLog_var log =
    notifyLogFactory->create (DsLogAdmin::halt, 0, thresholds_, static_cast<const CosNotification::QoSProperties> (*qos),
                               static_cast<const CosNotification::AdminProperties> (*admin), id);

  this->copy_attributes (log.in ());

  return log._retn ();
}

DsLogAdmin::Log_ptr
TAO_NotifyLog_i::copy_with_id (DsLogAdmin::LogId id)
{
  DsNotifyLogAdmin::NotifyLogFactory_var notifyLogFactory =
    DsNotifyLogAdmin::NotifyLogFactory::_narrow (factory_.in ());

  CosNotification::QoSProperties* qos = get_qos ();

  CosNotification::AdminProperties* admin = get_admin ();

  DsNotifyLogAdmin::NotifyLog_var log =
    notifyLogFactory->create_with_id (id, DsLogAdmin::halt, 0, thresholds_, static_cast<const CosNotification::QoSProperties> (*qos),
                               static_cast<const CosNotification::AdminProperties> (*admin));

  this->copy_attributes (log.in ());

  return log._retn ();
}


void
TAO_NotifyLog_i::destroy (void)
{
  notifier_->object_deletion (logid_);

  // Remove ourselves from the list of logs.
  this->logmgr_i_.remove (this->logid_);

  // Deregister with POA.
  PortableServer::ObjectId_var id =
    this->poa_->servant_to_id (this);

  this->poa_->deactivate_object (id.in ());
}

void
TAO_NotifyLog_i::activate (void)
{

  CosNotifyChannelAdmin::AdminID adminid = 0;
  CosNotifyChannelAdmin::InterFilterGroupOperator ifgop =
    CosNotifyChannelAdmin::OR_OP;

  this->consumer_admin_ =
    this->event_channel_->new_for_consumers (ifgop, adminid);

  ACE_ASSERT (!CORBA::is_nil (consumer_admin_.in ()));

  // Setup the CA to receive all type of events
  CosNotification::EventTypeSeq added(1);
  CosNotification::EventTypeSeq removed (0);
  added.length (1);
  removed.length (0);

  added[0].domain_name =  CORBA::string_dup ("*");
  added[0].type_name = CORBA::string_dup ("*");

  this->consumer_admin_->subscription_change (added, removed);

  ACE_NEW_THROW_EX (this->my_log_consumer_,
                    TAO_Notify_LogConsumer (this),
                    CORBA::NO_MEMORY ());

  this->my_log_consumer_->connect (this->consumer_admin_.in ());
}

//IDL to C++
CosNotifyFilter::Filter_ptr
TAO_NotifyLog_i::get_filter (void)
{
  //TODO: need to add impl
  throw CORBA::NO_IMPLEMENT ();
}

void
TAO_NotifyLog_i::set_filter (CosNotifyFilter::Filter_ptr /* filter */)
{
  throw CORBA::NO_IMPLEMENT ();

  //TODO: need to add impl
}

CosNotifyChannelAdmin::EventChannelFactory_ptr
TAO_NotifyLog_i::MyFactory (void)
{
  //TODO: need to add impl
  throw CORBA::NO_IMPLEMENT ();
}

CosNotifyChannelAdmin::ConsumerAdmin_ptr
TAO_NotifyLog_i::default_consumer_admin (void)
{
  return this->event_channel_->default_consumer_admin ();
}

CosNotifyChannelAdmin::SupplierAdmin_ptr
TAO_NotifyLog_i::default_supplier_admin (void)
{
  return this->event_channel_->default_supplier_admin ();
}

CosNotifyFilter::FilterFactory_ptr
TAO_NotifyLog_i::default_filter_factory (void)
{
  return this->event_channel_->default_filter_factory ();
}

CosNotifyChannelAdmin::ConsumerAdmin_ptr
TAO_NotifyLog_i::new_for_consumers (CosNotifyChannelAdmin::InterFilterGroupOperator op,
    CosNotifyChannelAdmin::AdminID& id)
{
  return this->event_channel_->new_for_consumers (op,id);
}

CosNotifyChannelAdmin::SupplierAdmin_ptr
TAO_NotifyLog_i::new_for_suppliers (CosNotifyChannelAdmin::InterFilterGroupOperator op,
    CosNotifyChannelAdmin::AdminID& id)
{
  return this->event_channel_->new_for_suppliers (op,id);
}

CosNotifyChannelAdmin::ConsumerAdmin_ptr
TAO_NotifyLog_i::get_consumeradmin (CosNotifyChannelAdmin::AdminID id)
{
  return this->event_channel_->get_consumeradmin (id);
}

CosNotifyChannelAdmin::SupplierAdmin_ptr
TAO_NotifyLog_i::get_supplieradmin (CosNotifyChannelAdmin::AdminID id)
{
  return this->event_channel_->get_supplieradmin (id);
}

CosNotifyChannelAdmin::AdminIDSeq*
TAO_NotifyLog_i::get_all_consumeradmins (void)
{
  return this->event_channel_->get_all_consumeradmins ();
}

CosNotifyChannelAdmin::AdminIDSeq*
TAO_NotifyLog_i::get_all_supplieradmins (void)
{
  return this->event_channel_->get_all_supplieradmins ();
}

CosNotification::AdminProperties*
TAO_NotifyLog_i::get_admin (void)
{
  return this->event_channel_->get_admin ();
}

void
TAO_NotifyLog_i::set_admin (const CosNotification::AdminProperties& admin)
{
  this->event_channel_->set_admin (admin);
}

CosNotification::QoSProperties*
TAO_NotifyLog_i::get_qos (void)
{
  //need to add merging of QoS from Log_i and EventChannel_i
  throw CORBA::NO_IMPLEMENT ();
}

void
TAO_NotifyLog_i::set_qos (const CosNotification::QoSProperties& /* qos */)
{
  throw CORBA::NO_IMPLEMENT ();
  //TODO: need to add later
}

void
TAO_NotifyLog_i::validate_qos (
    const CosNotification::QoSProperties& /* required_qos */,
    CosNotification::NamedPropertyRangeSeq_out /* available_qos */)
{
  throw CORBA::NO_IMPLEMENT ();

  //TODO: need to add later
}

CosEventChannelAdmin::ConsumerAdmin_ptr
TAO_NotifyLog_i::for_consumers (void)
{
  return this->event_channel_->for_consumers();
}

CosEventChannelAdmin::SupplierAdmin_ptr
TAO_NotifyLog_i::for_suppliers (
    )
{
  return this->event_channel_->for_suppliers();
}

TAO_END_VERSIONED_NAMESPACE_DECL
