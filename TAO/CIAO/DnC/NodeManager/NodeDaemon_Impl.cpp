// $Id$

#include "NodeDaemon_Impl.h"
#include "../NodeApplicationManager/NodeApplicationManager_Impl.h"

CIAO::NodeDaemon_Impl::NodeDaemon_Impl (const char *name,
                                        CORBA::ORB_ptr orb,
                                        PortableServer::POA_ptr poa,
                                        const char * nodapp_loc,
                                        int spawn_delay)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    poa_ (PortableServer::POA::_duplicate (poa)),
    name_ (CORBA::string_dup (name)),
    nodeapp_location_ (CORBA::string_dup (nodapp_loc)),
    manager_ (Deployment::NodeApplicationManager::_nil ()),
    spawn_delay_ (spawn_delay)

{
}

CIAO::NodeDaemon_Impl::~NodeDaemon_Impl ()
{

}

PortableServer::POA_ptr
CIAO::NodeDaemon_Impl::_default_POA (void)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

char *
CIAO::NodeDaemon_Impl::name (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup (this->name_.in ());
}

void
CIAO::NodeDaemon_Impl::shutdown (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
CIAO::NodeDaemon_Impl::joinDomain (const Deployment::Domain & ,
                                   Deployment::TargetManager_ptr ,
                                   Deployment::Logger_ptr
                                   ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException))
{

}

void
CIAO::NodeDaemon_Impl::leaveDomain (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  //Implementation undefined.
}


Deployment::NodeApplicationManager_ptr
CIAO::NodeDaemon_Impl::preparePlan (const Deployment::DeploymentPlan &plan
                                    ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Deployment::StartError,
                   Deployment::PlanError))
{
  // Return cached manager
  ACE_TRY
  {
    if (CORBA::is_nil (this->manager_.in ()))
    {
      //Implementation undefined.
      CIAO::NodeApplicationManager_Impl *app_mgr;
      ACE_NEW_THROW_EX (app_mgr,
                        CIAO::NodeApplicationManager_Impl (this->orb_.in (),
                                                           this->poa_.in ()),
                        CORBA::NO_MEMORY ());
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "NM:prepare before init on NAM\n"));
      PortableServer::ServantBase_var safe (app_mgr);

      //@@ Note: after the init call the servant ref count would become 2. so
      //   we can leave the safeservant along and be dead. Also note that I added
      //   the callback poa as the last augment. Letting the NDeamon to create the
      //   poa and pass it to the NAM is a better idea, so I will come back here. --Tao
      this->callback_poa_ =
	app_mgr->init (this->nodeapp_location_,
		       this->spawn_delay_,
		       plan,
		       this->callback_poa_.in ()
		       ACE_ENV_ARG_PARAMETER);
      ACE_DEBUG ((LM_DEBUG, "NM:prepare 3\n"));

      ACE_TRY_CHECK;

      // Obtain the Object Reference
      CORBA::Object_var obj =
        this->poa_->servant_to_reference (app_mgr ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      this->manager_ =
        Deployment::NodeApplicationManager::_narrow (obj. in ());

      if (CORBA::is_nil (this->manager_.in ()))
        {
          ACE_DEBUG ((LM_DEBUG, "preparePlan: NodeApplicationManager ref is nil\n"));
          ACE_THROW (Deployment::StartError ());
        }
    }
    // Duplicate this reference to the caller
    return
      Deployment::NodeApplicationManager::_duplicate (this->manager_.in ());
  }
  ACE_CATCHANY
  {
    ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
			 "NodeDaemon_Impl::preparePlan\t\n");
    ACE_RE_THROW;
  }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);
}


void
CIAO::NodeDaemon_Impl::destroyManager (Deployment::NodeApplicationManager_ptr
                                       ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException, Deployment::StopError))
{
  // Deactivate this object
  PortableServer::ObjectId_var id =
    this->poa_->reference_to_id (this->manager_.in ());
  this->poa_->deactivate_object (id.in ());

  // This causes the POA to delete the actual ApplicationManager
  this->manager_ = Deployment::NodeApplicationManager::_nil ();
}
