/* -*- C++  -*- */
// $Id$

#include "ExecutionManager_Impl.h"

CIAO::ExecutionManager_Impl::ExecutionManager_Impl (CORBA::ORB_ptr orb,
                                                    PortableServer::POA_ptr poa,
                                                    const char * init_file)
  : orb_ (CORBA::ORB::_duplicate  (orb)),
    poa_ (PortableServer::POA::_duplicate (poa)),
    init_file_ (CORBA::string_dup (init_file))
{

}

CIAO::ExecutionManager_Impl::~ExecutionManager_Impl ()
{
  // Delete the Map for maintaining
  for (Iterator i = this->table_.begin ();
       i != this->table_.end ();
       ++i)
    {
      // Deallocate the id.
      CORBA::string_free (ACE_const_cast (char *, (*i).ext_id_));

      // Release the Object.
      CORBA::release ((*i).int_id_);
    }

  this->table_.unbind_all ();

  // Release memory for init file
  CORBA::string_free (this->init_file_);

}

int
CIAO::ExecutionManager_Impl::bind (const char *id,
                                   Deployment::DomainApplicationManager_ptr obj)
{
  // Make sure that the supplied Object reference is valid,
  // i.e. not nil.
  if (id == 0 || CORBA::is_nil (obj))
    {
      errno = EINVAL;
      return -1;
    };

  CORBA::String_var name = CORBA::string_dup (id);
  Deployment::DomainApplicationManager_var object =
    Deployment::DomainApplicationManager::_duplicate (obj);

  int result = this->table_.bind (name.in (),
                                  object.in ());

  if (result == 0)
    {
      // Transfer ownership to the Object Table.
      (void) name._retn ();
      (void) object._retn ();
    }

  return result;
}

int
CIAO::ExecutionManager_Impl::unbind (const char *id)
{
  Table::ENTRY *entry = 0;

  int result = this->table_.find (id, entry);

  if (result == 0)
    {
      // Deallocate the external ID and obtain the ORB core pointer
      // before unbinding the entry since the entry is deallocated
      // during the call to unbind().
      CORBA::string_free (ACE_const_cast (char *, entry->ext_id_));
      CORBA::Object_ptr obj = entry->int_id_;

      result = this->table_.unbind (entry);

      if (result != 0)
        return result;

      CORBA::release (obj);
    }

  return result;
}

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
  // Check if the plan already exists and the ApplicationManager is active
  // Then return the same
  Deployment::DomainApplicationManager * temp = 0;
  if (this->table_.find (plan.UUID, temp) == 0)
    return Deployment::DomainApplicationManager::_duplicate (temp);

  // Create a new DomainApplicationManager
  CIAO::DomainApplicationManager_Impl * servant = 0;
  ACE_NEW_THROW_EX (servant,
                    CIAO::DomainApplicationManager_Impl
                    (this->orb_.in (),
                     this->poa_.in (),
                     Deployment::TargetManager::_nil (),
                     plan,
                     this->init_file_),
                    CORBA::NO_MEMORY ());

  /**
   *===================================================================
   * MAIN STEP: This call parses the deployment plan, generates the Node
   * specific plan and starts the deployment process
   *===================================================================
   */
  servant->init ();

  ACE_DEBUG ((LM_DEBUG, "ExecutionManager:init () DAM successful \n"));

  // Things are ready for Launching Applications
  PortableServer::ServantBase_var save_servant (servant);

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

  Deployment::DomainApplicationManager_var manager =
    Deployment::DomainApplicationManager::_narrow (objref.in ()
                                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  // Bind this reference on to the cache
  this->bind (plan.UUID, manager.in ());

  // Return the ApplicationManager instance
  return manager._retn ();
}


Deployment::DomainApplicationManagers *
CIAO::ExecutionManager_Impl::getManagers (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Initialize the list of DomainApplication Managers
  Deployment::DomainApplicationManagers_var list = 0;
  ACE_NEW_THROW_EX (list,
                    Deployment::DomainApplicationManagers,
                    CORBA::NO_MEMORY());

  // Iterate over the table and get all the Managers
  // Delete the Map for maintaining
  for (Iterator itr = this->table_.begin ();
       itr != this->table_.end ();
       ++itr)
    {
      CORBA::ULong i = list->length ();
      list->length (i + 1);
      list [i] =
        Deployment::DomainApplicationManager::_duplicate ((*itr).int_id_);
    }

  // Return the list
  return list._retn ();
}


void
CIAO::ExecutionManager_Impl::destroyManager (Deployment::DomainApplicationManager_ptr manager
                                             ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Deployment::StopError))
{
  if (CORBA::is_nil (manager))
    ACE_THROW (Deployment::StopError ());

  //@@ I probably should make sure that the pointer is a valid
  // objectref here, which implies searching in the list. --Tao
  // An easier way is below: Do a reference to id, if it fails, then this
  // manager was not created and registered in the first place! -- Arvind
  ACE_TRY
  {
    Deployment::DomainApplicationManager_var
      tmp (manager);

    // Deactivate the Object in the POA
    PortableServer::ObjectId_var oid
      = this->poa_->reference_to_id (tmp.in ()
                                     ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;

    this->poa_->deactivate_object (oid.in ()
                                   ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;

    //Obtain the servant pointer
    PortableServer::ServantBase_var serv_base =
      this->poa_->reference_to_servant (tmp.in ()
                                        ACE_ENV_ARG_PARAMETER);

    // Narrow it to a DomainApplicationManager_Impl
    CIAO::DomainApplicationManager_Impl * serv =
    ACE_dynamic_cast (CIAO::DomainApplicationManager_Impl *,
                      serv_base.in ());

    // Remove the reference from the table using the uuid
    this->unbind (serv->get_uuid ());
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
CIAO::ExecutionManager_Impl::shutdown (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Shtudown the ORB on which it is runing
  this->orb_->shutdown (1 ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);
}
