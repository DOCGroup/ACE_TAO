#include "orbsvcs/Log/NotifyLog_i.h"
#include "orbsvcs/Log/LogMgr_i.h"
#include "orbsvcs/Log/LogNotification.h"

ACE_RCSID (Log,
           NotifyLog_i,
           "$Id$")


#define CA_FILTER "threshold > 10"
#define TCL_GRAMMAR "TCL"

TAO_NotifyLog_i::TAO_NotifyLog_i (TAO_LogMgr_i &logmgr_i,
                                  DsLogAdmin::LogMgr_ptr factory,
                                  TAO_NotifyLogFactory_i *notify_log_factory,
                                  CosNotifyChannelAdmin::EventChannelFactory_ptr ecf,
                                  TAO_LogNotification *log_notifier,
                                  DsLogAdmin::LogId id,
                                  DsLogAdmin::LogFullActionType log_full_action,
                                  CORBA::ULongLong max_size,
                                  ACE_Reactor *reactor)
  : TAO_Log_i (factory, id, log_notifier, log_full_action, max_size, reactor),
  logmgr_i_(logmgr_i), notify_factory_ (CosNotifyChannelAdmin::EventChannelFactory::_duplicate (ecf))
{
  ACE_UNUSED_ARG (notify_log_factory);

  ACE_DECLARE_NEW_CORBA_ENV;

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

TAO_NotifyLog_i::~TAO_NotifyLog_i ()
{
  // No-Op.
}

DsLogAdmin::Log_ptr
TAO_NotifyLog_i::copy (DsLogAdmin::LogId &id ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
  DsNotifyLogAdmin::NotifyLogFactory_var notifyLogFactory =
    DsNotifyLogAdmin::NotifyLogFactory::_narrow (factory_.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (DsLogAdmin::Log::_nil ());

  CosNotification::QoSProperties* qos = get_qos (ACE_ENV_SINGLE_ARG_PARAMETER);
  CosNotification::AdminProperties* admin = get_admin (ACE_ENV_SINGLE_ARG_PARAMETER);

  DsNotifyLogAdmin::NotifyLog_var log =
    notifyLogFactory->create (DsLogAdmin::halt, 0, thresholds_, ACE_static_cast(const CosNotification::QoSProperties, *qos),
                               ACE_static_cast(const CosNotification::AdminProperties, *admin), id);

  this->copy_attributes (log.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (DsLogAdmin::Log::_nil ());

  return log._retn ();
}

DsLogAdmin::Log_ptr
TAO_NotifyLog_i::copy_with_id (DsLogAdmin::LogId id ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
  DsNotifyLogAdmin::NotifyLogFactory_var notifyLogFactory =
    DsNotifyLogAdmin::NotifyLogFactory::_narrow (factory_.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (DsLogAdmin::Log::_nil ());

  CosNotification::QoSProperties* qos = get_qos (ACE_ENV_SINGLE_ARG_PARAMETER);
  CosNotification::AdminProperties* admin = get_admin (ACE_ENV_SINGLE_ARG_PARAMETER);

  DsNotifyLogAdmin::NotifyLog_var log =
    notifyLogFactory->create_with_id (id, DsLogAdmin::halt, 0, thresholds_, ACE_static_cast(const CosNotification::QoSProperties, *qos),
                               ACE_static_cast(const CosNotification::AdminProperties, *admin));

  this->copy_attributes (log.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (DsLogAdmin::Log::_nil ());

  return log._retn ();
}


void
TAO_NotifyLog_i::destroy (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  notifier_->object_deletion (logid_ ACE_ENV_ARG_PARAMETER);
  // Remove ourselves from the list of logs.
  this->logmgr_i_.remove (this->logid_); // check for error?

  // Deregister with POA.
  PortableServer::POA_var poa =
    this->_default_POA (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  PortableServer::ObjectId_var id =
    poa->servant_to_id (this
                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  poa->deactivate_object (id.in ()
                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_NotifyLog_i::activate (ACE_ENV_SINGLE_ARG_DECL)
{

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
                    TAO_Notify_LogConsumer (this),
                    CORBA::NO_MEMORY ());

  this->my_log_consumer_->connect (this->consumer_admin_.in () ACE_ENV_ARG_PARAMETER);
}

//IDL to C++
CosNotifyFilter::Filter_ptr
TAO_NotifyLog_i::get_filter (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
  //TODO: need to add impl
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

void
TAO_NotifyLog_i::set_filter (CosNotifyFilter::Filter_ptr filter ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
  ACE_UNUSED_ARG (filter);

  ACE_THROW (CORBA::NO_IMPLEMENT ());

  //TODO: need to add impl
}

CosNotifyChannelAdmin::EventChannelFactory_ptr
TAO_NotifyLog_i::MyFactory (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
  //TODO: need to add impl
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

CosNotifyChannelAdmin::ConsumerAdmin_ptr
TAO_NotifyLog_i::default_consumer_admin (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
  return this->event_channel_->default_consumer_admin (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CosNotifyChannelAdmin::SupplierAdmin_ptr
TAO_NotifyLog_i::default_supplier_admin (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
  return this->event_channel_->default_supplier_admin (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CosNotifyFilter::FilterFactory_ptr
TAO_NotifyLog_i::default_filter_factory (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
  return this->event_channel_->default_filter_factory (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CosNotifyChannelAdmin::ConsumerAdmin_ptr
TAO_NotifyLog_i::new_for_consumers (CosNotifyChannelAdmin::InterFilterGroupOperator op,
    CosNotifyChannelAdmin::AdminID& id ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
  return this->event_channel_->new_for_consumers (op,id ACE_ENV_ARG_PARAMETER);
}

CosNotifyChannelAdmin::SupplierAdmin_ptr
TAO_NotifyLog_i::new_for_suppliers (CosNotifyChannelAdmin::InterFilterGroupOperator op,
    CosNotifyChannelAdmin::AdminID& id ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
  return this->event_channel_->new_for_suppliers (op,id ACE_ENV_ARG_PARAMETER);
}

CosNotifyChannelAdmin::ConsumerAdmin_ptr
TAO_NotifyLog_i::get_consumeradmin (CosNotifyChannelAdmin::AdminID id ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
      CosNotifyChannelAdmin::AdminNotFound,
      CORBA::SystemException
    ))
{
  return this->event_channel_->get_consumeradmin (id ACE_ENV_ARG_PARAMETER);
}

CosNotifyChannelAdmin::SupplierAdmin_ptr
TAO_NotifyLog_i::get_supplieradmin (CosNotifyChannelAdmin::AdminID id ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
      CosNotifyChannelAdmin::AdminNotFound,
      CORBA::SystemException
    ))
{
  return this->event_channel_->get_supplieradmin (id ACE_ENV_ARG_PARAMETER);
}

CosNotifyChannelAdmin::AdminIDSeq*
TAO_NotifyLog_i::get_all_consumeradmins (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
  return this->event_channel_->get_all_consumeradmins (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CosNotifyChannelAdmin::AdminIDSeq*
TAO_NotifyLog_i::get_all_supplieradmins (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
  return this->event_channel_->get_all_supplieradmins (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CosNotification::AdminProperties*
TAO_NotifyLog_i::get_admin (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
  return this->event_channel_->get_admin (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_NotifyLog_i::set_admin (const CosNotification::AdminProperties& admin ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
      CosNotification::UnsupportedAdmin,
      CORBA::SystemException
    ))
{
  this->event_channel_->set_admin (admin ACE_ENV_ARG_PARAMETER);
}

CosNotification::QoSProperties*
TAO_NotifyLog_i::get_qos (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
  //need to add merging of QoS from Log_i and EventChannel_i
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

void
TAO_NotifyLog_i::set_qos (const CosNotification::QoSProperties& qos ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
      CosNotification::UnsupportedQoS,
      CORBA::SystemException
    ))
{
  ACE_UNUSED_ARG (qos);

  ACE_THROW (CORBA::NO_IMPLEMENT ());
  //TODO: need to add later
}

void
TAO_NotifyLog_i::validate_qos (const CosNotification::QoSProperties& required_qos,
    CosNotification::NamedPropertyRangeSeq_out available_qos ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
      CosNotification::UnsupportedQoS,
      CORBA::SystemException
    ))
{
  ACE_UNUSED_ARG (required_qos);

  ACE_UNUSED_ARG (available_qos);

  ACE_THROW (CORBA::NO_IMPLEMENT ());

  //TODO: need to add later
}

CosEventChannelAdmin::ConsumerAdmin_ptr
TAO_NotifyLog_i::for_consumers (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
  return this->event_channel_->for_consumers(ACE_ENV_SINGLE_ARG_PARAMETER);
}

CosEventChannelAdmin::SupplierAdmin_ptr
TAO_NotifyLog_i::for_suppliers (
      ACE_ENV_SINGLE_ARG_DECL
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
  return this->event_channel_->for_suppliers(ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_NotifyLog_i::write_recordlist (const DsLogAdmin::RecordList & list
                                   ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   DsLogAdmin::LogFull,
                   DsLogAdmin::LogLocked
  ))
{
  TAO_Log_i::write_recordlist (list ACE_ENV_ARG_PARAMETER);
}
