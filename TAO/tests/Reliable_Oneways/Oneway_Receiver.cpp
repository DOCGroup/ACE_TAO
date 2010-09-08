//
// $Id$
//
#include "Oneway_Receiver.h"
#include "Shutdown_Helper.h"

Oneway_Receiver::Oneway_Receiver (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
Oneway_Receiver::raise_no_permission (void)
{
  throw CORBA::NO_PERMISSION ();
}

void
Oneway_Receiver::destroy (void)
{
  PortableServer::POA_var poa = this->_default_POA ();

  PortableServer::ObjectId_var id =
    poa->servant_to_id (this);

  poa->deactivate_object (id.in ());
}

Test::Shutdown_Helper_ptr
Oneway_Receiver::get_shutdown_helper (void)
{
  Shutdown_Helper *shutdown_helper;
  ACE_NEW_THROW_EX (shutdown_helper,
                    Shutdown_Helper (this->orb_.in ()),
                    CORBA::NO_MEMORY ());

  PortableServer::ServantBase_var transfer_ownership(shutdown_helper);

  return shutdown_helper->_this ();
}
