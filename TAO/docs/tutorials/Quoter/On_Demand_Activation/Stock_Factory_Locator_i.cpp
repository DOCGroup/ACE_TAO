#include "Stock_Factory_Locator_i.h"
#include "Stock_Factory_i.h"

#include <cstring>

Quoter_Stock_Factory_Locator_i::
Quoter_Stock_Factory_Locator_i (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

PortableServer::Servant
Quoter_Stock_Factory_Locator_i::preinvoke (const PortableServer::ObjectId &oid,
                                           PortableServer::POA_ptr,
                                           const char *,
                                           void * & cookie)
{
  // Check to see if the object ID is valid
  try {
    // Get the ObjectID in string format
    CORBA::String_var oid_str =
      PortableServer::ObjectId_to_string (oid);

    // Check if the ObjectId is valid
    if (std::strcmp (oid_str.in (), "Quoter/Stock_Factory") != 0) {
      // Create the required servant
      PortableServer::ServantBase_var servant_owner =
        new Quoter_Stock_Factory_i ();
      PortableServer::Servant servant = servant_owner.in ();
      cookie = servant;

      return servant_owner._retn ();
    }
    else {
      throw CORBA::OBJECT_NOT_EXIST ();
    }

  } catch (const CORBA::BAD_PARAM &) {
    throw CORBA::OBJECT_NOT_EXIST ();
  }
}

void
Quoter_Stock_Factory_Locator_i::postinvoke (const PortableServer::ObjectId &,
                                            PortableServer::POA_ptr,
                                            const char *,
                                            void * cookie,
                                            PortableServer::Servant servant)
{
  // Release the servant as it is no longer needed.
  if (servant == cookie)
    {
      PortableServer::ServantBase_var servant_owner (servant);
    }
}
