//$Id$

#include "NodeApplication_Impl.h"

#if !defined (__ACE_INLINE__)
# include "NodeApplication_Impl.inl"
#endif /* __ACE_INLINE__ */

CIAO::NodeApplication_Impl::~NodeApplication_Impl ()
{
  //@@ This should clean up all components and homes.
  //@@ I am not clear about if the destroy of home will also clean
  //   components.
}

void
CIAO::NodeApplication_Impl::
finishLaunch (const Deployment::Connections & providedReference,
	      CORBA::Boolean start
	      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException,
		   Deployment::StartError,
		   Deployment::InvalidConnection))
{
  //@@ not implemented yet.
}

void
start (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException,
		   Deployment::StartError))
{
  //@@ Call CCM_activate ....
}

::Deployment::Properties *
CIAO::NodeApplication_Impl::
properties (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ::Deployment::Properties *retval;

  ACE_NEW_THROW_EX (retval,
                    Deployment::Properties (),
                    CORBA::INTERNAL ());
  ACE_CHECK_RETURN (0);

  *retval = this->properties_;

  return retval;
}

::Components::CCMHome_ptr
CIAO::NodeApplication_Impl::
install_home (const ::Deployment::Properties  & properties
	      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException,
		   Deployment::UnknownImplId,
		   Deployment::ImplEntryPointNotFound,
		   Deployment::InstallationFailure,
		   Components::InvalidConfiguration))
{
  struct home_installation_info config_info;
  this->parse_config_values (properties,
			     config_info
                             ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  Components::CCMHome_var newhome
    = this->container_->ciao_install_home
    (config_info.executor_dll_.in (),
     config_info.executor_entrypt_.in (),
     config_info.servant_dll_.in (),
     config_info.servant_entrypt_.in ()
     ACE_ENV_ARG_PARAMETER);

  ACE_CHECK_RETURN (0);

  this->home_set_.add (newhome.in ());
  return newhome._retn ();
}

void
CIAO::NodeApplication_Impl::
remove_home (Components::CCMHome_ptr href
	     ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException,
		   Components::RemoveFailure))
{
  if (this->home_set_.object_in_set (href) == 0)
    ACE_THROW (CORBA::BAD_PARAM ());

  // @@ Finalizing home... how?  Removing all the components, but how?
  // It looks like the component home will also need to keep a record
  // of all the components it creates.
  this->container_->ciao_uninstall_home (href
                                         ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // @@ Still need to remove the home if the previous operation fails?
  if (this->home_set_.remove (href) == -1)
    ACE_THROW (::Components::RemoveFailure ());
}

::Components::CCMHomes *
CIAO::NodeApplication_Impl::
get_homes (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  Components::CCMHomes_var retval;

  ACE_NEW_THROW_EX (retval.out (),
                    Components::CCMHomes (),
                    CORBA::INTERNAL ());
  ACE_CHECK_RETURN (0);

  CORBA::ULong len = this->home_set_.size ();
  retval->length (len);

#if 0
  // TAO is broken here.  Both <replace>, <get_buffer> and friends are missing.
  this->home_set_.copy (len, retval->get_buffer (0));
#else

  for (CORBA::ULong i = 0; i < len; ++i)
    {
      retval[i] = this->home_set_.at (i);
    }
#endif

  return retval._retn ();

}

void
CIAO::NodeApplication_Impl::
remove (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException,
		   Components::RemoveFailure))
{
  // @@ Need to remove all CCMHome

  ACE_THROW (CORBA::NO_IMPLEMENT ());
  //ACE_DEBUG ((LM_DEBUG, "CIAO::NodeApplication_Impl::remove\n"));
}

CORBA::Long
CIAO::NodeApplication_Impl::
init (const ::Deployment::Properties & properties
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException,
		   Components::InvalidConfiguration))
{
  // Create an internal Container
  // The underlying container implementation supports old static configuration.
  // Here I just ignore it.    --Tao
  ACE_NEW_THROW_EX (this->container_,
                    CIAO::Session_Container (this->orb_.in ()),
                    CORBA::INTERNAL ());
  ACE_CHECK_RETURN (-1);

  return this->container_->init (0,
                                 0
                                 ACE_ENV_ARG_PARAMETER);
}

::CORBA::Object_ptr
CIAO::NodeApplication_Impl::
get_node_application_manager (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return ::CORBA::Object::_duplicate (this->node_app_manager_.in ());
}

void
CIAO::NodeApplication_Impl::
parse_config_values (const ::Deployment::Properties & properties,
  struct home_installation_info &component_install_info
  ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException,
    Deployment::UnknownImplId,
    Deployment::ImplEntryPointNotFound,
    Components::InvalidConfiguration))
{

  for (CORBA::ULong i = 0; i < properties.length(); ++i)
  {
    // Place holder for string values
    const char * str = 0;
    const char * name = properties[i].name.in();

    // I assume the property will be given in the following format! --Tao
    if (ACE_OS::strcmp (name, "CIAO-servant-location"))
    {
      properties[i].value >>= str;
      component_install_info.servant_dll_= str;  //deep copy happens here.
    }
    else if (ACE_OS::strcmp (name, "CIAO-servant-entryPoint"))
    {
      properties[i].value >>= str;
      component_install_info.servant_entrypt_= str;  //deep copy happens here.
    }
    else if (ACE_OS::strcmp (name, "CIAO-executor-location"))
    {
      properties[i].value >>= str;
      component_install_info.executor_dll_= str;  //deep copy happens here.
    }
    else if (ACE_OS::strcmp (name, "CIAO-executor-entryPoint"))
    {
      properties[i].value >>= str;
      component_install_info.executor_entrypt_= str;  //deep copy happens here.
    }
    else
    {
      ACE_DEBUG ((LM_DEBUG, "Found unrecognized property: %s\n",name));
      //I should put the name of the configuration inside of the exception. --Tao
      ACE_THROW (Components::InvalidConfiguration ());
    }
  }

  // We don't need to worry about the case when some info is missing since
  // the modeling tool will ensure the complete info to presented in the properties.
}

PortableServer::POA_ptr
CIAO::NodeApplication_Impl::
_default_POA (void)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}
