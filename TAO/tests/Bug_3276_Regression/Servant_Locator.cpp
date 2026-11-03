#include "Servant_Locator.h"
#include "ace/Log_Msg.h"

PortableServer::Servant
Servant_Locator::preinvoke (PortableServer::ObjectId const & /* oid */,
                            PortableServer::POA_ptr /* poa_ptr */,
                            char const * /* op */,
                            PortableServer::ServantLocator::Cookie & /* cookie */)
{
  ACE_DEBUG ((LM_DEBUG,
              "About to throw forward request exception for target...\n"));
  // Throw a nil forward.
  throw PortableServer::ForwardRequest (CORBA::Object::_nil ());
}

void
Servant_Locator::postinvoke (PortableServer::ObjectId const &,
                             PortableServer::POA_ptr,
                             char const *,
                             PortableServer::ServantLocator::Cookie,
                             PortableServer::Servant)
{
}
