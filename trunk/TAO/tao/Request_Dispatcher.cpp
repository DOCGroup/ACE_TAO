// $Id$

#include "Request_Dispatcher.h"
#include "TAO_Server_Request.h"
#include "ORB_Core.h"

ACE_RCSID(tao, GIOP_Message_Base, "$Id$")

TAO_Request_Dispatcher::~TAO_Request_Dispatcher (void)
{
}

void
TAO_Request_Dispatcher::dispatch (TAO_ORB_Core *orb_core,
                                  TAO_ServerRequest &request,
                                  CORBA::Object_out forward_to
                                  TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Dispatch based on object key
  orb_core->adapter_registry ()->dispatch (request.object_key (),
                                           request,
                                           forward_to
                                           TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
}
