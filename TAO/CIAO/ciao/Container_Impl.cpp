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
  return PortableServer::_duplicate (this->poa_.in ());
}

int
CIAO::Container_Impl::init (::Components::ConfigValues &options
                               ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // @@ Initialize container and create the internal container
  // implementation that actually interacts with installed
  // homes/components.
}

::Components::ConfigValues *
CIAO::Container_Impl::configuration (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // return a copy of configration values.
}

::Components::Deployment::ComponentServer_ptr
CIAO::Container_Impl::get_component_server (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
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

  // It seems to me that the intention of CCM spec is to get
  // Assembly/AssemblyFactory to parse all the XML files and convert
  // all the properties files and such into ConfigValues.
}

void
CIAO::Container_Impl::remove_home (Components::CCMHome_ptr href
                                      ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::RemoveFailure))
{
}

::Components::CCMHomes *
CIAO::Container_Impl::get_homes (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
CIAO::Container_Impl::remove (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::RemoveFailure))
{
}
