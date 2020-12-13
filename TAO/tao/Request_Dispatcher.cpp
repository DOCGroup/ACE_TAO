#include "Request_Dispatcher.h"
#include "TAO_Server_Request.h"
#include "ORB_Core.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Request_Dispatcher::~TAO_Request_Dispatcher ()
{
}

void
TAO_Request_Dispatcher::dispatch (TAO_ORB_Core *orb_core,
                                  TAO_ServerRequest &request,
                                  CORBA::Object_out forward_to)
{
  // Dispatch based on object key
  orb_core->adapter_registry ().dispatch (request.object_key (),
                                          request,
                                          forward_to);
}

TAO_END_VERSIONED_NAMESPACE_DECL
