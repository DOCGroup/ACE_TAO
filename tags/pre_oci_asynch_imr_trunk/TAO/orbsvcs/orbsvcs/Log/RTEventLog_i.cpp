// $Id$

#include "orbsvcs/Log/RTEventLog_i.h"
#include "orbsvcs/Log/LogMgr_i.h"
#include "orbsvcs/Log/LogNotification.h"
#include "orbsvcs/Event_Utilities.h"
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

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_RTEventLog_i::TAO_RTEventLog_i (CORBA::ORB_ptr orb,
                                    PortableServer::POA_ptr poa,
                                    PortableServer::POA_ptr log_poa,
                                    TAO_LogMgr_i &logmgr_i,
                                    DsLogAdmin::LogMgr_ptr factory,
                                    TAO_LogNotification *log_notifier,
                                    DsLogAdmin::LogId id)
  : TAO_Log_i (orb, logmgr_i, factory, id, log_notifier),
    event_channel_ (0),
    my_log_consumer_ (0),
    observer_strategy_ (0),
    poa_ (PortableServer::POA::_duplicate (poa)),
    log_poa_ (PortableServer::POA::_duplicate (log_poa))
{

  TAO_EC_Event_Channel_Attributes attr (poa_.in(), poa_.in());

  ACE_NEW_THROW_EX (this->event_channel_,
                    TAO_EC_Event_Channel(attr),
                    CORBA::NO_MEMORY ());
}

TAO_RTEventLog_i::~TAO_RTEventLog_i ()
{
  this->event_channel_->destroy ();

  delete this->event_channel_;
}


DsLogAdmin::Log_ptr
TAO_RTEventLog_i::copy (DsLogAdmin::LogId &id)
{
  RTEventLogAdmin::EventLogFactory_var eventLogFactory =
    RTEventLogAdmin::EventLogFactory::_narrow (factory_.in ());

  RTEventLogAdmin::EventLog_var log =
    eventLogFactory->create (DsLogAdmin::halt, 0, thresholds_, id);

  copy_attributes (log.in ());

  return log._retn ();
}

DsLogAdmin::Log_ptr
TAO_RTEventLog_i::copy_with_id (DsLogAdmin::LogId id)
{
  RTEventLogAdmin::EventLogFactory_var eventLogFactory =
    RTEventLogAdmin::EventLogFactory::_narrow (factory_.in ());

  RTEventLogAdmin::EventLog_var log =
    eventLogFactory->create_with_id (id, DsLogAdmin::halt, 0, thresholds_);

  copy_attributes (log.in ());

  return log._retn ();
}

void
TAO_RTEventLog_i::destroy (void)
{
  notifier_->object_deletion (logid_);

  // Remove ourselves from the list of logs.
  this->logmgr_i_.remove (this->logid_);

  // Deregister with POA.
  PortableServer::ObjectId_var id = this->log_poa_->servant_to_id (this);

  this->log_poa_->deactivate_object (id.in ());
}

void
TAO_RTEventLog_i::activate (void)
{
  RtecEventChannelAdmin::ConsumerAdmin_var consumer_admin =
    this->event_channel_->for_consumers ();

  this->my_log_consumer_ = new TAO_Rtec_LogConsumer (this);
  this->my_log_consumer_->connect (consumer_admin.in ());
}

RtecEventChannelAdmin::ConsumerAdmin_ptr
TAO_RTEventLog_i::for_consumers (void)
{
  return this->event_channel_->for_consumers();
}

RtecEventChannelAdmin::SupplierAdmin_ptr
TAO_RTEventLog_i::for_suppliers (void)
{
  return this->event_channel_->for_suppliers();
}

RtecEventChannelAdmin::Observer_Handle
TAO_RTEventLog_i::append_observer (
       RtecEventChannelAdmin::Observer_ptr observer)
{
  return this->observer_strategy_->append_observer (observer);
}

void
TAO_RTEventLog_i::remove_observer (
       RtecEventChannelAdmin::Observer_Handle handle)
{
  this->observer_strategy_->remove_observer (handle);
}

TAO_END_VERSIONED_NAMESPACE_DECL
