#include "orbsvcs/Log/RTEventLog_i.h"
#include "orbsvcs/Log/LogMgr_i.h"
#include "orbsvcs/Log/LogNotification.h"
#include "orbsvcs/orbsvcs/Event_Utilities.h"
#include "orbsvcs/Event/EC_Event_Channel.h"
#include "orbsvcs/Event/EC_Default_Factory.h"
#include "orbsvcs/Event/EC_Dispatching.h"
#include "orbsvcs/Event/EC_ConsumerAdmin.h"
#include "orbsvcs/Event/EC_SupplierAdmin.h"
#include "orbsvcs/Event/EC_Timeout_Generator.h"
#include "orbsvcs/Event/EC_ObserverStrategy.h"
#include "orbsvcs/Event/EC_ConsumerControl.h"
#include "orbsvcs/Event/EC_SupplierControl.h"
#include "ace/Dynamic_Service.h"

ACE_RCSID (Log,
           RTEventLog_i,
           "$Id$")


TAO_RTEventLog_i::TAO_RTEventLog_i (TAO_LogMgr_i &logmgr_i,
                                    DsLogAdmin::LogMgr_ptr factory,
                                    TAO_RTEventLogFactory_i *event_log_factory,
                                    TAO_LogNotification *log_notifier,
                                    DsLogAdmin::LogId id,
                                    DsLogAdmin::LogFullActionType log_full_action,
                                    CORBA::ULongLong max_size,
                                    ACE_Reactor *reactor)
  : TAO_Log_i (factory, id, log_notifier, log_full_action, max_size, reactor),
    logmgr_i_(logmgr_i)
{
  ACE_UNUSED_ARG (event_log_factory);

  ACE_DECLARE_NEW_CORBA_ENV;

  PortableServer::POA_var poa = this->_default_POA (ACE_ENV_SINGLE_ARG_PARAMETER);

  TAO_EC_Event_Channel_Attributes attr (poa.in(), poa.in());

  ACE_NEW_THROW_EX (this->event_channel_,
                    TAO_EC_Event_Channel(attr),
                    CORBA::NO_MEMORY ());
}

TAO_RTEventLog_i::~TAO_RTEventLog_i ()
{
  // No-Op.
}


DsLogAdmin::Log_ptr
TAO_RTEventLog_i::copy (DsLogAdmin::LogId &id ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  RTEventLogAdmin::EventLogFactory_var eventLogFactory =
    RTEventLogAdmin::EventLogFactory::_narrow (factory_.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (DsLogAdmin::Log::_nil ());

  RTEventLogAdmin::EventLog_var log =
    eventLogFactory->create (DsLogAdmin::halt, 0, thresholds_,
                             id ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (DsLogAdmin::Log::_nil ());

  copy_attributes (log.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (DsLogAdmin::Log::_nil ());

  return log._retn ();

}

DsLogAdmin::Log_ptr
TAO_RTEventLog_i::copy_with_id (DsLogAdmin::LogId id ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((DsLogAdmin::LogIdAlreadyExists, CORBA::SystemException))
{
  RTEventLogAdmin::EventLogFactory_var eventLogFactory =
    RTEventLogAdmin::EventLogFactory::_narrow (factory_.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (DsLogAdmin::Log::_nil ());

  RTEventLogAdmin::EventLog_var log =
    eventLogFactory->create_with_id (id, DsLogAdmin::halt, 0,
                                     thresholds_ ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (DsLogAdmin::Log::_nil ());

  copy_attributes (log.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (DsLogAdmin::Log::_nil ());

  return log._retn ();
}

void
TAO_RTEventLog_i::destroy (ACE_ENV_SINGLE_ARG_DECL)
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
TAO_RTEventLog_i::activate (ACE_ENV_SINGLE_ARG_DECL)
{
  RtecEventChannelAdmin::ConsumerAdmin_var consumer_admin =
    this->event_channel_->for_consumers (ACE_ENV_SINGLE_ARG_PARAMETER);

  this->my_log_consumer_ = new TAO_Rtec_LogConsumer (this);
  this->my_log_consumer_->connect (consumer_admin.in ());
}

RtecEventChannelAdmin::ConsumerAdmin_ptr
TAO_RTEventLog_i::for_consumers (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->event_channel_->for_consumers(ACE_ENV_SINGLE_ARG_PARAMETER);
}

RtecEventChannelAdmin::SupplierAdmin_ptr
TAO_RTEventLog_i::for_suppliers (
      ACE_ENV_SINGLE_ARG_DECL
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
  return this->event_channel_->for_suppliers(ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_RTEventLog_i::write_recordlist (const DsLogAdmin::RecordList & list
                              ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   DsLogAdmin::LogFull,
                   DsLogAdmin::LogLocked
  ))
{
  TAO_Log_i::write_recordlist (list ACE_ENV_ARG_PARAMETER);
}

RtecEventChannelAdmin::Observer_Handle
TAO_RTEventLog_i::append_observer (
       RtecEventChannelAdmin::Observer_ptr observer
       ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
        CORBA::SystemException,
        RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR,
        RtecEventChannelAdmin::EventChannel::CANT_APPEND_OBSERVER))
{
  return this->observer_strategy_->append_observer (observer ACE_ENV_ARG_PARAMETER);
}

void
TAO_RTEventLog_i::remove_observer (
       RtecEventChannelAdmin::Observer_Handle handle
       ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
        CORBA::SystemException,
        RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR,
        RtecEventChannelAdmin::EventChannel::CANT_REMOVE_OBSERVER))
{
  this->observer_strategy_->remove_observer (handle ACE_ENV_ARG_PARAMETER);
}
