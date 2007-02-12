//
// $Id$
//
#include "Receiver_Factory.h"
#include "Receiver.h"

ACE_RCSID(Throughput, Receiver_Factory, "$Id$")

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

  return receiver_impl->_this ();
}

void
Receiver_Factory::shutdown (void)
{
  this->orb_->shutdown (0);
}
