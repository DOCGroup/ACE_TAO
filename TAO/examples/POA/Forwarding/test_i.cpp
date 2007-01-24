// $Id$

#include "test_i.h"
#include "Servant_Activator.h"

ACE_RCSID(Forwarding, test_i, "$Id$")

// Constructor
test_i::test_i (CORBA::ORB_ptr orb,
                PortableServer::POA_ptr poa,
                ServantActivator &activator,
                CORBA::Long value)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    poa_ (PortableServer::POA::_duplicate (poa)),
    activator_ (activator),
    value_ (value)
{
}

CORBA::Long
test_i::doit (void)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->value_++;
}

void
test_i::forward (void)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   test::Cannot_Forward))
{
  this->activator_.forward_requests ();

  PortableServer::ObjectId_var id =
    this->poa_->servant_to_id (this
                               ACE_ENV_ARG_PARAMETER);

  this->poa_->deactivate_object (id.in ()
                                 ACE_ENV_ARG_PARAMETER);
}


void
test_i::shutdown (void)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0
                        ACE_ENV_ARG_PARAMETER);
}

PortableServer::POA_ptr
test_i::_default_POA (void)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}
