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
    Execution_Manager_Impl::preparePlan (const ::Deployment::DeploymentPlan & plan,
                                         ::Deployment::ResourceCommitmentManager_ptr resourceCommitment)
      throw (CORBA::SystemException,
             Deployment::ResourceNotAvailable,
             Deployment::PlanError,
             Deployment::StartError)
    {
      CIAO_TRACE("Execution_Manager::Execution_Manager_Impl::preparePlan");
      
      CIAO_DEBUG (5, (LM_DEBUG,
                      "CIAO (%P|%t) Domain Application Manager "
                      "invoked CIAO_Execution_Manager: preparePlan \n"));
      
      try
        {
          
          // There is a Domain Application Manager already existing
          // for this DeploymentPlan.
          // No need to create a new DAM. Hence pass the
          // reference that is already created.
          //
          if (this->map_.is_plan_available (plan.UUID.in ()))
            {      
              CIAO_DEBUG (6, (LM_DEBUG,
                              "CIAO (%P|%t) DAM - "
                              " found available DAM for plan UUID %s\n",
                              plan.UUID.in ()));
              return this->map_.fetch_dam_reference (plan.UUID.in ());
            }
          else
            CIAO_DEBUG (6, (LM_DEBUG,
                            "CIAO (%P|%t) EM - "
                            " no existing DAM for UUID %s\n",
                            plan.UUID.in ()));
          
          // We are about to begin working on a new DeploymentPlan.
          // Create a DAM servant, which will be populated
          // to be sent back to the PlanLauncher.
          //
          CIAO::DomainApplicationManager_Impl *dam_servant = 0;

          // Create a new Domain Application Manager servant
          // to be sent back to the Plan Launcher.
          //
          CIAO_DEBUG (9, (LM_DEBUG,
                          "CIAO (%P|%t) EM - "
                          " Creating a new EM instance \n"));
          ACE_NEW_THROW_EX (dam_servant,
                            CIAO::DomainApplicationManager_Impl (this->orb_.in (),
                                                                 this->poa_.in (),
                                                                 ::Deployment::TargetManager::_nil (),
                                                                 this, // a plain C++ pointer
                                                                 plan,
                                                                 this->init_file_.c_str ()),
                            Deployment::StartError ("ExecutionManager",
                                                    "Insufficient memory to create DAM."));
          
          // Initialize the DAM
          dam_servant.init (resourceCommitment);
          
          // Standard owner transfer mechanisms.
          //
          PortableServer::ServantBase_var safe_daemon (dam_servant);

          dam_servant->set_uuid (plan.UUID.in ());

          Deployment::DomainApplicationManager_var dam =
            dam_servant->_this ();
      
          if (CORBA::is_nil (dam.in ()))
            {
              CIAO_DEBUG (1, (LM_ERROR,
                              "CIAO (%P|%t) EM - "
                              " Implicit activation of DAM instance failed.\n"));
              throw Deployment::StartError ("ExecutionManager",
                                            "Implicit activation of DAM instance failed\n");
            }
      
          this->map_.bind_dam_reference (plan.UUID.in (),
                                         Deployment::DomainApplicationManager::_duplicate (dam.in ()));
              
        }
      catch (Deployment::StartError &ex)
        {
          CIAO_DEBUG (1, (LM_ERROR, "CIAO (%P|%t) EM - Caught StartError Exception, rethrowing\n"));
          throw;
        }
      catch (Deployment::ResourceNotAvailable &ex)
        {
          CIAO_DEBUG (1, (LM_ERROR, "CIAO (%P|%t) EM - Caught ResourceNotAvailable Exception, rethrowing\n"));
          throw;
        }
      catch (Deployment::PlanError &ex)
        {
          CIAO_DEBUG (1, (LM_ERROR, "CIAO (%P|%t) EM - Caught PlanError Exception, rethrowing\n"));
          throw;
        }
      catch (CORBA::SystemException &ex)
        {
          CIAO_DEBUG (1, (LM_ERROR, "CIAO (%P|%t) EM - Caught CORBA system exception: %s\n",
                          ex._info ().c_str ()));
          throw Deployment::StartError ("ExecutionManager::preparePlan",
                                        ex._info ().c_str ());
        }
      catch (...)
        {
          CIAO_DEBUG (1, (LM_ERROR, "CIAO (%P|%t) EM - Caught unknown exception, throwin StartError\n"));
          throw Deployment::StartError ("ExecutionManager::preparePlan",
                                        "Unknown exception");
        }
      
      // Return the ApplicationManager instance
      return dam._retn ();
    }

    Deployment::DomainApplicationManagers *
    Execution_Manager_Impl::getManagers ()
      throw (CORBA::SystemException)
    {
      CIAO_TRACE("Execution_Manager::Execution_Manager_Impl::getManagers");

      // TODO Need to check the return value.
      //
      return this->map_.get_dams (ACE_ENV_SINGLE_ARG_PARAMETER);
    }

    Deployment::DomainApplicationManager_ptr
    Execution_Manager_Impl::getManager (const char * plan_uuid)
      throw (CORBA::SystemException, Deployment::PlanNotExist)
    {
      CIAO_TRACE ("Execution_Manager::getManager");
      return this->map_.fetch_dam_reference (plan_uuid);
    }

    void
    Execution_Manager_Impl::destroyManager (
      Deployment::DomainApplicationManager_ptr manager)
      throw (CORBA::SystemException,
             Deployment::StopError)
    {
      CIAO_TRACE("Execution_Manager::Execution_Manager_Impl::destroyManagers");

      try
        {
          ::Deployment::DeploymentPlan_var plan =
              manager->getPlan ();

          // What if we still have components running within this plan?
          //
          (void) this->map_.unbind_dam (plan->UUID.in ());

          // Where does the POA deactivate happen?
          //
          manager->destroyManager (void);

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
      catch (CORBA::SystemException &ex)
        {
          CIAO_DEBUG (1, (LM_ERROR, "CIAO (%P|%t) EM - Caught CORBA system exception: %s\n",
                          ex._info ().c_str ()));
          throw Deployment::StartError ("ExecutionManager::preparePlan",
                                        ex._info ().c_str ());
        }
      catch (...)
        {
          CIAO_DEBUG (1, (LM_ERROR, "CIAO (%P|%t) EM - Caught unknown exception, throwin StartError\n"));
          throw Deployment::StartError ("ExecutionManager::preparePlan",
                                        "Unknown exception");
        }
    }

    void
    Execution_Manager_Impl::shutdown ()
      throw (CORBA::SystemException)
    {
      CIAO_TRACE("Execution_Manager::Execution_Manager_Impl::shutdown");
      // Shutdown the ORB on which it is runing
      this->orb_->shutdown (0);
    }
  }
}
