#include "MessengerLocator_i.h"
#include "Messenger_i.h"

#include <iostream>
#include <string>

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
  std::string const object_id (oid_str.in ());
  if (object_id == "Messenger") {
    // Create the required servant
    PortableServer::ServantBase_var servant = new Messenger_i ();

    // Keep the servant pointer as a marker so postinvoke() knows to delete
    // this per-request servant.
    cookie = servant.in ();

    return servant._retn();
  }
  else
    {
      throw CORBA::OBJECT_NOT_EXIST ();
    }
}

void
Messenger_Locator_i::postinvoke (PortableServer::ObjectId const &,
                                 PortableServer::POA_ptr,
                                 char const *,
                                 void *cookie,
                                 PortableServer::Servant servant)
{
  std::cout << "postinvoke called..." << std::endl;

  // Delete the servant as it is no longer needed.
  if (cookie != nullptr)
    {
      delete servant;
    }
}
