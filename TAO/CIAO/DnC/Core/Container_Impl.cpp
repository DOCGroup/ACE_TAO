// $Id$
#include "Container_Impl.h"

#if !defined (__ACE_INLINE__)
# include "Container_Impl.inl"
#endif /* __ACE_INLINE__ */

CIAO::Container_Impl::~Container_Impl ()
{
  // @@ remove all home?
  // @@ To-do: We need to do cleaning if the container are going
  //    to be destroyed.
}

PortableServer::POA_ptr
CIAO::Container_Impl::_default_POA (void)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

int
CIAO::Container_Impl::init (const ::Deployment::Properties &properties
                            ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  //@@ This is definitely not right. I need to fix this! --Tao
  this->con_properties_ = properties;

  // @@ Initialize container and create the internal container
  // implementation that actually interacts with installed
  // homes/components.

  // @@ We will need a container factory here later on when we support
  // more kinds of container implementations.
  // @@ Ignore all static configuration. --Tao
  ACE_NEW_THROW_EX (this->container_,
                    CIAO::Session_Container (this->orb_.in ()),
                    CORBA::INTERNAL ());
  ACE_CHECK_RETURN (-1);

  return this->container_->init (0,                     //name of the container
                                 0                      //POA_Policies
                                 ACE_ENV_ARG_PARAMETER);
}

::Deployment::Properties *
CIAO::Container_Impl::con_properties (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ::Deployment::Properties *retval;

  ACE_NEW_THROW_EX (retval,
                    Deployment::Properties (),
                    CORBA::INTERNAL ());
  ACE_CHECK_RETURN (0);

  *retval = this->con_properties_;

  return retval;
}

::CORBA::Object_ptr
CIAO::Container_Impl::get_node_application (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return ::CORBA::Object::_duplicate (this->nodeapp_.in ());
}

::Components::CCMHome_ptr
CIAO::Container_Impl::install_home (const ::Deployment::Properties & properties
                                    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::Deployment::UnknownImplId,
                   Components::Deployment::ImplEntryPointNotFound,
                   Components::Deployment::InstallationFailure,
                   Components::InvalidConfiguration))
{
  // use id to locate the softpkg from the ComponentInstallation interface.
  // depending on where we want to parse the softpkg descriptor, we
  // can either use the <entrypt> directly (if we parse the softpkg in
  // Assembly), simply verify that is correct, or even discard this
  // value.

  // @@ Here we need to resolve paths to both component executors and
  // component servants (that matches the container type) for the
  // executor and their entry points before we can install the home.
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

  {
    ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->lock_, 0);

    this->home_set_.add (newhome.in ());
  }
  return newhome._retn ();
}

void
CIAO::Container_Impl::remove_home (Components::CCMHome_ptr href
                                   ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::RemoveFailure))
{
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock_);

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
CIAO::Container_Impl::get_homes (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->lock_, 0);

  Components::CCMHomes_var retval;

  ACE_NEW_THROW_EX (retval.out (),
                    Components::CCMHomes (),
                    CORBA::INTERNAL ());
  ACE_CHECK_RETURN (0);

  CORBA::ULong len = this->home_set_.size ();
  retval->length (len);          // resize

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
CIAO::Container_Impl::remove (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::RemoveFailure))
{
  // @@ Need to remove all CCMHome

  ACE_THROW (CORBA::NO_IMPLEMENT ());

  ACE_DEBUG ((LM_DEBUG, "CIAO::Container_Impl::remove\n"));
}

void
CIAO::Container_Impl::parse_config_values (const ::Deployment::Properties & properties,
                                           struct home_installation_info &component_install_info
                                           ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::Deployment::UnknownImplId,
                   Components::Deployment::ImplEntryPointNotFound,
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
}
