// $Id$

#include "NodeDaemon_Impl.h"
#include "ciao/Container_Base.h"

CIAO::NodeDaemon_Impl_Base::NodeDaemon_Impl_Base (const char *name,
                                        CORBA::ORB_ptr orb,
                                        PortableServer::POA_ptr poa,
                                        const char * nodapp_loc,
                                        int spawn_delay)
  ACE_THROW_SPEC ((CORBA::SystemException))
  : orb_ (CORBA::ORB::_duplicate (orb)),
    poa_ (PortableServer::POA::_duplicate (poa)),
    name_ (CORBA::string_dup (name)),
    nodeapp_location_ (CORBA::string_dup (nodapp_loc)),
    callback_poa_ (PortableServer::POA::_nil ()),
    spawn_delay_ (spawn_delay),
    manager_ (Deployment::NodeApplicationManager::_nil ())
{
  ACE_DECLARE_NEW_CORBA_ENV;
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
                           "NodeDaemon_Impl_Base::constructor\t\n");
      // @@ This is bogus and will not work with emulated exceptions
      // -- Bala
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK;
}

CIAO::NodeDaemon_Impl_Base::~NodeDaemon_Impl_Base ()
{

}

PortableServer::POA_ptr
CIAO::NodeDaemon_Impl_Base::_default_POA (void)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}


char *
CIAO::NodeDaemon_Impl_Base::name (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup (this->name_.in ());
}

void
CIAO::NodeDaemon_Impl_Base::shutdown (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
CIAO::NodeDaemon_Impl_Base::joinDomain (const Deployment::Domain & ,
                                   Deployment::TargetManager_ptr ,
                                   Deployment::Logger_ptr
                                   ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

void
CIAO::NodeDaemon_Impl_Base::leaveDomain (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  //Implementation undefined.
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

Deployment::NodeApplicationManager_ptr
CIAO::NodeDaemon_Impl_Base::preparePlan (const Deployment::DeploymentPlan &plan
                                    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Deployment::StartError,
                   Deployment::PlanError))
{
  ACE_DEBUG ((LM_DEBUG, "In NDI:preparePlan\n"));
  // Return cached manager
  ACE_TRY
    {
      if (CORBA::is_nil (this->manager_.in ()))
        {
          //Implementation undefined.
          CIAO::NodeApplicationManager_Impl_Base *app_mgr;
          app_mgr = create_node_app_manager (this->orb_.in (), this->poa_.in ());

          PortableServer::ServantBase_var safe (app_mgr);

          //@@ Note: after the init call the servant ref count would
          //   become 2. so we can leave the safeservant along and be
          //   dead. Also note that I added
          this->manager_ =
            app_mgr->init (this->nodeapp_location_,
                           this->spawn_delay_,
                           plan,
                           this->callback_poa_.in ()
                           ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          if (CORBA::is_nil (this->manager_.in ()))
            {
              ACE_ERROR ((LM_ERROR,
                          "CIAO.NodeDaemon (%P|%t) -- ",
                          "preparePlan: NodeApplicationManager ref",
                          "is nil\n"));
              ACE_TRY_THROW (Deployment::StartError ());
            }
        }
    }
  ACE_CATCHANY
  {
    ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                         "NodeDaemon_Impl_Base::preparePlan\t\n");
    ACE_RE_THROW;
  }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (Deployment::NodeApplicationManager::_nil ());

  // Duplicate this reference to the caller
  return
    Deployment::NodeApplicationManager::_duplicate (this->manager_.in ());
}

void
CIAO::NodeDaemon_Impl_Base::destroyManager (Deployment::NodeApplicationManager_ptr
                                       ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Deployment::StopError))
{
  ACE_TRY
    {
      // Deactivate this object
      PortableServer::ObjectId_var id =
        this->poa_->reference_to_id (this->manager_.in ()
                                     ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    this->poa_->deactivate_object (id.in ()
                                   ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    this->manager_ =
      Deployment::NodeApplicationManager::_nil ();
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "NodeDaemon_Impl_Base::destroyManager\t\n");
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
}


