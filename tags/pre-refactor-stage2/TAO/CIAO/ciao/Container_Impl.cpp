// $Id$
#include "Container_Impl.h"

#if !defined (__ACE_INLINE__)
# include "Container_Impl.inl"
#endif /* __ACE_INLINE__ */

CIAO::Container_Impl::~Container_Impl ()
{
  // @@ remove all home?
}

PortableServer::POA_ptr
CIAO::Container_Impl::_default_POA (void)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

int
CIAO::Container_Impl::init (const ::Components::ConfigValues &options,
                            Components::Deployment::ComponentInstallation_ptr inst
                            ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_NEW_THROW_EX (this->config_,
                    Components::ConfigValues (),
                    CORBA::INTERNAL ());
  ACE_CHECK_RETURN (-1);

  *this->config_ = options;
  this->installation_ =
    Components::Deployment::ComponentInstallation::_duplicate (inst);

  // @@ Initialize container and create the internal container
  // implementation that actually interacts with installed
  // homes/components.

  // @@ We will need a container factory here later on when we support
  // more kinds of container implementations.

  // @@ Fish out the ComponentServer object reference from <options>.

  ACE_NEW_THROW_EX (this->container_,
                    CIAO::Session_Container (this->orb_.in ()),
                    CORBA::INTERNAL ());
  ACE_CHECK_RETURN (-1);

  return this->container_->init (ACE_ENV_SINGLE_ARG_PARAMETER);
}

::Components::ConfigValues *
CIAO::Container_Impl::configuration (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  Components::ConfigValues *retval;

  ACE_NEW_THROW_EX (retval,
                    Components::ConfigValues (),
                    CORBA::INTERNAL ());
  ACE_CHECK_RETURN (0);

  *retval = this->config_;

  return retval;
}

::Components::Deployment::ComponentServer_ptr
CIAO::Container_Impl::get_component_server (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return Components::Deployment::ComponentServer::_duplicate (this->comserv_.in ());
}

::Components::CCMHome_ptr
CIAO::Container_Impl::install_home (const char * id,
                                    const char * entrypt,
                                    const Components::ConfigValues & config
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
  this->parse_config_values (id,
                             config,
                             config_info
                             ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  Components::CCMHome_var newhome
    = this->container_->ciao_install_home
    (config_info.executor_dll_.in (),
     entrypt,
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
CIAO::Container_Impl::remove (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::RemoveFailure))
{
  // @@ Need to remove all CCMHome

  //  ACE_THROW (CORBA::NO_IMPLEMENT ());

  ACE_DEBUG ((LM_DEBUG, "CIAO::Container_Impl::remove\n"));
}

void
CIAO::Container_Impl::parse_config_values (const char *id,
                                           const Components::ConfigValues &options,
                                           struct home_installation_info &component_install_info
                                           ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::Deployment::UnknownImplId,
                   Components::Deployment::ImplEntryPointNotFound,
                   Components::InvalidConfiguration))
{
  CORBA::String_var servant_uuid;

  for (CORBA::ULong i = 0; i < options.length (); ++i)
    {
      CORBA::String_var *info;
      const char *str_in = 0;

      // @@ The following code need cleaning up.
      if (ACE_OS::strcmp (options[i]->name (), "CIAO-servant-UUID") == 0)
        info = &servant_uuid;
      else if (ACE_OS::strcmp (options[i]->name (), "CIAO-servant-entrypt") == 0)
        info = &component_install_info.servant_entrypt_;
      else
        {
          Components::InvalidConfiguration exc;
          exc.name = CORBA::string_dup (options[i]->name ());
          exc.reason = Components::UnknownConfigValueName;
          ACE_THROW (exc);
        }

      if (options[i]->value () >>= str_in)
        {
          *info = CORBA::string_dup (str_in);
#if 0
          ACE_DEBUG ((LM_DEBUG, "*parse_config_values got (%s) = %s\n",
                      options[i]->name (),
                      str_in));
#endif /* 0 */
        }
      else
        {
          Components::InvalidConfiguration exc;
          exc.name = CORBA::string_dup (options[i]->name ());
          exc.reason = Components::InvalidConfigValueType;
          ACE_THROW (exc);
        }
    }

  component_install_info.executor_dll_ =
    this->installation_->get_implementation (id
                                             ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  component_install_info.servant_dll_ =
    this->installation_->get_implementation (servant_uuid.in ()
                                             ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (component_install_info.executor_dll_.in () == 0 ||
      component_install_info.servant_dll_.in () == 0 ||
      component_install_info.servant_entrypt_.in () == 0)
    {
      Components::InvalidConfiguration exc;
      // The following should really be the exact missing configvalue name:
      exc.name = CORBA::string_dup ("home_installation_info");
      exc.reason = Components::ConfigValueRequired;
      ACE_THROW (exc);
    }
}
