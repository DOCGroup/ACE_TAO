// $Id$

#include "Assembly_Impl.h"

#if !defined (__ACE_INLINE__)
# include "Assembly_Impl.inl"
#endif /* __ACE_INLINE__ */

CIAO::AssemblyFactory_Impl::~AssemblyFactory_Impl ()
{
  // @@ remove all Containers?
}

PortableServer::POA_ptr
CIAO::AssemblyFactory_Impl::_default_POA (void)
{
  return PortableServer::_duplicate (this->poa_.in ());
}

int
CIAO::AssemblyFactory_Impl::init (ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // @@ Initialize AssemblyFactory and create the internal container
  // implementation that actually interacts with installed
  // homes/components.

  // We will probably need two ORBs in this process.  One for the
  // deployment framework, and one for the actual components.
}

::Components::Cookie *
CIAO::AssemblyFactory::create (const char * assembly_loc
                               ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::Deployment::InvalidLocation,
                   Components::CreateFailure))
{
}

::Components::Deployment::Assembly_ptr
CIAO::AssemblyFactory::lookup (Components::Cookie * c
                               ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::Deployment::InvalidAssembly))
{
}

void
CIAO::AssemblyFactory::destroy (Components::Cookie * c
                                ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::Deployment::InvalidAssembly,
                   Components::RemoveFailure))
{
}


CIAO::Assembly_Impl::~Assembly_Impl ()
{
  // @@ remove all Containers?
}

PortableServer::POA_ptr
CIAO::Assembly_Impl::_default_POA (void)
{
  return PortableServer::_duplicate (this->poa_.in ());
}

int
CIAO::Assembly_Impl::init (ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // @@ Initialize Assembly and create the internal container
  // implementation that actually interacts with installed
  // homes/components.

  // We will probably need two ORBs in this process.  One for the
  // deployment framework, and one for the actual components.
}

void
CIAO::Assembly_Impl::build (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
CIAO::Assembly_Impl::tear_down (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::RemoveFailure))
{
}

::Components::Deployment::AssemblyState
CIAO::Assembly_Impl::get_state (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}
