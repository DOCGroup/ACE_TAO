// $Id$

#include "ComponentServer_Impl.h"

#if !defined (__ACE_INLINE__)
# include "ComponentServer_Impl.inl"
#endif /* __ACE_INLINE__ */

CIAO::ComponentServer_Impl::~ComponentServer_Impl ()
{
  // @@ remove all Containers?
}

PortableServer::POA_ptr
CIAO::ComponentServer_Impl::_default_POA (void)
{
  return PortableServer::_duplicate (this->poa_.in ());
}

int
CIAO::ComponentServer_Impl::init (::Components::ConfigValues &options
                                  ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // @@ Initialize ComponentServer and create the internal container
  // implementation that actually interacts with installed
  // homes/components.

  // We will probably need two ORBs in this process.  One for the
  // deployment framework, and one for the actual components.
}

::Components::ConfigValues *
CIAO::ComponentServer_Impl::configuration (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // return a copy of configration values.
}

::Components::Deployment::ServerActivator_ptr
CIAO::ComponentServer_Impl::get_server_activator (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

::Components::Deployment::Container_ptr
CIAO::ComponentServer_Impl::create_container (const Components::ConfigValues & config
                                              ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CreateFailure,
                   Components::InvalidConfiguration))
{

}

void
CIAO::ComponentServer_Impl::remove_container (Components::Deployment::Container_ptr cref
                                              ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::RemoveFailure))
{
}

::Components::Deployment::Containers *
CIAO::ComponentServer_Impl::get_containers (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
CIAO::ComponentServer_Impl::remove (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::RemoveFailure))
{
}
