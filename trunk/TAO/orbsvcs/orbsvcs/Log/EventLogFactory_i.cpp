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
TAO_EventLogFactory_i::activate (CORBA::ORB_ptr orb,
                                 PortableServer::POA_ptr poa
                                 ACE_ENV_ARG_DECL)
{
  TAO_LogMgr_i::init (orb);

  this->orb_ = CORBA::ORB::_duplicate (orb);

  this->poa_ = PortableServer::POA::_duplicate (poa);

  this->event_channel_ = init (this->poa_.in () ACE_ENV_ARG_PARAMETER);

  this->consumer_admin_ =
    this->event_channel_->for_consumers (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (DsEventLogAdmin::EventLogFactory::_nil ());

  ACE_NEW_THROW_EX (this->notifier_,
                    TAO_EventLogNotification(this->event_channel_.in ()),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (DsEventLogAdmin::EventLogFactory::_nil ());

  
  PortableServer::POAManager_var poa_manager =
    this->poa_->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (DsEventLogAdmin::EventLogFactory::_nil ());

  CORBA::PolicyList policies (1);
  policies.length (1);

  policies[0] =
    this->poa_->create_lifespan_policy (PortableServer::PERSISTENT
					ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (DsEventLogAdmin::EventLogFactory::_nil ());

  this->factory_poa_ = this->poa_->create_POA ("factory_POA",
					       poa_manager.in (),
					       policies
					       ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (DsEventLogAdmin::EventLogFactory::_nil ());

  // Creation of the new POA is over, so destroy the Policy_Ptr's.
  for (CORBA::ULong i = 0;
       i < policies.length ();
       ++i)
    {
      CORBA::Policy_ptr policy = policies[i];
      policy->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (DsEventLogAdmin::EventLogFactory::_nil ());
    }

  
  PortableServer::ObjectId_var oid =
    this->factory_poa_->activate_object (this
					 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (DsEventLogAdmin::EventLogFactory::_nil ());

  CORBA::Object_var obj =
    this->factory_poa_->id_to_reference (oid.in ()
					 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (DsEventLogAdmin::EventLogFactory::_nil ());

  // narrow and store the result..
  this->log_mgr_ =
    DsLogAdmin::LogMgr::_narrow (obj.in ()
                                 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (DsEventLogAdmin::EventLogFactory::_nil ());

  DsEventLogAdmin::EventLogFactory_var v_return =
    DsEventLogAdmin::EventLogFactory::_narrow (obj.in ()
                                               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (DsEventLogAdmin::EventLogFactory::_nil ());


  policies.length (3);
  policies[0] =
    this->poa_->create_lifespan_policy (PortableServer::PERSISTENT
					ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (DsEventLogAdmin::EventLogFactory::_nil ());

  policies[1] = 
    this->poa_->create_id_assignment_policy (PortableServer::USER_ID
					     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (DsEventLogAdmin::EventLogFactory::_nil ());

  policies[2] = 
    this->poa_->create_servant_retention_policy (PortableServer::RETAIN
						 ACE_ENV_ARG_PARAMETER)
  ACE_CHECK_RETURN (DsEventLogAdmin::EventLogFactory::_nil ());


  this->log_poa_ = this->factory_poa_->create_POA ("log_POA",
						   poa_manager.in (),
						   policies
						   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (DsEventLogAdmin::EventLogFactory::_nil ());

  // Creation of the new POA is over, so destroy the Policy_Ptr's.
  for (CORBA::ULong i = 0;
       i < policies.length ();
       ++i)
    {
      CORBA::Policy_ptr policy = policies[i];
      policy->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (DsEventLogAdmin::EventLogFactory::_nil ());
    }

  return v_return._retn ();
}

DsEventLogAdmin::EventLog_ptr
TAO_EventLogFactory_i::create (
    DsLogAdmin::LogFullActionType full_action,
    CORBA::ULongLong max_size,
    const DsLogAdmin::CapacityAlarmThresholdList & thresholds,
    DsLogAdmin::LogId_out id_out
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   DsLogAdmin::InvalidLogFullAction,
                   DsLogAdmin::InvalidThreshold))
{
  this->create_i (full_action,
		  max_size,
		  & thresholds,
		  id_out
		  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (DsEventLogAdmin::EventLog::_nil ());
  DsLogAdmin::LogId id = id_out;

  DsLogAdmin::Log_var log =
    this->create_log_object (id
			     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (DsEventLogAdmin::EventLog::_nil ());

  // narrow to EventLog
  DsEventLogAdmin::EventLog_var event_log = 
    DsEventLogAdmin::EventLog::_narrow (log);

  // @@ JTC - squelch exception?
  notifier_->object_creation (id ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (DsEventLogAdmin::EventLog::_nil ());

  return event_log._retn();
}

DsEventLogAdmin::EventLog_ptr
TAO_EventLogFactory_i::create_with_id (
    DsLogAdmin::LogId id,
    DsLogAdmin::LogFullActionType full_action,
    CORBA::ULongLong max_size,
    const DsLogAdmin::CapacityAlarmThresholdList & thresholds 
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   DsLogAdmin::LogIdAlreadyExists,
                   DsLogAdmin::InvalidLogFullAction,
                   DsLogAdmin::InvalidThreshold))
{
  this->create_with_id_i (id,
			  full_action,
			  max_size,
			  & thresholds
			  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (DsEventLogAdmin::EventLog::_nil ());

  DsLogAdmin::Log_var log =
    this->create_log_object (id
			     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (DsEventLogAdmin::EventLog::_nil ());

  // narrow to EventLog
  DsEventLogAdmin::EventLog_var event_log = 
    DsEventLogAdmin::EventLog::_narrow (log);

  // @@ JTC - squelch exception?
  notifier_->object_creation (id ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (DsEventLogAdmin::EventLog::_nil ());

  return event_log._retn ();
}

PortableServer::ObjectId*
TAO_EventLogFactory_i::create_objectid (DsLogAdmin::LogId id)
{
  char buf[32]; 
  ACE_OS::sprintf(buf, "%lu", static_cast<unsigned long>(id)); 
 
  PortableServer::ObjectId_var oid = 
        PortableServer::string_to_ObjectId(buf);

  return oid._retn ();
}

DsLogAdmin::Log_ptr
TAO_EventLogFactory_i::create_log_reference (DsLogAdmin::LogId id
					     ACE_ENV_ARG_DECL)
{
  PortableServer::ObjectId_var oid =
    this->create_objectid (id);
  const char *intf =  
    "IDL:omg.org/DsEventLogAdmin:EventLog:1.0";
  
  CORBA::Object_var obj = 
    this->log_poa_->create_reference_with_id (oid.in (),
					      intf
					      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (DsLogAdmin::Log::_nil ());
  
  DsEventLogAdmin::EventLog_var event_log = 
    DsEventLogAdmin::EventLog::_narrow (obj.in ()
					ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (DsLogAdmin::Log::_nil ());
  
  return event_log._retn(); 
}

DsLogAdmin::Log_ptr
TAO_EventLogFactory_i::create_log_object (DsLogAdmin::LogId id
				          ACE_ENV_ARG_DECL)
{
  TAO_EventLog_i* event_log_i;

  ACE_NEW_THROW_EX (event_log_i,
                    TAO_EventLog_i (this->orb_.in (),
				    this->log_poa_.in (),
                                    *this,
                                    this->log_mgr_.in (),
                                    this->notifier_,
                                    id
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
  ACE_CHECK_RETURN (DsEventLogAdmin::EventLog::_nil ());

  // Obtain ObjectId
  PortableServer::ObjectId_var oid =
    this->create_objectid (id);
  
  // Register with the poa
  this->log_poa_->activate_object_with_id (oid.in (),
					   event_log_i
					   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (DsEventLogAdmin::EventLog::_nil ());

  CORBA::Object_var obj =
    this->log_poa_->id_to_reference (oid.in ()
				     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (DsEventLogAdmin::EventLog::_nil ());

  // Narrow
  DsEventLogAdmin::EventLog_var event_log =
    DsEventLogAdmin::EventLog::_narrow (obj.in () 
					ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (DsEventLogAdmin::EventLog::_nil ());

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
