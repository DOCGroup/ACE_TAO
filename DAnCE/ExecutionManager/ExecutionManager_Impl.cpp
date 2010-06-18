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

void
ExecutionManager_Impl::preparePlan (::Deployment::AMH_ExecutionManagerResponseHandler_ptr _tao_rh,
                                    const ::Deployment::DeploymentPlan & plan,
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
      _tao_rh->preparePlan (Deployment::DomainApplicationManager::_narrow (ref.in ()));
      return;
    }

  try {
    DomainApplicationManager_Impl* dam_servant_ptr = 0;
    ACE_NEW_THROW_EX (dam_servant_ptr,
                      DAnCE::DomainApplicationManager_Impl (this->orb_.in (),
                                                            this->poa_.in (),
                                                            plan,
                                                            this->locator_),
                      CORBA::NO_MEMORY ());
    DomainApplicationManager_Impl_var dam_servant (dam_servant_ptr);

    DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("ExecutionManager_Impl::preparePlan - ")
                ACE_TEXT("Domain Application Manager was successfully created.\n")));

    PreparePlanCompletionHandler* ppch = 0;
    ACE_NEW_THROW_EX (ppch,
                      PreparePlanCompletionHandler (this,
                                                    _tao_rh),
                      CORBA::NO_MEMORY ());

    dam_servant->preparePlan (ppch);

    DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("ExecutionManager_Impl::preparePlan - ")
                ACE_TEXT("Domain Application Manager preparePlan has been called.\n")));
  }
  catch (CORBA::Exception& ex) {
    DANCE_ERROR (1, (LM_ERROR, DLINFO
                  ACE_TEXT("ExecutionManager_Impl::preparePlan - ")
                  ACE_TEXT("Propagating CORBA exception caught here\n")));
    CORBA::Exception* local_ex = ex._tao_duplicate ();
    ::Deployment::AMH_ExecutionManagerExceptionHolder amh_exholder (local_ex);
    _tao_rh->preparePlan_excep (&amh_exholder);
  }
  catch (...) {
    DANCE_ERROR (1, (LM_ERROR, DLINFO
                  ACE_TEXT("ExecutionManager_Impl::preparePlan - ")
                  ACE_TEXT("Propagating exception caught here\n")));
    CORBA::Exception* unknown_ex = new CORBA::UNKNOWN;
    ::Deployment::AMH_ExecutionManagerExceptionHolder amh_exholder (unknown_ex);
    _tao_rh->preparePlan_excep (&amh_exholder);
  }
  DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("ExecutionManager_Impl::preparePlan - finished\n")));
}

void
ExecutionManager_Impl::finish_preparePlan (::Deployment::AMH_ExecutionManagerResponseHandler_ptr _tao_rh,
                                           DomainApplicationManager_Impl *dam_servant)
{
  DANCE_DEBUG (6, (LM_NOTICE, DLINFO ACE_TEXT("ExecutionManager_Impl::finish_preparePlan - ")
              ACE_TEXT("Plan with UUID %C was successfully prepared.\n"),
                        dam_servant->getPlanUUID ()));

  this->managers_.rebind (dam_servant->getPlanUUID (), dam_servant);

  PortableServer::ObjectId_var id = this->poa_->activate_object (dam_servant);

  DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("ExecutionManager_Impl::finish_preparePlan - ")
              ACE_TEXT("Domain Application Manager was successfully activated.\n")));

  CORBA::Object_var ref = this->poa_->id_to_reference (id.in());
  _tao_rh->preparePlan (Deployment::DomainApplicationManager::_narrow (ref.in ()));
}

void
ExecutionManager_Impl::fail_preparePlan (::Deployment::AMH_ExecutionManagerResponseHandler_ptr _tao_rh,
                                         CORBA::Exception* local_ex)
{
  DANCE_DEBUG (6, (LM_NOTICE, DLINFO ACE_TEXT("ExecutionManager_Impl::fail_preparePlan called\n")));

  ::Deployment::AMH_ExecutionManagerExceptionHolder amh_exholder (local_ex);
  _tao_rh->preparePlan_excep (&amh_exholder);
}

void
ExecutionManager_Impl::getManagers (::Deployment::AMH_ExecutionManagerResponseHandler_ptr _tao_rh)
{
  DANCE_TRACE ( "ExecutionManager_Impl::getManagers ()");

  try {
    ::Deployment::DomainApplicationManagers_var managers;
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

    _tao_rh->getManagers (managers);
    return;
  }
  catch (CORBA::Exception& ex) {
    CORBA::Exception* local_ex = ex._tao_duplicate ();
    ::Deployment::AMH_ExecutionManagerExceptionHolder amh_exholder (local_ex);
    _tao_rh->getManagers_excep (&amh_exholder);
  }
  catch (...) {
    CORBA::Exception* unknown_ex = new CORBA::UNKNOWN;
    ::Deployment::AMH_ExecutionManagerExceptionHolder amh_exholder (unknown_ex);
    _tao_rh->getManagers_excep (&amh_exholder);
  }
}

void
ExecutionManager_Impl::destroyManager (::Deployment::AMH_ExecutionManagerResponseHandler_ptr _tao_rh,
                                       ::Deployment::DomainApplicationManager_ptr appManager)
{
  DANCE_DEBUG (6, (LM_DEBUG, DLINFO ACE_TEXT("ExecutionManager_Impl::destroyManager - started\n")));

  try {
    for (TDomainManagers::iterator iter = this->managers_.begin();
        iter != this->managers_.end();
        ++iter)
      {
        CORBA::Object_var app = this->poa_->servant_to_reference ( (*iter).int_id_);
        if (appManager->_is_equivalent (app.in ()))
          {
            DomainApplicationManager_Impl *dam_servant = (*iter).int_id_;
            // create completion handler
            DestroyManagerCompletionHandler* dmch = 0;
            ACE_NEW_THROW_EX (dmch,
                              DestroyManagerCompletionHandler (this,
                                                               _tao_rh,
                                                               dam_servant),
                              CORBA::NO_MEMORY ());

            // deregister manager
            this->managers_.unbind ( (*iter).ext_id_);
            PortableServer::ObjectId_var id = this->poa_->reference_to_id (appManager);
            this->poa_->deactivate_object (id.in());

            (*iter).int_id_ = 0;

            // remove default ref so servant will be deleted when
            // var in DestroyManagerCompletionHandler gets destructed
            // at the finish of DomainApplicationManager_Impl::destroyManager
            dam_servant->_remove_ref ();

            dam_servant->destroyManager (dmch);

            DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("ExecutionManager_Impl::destroyManager - ")
                        ACE_TEXT("Domain Application Manager destroyManager has been called.\n")));
            return;
          }
      }
    DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT("ExecutionManager_Impl::destroyManager - ")
                  ACE_TEXT("corresponding DomainApplicationManager cannot be found\n")));
    CORBA::Exception* local_ex =
        new ::Deployment::StopError("ExecutionManager_Impl::destroyManager",
                                    "corresponding DomainApplicationManager cannot be found");
    ::Deployment::AMH_ExecutionManagerExceptionHolder amh_exholder (local_ex);
    _tao_rh->destroyManager_excep (&amh_exholder);
  }
  catch (CORBA::Exception& ex) {
    CORBA::Exception* local_ex = ex._tao_duplicate ();
    ::Deployment::AMH_ExecutionManagerExceptionHolder amh_exholder (local_ex);
    _tao_rh->destroyManager_excep (&amh_exholder);
  }
  catch (...) {
    CORBA::Exception* unknown_ex = new CORBA::UNKNOWN;
    ::Deployment::AMH_ExecutionManagerExceptionHolder amh_exholder (unknown_ex);
    _tao_rh->destroyManager_excep (&amh_exholder);
  }
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

ExecutionManager_Impl::PreparePlanCompletionHandler::PreparePlanCompletionHandler (
    ExecutionManager_Impl *em_servant,
    ::Deployment::AMH_ExecutionManagerResponseHandler_ptr _tao_rh)
  : em_servant_ (ExecutionManager_Impl_var::_duplicate (em_servant)),
    em_rh_ (::Deployment::AMH_ExecutionManagerResponseHandler::_duplicate (_tao_rh))
{
}

void
ExecutionManager_Impl::PreparePlanCompletionHandler::handle_completion (DomainApplicationManager_Impl *dam_servant)
{
  this->em_servant_->finish_preparePlan (this->em_rh_.in (),
                                         dam_servant);
}

void ExecutionManager_Impl::PreparePlanCompletionHandler::handle_exception (CORBA::Exception* local_ex)
{
  this->em_servant_->fail_preparePlan (this->em_rh_.in (),
                                        local_ex);
}

ExecutionManager_Impl::DestroyManagerCompletionHandler::DestroyManagerCompletionHandler (
    ExecutionManager_Impl *em_servant,
    ::Deployment::AMH_ExecutionManagerResponseHandler_ptr _tao_rh,
    DomainApplicationManager_Impl* dam_servant)
  : em_servant_ (ExecutionManager_Impl_var::_duplicate (em_servant)),
    em_rh_ (::Deployment::AMH_ExecutionManagerResponseHandler::_duplicate (_tao_rh)),
    dam_servant_ (DomainApplicationManager_Impl_var::_duplicate (dam_servant))
{
}

void ExecutionManager_Impl::DestroyManagerCompletionHandler::handle_completion (
    DomainApplicationManager_Impl * /*dam_servant*/)
{
  DANCE_DEBUG (8, (LM_INFO, DLINFO
      ACE_TEXT("ExecutionManager_Impl::DestroyManagerCompletionHandler::handle_completion - ")
      ACE_TEXT("finished\n")));
  em_rh_->destroyManager ();
}

void ExecutionManager_Impl::DestroyManagerCompletionHandler::handle_exception (
    CORBA::Exception* local_ex)
{
  DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT("ExecutionManager_Impl::DestroyManagerCompletionHandler::handle_exception - ")
                ACE_TEXT("propagating exception\n")));
  ::Deployment::AMH_ExecutionManagerExceptionHolder amh_exholder (local_ex);
  em_rh_->destroyManager_excep (&amh_exholder);
}
