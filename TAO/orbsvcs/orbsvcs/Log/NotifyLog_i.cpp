/* -*- C++ -*- $Id$ */

#include "orbsvcs/Log/NotifyLog_i.h"
#include "orbsvcs/Log/LogMgr_i.h"
#include "orbsvcs/Log/LogNotification.h"

#define CA_FILTER "threshold > 10"
#define TCL_GRAMMAR "TCL"

LogConsumer::LogConsumer (NotifyLog_i *log)
: log_ (log)
{
  // No-Op.
}

LogConsumer::~LogConsumer (void)
{
  // No-Op.
}

void
LogConsumer::connect (CosNotifyChannelAdmin::ConsumerAdmin_ptr consumer_admin)
{
  // Activate the consumer with the default_POA_
  CosNotifyComm::StructuredPushConsumer_var objref =
    this->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  CosNotifyChannelAdmin::ProxySupplier_var proxysupplier =
    consumer_admin->obtain_notification_push_supplier (CosNotifyChannelAdmin::STRUCTURED_EVENT, proxy_supplier_id_ ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ACE_ASSERT (!CORBA::is_nil (proxysupplier.in ()));

  // narrow
  this->proxy_supplier_ =
    CosNotifyChannelAdmin::StructuredProxyPushSupplier::
    _narrow (proxysupplier.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ACE_ASSERT (!CORBA::is_nil (proxy_supplier_.in ()));

  proxy_supplier_->connect_structured_push_consumer (objref.in ()
                                                     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

}

void
LogConsumer::disconnect (ACE_ENV_SINGLE_ARG_DECL)
{
  this->proxy_supplier_->
    disconnect_structured_push_supplier(ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

void
LogConsumer::offer_change
   (const CosNotification::EventTypeSeq & /*added*/,
    const CosNotification::EventTypeSeq & /*removed*/
    ACE_ENV_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        CosNotifyComm::InvalidEventType
      ))
{
  // No-Op.
}

void
LogConsumer::push_structured_event
   (const CosNotification::StructuredEvent & notification
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosEventComm::Disconnected
                   ))
{
  CORBA::Long val;

  notification.remainder_of_body >>= val;

  CORBA::Any any;

  any <<= val;

  // create a record list...
  DsLogAdmin::RecordList recList (1);
  recList.length (1);

  recList [0].info = any;

  this->log_->write_recordlist (recList ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
LogConsumer::disconnect_structured_push_consumer
   (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  // No-Op.
}

NotifyLog_i::NotifyLog_i (LogMgr_i &logmgr_i,
                        DsLogAdmin::LogMgr_ptr factory,
                        NotifyLogFactory_i *notify_log_factory,
                        CosNotifyChannelAdmin::EventChannelFactory_ptr ecf,
                        LogNotification *log_notifier,
                        DsLogAdmin::LogId id,
                        DsLogAdmin::LogFullActionType log_full_action,
                        CORBA::ULongLong max_size,
                        ACE_Reactor *reactor)
  : Log_i (factory, id, log_notifier, log_full_action, max_size, reactor),
  logmgr_i_(logmgr_i), notify_factory_ (CosNotifyChannelAdmin::EventChannelFactory::_duplicate (ecf))
{
 ACE_UNUSED_ARG (notify_log_factory);

  ACE_ENV_SINGLE_ARG_DECL

  CosNotifyChannelAdmin::ChannelID channel_id;

  CosNotification::QoSProperties initial_qos;
  CosNotification::AdminProperties initial_admin;


  ACE_ASSERT (!CORBA::is_nil (this->notify_factory_.in ()));

  this->event_channel_ =
          this->notify_factory_->create_channel (initial_qos,
                                                 initial_admin,
                                                 channel_id
                                                 ACE_ENV_ARG_PARAMETER);
}

NotifyLog_i::~NotifyLog_i ()
{
  // No-Op.
}

DsLogAdmin::Log_ptr 
NotifyLog_i::copy (DsLogAdmin::LogId &id)
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
  DsNotifyLogAdmin::NotifyLogFactory_var notifyLogFactory =
    DsNotifyLogAdmin::NotifyLogFactory::_narrow (factory_.in ());

  CosNotification::QoSProperties* qos = get_qos ();
  CosNotification::AdminProperties* admin = get_admin ();

  DsNotifyLogAdmin::NotifyLog_var log = 
    notifyLogFactory->create (DsLogAdmin::halt, 0, thresholds_, ACE_static_cast(const CosNotification::QoSProperties, *qos),
			       ACE_static_cast(const CosNotification::AdminProperties, *admin), id);

  copy_attributes (log.in ());

  return log._retn ();
}

DsLogAdmin::Log_ptr 
NotifyLog_i::copy_with_id (DsLogAdmin::LogId id)
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
  DsNotifyLogAdmin::NotifyLogFactory_var notifyLogFactory =
    DsNotifyLogAdmin::NotifyLogFactory::_narrow (factory_.in ());

  CosNotification::QoSProperties* qos = get_qos ();
  CosNotification::AdminProperties* admin = get_admin ();

  DsNotifyLogAdmin::NotifyLog_var log = 
    notifyLogFactory->create_with_id (id, DsLogAdmin::halt, 0, thresholds_, ACE_static_cast(const CosNotification::QoSProperties, *qos),
			       ACE_static_cast(const CosNotification::AdminProperties, *admin));

  copy_attributes (log.in ());

  return log._retn ();
}


void
NotifyLog_i::destroy (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  notifier_->object_deletion (logid_);
  // Remove ourselves from the list of logs.
  this->logmgr_i_.remove (this->logid_); // check for error?

  // Deregister with POA.
  PortableServer::POA_var poa =
    this->_default_POA (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  PortableServer::ObjectId_var id =
    poa->servant_to_id (this//,
                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  poa->deactivate_object (id.in ()//,
                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
NotifyLog_i::activate (void)
{
  ACE_ENV_SINGLE_ARG_DECL

  CosNotifyChannelAdmin::AdminID adminid = 0;
  CosNotifyChannelAdmin::InterFilterGroupOperator ifgop = 
    CosNotifyChannelAdmin::OR_OP;

  this->consumer_admin_ =
    this->event_channel_->new_for_consumers (ifgop, adminid ACE_ENV_ARG_PARAMETER);
  ACE_ASSERT (!CORBA::is_nil (consumer_admin_.in ()));

  CosNotifyFilter::FilterFactory_var ffact =
    this->event_channel_->default_filter_factory (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // setup a filter at the consumer admin
  CosNotifyFilter::Filter_var ca_filter =
    ffact->create_filter (TCL_GRAMMAR ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ACE_ASSERT (!CORBA::is_nil (ca_filter.in ()));

  CosNotifyFilter::ConstraintExpSeq constraint_list (1);
  constraint_list.length (1);

  constraint_list[0].event_types.length (0);
  constraint_list[0].constraint_expr = CORBA::string_dup (CA_FILTER);

  ca_filter->add_constraints (constraint_list ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  consumer_admin_->add_filter (ca_filter.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Setup the CA to receive all type of events
  CosNotification::EventTypeSeq added(1);
  CosNotification::EventTypeSeq removed (0);
  added.length (1);
  removed.length (0);

  added[0].domain_name =  CORBA::string_dup ("*");
  added[0].type_name = CORBA::string_dup ("*");

  this->consumer_admin_->subscription_change (added, removed ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ACE_NEW_THROW_EX (this->my_log_consumer_,
                    LogConsumer (this),
                    CORBA::NO_MEMORY ()); 

  this->my_log_consumer_->connect (this->consumer_admin_.in ());         
}

//IDL to C++
CosNotifyFilter::Filter_ptr
NotifyLog_i::get_filter (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{  
  //TODO: need to add impl
  //dummy impl to get it to compile
  CosNotifyFilter::Filter_ptr filter;
  return filter;
}

void
NotifyLog_i::set_filter (CosNotifyFilter::Filter_ptr filter ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
  ACE_UNUSED_ARG (filter);

  //TODO: need to add impl
}

CosNotifyChannelAdmin::EventChannelFactory_ptr
NotifyLog_i::MyFactory (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
  //TODO: need to add impl
  //dummy impl to compile
  CosNotifyChannelAdmin::EventChannelFactory_ptr ecf;
  return ecf;
}

CosNotifyChannelAdmin::ConsumerAdmin_ptr
NotifyLog_i::default_consumer_admin (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
  return this->event_channel_->default_consumer_admin();
}

CosNotifyChannelAdmin::SupplierAdmin_ptr
NotifyLog_i::default_supplier_admin (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
  return this->event_channel_->default_supplier_admin();
}

CosNotifyFilter::FilterFactory_ptr
NotifyLog_i::default_filter_factory (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
  return this->event_channel_->default_filter_factory();
}

CosNotifyChannelAdmin::ConsumerAdmin_ptr
NotifyLog_i::new_for_consumers (CosNotifyChannelAdmin::InterFilterGroupOperator op,
    CosNotifyChannelAdmin::AdminID& id ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
  return this->event_channel_->new_for_consumers(op,id);
}

CosNotifyChannelAdmin::SupplierAdmin_ptr
NotifyLog_i::new_for_suppliers (CosNotifyChannelAdmin::InterFilterGroupOperator op,
    CosNotifyChannelAdmin::AdminID& id ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{  
  return this->event_channel_->new_for_suppliers(op,id);
}

CosNotifyChannelAdmin::ConsumerAdmin_ptr
NotifyLog_i::get_consumeradmin (CosNotifyChannelAdmin::AdminID id ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
      CosNotifyChannelAdmin::AdminNotFound, 
      CORBA::SystemException
    ))
{
  return this->event_channel_->get_consumeradmin(id);
}

CosNotifyChannelAdmin::SupplierAdmin_ptr
NotifyLog_i::get_supplieradmin (CosNotifyChannelAdmin::AdminID id ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
      CosNotifyChannelAdmin::AdminNotFound, 
      CORBA::SystemException
    ))
{
  return this->event_channel_->get_supplieradmin(id);
}

CosNotifyChannelAdmin::AdminIDSeq*
NotifyLog_i::get_all_consumeradmins (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
  return this->event_channel_->get_all_consumeradmins();
}

CosNotifyChannelAdmin::AdminIDSeq*
NotifyLog_i::get_all_supplieradmins (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
  return this->event_channel_->get_all_supplieradmins();
}

CosNotification::AdminProperties*
NotifyLog_i::get_admin (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
  return this->event_channel_->get_admin();
}

void
NotifyLog_i::set_admin (const CosNotification::AdminProperties& admin ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
      CosNotification::UnsupportedAdmin, 
      CORBA::SystemException
    ))
{
  this->event_channel_->set_admin(admin);
}

CosNotification::QoSProperties*
NotifyLog_i::get_qos (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
  //need to add merging of QoS from Log_i and EventChannel_i
  CosNotification::QoSProperties* temp;
  return temp;
}

void
NotifyLog_i::set_qos (const CosNotification::QoSProperties& qos ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
      CosNotification::UnsupportedQoS, 
      CORBA::SystemException
    ))
{
  ACE_UNUSED_ARG (qos);

  //TODO: need to add later
}

void
NotifyLog_i::validate_qos (const CosNotification::QoSProperties& required_qos,
    CosNotification::NamedPropertyRangeSeq_out available_qos ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
      CosNotification::UnsupportedQoS, 
      CORBA::SystemException
    ))
{
  ACE_UNUSED_ARG (required_qos);
 
  ACE_UNUSED_ARG (available_qos);

  //TODO: need to add later
}

CosEventChannelAdmin::ConsumerAdmin_ptr 
NotifyLog_i::for_consumers (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
  return this->event_channel_->for_consumers(ACE_ENV_SINGLE_ARG_PARAMETER);
}

CosEventChannelAdmin::SupplierAdmin_ptr 
NotifyLog_i::for_suppliers (
      ACE_ENV_SINGLE_ARG_DECL
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
  return this->event_channel_->for_suppliers(ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
NotifyLog_i::write_recordlist (const DsLogAdmin::RecordList & list//,
                              ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   DsLogAdmin::LogFull,
                   DsLogAdmin::LogLocked
  ))
{
  Log_i::write_recordlist (list ACE_ENV_ARG_PARAMETER);
}
