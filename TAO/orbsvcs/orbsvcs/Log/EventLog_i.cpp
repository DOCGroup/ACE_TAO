#include "orbsvcs/Log/EventLog_i.h"
#include "orbsvcs/Log/LogMgr_i.h"
#include "orbsvcs/Log/LogNotification.h"

ACE_RCSID (Log,
           EventLog_i,
           "$Id$")


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_EventLog_i::TAO_EventLog_i (CORBA::ORB_ptr orb,
				PortableServer::POA_ptr poa,
				PortableServer::POA_ptr log_poa,
                                TAO_LogMgr_i &logmgr_i,
                                DsLogAdmin::LogMgr_ptr factory,
                                TAO_LogNotification *log_notifier,
                                DsLogAdmin::LogId id)
  : TAO_Log_i (orb, logmgr_i, factory, id, log_notifier),
    poa_(PortableServer::POA::_duplicate(poa)),
    log_poa_(PortableServer::POA::_duplicate(log_poa))
{
  ACE_DECLARE_NEW_CORBA_ENV;

  // Create an instance of the event channel.
  TAO_CEC_EventChannel_Attributes attr (this->poa_.in(),
					this->poa_.in());

  ACE_NEW_THROW_EX (this->event_channel_,
                    TAO_CEC_EventChannel(attr),
                    CORBA::NO_MEMORY ());
}

TAO_EventLog_i::~TAO_EventLog_i ()
{
  this->event_channel_->destroy ();

  delete this->event_channel_;
}


DsLogAdmin::Log_ptr
TAO_EventLog_i::copy (DsLogAdmin::LogId &id ACE_ENV_ARG_DECL)
ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Duplicate the log.
  DsEventLogAdmin::EventLogFactory_var eventLogFactory =
    DsEventLogAdmin::EventLogFactory::_narrow (factory_.in ()
                                               ACE_ENV_ARG_PARAMETER);

  DsEventLogAdmin::EventLog_var log =
    eventLogFactory->create (DsLogAdmin::halt, 0, thresholds_, id
                             ACE_ENV_ARG_PARAMETER);

  this->copy_attributes (log.in () ACE_ENV_ARG_PARAMETER);

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

  DsEventLogAdmin::EventLog_var log =
    eventLogFactory->create_with_id (id, DsLogAdmin::halt, 0, thresholds_
                                     ACE_ENV_ARG_PARAMETER);

  this->copy_attributes (log.in () ACE_ENV_ARG_PARAMETER);

  return log._retn ();

}


void
TAO_EventLog_i::destroy (void)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Send event to indicate the log has been deleted.
  notifier_->object_deletion (logid_ ACE_ENV_ARG_PARAMETER);

  // Remove ourselves from the list of logs.
  this->logmgr_i_.remove (this->logid_
			  ACE_ENV_ARG_PARAMETER);

  // Deregister with POA.
  PortableServer::ObjectId_var id =
    this->log_poa_->servant_to_id (this
                                   ACE_ENV_ARG_PARAMETER);

  this->log_poa_->deactivate_object (id.in ()
                                     ACE_ENV_ARG_PARAMETER);
}

void
TAO_EventLog_i::activate (void)
{
  CosEventChannelAdmin::ConsumerAdmin_var consumer_admin =
  this->event_channel_->for_consumers ();

  // Create the PushConsumer that will log the events.
  this->my_log_consumer_ = new TAO_Event_LogConsumer (this);
  this->my_log_consumer_->connect (consumer_admin.in ());
}


CosEventChannelAdmin::ConsumerAdmin_ptr
TAO_EventLog_i::for_consumers (void)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->event_channel_->for_consumers ();
}

CosEventChannelAdmin::SupplierAdmin_ptr
TAO_EventLog_i::for_suppliers (void)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->event_channel_->for_suppliers ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
