// $Id$

#include "tao/Forwarding_Servant.h"
#include "tao/Object.h"
#include "tao/POAC.h"
#include "tao/Any.h"
#include "tao/Server_Request.h"

ACE_RCSID(tao, Forwarding_Servant, "$Id$")

TAO_Forwarding_Servant::TAO_Forwarding_Servant (CORBA::Object_ptr forward_to,
                                                const char *interface_repository_id)
  : forward_to_ (CORBA::Object::_duplicate (forward_to)),
    interface_repository_id_ (CORBA::string_dup (interface_repository_id))
{
}

void
TAO_Forwarding_Servant::invoke (CORBA::ServerRequest_ptr request,
                                CORBA::Environment &env)
{
  ACE_UNUSED_ARG (request);

  // Throw forward exception
  CORBA::Exception *exception
    = new PortableServer::ForwardRequest (this->forward_to_.in ());

  CORBA::Any any (exception->_type (), exception);

  request->set_exception (any, env);

  return;
}

CORBA::RepositoryId
TAO_Forwarding_Servant::_primary_interface (const PortableServer::ObjectId &oid,
                                            PortableServer::POA_ptr poa,
                                            CORBA::Environment &env)
{
  ACE_UNUSED_ARG (oid);
  ACE_UNUSED_ARG (poa);
  ACE_UNUSED_ARG (env);

  return CORBA::string_dup (this->interface_repository_id_.in ());
}
