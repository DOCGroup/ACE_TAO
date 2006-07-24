//
// $Id$
//
#include "Factory.h"
#include "Simple.h"

ACE_RCSID(Activation, Factory, "$Id$")

Factory::Factory (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

Test::Simple_ptr
Factory::create_simple_object (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  Simple *simple_impl;
  ACE_NEW_THROW_EX (simple_impl,
                    Simple,
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (Test::Simple::_nil ());

  PortableServer::ServantBase_var owner_transfer(simple_impl);

  return simple_impl->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
Factory::shutdown (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0 ACE_ENV_ARG_PARAMETER);
}
