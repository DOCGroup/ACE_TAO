// $Id$

#include "ComponentInstallation_Impl.h"

#if !defined (__ACE_INLINE__)
# include "ComponentInstallation_Impl.inl"
#endif /* __ACE_INLINE__ */

CIAO::ComponentInstallation_Impl::~ComponentInstallation_Impl ()
{
  // @@ remove all Containers?
}

PortableServer::POA_ptr
CIAO::ComponentInstallation_Impl::_default_POA (void)
{
  return PortableServer::_duplicate (this->poa_.in ());
}

int
CIAO::ComponentInstallation_Impl::init (ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // @@ Initialize ComponentInstallation and create the internal container
  // implementation that actually interacts with installed
  // homes/components.

  // We will probably need two ORBs in this process.  One for the
  // deployment framework, and one for the actual components.
}

void
CIAO::ComponentInstallation_Impl::install (const char * implUUID,
                                           const char * component_loc
                                           ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::Deployment::InvalidLocation,
                   Components::Deployment::InstallationFailure))
{
}

void
CIAO::ComponentInstallation_Impl::replace (const char * implUUID,
                                           const char * component_loc
                                           ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::Deployment::InvalidLocation,
                   Components::Deployment::InstallationFailure))
{
}

void
CIAO::ComponentInstallation_Impl::remove (const char * implUUID
                                          ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::Deployment::UnknownImplId,
                   Components::RemoveFailure))
{
}

char *
CIAO::ComponentInstallation_Impl::get_implementation (const char * implUUID
                                                      ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::Deployment::UnknownImplId,
                   Components::Deployment::InstallationFailure))
{
}
