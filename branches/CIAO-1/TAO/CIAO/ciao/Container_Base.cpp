// $Id$

#include "ciao/Container_Base.h"

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
       this->number_ = ++serial_number_;
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
  return 0;
}

void
CIAO::Session_Container::uninstall_reference (CORBA::Object_ptr objref
                                              ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}
