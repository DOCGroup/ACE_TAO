#include "orbsvcs/Log/EventLog_i.h"
#include "orbsvcs/Log/LogMgr_i.h"
#include "orbsvcs/Log/LogNotification.h"


ACE_RCSID (Log,
           EventLog_i,
           "$Id$")


TAO_EventLog_i::TAO_EventLog_i (TAO_LogMgr_i &logmgr_i,
                                DsLogAdmin::LogMgr_ptr factory,
                                TAO_EventLogFactory_i *event_log_factory,
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

  // Create an instance of the event channel.
  PortableServer::POA_var poa = this->_default_POA (ACE_ENV_SINGLE_ARG_PARAMETER);

  TAO_CEC_EventChannel_Attributes attr (poa.in(), poa.in());

  ACE_NEW_THROW_EX (this->event_channel_,
                    TAO_CEC_EventChannel(attr),
                    CORBA::NO_MEMORY ());
}

TAO_EventLog_i::~TAO_EventLog_i ()
{
  // No-Op.
}


DsLogAdmin::Log_ptr
TAO_EventLog_i::copy (DsLogAdmin::LogId &id ACE_ENV_ARG_DECL)
ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Duplicate the log.
  DsEventLogAdmin::EventLogFactory_var eventLogFactory =
    DsEventLogAdmin::EventLogFactory::_narrow (factory_.in ()
                                               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (DsLogAdmin::Log::_nil ());

  DsEventLogAdmin::EventLog_var log =
    eventLogFactory->create (DsLogAdmin::halt, 0, thresholds_, id
                             ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (DsLogAdmin::Log::_nil ());

  this->copy_attributes (log.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (DsLogAdmin::Log::_nil ());

  return log._retn ();

}

DsLogAdmin::Log_ptr
TAO_EventLog_i::copy_with_id (DsLogAdmin::LogId id ACE_ENV_ARG_DECL)
ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Duplicate the log supplying the log id.
  DsEventLogAdmin::EventLogFactory_var eventLogFactory =
    DsEventLogAdmin::EventLogFactory::_narrow (factory_.in ()
                                               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (DsLogAdmin::Log::_nil ());

  DsEventLogAdmin::EventLog_var log =
    eventLogFactory->create_with_id (id, DsLogAdmin::halt, 0, thresholds_
                                     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (DsLogAdmin::Log::_nil ());

  this->copy_attributes (log.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (DsLogAdmin::Log::_nil ());

  return log._retn ();

}


void
TAO_EventLog_i::destroy (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Send event to indicate the log has been deleted.
  notifier_->object_deletion (logid_ ACE_ENV_ARG_PARAMETER);

  // Remove ourselves from the list of logs.
  this->logmgr_i_.remove (this->logid_); // check for error?
  ACE_CHECK;

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
TAO_EventLog_i::activate (ACE_ENV_SINGLE_ARG_DECL)
{
  CosEventChannelAdmin::ConsumerAdmin_var consumer_admin =
  this->event_channel_->for_consumers (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // Create the PushConsumer that will log the events.
  this->my_log_consumer_ = new TAO_Event_LogConsumer (this);
  ACE_CHECK;
  this->my_log_consumer_->connect (consumer_admin.in ());
}


CosEventChannelAdmin::ConsumerAdmin_ptr
TAO_EventLog_i::for_consumers (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->event_channel_->for_consumers (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CosEventChannelAdmin::SupplierAdmin_ptr
TAO_EventLog_i::for_suppliers (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->event_channel_->for_suppliers (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_EventLog_i::write_recordlist (const DsLogAdmin::RecordList & list
                                  ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   DsLogAdmin::LogFull,
                   DsLogAdmin::LogLocked,
                   DsLogAdmin::LogDisabled))
{
  TAO_Log_i::write_recordlist (list ACE_ENV_ARG_PARAMETER);
}
