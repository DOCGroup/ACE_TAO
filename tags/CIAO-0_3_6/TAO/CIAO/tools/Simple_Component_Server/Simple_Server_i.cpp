// $Id$

#include "Simple_Server_i.h"

/// Constructor
CIAO::Simple_Server_i::Simple_Server_i (CORBA::ORB_ptr o,
                                        PortableServer::POA_ptr p,
                                        Components::CCMHome_ptr h)
  : orb_ (CORBA::ORB::_duplicate (o)),
    poa_ (PortableServer::POA::_duplicate (p)),
    home_ (Components::CCMHome::_duplicate (h))
{
}

/// Destructor
CIAO::Simple_Server_i::~Simple_Server_i (void)
{
}

PortableServer::POA_ptr
CIAO::Simple_Server_i::_default_POA (void)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

Components::CCMHome_ptr
CIAO::Simple_Server_i::get_home (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return Components::CCMHome::_duplicate (this->home_.in ());
}

Components::CCMObject_ptr
CIAO::Simple_Server_i::get_component (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (CORBA::is_nil (this->component_.in ()))
    {
      Components::KeylessCCMHome_var h
        = Components::KeylessCCMHome::_narrow (this->home_.in ()
                                               ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      this->component_ =
        h->create_component (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);
    }
  return Components::CCMObject::_duplicate (this->component_.in ());
}

void
CIAO::Simple_Server_i::shutdown (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown ();
}
