#include "orbsvcs/Log/EventLogFactory_i.h"
#include "orbsvcs/Log/LogNotification.h"
#include "orbsvcs/Log/EventLogNotification.h"
#include "ace/Auto_Ptr.h"

ACE_RCSID (Log,
           EventLogFactory_i,
           "$Id$")


TAO_EventLogFactory_i::TAO_EventLogFactory_i (void)
{
  TAO_CEC_Default_Factory::init_svcs ();

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      PortableServer::POA_var defPOA =
        this->_default_POA (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      TAO_CEC_EventChannel_Attributes attr (defPOA.in (), defPOA.in ());

      ACE_NEW_THROW_EX (impl,
                        TAO_CEC_EventChannel (attr),
                        CORBA::NO_MEMORY ());
      ACE_TRY_CHECK;

      impl->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Caught exception in "
                           "EventLogFactory_i constructor.");
    }
  ACE_ENDTRY;
  ACE_CHECK;
}

TAO_EventLogFactory_i::~TAO_EventLogFactory_i (void)
{
  // No-Op.
}

CosEventChannelAdmin::EventChannel_ptr
TAO_EventLogFactory_i::init (PortableServer::POA_ptr /* poa */
                             ACE_ENV_ARG_DECL)
{

  ACE_ASSERT (!CORBA::is_nil (this->poa_.in ()));

  CosEventChannelAdmin::EventChannel_var ec_return;

  auto_ptr <TAO_CEC_EventChannel> ec (impl);

  PortableServer::ObjectId_var oid =
    this->poa_->activate_object (ec.get ()
                                 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CosEventChannelAdmin::EventChannel::_nil ());

  ec.release ();

  CORBA::Object_var obj =
    this->poa_->id_to_reference (oid.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CosEventChannelAdmin::EventChannel::_nil ());

  ec_return =
    CosEventChannelAdmin::EventChannel::_narrow (obj.in ()
                                                 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CosEventChannelAdmin::EventChannel::_nil ());

  return ec_return._retn ();
}

DsEventLogAdmin::EventLogFactory_ptr
TAO_EventLogFactory_i::activate (PortableServer::POA_ptr poa
                                 ACE_ENV_ARG_DECL)
{
  this->poa_ = poa;
  this->event_channel_ = init (this->poa_.in () ACE_ENV_ARG_PARAMETER);

  this->consumer_admin_ =
    this->event_channel_->for_consumers (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (DsEventLogAdmin::EventLogFactory::_nil ());

  ACE_NEW_THROW_EX (this->notifier_,
                    TAO_EventLogNotification(this->event_channel_.in ()),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (DsEventLogAdmin::EventLogFactory::_nil ());

  DsEventLogAdmin::EventLogFactory_var v_return;

  PortableServer::ObjectId_var oid =
    poa->activate_object (this
                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (DsEventLogAdmin::EventLogFactory::_nil ());

  CORBA::Object_var obj =
    poa->id_to_reference (oid.in ()
                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (DsEventLogAdmin::EventLogFactory::_nil ());

  // narrow and store the result..
  this->log_mgr_ =
    DsLogAdmin::LogMgr::_narrow (obj.in ()
                                 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (DsEventLogAdmin::EventLogFactory::_nil ());

  v_return =
    DsEventLogAdmin::EventLogFactory::_narrow (obj.in ()
                                               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (DsEventLogAdmin::EventLogFactory::_nil ());

  return v_return._retn ();
}

DsEventLogAdmin::EventLog_ptr
TAO_EventLogFactory_i::create (
    DsLogAdmin::LogFullActionType full_action,
    CORBA::ULongLong max_rec_size,
    const DsLogAdmin::CapacityAlarmThresholdList & thresholds,
    DsLogAdmin::LogId_out id
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   DsLogAdmin::InvalidLogFullAction,
                   DsLogAdmin::InvalidThreshold))
{
  // Get an id for this Log.
  this->max_id_++;

  DsEventLogAdmin::EventLog_ptr eventlog =
    this->create_with_id (this->max_id_,
                          full_action,
                          max_rec_size,
                          thresholds
                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (DsEventLogAdmin::EventLog::_nil ());

  // Set the id to return..
  id = this->max_id_;

  // Store the id in the LogIdList.
  CORBA::ULong len = logid_list_.length();
  logid_list_.length(len+1);
  logid_list_[len] = id;

  return eventlog;
}

DsEventLogAdmin::EventLog_ptr
TAO_EventLogFactory_i::create_with_id (
    DsLogAdmin::LogId id,
    DsLogAdmin::LogFullActionType full_action,
    CORBA::ULongLong max_size,
    const DsLogAdmin::CapacityAlarmThresholdList & /* thresholds */
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   DsLogAdmin::LogIdAlreadyExists,
                   DsLogAdmin::InvalidLogFullAction,
                   DsLogAdmin::InvalidThreshold))
{
  // Make sure the id not used up.
  if (hash_map_.find (id) == 0)
    ACE_THROW_RETURN (DsLogAdmin::LogIdAlreadyExists (),
                      DsEventLogAdmin::EventLog::_nil ());

  DsEventLogAdmin::EventLog_var event_log;
  // Object to return.

  TAO_EventLog_i* event_log_i;

  ACE_NEW_THROW_EX (event_log_i,
                    TAO_EventLog_i (*this,
                                    this->log_mgr_.in (),
                                    this,
                                    this->notifier_,
                                    id,
                                    full_action,
                                    max_size
                                    ),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (DsEventLogAdmin::EventLog::_nil ());

  PortableServer::ServantBase_var safe_event_log_i = event_log_i;
  //Transfer ownership to the POA.

  event_log_i->init (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (DsEventLogAdmin::EventLog::_nil ());

  //dhanvey
  //initialise the LogConsumer object
  event_log_i->activate (ACE_ENV_SINGLE_ARG_PARAMETER);

  // Register with the poa
  event_log = event_log_i->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (DsEventLogAdmin::EventLog::_nil ());

  // Add to the Hash table..
  if (hash_map_.bind (id, DsEventLogAdmin::EventLog::_duplicate (event_log.in ())) == -1)
    ACE_THROW_RETURN (CORBA::INTERNAL (),
                      DsEventLogAdmin::EventLog::_nil ());

  notifier_->object_creation (DsEventLogAdmin::EventLog::_duplicate (event_log.in ()), id
                              ACE_ENV_ARG_PARAMETER);
  //ACE_CHECK_RETURN (DsEventLogAdmin::EventLog::_nil ());

  return event_log._retn ();
}

CosEventChannelAdmin::ProxyPushSupplier_ptr
TAO_EventLogFactory_i::obtain_push_supplier (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return consumer_admin_->obtain_push_supplier (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CosEventChannelAdmin::ProxyPullSupplier_ptr
TAO_EventLogFactory_i::obtain_pull_supplier (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))

{
  return consumer_admin_->obtain_pull_supplier (ACE_ENV_SINGLE_ARG_PARAMETER);
}


