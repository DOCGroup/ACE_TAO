// $Id$

#include "tao/BiDir_GIOP/BiDir_Service_Context_Handler.h"
#include "tao/CDR.h"
#include "tao/TAO_Server_Request.h"
#include "tao/Transport.h"
#include "tao/ORB_Core.h"
#include "tao/GIOP_Message_Base.h"
#include "tao/operation_details.h"
#include "tao/Transport_Mux_Strategy.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

int
TAO_BiDIR_Service_Context_Handler::process_service_context (
  TAO_Transport& transport,
  const IOP::ServiceContext& context,
  TAO_ServerRequest * /*request*/)
{
  TAO_InputCDR cdr (reinterpret_cast<const char*> (
                      context.context_data.get_buffer ()),
                    context.context_data.length ());
  return transport.tear_listen_point_list (cdr);
}

int
TAO_BiDIR_Service_Context_Handler::generate_service_context (
  TAO_Stub *,
  TAO_Transport& transport,
  TAO_Operation_Details &opdetails,
  TAO_Target_Specification &,
  TAO_OutputCDR &msg)
{
  if (transport.orb_core ()->bidir_giop_policy () &&
      transport.messaging_object ()->is_ready_for_bidirectional (msg) &&
      transport.bidirectional_flag () < 0)
    {
      transport.set_bidir_context_info (opdetails);

      // Set the flag to 1 (i.e., originating side)
      transport.bidirectional_flag (1);

      // At the moment we enable BiDIR giop we have to get a new
      // request id to make sure that we follow the even/odd rule
      // for request id's. We only need to do this when enabled
      // it, after that the Transport Mux Strategy will make sure
      // that the rule is followed
      opdetails.request_id (transport.tms ()->request_id ());
    }

  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL
