// $Id$

#include "MyFooServant.h"
#include "Servant_Activator.h"

ACE_RCSID(Forwarding, MyFooServant, "$Id$")

// Constructor
MyFooServant::MyFooServant (CORBA::ORB_ptr orb,
                            PortableServer::POA_ptr poa,
                            MyFooServantActivator &activator,
                            CORBA::Long value)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    poa_ (PortableServer::POA::_duplicate (poa)),
    activator_ (activator),
    value_ (value)
{
}

CORBA::Long
MyFooServant::doit (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->value_++;
}

void
MyFooServant::forward (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Foo::Cannot_Forward))
{
  this->activator_.forward_requests (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  PortableServer::ObjectId_var id =
    this->poa_->servant_to_id (this
                               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->poa_->deactivate_object (id.in ()
                                 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}


void
MyFooServant::shutdown (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0
                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

PortableServer::POA_ptr
MyFooServant::_default_POA (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}
