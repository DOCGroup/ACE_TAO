// $Id$

#include "tao/BiDir_GIOP/BiDir_Service_Context_Handler.h"

ACE_RCSID (BiDir_GIOP,
           BiDir_Service_Context_Handler,
           "$Id$")

#include "tao/CDR.h"
#include "tao/TAO_Server_Request.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

int
TAO_BiDIR_Service_Context_Handler::process_service_context (
  TAO_ServerRequest& server_request,
  const IOP::ServiceContext& context)
{
  TAO_InputCDR cdr (reinterpret_cast<const char*> (
                      context.context_data.get_buffer ()),
                    context.context_data.length ());
  return server_request.transport()->tear_listen_point_list (cdr);
}

TAO_END_VERSIONED_NAMESPACE_DECL
