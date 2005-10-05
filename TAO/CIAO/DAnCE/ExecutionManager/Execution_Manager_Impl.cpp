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
                    "CIAO (%P|%t) Domain Application Manager 
                     invoked CIAO_Execution_Manager: preparePlan \n"));

      // There is a Domain Application Manager already existing
      // for this DeploymentPlan.
      // No need to create a new DAM. Hence pass the
      // reference that is already created.
      //
      if (this->map_.is_plan_available (plan.UUID.in ()))
        return this->map_.fetch_dam_reference (plan.UUID.in ());

      // We are about to begin working on a new
      // DeploymentPlan.
      // Create a DAM servant, which will be populated
      // to be sent back to the PlanLauncher.
      //
      CIAO::DomainApplicationManager_Impl *dam_servant = 0;

      // Create a new Domain Application Manager servant
      // to be sent back to the Plan Launcher.
      //
      ACE_NEW_THROW_EX (
        dam_servant,
        CIAO::DomainApplicationManager_Impl (
          this->orb_.in (),
          this->poa_.in (),
          ::Deployment::TargetManager::_nil (),
          plan,
          this->init_file_.c_str ()),
          CORBA::NO_MEMORY ());

      // Sanity check for NULL pointer
      // Should we throw an exception here?
      // We have exceptions like PlanError or StartError at
      // our disposal already in this function.
      // Why did the creation of DAM fail in the first place?
      //
      ACE_CHECK_RETURN (::Deployment::DomainApplicationManager::_nil ());

      // Standard owner transfer mechanisms.
      //
      PortableServer::ServantBase_var safe_daemon (dam_servant);

      // Calling the init function on the DAM.
      // This function will split the plan into node specific
      // plans, so that those plans can be sent off to individual
      // Node Application Managers.
      //
      dam_servant->init (ACE_ENV_SINGLE_ARG_PARAMETER);

      // This is a wrong exception to be thrown here.
      // We already had a DAM servant, the DAM servant is
      // not NIL any more.
      // We need to throw the right exception here.
      //
      ACE_CHECK_RETURN (::Deployment::DomainApplicationManager::_nil ());

      /// @@ Can be removed -- Bala
      /// Gan, have you addressed this comment by Bala?
      /// Do we still need this code lying around?
      ///
      dam_servant->set_uuid (plan.UUID.in ());

      Deployment::DomainApplicationManager_var dam =
        dam_servant->_this ();

      /// @@ TODO:Need to check the return value......
      /// 
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

      // TODO Need to check the return value.
      //
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

          // Where does the POA deactivate happen?
          //
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
