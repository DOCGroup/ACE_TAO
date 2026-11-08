#include "MessengerLocator_i.h"
#include "Messenger_i.h"

#include <cstring>
#include <iostream>

PortableServer::Servant
Messenger_Locator_i::preinvoke (PortableServer::ObjectId const &oid,
                                PortableServer::POA_ptr,
                                char const *,
                                void *&cookie)
{
  // Get the ObjectId in string format.
  CORBA::String_var const oid_str = PortableServer::ObjectId_to_string (oid);

  std::cout << "preinvoke called..." << oid_str << std::endl;

  // Check if the ObjectId is valid.
  if (std::strcmp (oid_str.in (), "Messenger") == 0) {
    // Create the required servant
    PortableServer::ServantBase_var servant = new Messenger_i ();

    // Keep the servant pointer as a marker so postinvoke() knows to delete
    // this per-request servant.
    cookie = servant.in ();

    return servant._retn();
  }
  throw CORBA::OBJECT_NOT_EXIST ();
}

void
Messenger_Locator_i::postinvoke (PortableServer::ObjectId const &,
                                 PortableServer::POA_ptr,
                                 char const *,
                                 void *cookie,
                                 PortableServer::Servant servant)
{
  std::cout << "postinvoke called..." << std::endl;

  // Release the servant as it is no longer needed.
  if (cookie != nullptr)
    {
      PortableServer::ServantBase_var const servant_guard (servant);
    }
}
