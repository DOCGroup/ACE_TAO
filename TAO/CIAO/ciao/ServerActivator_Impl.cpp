// $Id$

#include "ServerActivator_Impl.h"

#if !defined (__ACE_INLINE__)
# include "ServerActivator_Impl.inl"
#endif /* __ACE_INLINE__ */

CIAO::ServerActivator_Impl::~ServerActivator_Impl ()
{
  // @@ remove all Containers?
}

PortableServer::POA_ptr
CIAO::ServerActivator_Impl::_default_POA (void)
{
  return PortableServer::_duplicate (this->poa_.in ());
}

int
CIAO::ServerActivator_Impl::init (ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // @@ Initialize ServerActivator and create the internal container
  // implementation that actually interacts with installed
  // homes/components.

  // We will probably need two ORBs in this process.  One for the
  // deployment framework, and one for the actual components.
}

::Components::Deployment::ComponentServer_ptr
CIAO::ServerActivator::create_component_server (const Components::ConfigValues & config
                                                ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CreateFailure,
                   Components::InvalidConfiguration))
{
}

void
CIAO::ServerActivator::remove_component_server (Components::Deployment::ComponentServer_ptr server
                                                ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::RemoveFailure))
{
}

::Components::Deployment::ComponentServers *
CIAO::ServerActivator::get_component_servers (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}
