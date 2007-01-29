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
      CORBA::Boolean)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Deployment::ResourceNotAvailable,
                       Deployment::PlanError,
                       Deployment::StartError
                       ))
    {
      CIAO_TRACE("Execution_Manager::Execution_Manager_Impl::preparePlan");

      if (CIAO::debug_level () > 9)
        ACE_DEBUG ((LM_DEBUG,
                    "CIAO (%P|%t) Domain Application Manager "
                    "invoked CIAO_Execution_Manager: preparePlan \n"));

      // There is a Domain Application Manager already existing
      // for this DeploymentPlan.
      // No need to create a new DAM. Hence pass the
      // reference that is already created.
      //
      ACE_DEBUG ((LM_DEBUG, "CIAO (%P|%t) calling this->man_.is_plan_available()...\n"));
      if (this->map_.is_plan_available (plan.UUID.in ()))
        {
          ACE_DEBUG ((LM_DEBUG, "CIAO (%P|%t) Plan is already available; "
            "calling this->man_.fetch_dam_reference()...\n"));

          return this->map_.fetch_dam_reference (plan.UUID.in ());
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG, "CIAO (%P|%t) Plan wasn't already available\n"));
        }

      // We are about to begin working on a new DeploymentPlan.
      // Create a DAM servant, which will be populated
      // to be sent back to the PlanLauncher.
      //
      CIAO::DomainApplicationManager_Impl *dam_servant = 0;

      // Create a new Domain Application Manager servant
      // to be sent back to the Plan Launcher.
      //
      ACE_DEBUG ((LM_DEBUG, "CIAO (%P|%t) About to instantiate CIAO::DomainApplicationManager_Impl\n"));
      ACE_NEW_THROW_EX (
        dam_servant,
        CIAO::DomainApplicationManager_Impl (
          this->orb_.in (),
          this->poa_.in (),
          ::Deployment::TargetManager::_nil (),
          this, // a plain C++ pointer
          plan,
          this->init_file_.c_str ()),
          CORBA::NO_MEMORY ());
      ACE_DEBUG ((LM_DEBUG, "CIAO (%P|%t) Instantiated CIAO::DomainApplicationManager_Impl\n"));

      // Sanity check for NULL pointer
      // Should we throw an exception here?
      // We have exceptions like PlanError or StartError at
      // our disposal already in this function.
      // Why did the creation of DAM fail in the first place?
      //

      // Standard owner transfer mechanisms.
      //
      PortableServer::ServantBase_var safe_daemon (dam_servant);

      // Calling the init function on the DAM.
      // This function will split the plan into node specific
      // plans, so that those plans can be sent off to individual
      // Node Application Managers.
      //
      ACE_DEBUG ((LM_DEBUG, "CIAO (%P|%t) About to init...\n"));
      dam_servant->init ();

      // This is a wrong exception to be thrown here.
      // We already had a DAM servant, the DAM servant is
      // not NIL any more.
      // We need to throw the right exception here.
      //

      ACE_DEBUG ((LM_DEBUG, "CIAO (%P|%t) About to set uuid on DAM...\n"));
      dam_servant->set_uuid (plan.UUID.in ());

      Deployment::DomainApplicationManager_var dam =
        dam_servant->_this ();

      /// @@ TODO:Need to check the return value......
      ///
      this->map_.bind_dam_reference (
        plan.UUID.in (),
        Deployment::DomainApplicationManager::_duplicate (dam.in ()));
      ACE_DEBUG ((LM_DEBUG, "CIAO (%P|%t) Bound DAM reference...\n"));

      // Return the ApplicationManager instance
      return dam._retn ();
    }

    Deployment::DomainApplicationManagers *
    Execution_Manager_Impl::getManagers ()
      ACE_THROW_SPEC ((CORBA::SystemException))
    {
      CIAO_TRACE("Execution_Manager::Execution_Manager_Impl::getManagers");

      // TODO Need to check the return value.
      //
      return this->map_.get_dams ();
    }

    Deployment::DomainApplicationManager_ptr
    Execution_Manager_Impl::getManager (const char * plan_uuid)
      ACE_THROW_SPEC ((CORBA::SystemException, Deployment::PlanNotExist))
    {
      return this->map_.fetch_dam_reference (plan_uuid);
    }

    void
    Execution_Manager_Impl::destroyManager (
      Deployment::DomainApplicationManager_ptr manager)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Deployment::StopError))
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
          manager->destroyManager ();

#if 0
          PortableServer::ObjectId_var oid =
            this->poa_->reference_to_id (manager);

          this->poa_->deactivate_object (oid.in ());
#endif /*if 0*/
        }
      catch (const CORBA::Exception& ex)
        {
          ex._tao_print_exception (
            "Execution_Manager_Impl::destroyManager\t\n");
          throw Deployment::StopError ();
        }
    }


    void
    Execution_Manager_Impl::destroyManagerByPlan (
      const char * plan_uuid)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Deployment::StopError))
    {
      CIAO_TRACE("Execution_Manager::Execution_Manager_Impl::destroyManagerByPlan");
      try
        {
          // Get DomainApplicationManager first
          if (! this->map_.is_plan_available (plan_uuid))
            {
              ACE_ERROR ((LM_ERROR,
                          "Execution_Manager_Impl::destroyManagerByPlan - "
                          "Invalid plan uuid [%s]\n", plan_uuid));
              throw Deployment::StopError ();
            }

          Deployment::DomainApplicationManager_var
            dam = this->map_.fetch_dam_reference (plan_uuid);

          // Get the plan
          Deployment::DeploymentPlan_var plan = dam->getPlan ();

          // If any component is still running, then we return.
          CORBA::ULong const inst_lenth = plan->instance.length ();
          for (CORBA::ULong i = 0; i < inst_lenth; ++i)
            {
              if (this->is_component_running (plan->instance[i].name.in (),
                                              plan_uuid))
                return;
            }

          (void) this->map_.unbind_dam (plan->UUID.in ());

          // Where does the POA deactivate happen?
          //
          dam->destroyManager ();

#if 0
          PortableServer::ObjectId_var oid =
            this->poa_->reference_to_id (manager);

          this->poa_->deactivate_object (oid.in ());
#endif /*if 0*/
        }
      catch (const CORBA::Exception& ex)
        {
          ex._tao_print_exception (
            "Execution_Manager_Impl::destroyManager\t\n");
          throw Deployment::StopError ();
        }
    }


    void
    Execution_Manager_Impl::shutdown ()
      ACE_THROW_SPEC ((CORBA::SystemException))
    {
      CIAO_TRACE("Execution_Manager::Execution_Manager_Impl::shutdown");
      // Shutdown the ORB on which it is runing
      this->orb_->shutdown (0);
    }

    void
    Execution_Manager_Impl::perform_redeployment (
      const Deployment::DeploymentPlan & plan)
      ACE_THROW_SPEC ((::CORBA::SystemException,
                       ::Deployment::PlanError,
                       ::Deployment::InstallationFailure,
                       ::Deployment::UnknownImplId,
                       ::Deployment::ImplEntryPointNotFound,
                       ::Deployment::InvalidConnection,
                       ::Deployment::InvalidProperty,
                       ::Components::RemoveFailure))
    {
      CIAO_TRACE ("CIAO::Execution_Manager_Impl::perform_redeployment");

      ACE_DEBUG ((LM_DEBUG,
                  "CIAO (%P|%t) Dynamic Redeployment: "
                  "invoked CIAO::Execution_Manager_Impl::perform_redeployment \n"));

      Deployment::DomainApplicationManager_var dam;

      if (this->map_.is_plan_available (plan.UUID.in ()))
        {
          dam = this->map_.fetch_dam_reference (plan.UUID.in ());
        }
      else
        {
          ACE_ERROR ((LM_ERROR,
                      "DAnCE (%P|%t) ExecutionManager_Impl.cpp -"
                      "CIAO::Execution_Manager_Impl::perform_redeployment -"
                      "Invalid plan uuid: %s\n", plan.UUID.in ()));
          throw Deployment::PlanError (
            "Execution_Manager_Impl::perform_redeployment",
            "Invalid plan uuid specified.");
        }

      try
        {
          // Call perform_redeployment() on the DAM, which will do the
          // actual redeployment and reconfiguration on the dommain level.
          dam->perform_redeployment (plan);
        }
      catch (const CORBA::Exception& ex)
        {
          ex._tao_print_exception (
            "Execution_Manager_Impl::perform_redeployment\t\n");
          throw;
        }
    }

    Deployment::DeploymentPlan *
    Execution_Manager_Impl::getPlan (const char * plan_uuid)
      ACE_THROW_SPEC ((::CORBA::SystemException))
    {
      Deployment::DomainApplicationManager_var dam;

      if (this->map_.is_plan_available (plan_uuid))
        {
          dam = this->map_.fetch_dam_reference (plan_uuid);
        }
      else
        {
          ACE_ERROR ((LM_ERROR,
                      "DAnCE (%P|%t) ExecutionManager_Impl.cpp -"
                      "CIAO::Execution_Manager_Impl::getPlan -"
                      "Invalid plan uuid: %s\n", plan_uuid));
          throw ::CORBA::BAD_PARAM ();
        }

      try
        {
          return dam->getPlan ();
        }
      catch (const CORBA::Exception& ex)
        {
          ex._tao_print_exception ("Execution_Manager_Impl::getPlan\t\n");
          throw;
        }
    }

    void
    Execution_Manager_Impl::finalize_global_binding (
          const Component_Binding_Info & binding,
          CORBA::Boolean add_or_remove)
        ACE_THROW_SPEC ((
          ::CORBA::SystemException,
          ::Deployment::InvalidConnection))
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Execution_Manage::finalizing  global bindings.\n"));

      // Find the NodeApplication hosting the component, and then call
      // <finishLaunch> on it
      try
      {
        Deployment::NodeApplication_var
          node_app = this->find_node_application (binding);

        if (CORBA::is_nil (node_app.in ()))
          {
            ACE_ERROR ((LM_ERROR,
                        "Execution_Manager_Impl::finalize_global_binding - "
                        "nil NodeApplication object reference.\n"));
            throw Deployment::InvalidConnection ();
          }

        node_app->finishLaunch (binding.providedReference_.in (),
                                true, // start
                                add_or_remove);

        // Update the internal shared component list
        if (add_or_remove)
          this->add_shared_component (binding);
        else
          this->remove_shared_component (binding);
      }
      catch (const CORBA::Exception& ex)
        {
          ex._tao_print_exception (
            "Execution_Manager_Impl::finalize_global_binding\t\n");
          throw Deployment::InvalidConnection ();
        }
    }

    void
    Execution_Manager_Impl::passivate_shared_components (
          const Component_Binding_Info & binding)
        ACE_THROW_SPEC ((
          ::CORBA::SystemException,
          Deployment::StartError))
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Execution_Manage::passivate shared components.\n"));

      // Find the NodeApplication hosting the component, and then call
      // <finishLaunch> on it
      try
      {
        Deployment::NodeApplication_var
          node_app = this->find_node_application (binding);

        if (CORBA::is_nil (node_app.in ()))
          {
            ACE_ERROR ((LM_ERROR,
                        "Execution_Manager_Impl::passivate_shared_components - "
                        "nil NodeApplication object reference.\n"));
            throw Deployment::StartError ();
          }

        node_app->passivate_component (binding.name_.c_str ());
      }
      catch (const CORBA::Exception& ex)
        {
          ex._tao_print_exception (
            "Execution_Manager_Impl::passivate_shared_components\t\n");
          throw Deployment::StartError ();
        }
    }

    void
    Execution_Manager_Impl::activate_shared_components (
          const Component_Binding_Info & binding)
        ACE_THROW_SPEC ((
          ::CORBA::SystemException,
          Deployment::StartError))
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Execution_Manage::activate shared components.\n"));

      // Find the NodeApplication hosting the component, and then call
      // <ciao_activate> on it
      try
      {
        Deployment::NodeApplication_var
          node_app = this->find_node_application (binding);

        if (CORBA::is_nil (node_app.in ()))
          {
            ACE_ERROR ((LM_ERROR,
                        "Execution_Manager_Impl::activate_shared_components - "
                        "nil NodeApplication object reference.\n"));
            throw Deployment::StartError ();
          }

        node_app->activate_component (binding.name_.c_str ());
      }
      catch (const CORBA::Exception& ex)
        {
          ex._tao_print_exception (
            "Execution_Manager_Impl::passivate_shared_components\t\n");
          throw Deployment::StartError ();
        }
    }


    Deployment::NodeApplication_ptr

    Execution_Manager_Impl::
    find_node_application (const Component_Binding_Info & binding)
      ACE_THROW_SPEC ((
        ::CORBA::SystemException,
        ::Deployment::InvalidConnection))
    {
      // Find the DAM based on plan_UUID
      Deployment::DomainApplicationManager_var dam;

      if (this->map_.is_plan_available (binding.plan_uuid_))
        {
          dam = this->map_.fetch_dam_reference (binding.plan_uuid_);
        }
      else
        {
          ACE_ERROR ((LM_ERROR,
                      "DAnCE (%P|%t) ExecutionManager_Impl.cpp -"
                      "CIAO::Execution_Manager_Impl::find_node_application -"
                      "Invalid plan uuid: %s\n", binding.plan_uuid_.c_str ()));
          throw ::CORBA::BAD_PARAM ();
        }

      // Find the NA based on the NodeName field of the binding
      // This is a CORBA call on the DAM
      Deployment::NodeApplication_var
        node_app = dam->get_node_app (binding.node_.c_str ());

      if (CORBA::is_nil (node_app.in ()))
        {
          ACE_ERROR ((LM_ERROR,
                      "DAnCE (%P|%t) ExecutionManager_Impl.cpp -"
                      "CIAO::Execution_Manager_Impl::find_node_application -"
                      "Invalid node name: %s!\n", binding.node_.c_str ()));
          throw ::CORBA::BAD_PARAM ();
        }

      return node_app._retn ();
    }

    void
    Execution_Manager_Impl::
    add_shared_component (const Component_Binding_Info & comp)
    {
      this->shared_components_.insert (comp);
    }

    void
    Execution_Manager_Impl::
    remove_shared_component (const Component_Binding_Info & comp)
    {
      this->shared_components_.remove (comp);
    }

    bool
    Execution_Manager_Impl::
    is_component_running (const char * name, const char * plan_uuid)
    {
      for (ACE_Unbounded_Set<Component_Binding_Info>::iterator
           iter = this->shared_components_.begin ();
           iter != this->shared_components_.end ();
           ++iter)
        {
          if (ACE_OS::strcmp ((*iter).name_.c_str (), name) == 0 &&
              ACE_OS::strcmp ((*iter).plan_uuid_.c_str (), plan_uuid) == 0)
            return true;
        }

      return false;
    }
  }
}
