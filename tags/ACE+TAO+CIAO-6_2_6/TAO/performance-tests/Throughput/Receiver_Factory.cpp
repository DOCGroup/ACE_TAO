//
// $Id$
//
#include "Receiver_Factory.h"
#include "Receiver.h"

Receiver_Factory::Receiver_Factory (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

Test::Receiver_ptr
Receiver_Factory::create_receiver (void)
{
  Receiver *receiver_impl = 0;
  ACE_NEW_THROW_EX (receiver_impl,
                    Receiver,
                    CORBA::NO_MEMORY ());
  PortableServer::ServantBase_var transfer_ownership(receiver_impl);

  CORBA::Object_var poa_object =
    this->orb_->resolve_initial_references("RootPOA");

  PortableServer::POA_var root_poa =
    PortableServer::POA::_narrow (poa_object.in ());

  PortableServer::ObjectId_var id =
    root_poa->activate_object (receiver_impl);

  CORBA::Object_var object = root_poa->id_to_reference (id.in ());

  return Test::Receiver::_narrow (object.in ());
}

void
Receiver_Factory::shutdown (void)
{
  this->orb_->shutdown (0);
}
