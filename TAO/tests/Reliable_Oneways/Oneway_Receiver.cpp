#include "Oneway_Receiver.h"
#include "Shutdown_Helper.h"
#include <memory>

Oneway_Receiver::Oneway_Receiver (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
Oneway_Receiver::raise_no_permission ()
{
  throw CORBA::NO_PERMISSION ();
}

void
Oneway_Receiver::destroy ()
{
  PortableServer::POA_var const poa = this->_default_POA ();

  PortableServer::ObjectId_var const id =
    poa->servant_to_id (this);

  poa->deactivate_object (id.in ());
}

Test::Shutdown_Helper_ptr
Oneway_Receiver::get_shutdown_helper ()
{
  Shutdown_Helper *new_shutdown_helper {};
  ACE_NEW_THROW_EX (new_shutdown_helper,
                    Shutdown_Helper (this->orb_.in ()),
                    CORBA::NO_MEMORY ());
  std::unique_ptr<Shutdown_Helper> servant (new_shutdown_helper);
  Shutdown_Helper *const shutdown_helper = servant.get ();
  PortableServer::ServantBase_var const transfer_ownership (servant.release ());
  return shutdown_helper->_this ();
}
