//--*C++*--
// $Id$

#include "ExecutionManager_Impl.h"

#include "Config_Handlers/DnC_Dump.h"
#include "DAnCE/Logger/Log_Macros.h"

using namespace DAnCE;

ExecutionManager_Impl::ExecutionManager_Impl (CORBA::ORB_ptr orb,
                                              PortableServer::POA_ptr poa,
                                              CosNaming::NamingContext_ptr nc)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    poa_ (PortableServer::POA::_duplicate (poa)),
    locator_ (orb, nc)
{
  DANCE_TRACE ( "ExecutionManager_Impl::ExecutionManager_Impl");
}

ExecutionManager_Impl::~ExecutionManager_Impl()
{
  DANCE_TRACE ( "ExecutionManager_Impl::~ExecutionManager_Impl");
  for (TDomainManagers::iterator iter = this->managers_.begin();
       iter != this->managers_.end();
       ++iter)
    {
      DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("ExecutionManager_Impl::~ExecutionManager_Impl - ")
                    ACE_TEXT("deactivating DAM \"%C\"\n"), (*iter).ext_id_.c_str()));
      PortableServer::ObjectId_var id = this->poa_->servant_to_id ( (*iter).int_id_);
      DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("ExecutionManager_Impl::~ExecutionManager_Impl - ")
                    ACE_TEXT("before deactivate_object...\n")));
      this->poa_->deactivate_object (id.in());
      DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("ExecutionManager_Impl::~ExecutionManager_Impl - ")
                    ACE_TEXT("deleting DomainApplicationManager\n")));
      delete (*iter).int_id_;
      DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("ExecutionManager_Impl::~ExecutionManager_Impl - ")
                    ACE_TEXT("DomainApplicationManager deleted\n")));
    }
}

::Deployment::DomainApplicationManager_ptr
ExecutionManager_Impl::preparePlan (const ::Deployment::DeploymentPlan & plan,
                                    ::Deployment::ResourceCommitmentManager_ptr /*resourceCommitment*/)
{
  DANCE_TRACE ( "ExecutionManager_Impl::preparePlan");

  // Check if plan is already deployed.
  DomainApplicationManager_Impl * dam = 0;
  if (0 == this->managers_.find (plan.UUID.in(), dam))
    {
      DANCE_DEBUG (6, (LM_NOTICE, DLINFO ACE_TEXT("ExecutionManager_Impl::preparePlan - ")
                   ACE_TEXT("DomainApplicationManager with specified UUID already exists\n")));
      // Should we return on this situation reference on existed DomainApplicationManager or
      // we should throw PlanError exception?
      CORBA::Object_var ref = this->poa_->servant_to_reference (dam);
      return Deployment::DomainApplicationManager::_narrow (ref.in ());
    }

  DomainApplicationManager_Impl* dam_servant = 0;
  ACE_NEW_THROW_EX (dam_servant,
                    DAnCE::DomainApplicationManager_Impl (this->orb_.in (),
                                                          this->poa_.in (),
                                                          plan,
                                                          this->locator_),
                    CORBA::NO_MEMORY ());
  this->managers_.rebind (plan.UUID.in(), dam_servant);
  DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("ExecutionManager_Impl::preparePlan - ")
               ACE_TEXT("Domain Application Manager was successfully created.\n")));

  PortableServer::ObjectId_var id = this->poa_->activate_object (dam_servant);

  DANCE_DEBUG (6, (LM_NOTICE, DLINFO ACE_TEXT("ExecutionManager_Impl::preparePlan - ")
               ACE_TEXT("Plan with UUID %C was successfully prepared.\n"),
                        plan.UUID.in ()));

  CORBA::Object_var ref = this->poa_->id_to_reference (id.in());
  return Deployment::DomainApplicationManager::_narrow (ref.in ());
}

::Deployment::DomainApplicationManagers *
ExecutionManager_Impl::getManagers (void)
{
  DANCE_TRACE ( "ExecutionManager_Impl::getManagers ()");

  ::Deployment::DomainApplicationManagers * managers = 0;
  ACE_NEW_THROW_EX (managers,
                    ::Deployment::DomainApplicationManagers(),
                    CORBA::NO_MEMORY());

  managers->length (this->managers_.current_size());
  CORBA::ULong index = 0;
  for (TDomainManagers::iterator iter = this->managers_.begin();
       iter != this->managers_.end();
       ++iter)
    {
      CORBA::Object_var ref = this->poa_->servant_to_reference ( (*iter).int_id_);
      (*managers) [index]
      = Deployment::DomainApplicationManager::_narrow (ref.in ());
      ++index;
    }

  return managers;
}

void
ExecutionManager_Impl::destroyManager (::Deployment::DomainApplicationManager_ptr appManager)
{
  DANCE_DEBUG (6, (LM_DEBUG, DLINFO ACE_TEXT("ExecutionManager_Impl::destroyManager - started\n")));

  for (TDomainManagers::iterator iter = this->managers_.begin();
       iter != this->managers_.end();
       ++iter)
    {
      CORBA::Object_var app = this->poa_->servant_to_reference ( (*iter).int_id_);
      if (appManager->_is_equivalent (app.in ()))
        {
          this->managers_.unbind ( (*iter).ext_id_);
          PortableServer::ObjectId_var id = this->poa_->reference_to_id (appManager);
          this->poa_->deactivate_object (id.in());
          DANCE_DEBUG (8, (LM_INFO, DLINFO ACE_TEXT("ExecutionManager_Impl::destroyManager - deleting DomainApplicationManager\n")));
          delete (*iter).int_id_;
          (*iter).int_id_ = 0;
          DANCE_DEBUG (8, (LM_INFO, DLINFO ACE_TEXT("ExecutionManager_Impl::destroyManager - finished\n")));
          return;
        }
    }
  DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT("ExecutionManager_Impl::destroyManager - ")
                 ACE_TEXT("corresponding DomainApplicationManager cannot be found\n")));
  throw ::Deployment::StopError();
}

void
ExecutionManager_Impl::add_node_manager (const char *name, const char *ior)
{
  DANCE_TRACE ("ExecutionManager_Impl::add_node_manager");
  this->locator_.store_ior (name, ior);
}

void
ExecutionManager_Impl::load_node_map (const ACE_TCHAR *filename)
{
  DANCE_TRACE ("ExecutionManager_Impl::add_node_manager");
  this->locator_.process_node_map (filename);
}
