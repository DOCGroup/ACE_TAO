// $Id$

#include "NodeManager_Impl.h"
#include "../NodeApplicationManager/NodeApplicationManager_Impl.h"

CIAO::NodeManager_Impl::NodeManager_Impl (const char *name,
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

CIAO::NodeManager_Impl::~NodeManager_Impl ()
{

}

void
CIAO::NodeManager_Impl::init (ACE_ENV_SINGLE_ARG_DECL)
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
CIAO::NodeManager_Impl::_default_POA (void)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}


char *
CIAO::NodeManager_Impl::name (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup (this->name_.in ());
}

void
CIAO::NodeManager_Impl::shutdown (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
CIAO::NodeManager_Impl::joinDomain (const Deployment::Domain & ,
                                   Deployment::TargetManager_ptr ,
                                   Deployment::Logger_ptr
                                   ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

void
CIAO::NodeManager_Impl::leaveDomain (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  //Implementation undefined.
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}


Deployment::NodeApplicationManager_ptr
CIAO::NodeManager_Impl::preparePlan (const Deployment::DeploymentPlan &plan
                                     ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Deployment::StartError,
                   Deployment::PlanError))
{
  CIAO_TRACE("CIAO::NodeManager_Impl::preparePlan");

  if (! this->validate_plan (plan))
    {
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
          CIAO::NodeApplicationManager_Impl *app_mgr;
          ACE_NEW_THROW_EX (app_mgr,
                            CIAO::NodeApplicationManager_Impl (this->orb_.in (),
                                                               this->poa_.in ()),
                            CORBA::NO_MEMORY ());
          ACE_TRY_CHECK;

          PortableServer::ServantBase_var safe (app_mgr);

          //@@ Note: after the init call the servant ref count would
          //   become 2. so we can leave the safeservant along and be
          //   dead. Also note that I added
          PortableServer::ObjectId_var oid  =
            app_mgr->init (this->nodeapp_location_,
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
CIAO::NodeManager_Impl::destroyManager
        (Deployment::NodeApplicationManager_ptr manager
         ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Deployment::StopError,
                   Deployment::InvalidReference))
{
  CIAO_TRACE("CIAO::NodeManager_Impl::destroyManager");
  ACE_TRY
    {
      // Deactivate this object
      PortableServer::ObjectId_var id =
        this->poa_->reference_to_id (manager
                                     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (!this->map_.remove_nam (id))
        {
          ACE_ERROR ((LM_ERROR,
                      "NodeManager_Impl::destroyManager: "
                      "Unable to remove object from map!\n"));
        }

      this->poa_->deactivate_object (id.in ()
                                     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
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
CIAO::NodeManager_Impl::validate_plan (const Deployment::DeploymentPlan &plan)
{
  char * resource_id;
  CORBA::ULong i = 0;

  for (i = 0; i < plan.instance.length (); ++i)
    {
      if (plan.instance[i].deployedResource.length () != 0)
        {
          plan.instance[i].deployedResource[0].resourceValue >>= resource_id;
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
          plan.instance[i].deployedResource[0].resourceValue >>= my_resource_id;
          if (ACE_OS::strcmp (resource_id, my_resource_id) != 0)
            {
              // Error, inconsistent server resource id found.
              return false;
            }
        }
    }
  return true;
}
