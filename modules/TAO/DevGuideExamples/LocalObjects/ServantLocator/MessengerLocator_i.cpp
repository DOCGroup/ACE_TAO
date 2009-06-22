// $Id$

#include "MessengerLocator_i.h"
#include "Messenger_i.h"

#include "ace/SString.h"
#include <iostream>

Messenger_Locator_i::Messenger_Locator_i()
{
}

PortableServer::Servant
Messenger_Locator_i::preinvoke (const PortableServer::ObjectId &oid,
                                           PortableServer::POA_ptr,
                                           const char *,
                                           void * & cookie )
{
  // Get the ObjectId in string format.
  CORBA::String_var oid_str = PortableServer::ObjectId_to_string (oid);

  std::cout << "preinvoke called..." << oid_str << std::endl;

  // Check if the ObjectId is valid.
  ACE_CString cstr(oid_str.in());
  if (cstr == "Messenger") {
    // Create the required servant
    PortableServer::ServantBase_var servant = new Messenger_i ();

    // Set a flag so that we know to delete it in postinvoke().
    cookie = (void *)1;

    return servant._retn();
  }
  else {
    throw CORBA::OBJECT_NOT_EXIST ();
  }
}

void
Messenger_Locator_i::postinvoke (const PortableServer::ObjectId &,
                                 PortableServer::POA_ptr,
                                 const char *,
                                 void * cookie,
                                 PortableServer::Servant servant)
{

  std::cout << "postinvoke called..." << std::endl;

  // Delete the servant as it is no longer needed.
  if (cookie != 0) {
    delete servant;
  }
}
