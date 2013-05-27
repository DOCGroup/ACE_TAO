// $Id$

#include "Servant_Activator.h"
#include "test_i.h"

ServantActivator::ServantActivator (CORBA::ORB_ptr orb,
                                    CORBA::Object_ptr forward_to)
    : orb_ (CORBA::ORB::_duplicate (orb)),
      forward_to_ (CORBA::Object::_duplicate (forward_to))
{
}

PortableServer::Servant
ServantActivator::incarnate (const PortableServer::ObjectId &,
                             PortableServer::POA_ptr)
{
  this->orb_->shutdown (0);

  // Throw forward exception
  throw PortableServer::ForwardRequest (this->forward_to_.in ());
}

void
ServantActivator::etherealize (const PortableServer::ObjectId &,
                               PortableServer::POA_ptr,
                               PortableServer::Servant servant,
                               CORBA::Boolean,
                               CORBA::Boolean)
{
  delete servant;
}

void
ServantActivator::forward_requests (void)
{
  if (CORBA::is_nil (this->forward_to_.in ()))
    throw test::Cannot_Forward ();
}
