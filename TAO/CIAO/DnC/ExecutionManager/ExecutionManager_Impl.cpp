/* -*- C++  -*- */
// $Id$

#include "ExecutionManager_Impl.h"

CIAO::ExecutionManager_Impl::ExecutionManager_Impl (CORBA::ORB_ptr orb,
                                                    PortableServer::POA_ptr poa,
                                                    const char * init_file
                                                    ACE_ENV_ARG_DECL)
  : orb_ (CORBA::ORB::_duplicate  (orb)),
    poa_ (PortableServer::POA::_duplicate (poa)),
    init_file_ (init_file),
    dam_servant_ (0)
{
}

CIAO::ExecutionManager_Impl::~ExecutionManager_Impl ()
{
  if (CIAO::debug_level () > 1)
    ACE_DEBUG ((LM_DEBUG, "ExecutionManager Dtor\n"));
}

Deployment::DomainApplicationManager_ptr
CIAO::ExecutionManager_Impl::
preparePlan (const Deployment::DeploymentPlan &plan,
             CORBA::Boolean
             ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Deployment::ResourceNotAvailable,
                   Deployment::PlanError,
                   Deployment::StartError
                   ))
{
  //@@ Make sure that this call will only called once before destroymanager
  //   being called!!!!

  // Create a new DomainApplicationMananager servant
  ACE_NEW_THROW_EX (this->dam_servant_,
                    CIAO::DomainApplicationManager_Impl
                    (this->orb_.in (),
                     this->poa_.in (),
                     Deployment::TargetManager::_nil (),
                     plan,
                     this->init_file_.c_str ()),
                    CORBA::NO_MEMORY ());

  ACE_CHECK_RETURN (Deployment::DomainApplicationManager::_nil());


  // @@ (OO) If this method is ever called twice in a row, you're
  //         going to leak the previous instance since you don't
  //         decrease the reference count.  You may want to consider
  //         caching this->dam_servant_ into a
  //         TAO::Utils::Servant_Var<> to ease memory management,
  //         e.g. automatically decreasing reference counts, etc.

  /**
   *===================================================================
   * MAIN STEP: This call parses the deployment plan, generates the Node
   * specific plan and starts the deployment process
   *===================================================================
   */
  this->dam_servant_->init (ACE_ENV_SINGLE_ARG_PARAMETER);
  // @@ (OO) You're missing an
  //           ACE_CHECK_RETURN (Deployment::DomainApplicationManager::_nil())
  //         here.  Emulated exceptions won't function correctly
  //         without it.

  this->dam_servant_->set_uuid (plan.UUID.in ());

  // Register with our POA and activate the object.
  PortableServer::ObjectId_var oid
    = this->poa_->activate_object (this->dam_servant_
                                   ACE_ENV_ARG_PARAMETER);

  ACE_CHECK_RETURN (Deployment::DomainApplicationManager::_nil ());

  // Get the reference of the object.
  CORBA::Object_var objref
    = this->poa_->id_to_reference (oid.in ()
                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (Deployment::DomainApplicationManager::_nil ());

  this->dam_ =
    Deployment::DomainApplicationManager::_narrow (objref.in ()
                                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (Deployment::DomainApplicationManager::_nil ());

  // Return the ApplicationManager instance
  return Deployment::DomainApplicationManager::_duplicate (this->dam_.in ());
}


Deployment::DomainApplicationManagers *
CIAO::ExecutionManager_Impl::getManagers (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // @@ (OO) Since you're not doing anything with the allocated
  //         sequence, there is no need to store in a "_var".  Simply
  //         return the pointer.

  // Initialize the list of DomainApplication Managers
  Deployment::DomainApplicationManagers_var list;
  /*ACE_NEW_THROW_EX (list,
                    Deployment::DomainApplicationManagers,
                    CORBA::NO_MEMORY());
  ACE_CHECK_RETURN (0)
  */
  return list._retn ();
}

void
CIAO::ExecutionManager_Impl::destroyManager (Deployment::DomainApplicationManager_ptr manager
                                             ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Deployment::StopError))
{
  ACE_UNUSED_ARG (manager);
  //if (CORBA::is_nil (manager))
  //  ACE_THROW (Deployment::StopError ());

  ACE_TRY
  {
    PortableServer::ObjectId_var oid
      = this->poa_->reference_to_id (this->dam_.in ()
                                     ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    this->dam_servant_->destroyManager (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_TRY_CHECK;

    this->poa_->deactivate_object (oid.in ()
                                   ACE_ENV_ARG_PARAMETER);

    // @@ (OO) Will this this->dam_servant_ ever be used again after
    //         the reference count is decreased?  If not, you may want
    //         to set it to zero to force it to be in a consistent
    //         state.
    this->dam_servant_->_remove_ref ();
    this->dam_ = Deployment::DomainApplicationManager::_nil ();

    ACE_TRY_CHECK;

  }
  ACE_CATCHANY
  {
    ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                         "ExecutionManager_Impl::destroyManager\t\n");
    ACE_THROW (Deployment::StopError ());
  }

  ACE_ENDTRY;
}

void
CIAO::ExecutionManager_Impl::shutdown (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // @@ (OO) You're using the wrong emulated exception macro below.
  //         Please use ACE_ENV_ARG_PARAMETER instead.

  // Shutdown the ORB on which it is runing
  this->orb_->shutdown (0 ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);
}
