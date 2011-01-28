// $Id$

#include "Servant_Locator.h"
#include "ace/Log_Msg.h"
#include "testC.h"
#include "ace/OS_NS_string.h"

PortableServer::Servant
Servant_Locator::preinvoke (const PortableServer::ObjectId & /* oid */,
                            PortableServer::POA_ptr /* poa_ptr */,
                            const char * /* op */,
                            PortableServer::ServantLocator::Cookie & /* cookie */)
{
  ACE_DEBUG ((LM_DEBUG,
              "About to throw forward request exception for target...\n"));
  // Throw a nil forward.
  throw PortableServer::ForwardRequest (CORBA::Object::_nil ());
}

void
Servant_Locator::postinvoke (const PortableServer::ObjectId &,
                             PortableServer::POA_ptr ,
                             const char *,
                             PortableServer::ServantLocator::Cookie ,
                             PortableServer::Servant)
{
}
