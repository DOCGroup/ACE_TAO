// $Id$

#include "Container_Base.h"
#include "ace/DLL.h"

////////////////////////////////////////////////////////////////

CIAO::Container::Container (CORBA::ORB_ptr o)
  : orb_ (CORBA::ORB::_duplicate (o))
{
}

CIAO::Container::~Container ()
{
}

PortableServer::POA_ptr
CIAO::Container::the_POA (void)
{
  return this->poa_.in ();
}

///////////////////////////////////////////////////////////////

ACE_Atomic_Op <ACE_Thread_Mutex, long>
CIAO::Session_Container::serial_number_ (0);

CIAO::Session_Container::Session_Container (CORBA::ORB_ptr o)
  : Container (o),
    number_ (0)
{
}

CIAO::Session_Container::~Session_Container ()
{
}

int
CIAO::Session_Container::init (const char *name
                               ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  char buffer[MAXPATHLEN];

   if (name == 0)
     {
       this->number_ = ++CIAO::Session_Container::serial_number_;
       ACE_OS::sprintf (buffer, "CIAO::Session_Container-%d",
                        this->number_);
     }

   CORBA::Object_var poa_object =
     this->orb_->resolve_initial_references("RootPOA" ACE_ENV_ARG_PARAMETER);
   ACE_CHECK_RETURN (-1);

   if (CORBA::is_nil (poa_object.in ()))
     ACE_ERROR_RETURN ((LM_ERROR,
                        " (%P|%t) Unable to initialize the POA.\n"),
                       -1);

   PortableServer::POA_var root_poa =
     PortableServer::POA::_narrow (poa_object.in () ACE_ENV_ARG_PARAMETER);
   ACE_CHECK_RETURN (-1);

   // Set up proper poa policies here.  Default policies seems to be
   // fine for session container.
   CORBA::PolicyList policies (0);

   this->poa_ = root_poa->create_POA (name,
                                      PortableServer::POAManager::_nil (),
                                      policies
                                      ACE_ENV_ARG_PARAMETER);
   ACE_CHECK_RETURN (-1);

   PortableServer::POAManager_var poa_manager =
     this->poa_->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
   ACE_CHECK_RETURN (-1);

   poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
   ACE_CHECK_RETURN (-1);

   return 0;
}

CORBA::Object_ptr
CIAO::Session_Container::install_servant (PortableServer::Servant p
                                          ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  PortableServer::ObjectId_var oid
    = this->poa_->activate_object (p
                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  CORBA::Object_var objref
    = this->poa_->id_to_reference (oid
                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  p->_remove_ref (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return objref._retn ();
}

Components::CCMHome_ptr
CIAO::Session_Container::_ciao_install_home (const char *exe_dll_name,
                                             const char *exe_entrypt,
                                             const char *sv_dll_name,
                                             const char *sv_entrypt
                                             ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::Deployment::UnknownImplId,
                   Components::Deployment::ImplEntryPointNotFound,
                   Components::Deployment::InstallationFailure))
{
  ACE_DLL executor_dll, servant_dll;

  if (exe_dll_name == 0 || sv_dll_name == 0)
    ACE_THROW_RETURN (Components::Deployment::UnknownImplId (), 0);

  if (executor_dll.open (exe_dll_name,
                         ACE_DEFAULT_SHLIB_MODE,
                         0) != 0)
    ACE_THROW_RETURN (Components::Deployment::UnknownImplId (), 0);

  if (servant_dll.open (sv_dll_name,
                        ACE_DEFAULT_SHLIB_MODE,
                        0) != 0)
    {
      executor_dll.close ();
      ACE_THROW_RETURN (Components::Deployment::UnknownImplId (), 0);
    }

  if (exe_entrypt == 0 || sv_entrypt == 0)
    ACE_THROW_RETURN (Components::Deployment::ImplEntryPointNotFound (), 0);

  HomeFactory hcreator = (HomeFactory) executor_dll.symbol (exe_entrypt);
  ServantFactory screator = (ServantFactory) servant_dll.symbol (sv_entrypt);

  if (hcreator == 0 || screator == 0)
    ACE_THROW_RETURN (Components::Deployment::ImplEntryPointNotFound (), 0);

  Components::HomeExecutorBase_var home_executor = hcreator ();
  if (CORBA::is_nil (home_executor.in ()))
    ACE_THROW_RETURN (Components::Deployment::InstallationFailure (), 0);

  PortableServer::Servant home_servant = screator (home_executor.in (),
                                                   this
                                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  if (home_servant == 0)
    ACE_THROW_RETURN (Components::Deployment::InstallationFailure (), 0);

  CORBA::Object_var objref = this->install_servant (home_servant
                                                    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  Components::CCMHome_var homeref = Components::CCMHome::_narrow (objref.in ()
                                                                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return homeref._retn ();

}

void
CIAO::Session_Container::uninstall (CORBA::Object_ptr objref
                                    ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  PortableServer::ObjectId_var oid
    = this->poa_->reference_to_id (objref
                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->poa_->deactivate_object (oid
                                 ACE_ENV_ARG_PARAMETER);
}

void
CIAO::Session_Container::uninstall (PortableServer::Servant svt
                                    ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  PortableServer::ObjectId_var oid
    = this->poa_->servant_to_id (svt
                                 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->poa_->deactivate_object (oid
                                 ACE_ENV_ARG_PARAMETER);
}

void
CIAO::Session_Container::debug_uninstall (CORBA::Object_ptr objref
                                          ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  PortableServer::ObjectId_var oid
    = this->poa_->reference_to_id (objref
                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  PortableServer::Servant sv
    = this->poa_->id_to_servant (oid
                                 ACE_ENV_ARG_PARAMETER);

  sv = sv;

  this->poa_->deactivate_object (oid
                                 ACE_ENV_ARG_PARAMETER);
}
