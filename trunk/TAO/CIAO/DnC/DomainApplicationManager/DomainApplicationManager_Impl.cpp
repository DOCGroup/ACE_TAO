// $Id$

#include "DomainApplicationManager_Impl.h"

//===============================================================
//============   DomainApplicationManager_Impl  =================
//===============================================================

CIAO::DomainApplicationManager_Impl::
DomainApplicationManager_Impl (CORBA::ORB_ptr orb,
  	                           PortableServer::POA_ptr poa,
                               Deployment::TargetManager_ptr manager)
  : orb_ (CORBA::ORB::_duplicate (o)),
    poa_ (PortableServer::POA::_duplicate (poa)),
    target_manager_ (manager)
{
  // @@ 
}

CIAO::DomainApplicationManager_Impl::~DomainApplicationManager_Impl ()
{
  // @@ remove all NodeApplication's?
}


int
CIAO::DomainApplicationManager_Impl::
init (const ::Deployment::DeploymentPlan & plan
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  //this->plan_ = plan;
}


void 
CIAO::DomainApplicationManager_Impl::
split_plan (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Get the # of node-level plans inside the global plan.
  int num = (this->plan_.instance)->length ();
  ACE_NEW (this->child_plan_, ::Deployment::DeploymentPlan[num]);

  // (1) Iterate over the <instance> field of the global DeploymentPlan 
  //     variabl.
  // (2) 


}





::Deployment::Application_ptr 
CIAO::DomainApplicationManager_Impl::
startLaunch (const Deployment::Properties & configProperty,
             Deployment::Connections_out providedReference,
             CORBA::Boolean start
             ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Deployment::ResourceNotAvailable,
                   Deployment::StartError,
                   Deployment::InvalidProperty))
{
  /**
   *  1. First Map properties to TAO/CIAO specific property/configurations
   *  2. Necessary property checking (needed?)
   *  3. Call create_nade_application to spawn new process.
   *  4. Initialize the NodeApplication.
   *  5. get the provided connection endpoints back and return them.
   */
  ACE_TRY
    {

    }
  ACE_CATCHANY
    {
    }
  ACE_ENDTRY;

  ACE_CHECK_RETURN (0);

  return 0;
}


void
CIAO::DomainApplicationManager_Impl::
destroyApplication (Deployment::Application_ptr app
		                ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException, 
                   Deployment::StopError))
{
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock_);

  if (this->cs_set_.object_in_set (app) == 0)
    ACE_THROW (Deployment::StopError ());

  app->remove (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // Should we remove the server still even if the previous call failed.

  if (this->cs_set_.remove (app) == -1)
    ACE_THROW (Deployment::StopError ());
}
