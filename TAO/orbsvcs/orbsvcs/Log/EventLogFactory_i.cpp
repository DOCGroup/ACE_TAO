#include "orbsvcs/Log/EventLogFactory_i.h"
// @@ David, once you switch to a PortableServer::ServantBase_var, you
//    won't need to include the ace/Auto_Ptr.h header.
#include "ace/Auto_Ptr.h"
#include "orbsvcs/Log/LogNotification.h"
#include "orbsvcs/Log/EventLogNotification.h"

ACE_RCSID (Log,
           EventLogFactory_i,
           "$Id$")


EventLogFactory_i::EventLogFactory_i (void)
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

EventLogFactory_i::~EventLogFactory_i (void)
{
  // No-Op.
}

CosEventChannelAdmin::EventChannel_ptr
EventLogFactory_i::init (PortableServer::POA_ptr /* poa */
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
EventLogFactory_i::activate (PortableServer::POA_ptr poa
                             ACE_ENV_ARG_DECL)
{
  this->poa_ = poa;
  this->event_channel_ = init (this->poa_.in ());

  this->consumer_admin_ =
    this->event_channel_->for_consumers (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (DsEventLogAdmin::EventLogFactory::_nil ());

  ACE_NEW_THROW_EX (this->notifier_,
                    EventLogNotification(this->event_channel_.in ()),
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
EventLogFactory_i::create (
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
                          thresholds//,
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
EventLogFactory_i::create_with_id (
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

  EventLog_i* event_log_i;

  ACE_NEW_THROW_EX (event_log_i,
                    EventLog_i (*this,
                                this->log_mgr_.in (),
                                this,
                                this->notifier_,
                                id,
                                full_action,
                                max_size
                                ),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (DsEventLogAdmin::EventLog::_nil ());

  // @@ David, EventLog_i is reference count.  auto_ptr<> does not
  //    apply in this case.  Use PortableServer::ServantBase_var
  //    instead.  Also remove the below call to _remove_ref().
  auto_ptr<EventLog_i> event_log_auto (event_log_i);
  // just in case the activation fails.

  event_log_i->init (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (DsEventLogAdmin::EventLog::_nil ());

  //dhanvey
  //initialise the LogConsumer object
  event_log_i->activate();

  // Register with the poa
  event_log = event_log_i->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (DsEventLogAdmin::EventLog::_nil ());

  // @@ David, once you switch from an auto_ptr<> to a
  //    PortableServer::ServantBase_var above,  remove this
  //    _remove_ref() call (and the ACE_CHECK_RETURN after it).
  event_log_i->_remove_ref (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (DsEventLogAdmin::EventLog::_nil ());

  // Add to the Hash table..
  if (hash_map_.bind (id, event_log.in ()) == -1)
    ACE_THROW_RETURN (CORBA::INTERNAL (),
                      DsEventLogAdmin::EventLog::_nil ());

  // @@ David, zap this auto_ptr<>::release() call once you switch to
  //    a PortableServer::ServantBase_var above.
  // All is well, release the reference.
  event_log_auto.release ();

  notifier_->object_creation (event_log.in (), id
                              ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (DsEventLogAdmin::EventLog::_nil ());

  return event_log._retn ();
}

CosEventChannelAdmin::ProxyPushSupplier_ptr
EventLogFactory_i::obtain_push_supplier (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return consumer_admin_->obtain_push_supplier (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CosEventChannelAdmin::ProxyPullSupplier_ptr
EventLogFactory_i::obtain_pull_supplier (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))

{
  return consumer_admin_->obtain_pull_supplier (ACE_ENV_SINGLE_ARG_PARAMETER);
}


// @@ David, once you switch to a PortableServer::ServantBase_var, you
//    won't need these explicit template instantations.
#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class auto_ptr <EventLog_i>;
template class ACE_Auto_Basic_Ptr<EventLog_i>;

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate auto_ptr <EventLog_i>
#pragma instantiate ACE_Auto_Event_Ptr <EventLog_i>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
