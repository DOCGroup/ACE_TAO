#include "orbsvcs/Log/EventLog_i.h"
#include "orbsvcs/Log/LogMgr_i.h"
#include "orbsvcs/Log/LogNotification.h"


ACE_RCSID (Log,
           EventLog_i,
           "$Id$")


TAO_Event_LogConsumer::TAO_Event_LogConsumer (EventLog_i *log)
  : log_ (log)
{
}

TAO_Event_LogConsumer::~TAO_Event_LogConsumer (void)
{
}

void
TAO_Event_LogConsumer::connect (CosEventChannelAdmin::ConsumerAdmin_ptr consumer_admin)
{
  // Connect to the event channel.
  CosEventComm::PushConsumer_var myself = this->_this ();
  this->supplier_proxy_ = consumer_admin->obtain_push_supplier ();
  this->supplier_proxy_->connect_push_consumer (myself.in());
}

void
TAO_Event_LogConsumer::push (const CORBA::Any& data ACE_ENV_ARG_DECL)
ACE_THROW_SPEC ((
                CORBA::SystemException,
                CosEventComm::Disconnected
        ))
{
  // create a record list...
  DsLogAdmin::RecordList recList (1);
  recList.length (1);

  recList [0].info = data;

  // log the RecordList.
  this->log_->write_recordlist (recList ACE_ENV_ARG_PARAMETER);

  ACE_CHECK;
}

void
TAO_Event_LogConsumer::disconnect_push_consumer (ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((
                CORBA::SystemException
        ))
{
  this->supplier_proxy_->disconnect_push_supplier (ACE_ENV_SINGLE_ARG_PARAMETER);
}


EventLog_i::EventLog_i (LogMgr_i &logmgr_i,
                        DsLogAdmin::LogMgr_ptr factory,
                        EventLogFactory_i *event_log_factory,
                        LogNotification *log_notifier,
                        DsLogAdmin::LogId id,
                        DsLogAdmin::LogFullActionType log_full_action,
                        CORBA::ULongLong max_size,
                        ACE_Reactor *reactor)
  : Log_i (factory, id, log_notifier, log_full_action, max_size, reactor),
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

EventLog_i::~EventLog_i ()
{
  // No-Op.
}


DsLogAdmin::Log_ptr
EventLog_i::copy (DsLogAdmin::LogId &id ACE_ENV_ARG_DECL)
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
EventLog_i::copy_with_id (DsLogAdmin::LogId id ACE_ENV_ARG_DECL)
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
EventLog_i::destroy (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Send event to indicate the log has been deleted.
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
EventLog_i::activate (ACE_ENV_SINGLE_ARG_DECL)
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
EventLog_i::for_consumers (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->event_channel_->for_consumers (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CosEventChannelAdmin::SupplierAdmin_ptr
EventLog_i::for_suppliers (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->event_channel_->for_suppliers (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
EventLog_i::write_recordlist (const DsLogAdmin::RecordList & list
                              ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   DsLogAdmin::LogFull,
                   DsLogAdmin::LogLocked,
                   DsLogAdmin::LogDisabled))
{
  Log_i::write_recordlist (list ACE_ENV_ARG_PARAMETER);
}
