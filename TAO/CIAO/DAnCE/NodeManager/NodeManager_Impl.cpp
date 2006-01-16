// $Id$

#include "NodeManager_Impl.h"
#include "../NodeApplicationManager/NodeApplicationManager_Impl.h"
#include "ace/Log_Msg.h"

CIAO::NodeManager_Impl_Base::NodeManager_Impl_Base (const char *name,
                                          CORBA::ORB_ptr orb,
                                          PortableServer::POA_ptr poa,
                                          const char * nodeapp_loc,
                                          const char * nodeapp_options,
                                          int spawn_delay)
  ACE_THROW_SPEC ((CORBA::SystemException))
  : orb_ (CORBA::ORB::_duplicate (orb)),
    poa_ (PortableServer::POA::_duplicate (poa)),
    name_ (CORBA::string_dup (name)),
    nodeapp_location_ (CORBA::string_dup (nodeapp_loc)),
    nodeapp_options_ (CORBA::string_dup (nodeapp_options)),
    callback_poa_ (PortableServer::POA::_nil ()),
    spawn_delay_ (spawn_delay)
{
}

CIAO::NodeManager_Impl_Base::~NodeManager_Impl_Base ()
{

}

void
CIAO::NodeManager_Impl_Base::init (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TRY
    {
      //create the call back poa for NAM.
      PortableServer::POAManager_var mgr
        = this->poa_->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      this->callback_poa_ =
        this->poa_->create_POA ("callback_poa",
                                mgr.in (),
                                0
                                ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "NodeManager_Impl::init\t\n");
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK;
}

PortableServer::POA_ptr
CIAO::NodeManager_Impl_Base::_default_POA (void)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}


char *
CIAO::NodeManager_Impl_Base::name (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup (this->name_.in ());
}

void
CIAO::NodeManager_Impl_Base::shutdown (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
CIAO::NodeManager_Impl_Base::joinDomain (const Deployment::Domain & domain,
                                   Deployment::TargetManager_ptr target,
                                   Deployment::Logger_ptr
                                   ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
//  ACE_THROW (CORBA::NO_IMPLEMENT ());
// Here start the Monitor
   CIAO_TRACE("CIAO::NodeManager_Impl_Base::joinDomain");

   ::Deployment::Domain this_domain = domain;
//  MonitorController* monitor_controller
   monitor_controller_.reset (
      new MonitorController (orb_.in (),
                             this_domain,
                             target));

   if (CIAO::debug_level () > 9)
   {
      ACE_DEBUG ((LM_DEBUG , "Before Activate"));
   }
  /// Activate the Monitor Controller to
  //start the monitoring
   monitor_controller_->activate ();

   if (CIAO::debug_level () > 9)
   {
      ACE_DEBUG ((LM_DEBUG , "After Activate"));
   }
}

void
CIAO::NodeManager_Impl_Base::leaveDomain (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  //Implementation undefined.
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}


Deployment::NodeApplicationManager_ptr
CIAO::NodeManager_Impl_Base::preparePlan (const Deployment::DeploymentPlan &plan
                                     ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Deployment::StartError,
                   Deployment::PlanError))
{
  CIAO_TRACE("CIAO::NodeManager_Impl::preparePlan");

  if (! this->validate_plan (plan))
    {
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) NodeManager <%s>:prepare_plan:Plan_Error.\n",
        plan.instance[0].node.in ()));
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) All component instances hosted in the "
        "same component server must have the "
        "same \"resourceName\" defined.\n"));

      ACE_THROW_RETURN (Deployment::PlanError (),
                        Deployment::NodeApplicationManager::_nil ());
    }

  ACE_TRY
    {
      if (!this->map_.is_available (plan.UUID.in ()))
        {
          if (CIAO::debug_level () > 10)
            {
              ACE_DEBUG ((LM_DEBUG, "NM:prepare_plan: "
                          "creating a new NAM with UUID: %s\n",
                          plan.UUID.in ()));
            }

          //Implementation undefined.
          CIAO::NodeApplicationManager_Impl_Base *app_mgr;
          app_mgr = this->create_node_app_manager (this->orb_.in (), this->poa_.in ()
                                                   ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          PortableServer::ServantBase_var safe (app_mgr);

          //@@ Note: after the init call the servant ref count would
          //   become 2. so we can leave the safeservant along and be
          //   dead. Also note that I added
          PortableServer::ObjectId_var oid  =
            app_mgr->init (this->nodeapp_location_.in (),
                           this->nodeapp_options_.in (),
                           this->spawn_delay_,
                           plan,
                           this->callback_poa_.in ()
                           ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          this->map_.insert_nam (plan.UUID.in (), oid.in ());
        }
      else
        {
          if (CIAO::debug_level () > 10)
            {
              ACE_DEBUG ((LM_DEBUG, "NM:prepare_plan: reusing an old NAM "
                          "with UUID: %s\n",
                          plan.UUID.in ()));
            }
        }


      CORBA::Object_var obj =
        this->poa_->id_to_reference (this->map_.get_nam (plan.UUID.in ()));
      ACE_TRY_CHECK;

      // narrow should return a nil reference if it fails.
      return
        Deployment::NodeApplicationManager::_narrow (obj.in ());
    }
  ACE_CATCH (PortableServer::POA::ObjectNotActive, ex)
    {
      ACE_THROW_RETURN (Deployment::StartError (),
                        Deployment::NodeApplicationManager::_nil ());
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "(%P|%t) NodeManager_Impl::preparePlan\t\n");
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (Deployment::NodeApplicationManager::_nil ());

  return Deployment::NodeApplicationManager::_nil ();
}

void
CIAO::NodeManager_Impl_Base::destroyManager
        (Deployment::NodeApplicationManager_ptr manager
         ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Deployment::StopError,
                   Deployment::InvalidReference))
{
  CIAO_TRACE("CIAO::NodeManager_Impl::destroyManager");
  ACE_TRY
    {
      printf("Entering NM_Impl::destroyManager\n");
      // Deactivate this object
      PortableServer::ObjectId_var id =
        this->poa_->reference_to_id (manager
                                     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (!this->map_.remove_nam (id.in ()))
        {
          ACE_ERROR ((LM_ERROR,
                      "NodeManager_Impl::destroyManager: "
                      "Unable to remove object from map!\n"));
        }

      this->poa_->deactivate_object (id.in ()
                                     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      printf("Exiting NM_Impl::destroyManager\n");
    }
  ACE_CATCH (PortableServer::POA::WrongAdapter, ex)
    {
      ACE_ERROR ((LM_ERROR,
                  "NodeManager_Impl::destroyManager: EXCEPTION -  "
                  "Invalid reference passed to destroyManager\n"));

      ACE_THROW (::Deployment::InvalidReference ());
    }
  ACE_CATCH (PortableServer::POA::ObjectNotActive, ex)
    {
      ACE_ERROR ((LM_ERROR,
                  "NodeManager_Impl::destroyManager: EXCEPTION - "
                  " asked to destroy an already inactive object.\n"));
      ACE_THROW (::Deployment::InvalidReference ());
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "NodeManager_Impl::destroyManager\t\n");
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
}

bool
CIAO::NodeManager_Impl_Base::validate_plan (const Deployment::DeploymentPlan &plan)
{
  const char * resource_id = 0;
  CORBA::ULong i = 0;

  for (i = 0; i < plan.instance.length (); ++i)
    {
      if (plan.instance[i].deployedResource.length () != 0)
        {
          // Since the "name" field represents the server_resource_id, and
          // the "resourceValue" field represents the policy_set_id, so we
          // are checking to make sure that all component instances have
          // the same server_resource_id.
          resource_id =
            plan.instance[i].deployedResource[0].resourceName.in ();
          break;
        }
    }
  if (i == plan.instance.length ()) // No server resource id has been set for any instance
    return true;

  for (i = 0; i < plan.instance.length (); ++i)
    {
      const char * my_resource_id;
      if (plan.instance[i].deployedResource.length () == 0)
        {
          continue;
        }
      else
        {
          my_resource_id =
            plan.instance[i].deployedResource[0].resourceName.in ();;
          if (ACE_OS::strcmp (resource_id, my_resource_id) != 0)
            {
              // Error, inconsistent server resource id found.
              return false;
            }
        }
    }
  return true;
}

CIAO::NodeManager_Impl::~NodeManager_Impl ()
{
}

CIAO::NodeManager_Impl::
NodeManager_Impl (const char *name,
                 CORBA::ORB_ptr orb,
                 PortableServer::POA_ptr poa,
                 const char * nodeapp_loc,
                 const char * nodeapp_options,
                 int spawn_delay)
  : NodeManager_Impl_Base (name, orb, poa, nodeapp_loc, nodeapp_options, spawn_delay)
{}


::CIAO::NodeApplicationManager_Impl_Base *
::CIAO::NodeManager_Impl::
create_node_app_manager (CORBA::ORB_ptr orb,
                         PortableServer::POA_ptr poa
                         ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CIAO::NodeApplicationManager_Impl_Base *app_mgr;
  ACE_NEW_THROW_EX (app_mgr,
                    CIAO::NodeApplicationManager_Impl (orb,
                                                       poa),
                    CORBA::NO_MEMORY ());
  return app_mgr;
}

CIAO::Static_NodeManager_Impl::~Static_NodeManager_Impl ()
{
}

CIAO::Static_NodeManager_Impl::
Static_NodeManager_Impl (const char *name,
                        CORBA::ORB_ptr orb,
                        PortableServer::POA_ptr poa,
                        const char * nodeapp_loc,
                        const char * nodeapp_options,
                        int spawn_delay,
                        Static_Config_EntryPoints_Maps* static_config_entrypoints_maps)
  : NodeManager_Impl_Base (name, orb, poa, nodeapp_loc, nodeapp_options, spawn_delay),
    static_config_entrypoints_maps_ (static_config_entrypoints_maps)
{}

::CIAO::NodeApplicationManager_Impl_Base *
::CIAO::Static_NodeManager_Impl::
create_node_app_manager (CORBA::ORB_ptr orb,
                         PortableServer::POA_ptr poa
                         ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG, "creating static_node_app_manager\n"));
  CIAO::NodeApplicationManager_Impl_Base *app_mgr;
  ACE_NEW_THROW_EX (app_mgr,
    CIAO::Static_NodeApplicationManager_Impl (orb,
      poa,
      this->static_config_entrypoints_maps_),
    CORBA::NO_MEMORY ());
  return app_mgr;
}

void
CIAO::Static_NodeManager_Impl::destroyManager
        (Deployment::NodeApplicationManager_ptr manager
         ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Deployment::StopError,
                   Deployment::InvalidReference))
{
  CIAO_TRACE("CIAO::Static_NodeManager_Impl::destroyManager");
  ACE_TRY
    {
      CIAO::NodeManager_Impl_Base::destroyManager (manager ACE_ENV_ARG_PARAMETER);

      this->orb_->shutdown (0 ACE_ENV_ARG_PARAMETER);

      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Static_NodeManager_Impl::destroyManager\t\n");
      ACE_RE_THROW;
    }
  ACE_ENDTRY;

  ACE_CHECK;
}
