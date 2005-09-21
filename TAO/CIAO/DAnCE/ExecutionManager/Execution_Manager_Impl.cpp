// $Id$
#include "Execution_Manager_Impl.h"
#include "ciao/CIAO_common.h"
#include "DomainApplicationManager/DomainApplicationManager_Impl.h"

ACE_RCSID (ExecutionManager,
           Execution_Manager_Impl,
           "$Id$")

namespace CIAO
{
  namespace Execution_Manager
  {
    Execution_Manager_Impl::Execution_Manager_Impl (
      CORBA::ORB_ptr orb,
      PortableServer::POA_ptr poa,
      const char * init_file)
      : orb_ (CORBA::ORB::_duplicate  (orb))
      , poa_ (PortableServer::POA::_duplicate (poa))
      , init_file_ (init_file)
    {
    }

    Execution_Manager_Impl::~Execution_Manager_Impl (void)
    {
    }

    Deployment::DomainApplicationManager_ptr
    Execution_Manager_Impl::preparePlan (
      const Deployment::DeploymentPlan &plan,
      CORBA::Boolean
      ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Deployment::ResourceNotAvailable,
                       Deployment::PlanError,
                       Deployment::StartError
                       ))
    {
      CIAO_TRACE("Execution_Manager::Execution_Manager_Impl::preparePlan");
      if (CIAO::debug_level () > 10)
        ACE_DEBUG ((LM_DEBUG,
                    "(%P|%t) CIAO_Execution_Manager: preparePlan "
                    " invoked \n"));

      if (this->map_.is_plan_available (plan.UUID.in ()))
        return this->map_.fetch_dam_reference (plan.UUID.in ());



      CIAO::DomainApplicationManager_Impl *dam_servant = 0;

      // Create a new DomainApplicationMananager servant
      ACE_NEW_THROW_EX (
        dam_servant,
        CIAO::DomainApplicationManager_Impl (
          this->orb_.in (),
          this->poa_.in (),
          ::Deployment::TargetManager::_nil (),
          plan,
          this->init_file_.c_str ()),
        CORBA::NO_MEMORY ());

      ACE_CHECK_RETURN (::Deployment::DomainApplicationManager::_nil ());

      PortableServer::ServantBase_var safe_daemon (dam_servant);

      dam_servant->init (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (::Deployment::DomainApplicationManager::_nil ());

      /// @@ Can be removed -- Bala
      dam_servant->set_uuid (plan.UUID.in ());

      Deployment::DomainApplicationManager_var dam =
        dam_servant->_this ();

      /// @@ TODO:Need to check the return value......
      this->map_.bind_dam_reference (
        plan.UUID.in (),
        Deployment::DomainApplicationManager::_duplicate (dam.in ()));

      // Return the ApplicationManager instance
      return dam._retn ();
    }

    Deployment::DomainApplicationManagers *
    Execution_Manager_Impl::getManagers (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException))
    {
      CIAO_TRACE("Execution_Manager::Execution_Manager_Impl::getManagers");
      return this->map_.get_dams (ACE_ENV_SINGLE_ARG_PARAMETER);
    }

    void
    Execution_Manager_Impl::destroyManager (
      Deployment::DomainApplicationManager_ptr manager
      ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Deployment::StopError))
    {
      CIAO_TRACE("Execution_Manager::Execution_Manager_Impl::destroyManagers");
      ACE_TRY
        {
          ::Deployment::DeploymentPlan_var plan =
              manager->getPlan (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          (void) this->map_.unbind_dam (plan->UUID.in ());

          manager->destroyManager (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

#if 0
          PortableServer::ObjectId_var oid =
            this->poa_->reference_to_id (manager
                                         ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          this->poa_->deactivate_object (oid.in ()
                                         ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
#endif /*if 0*/
        }
      ACE_CATCHANY
        {
          ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                               "Execution_Manager_Impl::destroyManager\t\n");
          ACE_THROW (Deployment::StopError ());
        }
      ACE_ENDTRY;
      ACE_CHECK;
    }

    void
    Execution_Manager_Impl::shutdown (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException))
    {
      CIAO_TRACE("Execution_Manager::Execution_Manager_Impl::shutdown");
      // Shutdown the ORB on which it is runing
      this->orb_->shutdown (0 ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
  }
}
