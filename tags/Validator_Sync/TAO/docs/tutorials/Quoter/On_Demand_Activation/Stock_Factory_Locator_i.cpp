// $Id$

#include "Stock_Factory_Locator_i.h"
#include "Stock_Factory_i.h"

Quoter_Stock_Factory_Locator_i::
Quoter_Stock_Factory_Locator_i (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

PortableServer::Servant
Quoter_Stock_Factory_Locator_i::preinvoke (const PortableServer::ObjectId &oid,
                                           PortableServer::POA_ptr,
                                           const char *,
                                           void * & cookie
                                           ACE_ENV_ARG_DECL_NOT_USED)
  throw (CORBA::SystemException, PortableServer::ForwardRequest)
{

  // Check to see if the object ID is valid
  try {

    // Get the ObjectID in string format
    CORBA::String_var oid_str =
      PortableServer::ObjectId_to_string (oid);

    // Check if the ObjectId is valid
    if (strcmp (oid_str.in (), "Quoter/Stock_Factory") != 0) {
      // Create the required servant
      PortableServer::Servant servant =
        new Quoter_Stock_Factory_i ();
      cookie = servant;

      return servant;
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
                                            PortableServer::Servant servant
                                            ACE_ENV_ARG_DECL_NOT_USED)
  throw (CORBA::SystemException)
{

  // Delete the servant as it is no longer needed.
  PortableServer::Servant my_servant = (PortableServer::Servant) cookie;
  if (servant == my_servant)
    delete servant;

}
