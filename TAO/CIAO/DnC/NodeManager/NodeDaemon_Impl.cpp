// $Id$

#include "NodeDaemon_Impl.h"
#include "NodeApplicationManager_Impl.h"

CIAO::NodeDaemon_Impl::NodeDaemon_Impl (const char *name,
                                        CORBA::ORB_ptr orb,
                                        PortableServer::POA_ptr poa)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    poa_ (PortableServer::POA::_duplicate (poa)),
    name_ (CORBA::string_dup (name))
{
}

CIAO::NodeDaemon_Impl::~NodeDaemon_Impl ()
{
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

int
CIAO::NodeDaemon_Impl::bind (const char *id,
                             CORBA::Object_ptr obj)
{
  // Make sure that the supplied Object reference is valid,
  // i.e. not nil.
  if (id == 0 || CORBA::is_nil (obj))
    {
      errno = EINVAL;
      return -1;
    };

  CORBA::String_var name = CORBA::string_dup (id);
  CORBA::Object_var object = CORBA::Object::_duplicate (obj);

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
CIAO::NodeDaemon_Impl::unbind (const char *id)
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

///////////////////////////////////////////////////////////////////////
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
CIAO::NodeDaemon_Impl::preparePlan (const Deployment::DeploymentPlan & plan
                                    ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Deployment::StartError,
                   Deployment::PlanError))
{
  //Implementation undefined.
  CIAO::NodeApplicationManager_Impl *app_mgr;
  ACE_NEW_THROW_EX (app_mgr, 
                    CIAO::NodeApplicationManager_Impl (plan, 
                                                       this->orb_.in (), 
                                                       this->poa_.in ()),
                    CORBA::NO_MEMORY ());

  // Activate the object in this POA
  PortableServer::ObjectId_var id = this->poa_->activate_object (app_mgr);
  
  // Return the Application Manager
  CORBA::Object_var obj_ref = this->poa_->id_to_reference (id.in ());
  return Deployment::NodeApplicationManager::_narrow (obj_ref.in ());
}


void
CIAO::NodeDaemon_Impl::destroyManager (Deployment::NodeApplicationManager_ptr 
                                       ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException, Deployment::StopError))
{
}
