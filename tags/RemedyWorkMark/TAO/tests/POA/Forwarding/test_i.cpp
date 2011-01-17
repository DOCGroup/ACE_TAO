// $Id$

#include "test_i.h"
#include "Servant_Activator.h"

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
{
  return this->value_++;
}

void
test_i::forward (void)
{
  this->activator_.forward_requests ();

  PortableServer::ObjectId_var id =
    this->poa_->servant_to_id (this);

  this->poa_->deactivate_object (id.in ());
}


void
test_i::shutdown (void)
{
  this->orb_->shutdown (0);
}

PortableServer::POA_ptr
test_i::_default_POA (void)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}
