/* -*- C++  -*- */
// $Id$

#include "ExecutionManager_Impl.h"

#if !defined (__ACE_INLINE__)
# include "ExecutionManager_Impl.inl"
#endif /* __ACE_INLINE__ */

CIAO::ExecutionManager_Impl::~ExecutionManager_Impl ()
{
  // @@ functionality not clear for now.
}

int
CIAO::ExecutionManager_Impl::init (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException))
{

}

DomainApplicationManager_ptr
CIAO::ExecutionManager_Impl::preparePlan (const Deployment::DeploymentPlan & plan,
                                          CORBA::Boolean commitResources
					  ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Deployment::ResourceNotAvailable,
                   Deployment::PlanError,
		   Deployment::StartError
		 ))
{
  /*-----------------------------------------------------
   * 1. Note: possible manipulation of the plan?
   **/


  /*-----------------------------------------------------
   * 2. Create the DomainApplicationManager
   **/

  CIAO::DomainApplicationManager_Impl * servant = 0;

  // @@ Future changes are expected to happend for the constructor! -- Tao.
  ACE_NEW_RETURN (servant,
		  CIAO::DomainApplicationManager_Impl
		      (this->orb_.in (),
                       this->poa_.in (),
		       this->tm_cache_.in ()),
		  0);

  PortableServer::ServantBase_var save_servant (servant);

  /*-----------------------------------------------------
   * 3. Get ObjRef of the DomainApplicationManager
   *
   * @@ Note: Unlike Assembly_Factory I don't do the
   *    IOR table registration. Since the spec specifies
   *    that this operation should return the objectref only.
   *
   *    I will put the IOR registration into the client program which
   *    starts assembly process and instantiate the ExecutionManager_Impl
   *    interface.
   **/

  // Register with our POA and activate the object.
  PortableServer::ObjectId_var oid
    = this->poa_->activate_object (servant
                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  // Get the reference of the object.
  CORBA::Object_var objref
    = this->poa_->id_to_reference (oid.in ()
                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  this->dam_cache_ =
      Deployment::DomainApplicationManager::_narrow (objref.in ()
                                                     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  // Ask the DomainApplicationManager to initialize itself.
  if (this->dam_cache_->init (deployment_config_file_.in ()) == -1)
  {
    // should the DomainApplicationManager be deleted?
    // if the program got terminated at this point then we don't care
    // since all these interfaces reside in the same process.
    return 0;
  }
  // Put the object reference into the list.
  CORBA::ULong len = dam_list_->length ();
  dam_list_->length (len+1);

  (*dam_list)[len] = dam_cache_.in ();

  return this->dam_cache_.ptr ();
}



Deployment::DomainApplicationManagers *
CIAO::getManagers (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  Deployment::DomainApplicationManagers_var
    tmp (this->dam_list_);
  return tmp._retn ();
}


void destroyManager (Deployment::DomainApplicationManager_ptr manager
		     ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException,
		   Deployment::StopError))
{
  if (manager == 0)
    ACE_THROW (Deployment::StopError ());
  else
  {
    //@@ I probably should make sure that the pointer is a valid
    // objectref here, which implies searching in the list. --Tao
    Deployment::DomainApplicationManager_var
      tmp (manager);

    //@@ All bookkeeping could come in here! --Tao

    PortableServer::ObjectId_var oid
      = this->poa_->reference_to_id (tmp.in ()
				     ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;

    this->poa_->deactivate_object (oid.in ()
				   ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;
  }

}
