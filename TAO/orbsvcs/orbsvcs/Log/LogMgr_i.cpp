#include "orbsvcs/Log/LogMgr_i.h"
#include "orbsvcs/Log/LogActivator.h"
#include "ace/Dynamic_Service.h"
#include "tao/Utils/PolicyList_Destroyer.h"
#include "orbsvcs/Log/Hash_Persistence_Strategy.h"
#include "orbsvcs/Log/LogStore.h"
#include "ace/OS_NS_stdio.h"

ACE_RCSID (Log,
           LogMgr_i,
           "$Id$")


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_LogMgr_i::TAO_LogMgr_i ()
  : logstore_ (0)
{
}


TAO_LogMgr_i::~TAO_LogMgr_i ()
{
  delete logstore_;
}


void
TAO_LogMgr_i::init (CORBA::ORB_ptr orb,
		    PortableServer::POA_ptr poa
		    ACE_ENV_ARG_DECL)
{
  this->orb_ = CORBA::ORB::_duplicate (orb);
  this->poa_ = PortableServer::POA::_duplicate (poa);

  PortableServer::POAManager_var poa_manager =
    this->poa_->the_POAManager ();

  {
    TAO::Utils::PolicyList_Destroyer policies(1);

    // Create Factory POA
    policies.length (1);
    policies[0] =
      this->poa_->create_lifespan_policy (PortableServer::PERSISTENT
					  ACE_ENV_ARG_PARAMETER);

    this->factory_poa_ = this->poa_->create_POA ("factory_POA",
						 poa_manager.in (),
						 policies
						 ACE_ENV_ARG_PARAMETER);
  }


  {
    TAO::Utils::PolicyList_Destroyer policies(2);

    // Create Log POA
    policies.length (2);
    policies[0] =
      this->poa_->create_lifespan_policy (PortableServer::PERSISTENT
					  ACE_ENV_ARG_PARAMETER);

    policies[1] =
      this->poa_->create_id_assignment_policy (PortableServer::USER_ID
					       ACE_ENV_ARG_PARAMETER);

#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT)
    policies.length(4);
    policies[2] =
      this->poa_->create_servant_retention_policy (PortableServer::RETAIN
						   ACE_ENV_ARG_PARAMETER);

    policies[3] =
      this->poa_->create_request_processing_policy (PortableServer::USE_SERVANT_MANAGER
						    ACE_ENV_ARG_PARAMETER);
#endif

    this->log_poa_ = this->factory_poa_->create_POA ("log_POA",
						     poa_manager.in (),
						     policies
						     ACE_ENV_ARG_PARAMETER);
  }

#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT)
  PortableServer::ServantActivator* servant_activator = 0;

  ACE_NEW_THROW_EX (servant_activator,
		    TAO_LogActivator (*this),
		    CORBA::NO_MEMORY ());

  this->log_poa_->set_servant_manager(servant_activator
              ACE_ENV_ARG_PARAMETER);
#endif

  // Load Log Strategy
  TAO_Log_Persistence_Strategy* strategy_ = 0;

  strategy_ =
    ACE_Dynamic_Service<TAO_Log_Persistence_Strategy>::instance ("Log_Persistence");
  if (strategy_ == 0)
    {
      strategy_ = new TAO_Hash_Persistence_Strategy;
    }

  logstore_ = strategy_->create_log_store (this);
}

PortableServer::ObjectId*
TAO_LogMgr_i::create_objectid (DsLogAdmin::LogId id)
{
  char buf[32];
  ACE_OS::sprintf(buf, "%lu", static_cast<unsigned long>(id));

  PortableServer::ObjectId_var oid =
        PortableServer::string_to_ObjectId(buf);

  return oid._retn ();
}

DsLogAdmin::Log_ptr
TAO_LogMgr_i::create_log_reference (DsLogAdmin::LogId id
				    ACE_ENV_ARG_DECL)
{
  PortableServer::ObjectId_var oid =
    this->create_objectid (id);
  CORBA::RepositoryId_var intf =
    this->create_repositoryid ();

  CORBA::Object_var obj =
    this->log_poa_->create_reference_with_id (oid.in (),
                intf.in ()
					      ACE_ENV_ARG_PARAMETER);

  // Use _unchecked_narrow() because this may be called from a servant
  // activator's incarnate() method.  A plain _narrow() will result in
  // infinate recursion.
  DsLogAdmin::Log_var log =
    DsLogAdmin::Log::_unchecked_narrow (obj.in () ACE_ENV_ARG_PARAMETER);

  return log._retn();
}

DsLogAdmin::Log_ptr
TAO_LogMgr_i::create_log_object (DsLogAdmin::LogId id
				 ACE_ENV_ARG_DECL)
{
  PortableServer::ServantBase* servant = 0;

  servant = create_log_servant (id ACE_ENV_ARG_PARAMETER);

  PortableServer::ServantBase_var safe_servant = servant;
  // Transfer ownership to the POA.

  // Obtain ObjectId
  PortableServer::ObjectId_var oid = this->create_objectid (id);

  // Register with the poa
  this->log_poa_->activate_object_with_id (oid.in (),
					   servant
					   ACE_ENV_ARG_PARAMETER);

  return create_log_reference (id ACE_ENV_ARG_PARAMETER);
}

DsLogAdmin::LogList*
TAO_LogMgr_i::list_logs (void)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return this->logstore_->list_logs ();
}


DsLogAdmin::LogIdList*
TAO_LogMgr_i::list_logs_by_id (void)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return this->logstore_->list_logs_by_id ();
}


DsLogAdmin::Log_ptr
TAO_LogMgr_i::find_log (DsLogAdmin::LogId id
                        ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return this->logstore_->find_log (id ACE_ENV_ARG_PARAMETER);
}


TAO_LogRecordStore*
TAO_LogMgr_i::get_log_record_store (DsLogAdmin::LogId id
				    ACE_ENV_ARG_DECL)
{
  return this->logstore_->get_log_record_store (id ACE_ENV_ARG_PARAMETER);
}


bool
TAO_LogMgr_i::exists (DsLogAdmin::LogId id
		      ACE_ENV_ARG_DECL)
{
  return this->logstore_->exists (id ACE_ENV_ARG_PARAMETER);
}


int
TAO_LogMgr_i::remove (DsLogAdmin::LogId id
		      ACE_ENV_ARG_DECL)
{
  return this->logstore_->remove (id ACE_ENV_ARG_PARAMETER);
}

void
TAO_LogMgr_i::create_i (DsLogAdmin::LogFullActionType full_action,
			CORBA::ULongLong max_size,
			const DsLogAdmin::CapacityAlarmThresholdList* thresholds,
			DsLogAdmin::LogId_out id_out
			ACE_ENV_ARG_DECL)
{
  // Validate log_full_action before creating log
  if (full_action != DsLogAdmin::wrap && full_action != DsLogAdmin::halt)
    ACE_THROW (DsLogAdmin::InvalidLogFullAction ());

  if (thresholds)
    {
      // @@ JTC - validate thresholds here
    }

  this->logstore_->create (full_action,
			   max_size,
			   thresholds,
			   id_out
			   ACE_ENV_ARG_PARAMETER);
}

void
TAO_LogMgr_i::create_with_id_i (DsLogAdmin::LogId id,
				DsLogAdmin::LogFullActionType full_action,
				CORBA::ULongLong max_size,
				const DsLogAdmin::CapacityAlarmThresholdList* thresholds
				ACE_ENV_ARG_DECL)
{
  // Validate log_full_action before creating log
  if (full_action != DsLogAdmin::wrap && full_action != DsLogAdmin::halt)
    ACE_THROW (DsLogAdmin::InvalidLogFullAction ());

  if (thresholds)
    {
      // @@ JTC - validate thresholds here
    }

  this->logstore_->create_with_id (id,
				   full_action,
				   max_size,
				   thresholds
				   ACE_ENV_ARG_PARAMETER);
}

CORBA::ORB_ptr
TAO_LogMgr_i::orb ()
{
  return this->orb_.in ();
}

PortableServer::POA_ptr
TAO_LogMgr_i::factory_poa ()
{
  return this->factory_poa_.in ();
}

PortableServer::POA_ptr
TAO_LogMgr_i::log_poa ()
{
  return this->log_poa_.in ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
