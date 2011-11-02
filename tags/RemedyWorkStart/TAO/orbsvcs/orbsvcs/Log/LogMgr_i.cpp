// $Id$

#include "orbsvcs/Log/LogMgr_i.h"
#include "orbsvcs/Log/LogActivator.h"
#include "ace/Dynamic_Service.h"
#include "tao/Utils/PolicyList_Destroyer.h"
#include "orbsvcs/Log/Hash_Persistence_Strategy.h"
#include "orbsvcs/Log/LogStore.h"
#include "ace/OS_NS_stdio.h"

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
                    PortableServer::POA_ptr poa)
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
      this->poa_->create_lifespan_policy (PortableServer::PERSISTENT);

    this->factory_poa_ = this->poa_->create_POA ("factory_POA",
                                                 poa_manager.in (),
                                                 policies);
  }


  {
    TAO::Utils::PolicyList_Destroyer policies(2);

    // Create Log POA
    policies.length (2);
    policies[0] =
      this->poa_->create_lifespan_policy (PortableServer::PERSISTENT);

    policies[1] =
      this->poa_->create_id_assignment_policy (PortableServer::USER_ID);

#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT)
    policies.length(4);
    policies[2] =
      this->poa_->create_servant_retention_policy (PortableServer::RETAIN);

    policies[3] =
      this->poa_->create_request_processing_policy (PortableServer::USE_SERVANT_MANAGER);
#endif

    this->log_poa_ = this->factory_poa_->create_POA ("log_POA",
                                                     poa_manager.in (),
                                                     policies);
  }

#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT)
  PortableServer::ServantActivator* servant_activator = 0;

  ACE_NEW_THROW_EX (servant_activator,
                    TAO_LogActivator (*this),
                    CORBA::NO_MEMORY ());

  this->log_poa_->set_servant_manager(servant_activator);
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
TAO_LogMgr_i::create_log_reference (DsLogAdmin::LogId id)
{
  PortableServer::ObjectId_var oid =
    this->create_objectid (id);
  CORBA::RepositoryId_var intf =
    this->create_repositoryid ();

  CORBA::Object_var obj =
    this->log_poa_->create_reference_with_id (oid.in (),
                intf.in ());

  // Use _unchecked_narrow() because this may be called from a servant
  // activator's incarnate() method.  A plain _narrow() will result in
  // infinate recursion.
  DsLogAdmin::Log_var log =
    DsLogAdmin::Log::_unchecked_narrow (obj.in ());

  return log._retn();
}

DsLogAdmin::Log_ptr
TAO_LogMgr_i::create_log_object (DsLogAdmin::LogId id)
{
  PortableServer::ServantBase* servant = 0;

  servant = create_log_servant (id);

  PortableServer::ServantBase_var safe_servant = servant;
  // Transfer ownership to the POA.

  // Obtain ObjectId
  PortableServer::ObjectId_var oid = this->create_objectid (id);

  // Register with the poa
  this->log_poa_->activate_object_with_id (oid.in (),
                                           servant);

  return create_log_reference (id);
}

DsLogAdmin::LogList*
TAO_LogMgr_i::list_logs (void)
{
  return this->logstore_->list_logs ();
}


DsLogAdmin::LogIdList*
TAO_LogMgr_i::list_logs_by_id (void)
{
  return this->logstore_->list_logs_by_id ();
}


DsLogAdmin::Log_ptr
TAO_LogMgr_i::find_log (DsLogAdmin::LogId id)
{
  return this->logstore_->find_log (id);
}


TAO_LogRecordStore*
TAO_LogMgr_i::get_log_record_store (DsLogAdmin::LogId id)
{
  return this->logstore_->get_log_record_store (id);
}


bool
TAO_LogMgr_i::exists (DsLogAdmin::LogId id)
{
  return this->logstore_->exists (id);
}


int
TAO_LogMgr_i::remove (DsLogAdmin::LogId id)
{
  return this->logstore_->remove (id);
}

void
TAO_LogMgr_i::create_i (DsLogAdmin::LogFullActionType full_action,
                        CORBA::ULongLong max_size,
                        const DsLogAdmin::CapacityAlarmThresholdList* thresholds,
                        DsLogAdmin::LogId_out id_out)
{
  // Validate log_full_action before creating log
  if (full_action != DsLogAdmin::wrap && full_action != DsLogAdmin::halt)
    throw DsLogAdmin::InvalidLogFullAction ();

  if (thresholds)
    {
      // @@ JTC - validate thresholds here
    }

  this->logstore_->create (full_action,
                           max_size,
                           thresholds,
                           id_out);
}

void
TAO_LogMgr_i::create_with_id_i (DsLogAdmin::LogId id,
                                DsLogAdmin::LogFullActionType full_action,
                                CORBA::ULongLong max_size,
                                const DsLogAdmin::CapacityAlarmThresholdList* thresholds)
{
  // Validate log_full_action before creating log
  if (full_action != DsLogAdmin::wrap && full_action != DsLogAdmin::halt)
    throw DsLogAdmin::InvalidLogFullAction ();

  if (thresholds)
    {
      // @@ JTC - validate thresholds here
    }

  this->logstore_->create_with_id (id,
                                   full_action,
                                   max_size,
                                   thresholds);
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
