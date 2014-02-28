// $Id$

#include "orbsvcs/Log/RTEventLogFactory_i.h"
#include "orbsvcs/Log/LogNotification.h"
#include "orbsvcs/Log/RTEventLogNotification.h"
#include "orbsvcs/Event/EC_ConsumerAdmin.h"
#include "orbsvcs/Event/EC_ProxySupplier.h"
#include "orbsvcs/Event/EC_ProxyConsumer.h"
#include "orbsvcs/Event/EC_Event_Channel.h"
#include "orbsvcs/ESF/ESF_Peer_Workers.h"
#include "orbsvcs/ESF/ESF_Peer_Admin.h"
#include "orbsvcs/ESF/ESF_Shutdown_Proxy.h"
#include "orbsvcs/Event/EC_ConsumerAdmin.h"
#include "orbsvcs/Event_Utilities.h"
#include "orbsvcs/Sched/Config_Scheduler.h"
#include "orbsvcs/Event/EC_Default_Factory.h"
#include "orbsvcs/Event/EC_Event_Channel.h"
#include "ace/OS_NS_stdio.h"

#include "ace/Auto_Ptr.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_RTEventLogFactory_i::TAO_RTEventLogFactory_i (void) :
  impl (0),
  notifier_ (0)
{
}

TAO_RTEventLogFactory_i::~TAO_RTEventLogFactory_i()
{
}

int
TAO_RTEventLogFactory_i::init (CORBA::ORB_ptr orb,
                               PortableServer::POA_ptr poa)
{
  TAO_LogMgr_i::init (orb, poa);


  PortableServer::POA_var defPOA =
    this->_default_POA ();

  TAO_EC_Event_Channel_Attributes attr (defPOA.in (), defPOA.in ());

  this->impl = 0;
  ACE_NEW_THROW_EX (impl,
                    TAO_EC_Event_Channel (attr),
                    CORBA::NO_MEMORY ());

  auto_ptr <TAO_EC_Event_Channel> ec (impl);

  impl->activate ();

  PortableServer::ObjectId_var oidec = poa_->activate_object (ec.get ());

  ec.release ();

  CORBA::Object_var objec =
    poa_->id_to_reference (oidec.in ());

  this->event_channel_ = RtecEventChannelAdmin::EventChannel::_narrow (objec.in ());


  this->consumer_admin_ = this->event_channel_->for_consumers();

  ACE_NEW_THROW_EX (this->notifier_,
                    TAO_RTEventLogNotification(this->event_channel_.in ()),
                    CORBA::NO_MEMORY ());
  return 0;
}

RTEventLogAdmin::EventLogFactory_ptr
TAO_RTEventLogFactory_i::activate (void)
{
  RTEventLogAdmin::EventLogFactory_var v_return;

  PortableServer::ObjectId_var oid =
    this->factory_poa_->activate_object (this);

  CORBA::Object_var obj =
    this->factory_poa_->id_to_reference (oid.in ());

  // narrow and store the result..
  this->log_mgr_ =
    DsLogAdmin::LogMgr::_narrow (obj.in ());

  v_return =
    RTEventLogAdmin::EventLogFactory::_narrow (obj.in ());

  return v_return._retn ();
}

RTEventLogAdmin::EventLog_ptr
TAO_RTEventLogFactory_i::create (
        DsLogAdmin::LogFullActionType full_action,
        CORBA::ULongLong max_size,
        const DsLogAdmin::CapacityAlarmThresholdList & thresholds,
        DsLogAdmin::LogId_out id_out
      )
{
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

  // narrow to EventLog
  RTEventLogAdmin::EventLog_var event_log =
    RTEventLogAdmin::EventLog::_narrow (log.in ());

  // @@ JTC - squelch exception?
  notifier_->object_creation (id);

  return event_log._retn();
}

RTEventLogAdmin::EventLog_ptr
TAO_RTEventLogFactory_i::create_with_id (
        DsLogAdmin::LogId id,
        DsLogAdmin::LogFullActionType full_action,
        CORBA::ULongLong max_size,
        const DsLogAdmin::CapacityAlarmThresholdList & thresholds
      )
{
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

  // narrow to EventLog
  RTEventLogAdmin::EventLog_var event_log =
    RTEventLogAdmin::EventLog::_narrow (log.in ());

  // @@ JTC - squelch exception?
  notifier_->object_creation (id);

  return event_log._retn ();
}

CORBA::RepositoryId
TAO_RTEventLogFactory_i::create_repositoryid ()
{
  return CORBA::string_dup (RTEventLogAdmin::_tc_EventLog->id ());
}

PortableServer::ServantBase*
TAO_RTEventLogFactory_i::create_log_servant (DsLogAdmin::LogId id)
{
  TAO_RTEventLog_i* event_log_i;

  ACE_NEW_THROW_EX (event_log_i,
                    TAO_RTEventLog_i (this->orb_.in (),
                                      this->poa_.in (),
                                      this->log_poa_.in (),
                                      *this,
                                      this->log_mgr_.in (),
                                      this->notifier_,
                                      id
                                      ),
                    CORBA::NO_MEMORY ());

  event_log_i->init ();

  //initialise the LogConsumer object
  event_log_i->activate ();

  return event_log_i;
}

RtecEventChannelAdmin::ProxyPushSupplier_ptr
TAO_RTEventLogFactory_i::obtain_push_supplier (
      )
{
  return consumer_admin_->obtain_push_supplier();
}

TAO_END_VERSIONED_NAMESPACE_DECL
