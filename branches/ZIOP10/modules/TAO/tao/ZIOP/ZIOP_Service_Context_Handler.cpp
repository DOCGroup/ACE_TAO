// $Id$

#include "tao/ZIOP/ZIOP_Service_Context_Handler.h"

ACE_RCSID (ZIOP,
           ZIOP_Service_Context_Handler,
           "$Id$")

#include "tao/Compression/Compression.h"
#include "tao/CDR.h"
#include "tao/TAO_Server_Request.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

int
TAO_ZIOP_Service_Context_Handler::process_service_context (
  TAO_ServerRequest& server_request,
  const IOP::ServiceContext& context)
{
  TAO_InputCDR cdr (reinterpret_cast<const char*> (
                      context.context_data.get_buffer ()),
                      context.context_data.length ());

  CORBA::Boolean byte_order;
  if (!(cdr >> ACE_InputCDR::to_boolean (byte_order)))
    return -1;

  cdr.reset_byte_order (static_cast<int> (byte_order));

  CORBA::ULong message_length = 0;
  if (!(cdr >> message_length))
    return -1;

    // @TODO use real typedef
  Compression::CompressorId compressorid = 0;
  if (!(cdr >> compressorid))
    return -1;

//   request.original_message_length_ = message_length;
ACE_DEBUG ((LM_DEBUG, "Received compressor %d\n", compressorid));
//+  request.compressed_ = true;
//+  request.original_message_length_ = message_length;
//+

  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL
