/* -*- C++  -*- */
// $Id$

#include "ExecutionManager_Impl.h"

CIAO::ExecutionManager_Impl::ExecutionManager_Impl (CORBA::ORB_ptr orb,
                                                    PortableServer::POA_ptr poa,
                                                    const char * init_file
						    ACE_ENV_ARG_DECL)
  : orb_ (CORBA::ORB::_duplicate  (orb)),
    poa_ (PortableServer::POA::_duplicate (poa)),
    init_file_ (init_file)
  // @@ (OO) To be safe you should initialize dam_servant_ to zero to
  //         make it easier to spot memory access errors at run-time.
{
}

CIAO::ExecutionManager_Impl::~ExecutionManager_Impl ()
{
  // @@ (OO) What does this comment apply to?
  // Delete the Map for maintaining

  // @@ (OO) Production code should not display debugging output
  //         unless requested by the user.  You should probably make
  //         the following output dependent on CIAO's debugging flag.
  ACE_DEBUG ((LM_DEBUG, "Exec Dtor\n"));
}

// @@ (OO) Method definitions should never use "_WITH_DEFAULTS"
//         versions of emulated exception parameters.  Please remove
//         the "_WITH_DEFAULTS"

Deployment::DomainApplicationManager_ptr
CIAO::ExecutionManager_Impl::
preparePlan (const Deployment::DeploymentPlan &plan,
	     CORBA::Boolean
	     ACE_ENV_ARG_DECL_WITH_DEFAULTS)
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
  // @@ (OO) You're missing an
  //           ACE_CHECK_RETURN (Deployment::DomainApplicationManager::_nil())
  //         here.  Emulated exceptions won't function correctly
  //         without it.

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

  // @@ (OO) Even though the return value of zero works,
  //         Deployment::DomainApplicationManager::_nil () should
  //         technically be returned instead since "_ptr" need not be
  //         C++ pointer typedefs.  They could conceivably be classes,
  //         for example.
  ACE_CHECK_RETURN (0);

  // Get the reference of the object.
  CORBA::Object_var objref
    = this->poa_->id_to_reference (oid.in ()
				   ACE_ENV_ARG_PARAMETER);
  // @@ (OO) Same here.  Deployment::DomainApplicationManager::_nil ()
  //         instead of zero.
  ACE_CHECK_RETURN (0);

  this->dam_ =
    Deployment::DomainApplicationManager::_narrow (objref.in ()
						   ACE_ENV_ARG_PARAMETER);
  // @@ (OO) Same here.  Deployment::DomainApplicationManager::_nil ()
  //         instead of zero.
  ACE_CHECK_RETURN (0);

  // Return the ApplicationManager instance
  return Deployment::DomainApplicationManager::_duplicate (this->dam_.in ());
}


// @@ (OO) Method definitions should never use "_WITH_DEFAULTS"
//         versions of emulated exception parameters.  Please remove
//         the "_WITH_DEFAULTS"
Deployment::DomainApplicationManagers *
CIAO::ExecutionManager_Impl::getManagers (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // @@ (OO) Since you're not doing anything with the allocated
  //         sequence, there is no need to store in a "_var".  Simply
  //         return the pointer.

  // Initialize the list of DomainApplication Managers
  Deployment::DomainApplicationManagers_var list = 0;
  ACE_NEW_THROW_EX (list,
                    Deployment::DomainApplicationManagers,
                    CORBA::NO_MEMORY());
  // @@ (OO) You're missing an ACE_CHECK_RETURN (0) here.  Emulated
  //         exceptions won't function correctly without it.

  // @@ (OO) Should remove the above allocation and instead throw a
  //         CORBA::NO_IMPLEMENT() instead?
  //
  //         Production code should not display debugging output
  //         unless requested by the user.  You should probably make
  //         the following output dependent on CIAO's debugging flag.
  ACE_DEBUG ((LM_DEBUG, "Not Implemented!\n"));

  // @@ (OO) This is redundant.  A default constructed sequence
  //         already has a length of zero.  Please remove this.
  list->length (0);
  return list._retn ();
}

// @@ (OO) Method definitions should never use "_WITH_DEFAULTS"
//         versions of emulated exception parameters.  Please remove
//         the "_WITH_DEFAULTS"
void
CIAO::ExecutionManager_Impl::destroyManager (Deployment::DomainApplicationManager_ptr manager
                                             ACE_ENV_ARG_DECL_WITH_DEFAULTS)
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

// @@ (OO) Method definitions should never use "_WITH_DEFAULTS"
//         versions of emulated exception parameters.  Please remove
//         the "_WITH_DEFAULTS"
void
CIAO::ExecutionManager_Impl::shutdown (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // @@ (OO) You're using the wrong emulated exception macro below.
  //         Please use ACE_ENV_ARG_PARAMETER instead.

  // Shutdown the ORB on which it is runing
  this->orb_->shutdown (0 ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);
}
