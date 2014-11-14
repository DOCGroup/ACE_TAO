// $Id$

#include "orbsvcs/Log/EventLogFactory_i.h"
#include "orbsvcs/Log/LogNotification.h"
#include "orbsvcs/Log/EventLogNotification.h"
#include "ace/Auto_Ptr.h"
#include "ace/OS_NS_stdio.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_EventLogFactory_i::TAO_EventLogFactory_i (void) :
  impl (0),
  notifier_ (0)
{
  TAO_CEC_Default_Factory::init_svcs ();

  try
    {
      PortableServer::POA_var defPOA =
        this->_default_POA ();

      TAO_CEC_EventChannel_Attributes attr (defPOA.in (), defPOA.in ());

      ACE_NEW_THROW_EX (impl,
                        TAO_CEC_EventChannel (attr),
                        CORBA::NO_MEMORY ());

      impl->activate ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        "Caught exception in "
        "EventLogFactory_i constructor.");
    }
}

TAO_EventLogFactory_i::~TAO_EventLogFactory_i (void)
{
  // No-Op.
}

CosEventChannelAdmin::EventChannel_ptr
TAO_EventLogFactory_i::init (PortableServer::POA_ptr /* poa */)
{
  ACE_ASSERT (!CORBA::is_nil (this->poa_.in ()));

  CosEventChannelAdmin::EventChannel_var ec_return;

  auto_ptr <TAO_CEC_EventChannel> ec (impl);

  PortableServer::ObjectId_var oid =
    this->poa_->activate_object (ec.get ());

  ec.release ();

  CORBA::Object_var obj =
    this->poa_->id_to_reference (oid.in ());

  ec_return =
    CosEventChannelAdmin::EventChannel::_narrow (obj.in ());

  return ec_return._retn ();
}

DsEventLogAdmin::EventLogFactory_ptr
TAO_EventLogFactory_i::activate (CORBA::ORB_ptr orb,
                                 PortableServer::POA_ptr poa)
{
  TAO_LogMgr_i::init (orb, poa);


  this->event_channel_ = init (this->poa_.in ());

  this->consumer_admin_ =
    this->event_channel_->for_consumers ();

  ACE_NEW_THROW_EX (this->notifier_,
                    TAO_EventLogNotification(this->event_channel_.in ()),
                    CORBA::NO_MEMORY ());

  PortableServer::ObjectId_var oid =
    this->factory_poa_->activate_object (this);

  CORBA::Object_var obj =
    this->factory_poa_->id_to_reference (oid.in ());

  // narrow and store the result..
  this->log_mgr_ =
    DsLogAdmin::LogMgr::_narrow (obj.in ());

  DsEventLogAdmin::EventLogFactory_var v_return =
    DsEventLogAdmin::EventLogFactory::_narrow (obj.in ());

  return v_return._retn ();
}

DsEventLogAdmin::EventLog_ptr
TAO_EventLogFactory_i::create (
    DsLogAdmin::LogFullActionType full_action,
    CORBA::ULongLong max_size,
    const DsLogAdmin::CapacityAlarmThresholdList & thresholds,
    DsLogAdmin::LogId_out id_out)
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
  DsEventLogAdmin::EventLog_var event_log =
    DsEventLogAdmin::EventLog::_narrow (log.in ());

  // @@ JTC - squelch exception?
  notifier_->object_creation (id);

  return event_log._retn();
}

DsEventLogAdmin::EventLog_ptr
TAO_EventLogFactory_i::create_with_id (
    DsLogAdmin::LogId id,
    DsLogAdmin::LogFullActionType full_action,
    CORBA::ULongLong max_size,
    const DsLogAdmin::CapacityAlarmThresholdList & thresholds)
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
  DsEventLogAdmin::EventLog_var event_log =
    DsEventLogAdmin::EventLog::_narrow (log.in ());

  // @@ JTC - squelch exception?
  notifier_->object_creation (id);

  return event_log._retn ();
}

CORBA::RepositoryId
TAO_EventLogFactory_i::create_repositoryid ()
{
  return CORBA::string_dup (DsEventLogAdmin::_tc_EventLog->id ());
}

PortableServer::ServantBase*
TAO_EventLogFactory_i::create_log_servant (DsLogAdmin::LogId id)
{
  TAO_EventLog_i* event_log_i;

  ACE_NEW_THROW_EX (event_log_i,
                    TAO_EventLog_i (this->orb_.in (),
                                    this->poa_.in (),
                                    this->log_poa_.in (),
                                    *this,
                                    this->log_mgr_.in (),
                                    this->notifier_,
                                    id
                                    ),
                    CORBA::NO_MEMORY ());

  event_log_i->init ();

  //dhanvey
  //initialise the LogConsumer object
  event_log_i->activate ();

  return event_log_i;
}

CosEventChannelAdmin::ProxyPushSupplier_ptr
TAO_EventLogFactory_i::obtain_push_supplier (void)
{
  return consumer_admin_->obtain_push_supplier ();
}

CosEventChannelAdmin::ProxyPullSupplier_ptr
TAO_EventLogFactory_i::obtain_pull_supplier (void)

{
  return consumer_admin_->obtain_pull_supplier ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
