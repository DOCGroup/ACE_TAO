// $Id$

#include "Assembly_Service_Impl.h"

CIAO::Assembly_Service_Impl::Assembly_Service_Impl (CORBA::ORB_ptr o,
                                                    PortableServer::POA_ptr p,
                                                    Components::Deployment::AssemblyFactory_ptr f)
  : orb_ (CORBA::ORB::_duplicate (o)),
    poa_ (PortableServer::POA::_duplicate (p)),
    factory_ (Components::Deployment::AssemblyFactory::_duplicate (f))
{
}

CIAO::Assembly_Service_Impl::~Assembly_Service_Impl (void)
{
}

PortableServer::POA_ptr
CIAO::Assembly_Service_Impl::_default_POA (void)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

Components::Deployment::AssemblyFactory_ptr
CIAO::Assembly_Service_Impl::get_assemblyfactory (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return Components::Deployment::AssemblyFactory::_duplicate (this->factory_.in ());
}

void
CIAO::Assembly_Service_Impl::shutdown (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown ();
}
