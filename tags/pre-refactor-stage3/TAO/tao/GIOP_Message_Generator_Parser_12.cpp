#include "GIOP_Message_Generator_Parser_12.h"
#include "GIOP_Utils.h"
#include "GIOP_Message_State.h"
#include "GIOP_Message_Locate_Header.h"
#include "operation_details.h"
#include "CDR.h"
#include "Any.h"
#include "debug.h"
#include "Pluggable_Messaging_Utils.h"
#include "TAO_Server_Request.h"
#include "TAOC.h"
#include "Service_Context.h"
#include "ORB_Core.h"
#include "Transport.h"

#if !defined (__ACE_INLINE__)
# include "GIOP_Message_Generator_Parser_12.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID (tao,
           GIOP_Message_Gen_Parser_12,
           "$Id$")


// This is used by GIOP1.2. This is to align the message body on a
// 8-octet boundary. This is declared static so that it is in file
// scope.
static const size_t TAO_GIOP_MESSAGE_ALIGN_PTR = 8;

int
TAO_GIOP_Message_Generator_Parser_12::write_request_header (
    const TAO_Operation_Details &opdetails,
    TAO_Target_Specification &spec,
    TAO_OutputCDR &msg
  )
{
  // First the request id
  msg << opdetails.request_id ();

  const CORBA::Octet response_flags = opdetails.response_flags ();

  // Here are the Octet values for different policies
  // '00000000' for SYNC_NONE
  // '00000000' for SYNC_WITH_TRANSPORT
  // '00000010' for SYNC_WITH_SERVER
  // '00000011' for SYNC_WITH_TARGET
  // '00000011' for regular two ways, but if they are invoked via a
  // DII with INV_NO_RESPONSE flag set then we need to send '00000001'
  //
  // We have not implemented the policy INV_NO_RESPONSE for DII.
  if (response_flags == TAO_TWOWAY_RESPONSE_FLAG)
    msg << CORBA::Any::from_octet (3);
  // Second the response flags
  // Sync scope - ignored by server if request is not oneway.
  else if (response_flags == CORBA::Octet (Messaging::SYNC_NONE)
           || response_flags == CORBA::Octet (Messaging::SYNC_WITH_TRANSPORT)
           || response_flags == CORBA::Octet (TAO::SYNC_EAGER_BUFFERING)
           || response_flags == CORBA::Octet (TAO::SYNC_DELAYED_BUFFERING))
    // No response required.
    msg << CORBA::Any::from_octet (0);

  else if (response_flags == CORBA::Octet (Messaging::SYNC_WITH_SERVER))
    // Return before dispatching to the servant
    msg << CORBA::Any::from_octet (1);

  else if (response_flags == CORBA::Octet (Messaging::SYNC_WITH_TARGET))
    // Return after dispatching servant.
    msg << CORBA::Any::from_octet (3);
  else
    // Until more flags are defined by the OMG.
    return 0;

  // The reserved field
  CORBA::Octet reserved[3] = {0, 0, 0};

  msg.write_octet_array (reserved, 3);

  if (this->marshall_target_spec (spec,
                                  msg) == 0)
    return 0;

  // Write the operation name
  msg.write_string (opdetails.opname_len (),
                    opdetails.opname ());

  // Write the service context list
  msg << opdetails.request_service_info ();

  // We align the pointer only if the operation has arguments.
  if (opdetails.argument_flag ()
      && msg.align_write_ptr (TAO_GIOP_MESSAGE_ALIGN_PTR) == -1)
    {
      return 0;
    }

  return 1;
}


int
TAO_GIOP_Message_Generator_Parser_12::write_locate_request_header (
    CORBA::ULong request_id,
    TAO_Target_Specification    &spec,
    TAO_OutputCDR &msg
  )
{
  // Write the request id
  msg << request_id;

  // Write the target address
  if (this->marshall_target_spec (spec,
                                  msg) == 0)
    return 0;

  // I dont think we need to align the pointer to an 8 byte boundary
  // here.
  // We need to align the pointer
  //  if (msg.align_write_ptr (TAO_GIOP_MESSAGE_ALIGN_PTR) == -1)
  //  return 0;

  // Return success
  return 1;
}

int
TAO_GIOP_Message_Generator_Parser_12::write_reply_header (
    TAO_OutputCDR & output,
    TAO_Pluggable_Reply_Params_Base &reply
    ACE_ENV_ARG_DECL_NOT_USED /* ACE_ENV_SINGLE_ARG_PARAMETER */
  )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Write the request ID
  output.write_ulong (reply.request_id_);

   // Write the reply status
  if (reply.reply_status_ ==
      TAO_PLUGGABLE_MESSAGE_LOCATION_FORWARD_PERM)
    {
      // Not sure when we will use this.
      output.write_ulong (TAO_GIOP_LOCATION_FORWARD_PERM);
    }
  else if (reply.reply_status_ ==
           TAO_PLUGGABLE_MESSAGE_NEEDS_ADDRESSING_MODE)
    {
      // Not sure when we will use this.
      output.write_ulong (TAO_GIOP_LOC_NEEDS_ADDRESSING_MODE);
    }
  else
    {
      this->marshal_reply_status (output,
                                  reply);
    }

#if (TAO_HAS_MINIMUM_CORBA == 1)
  output << reply.service_context_notowned ();
#else
  if (reply.is_dsi_ == 0)
    {
      output << reply.service_context_notowned ();
    }
  else
    {
      IOP::ServiceContextList &svc_ctx =
        reply.service_context_notowned ();
      CORBA::ULong l = svc_ctx.length ();

      // Now marshal the rest of the service context objects
      output << l;

      for (CORBA::ULong i = 0; i != l; ++i)
        {
          output << svc_ctx[i];
        }

    }
#endif /*TAO_HAS_MINIMUM_CORBA */

  if (reply.argument_flag_)
    {
      // If we have some data to be marshalled, then we align the
      // pointer to a 8 byte boundary.  Else, we just leave it throu
      if (output.align_write_ptr (TAO_GIOP_MESSAGE_ALIGN_PTR) == -1)
        {
          return 0;
        }
    }
  return 1;
}


int
TAO_GIOP_Message_Generator_Parser_12::write_locate_reply_mesg (
    TAO_OutputCDR & output,
    CORBA::ULong request_id,
    TAO_GIOP_Locate_Status_Msg &status_info
  )
{
  output.write_ulong (request_id);

  // Make the header for the locate request
  output.write_ulong (status_info.status);

  // Note: We dont align the pointer to an 8 byte boundary for a
  // locate reply body. This is due to an urgent issue raised by Michi
  // in the OMG. I discussed this with Michi today (09/07/2001) and I
  // learn that this has been passed. Hence the change..
  /*
  if (status_info.status == TAO_GIOP_OBJECT_FORWARD ||
      status_info.status == TAO_GIOP_OBJECT_FORWARD_PERM)
    {
      // We have to send some data, so align the pointer
      if (output.align_write_ptr (TAO_GIOP_MESSAGE_ALIGN_PTR) == -1)
        {
          return 0;
        }
    }
  */
  switch (status_info.status)
    {
    case TAO_GIOP_OBJECT_FORWARD:

      // More likely than not we will not have this in TAO
    case TAO_GIOP_OBJECT_FORWARD_PERM:
      {
        CORBA::Object_ptr object_ptr =
          status_info.forward_location_var.in ();

        if ((output << object_ptr) == 0)
        {
          if (TAO_debug_level > 0)
            {
              ACE_DEBUG ((
                  LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t|%N|%l) write_locate_reply_mesg-")
                  ACE_TEXT (" cannot marshal object reference\n")
                ));
            }
        }
      }
      break;
    case TAO_GIOP_LOC_SYSTEM_EXCEPTION:
    case TAO_GIOP_LOC_NEEDS_ADDRESSING_MODE:
      // Do we do these in TAO??
      // What to do here???? I dont really know. I have to do a survey
      // of the specifications that uses this.
      break;
    default:
      break;
    }

  return 1;
}


int
TAO_GIOP_Message_Generator_Parser_12::parse_request_header (
    TAO_ServerRequest &request
  )
{
  // Get the input CDR in the request class
  TAO_InputCDR& input = request.incoming ();

  CORBA::Boolean hdr_status = (CORBA::Boolean) input.good_bit ();

  CORBA::ULong req_id;
  // Get the rest of the request header ...
  hdr_status = hdr_status && input.read_ulong (req_id);

  request.request_id (req_id);

  CORBA::Octet response_flags;
  hdr_status = hdr_status && input.read_octet (response_flags);

  request.response_expected ((response_flags > 0));

  // The high bit of the octet has been set if the SyncScope policy
  // value is SYNC_WITH_SERVER.
  request.sync_with_server ((response_flags == 1));

  // Reserved field
  input.skip_bytes (3);

  // Unmarshal the target address field.
  hdr_status =
    hdr_status && request.profile ().unmarshall_target_address(input);

  CORBA::ULong length = 0;
  hdr_status = hdr_status && input.read_ulong (length);

  if (hdr_status)
    {
      // Do not include NULL character at the end.
      // @@ This is not getting demarshaled using the codeset
      //    translators!

      // Notice that there are no memory allocations involved
      // here!

      request.operation (input.rd_ptr (),
                         length - 1,
                         0 /* TAO_ServerRequest does NOT own string */);
      hdr_status = input.skip_bytes (length);
    }

  // Tear out the service context ... we currently ignore it, but it
  // should probably be passed to each ORB service as appropriate
  // (e.g. transactions, security).
  //
  // NOTE: As security support kicks in, this is a good place to
  // verify a digital signature, if that is required in this security
  // environment.  It may be required even when using IPSEC security
  // infrastructure.
  IOP::ServiceContextList &req_service_info =
    request.request_service_info ();

  input >> req_service_info;

  // Check an process if BiDir contexts are available
  if (request.orb_core ()->bidir_giop_policy ())
    this->check_bidirectional_context (request);

  if (input.length () > 0)
    {
      // Reset the read_ptr to an 8-byte boundary.
      input.align_read_ptr (TAO_GIOP_MESSAGE_ALIGN_PTR);
    }

  return hdr_status ? 0 : -1;
}


int
TAO_GIOP_Message_Generator_Parser_12::parse_locate_header (
    TAO_GIOP_Locate_Request_Header &request
  )
{
  // Get the stream .
  TAO_InputCDR &msg = request.incoming_stream ();

  CORBA::Boolean hdr_status = 1;

  // Get the request id.
  CORBA::ULong req_id = 0;
  hdr_status = msg.read_ulong (req_id);

  // Store it in the Locate request classes
  request.request_id (req_id);

  // Unmarshal the target address field.
  hdr_status =
    hdr_status && request.profile ().unmarshall_target_address(msg);

  // Reset the pointer to an 8-byte bouns]dary
  msg.align_read_ptr (TAO_GIOP_MESSAGE_ALIGN_PTR);

  return hdr_status ? 0 : -1;
}

int
TAO_GIOP_Message_Generator_Parser_12::parse_reply (
    TAO_InputCDR &cdr,
    TAO_Pluggable_Reply_Params &params
  )
{
  if (TAO_GIOP_Message_Generator_Parser::parse_reply (cdr,
                                                      params) == -1)

    return -1;

  if ((cdr >> params.svc_ctx_) == 0)
    {
      if (TAO_debug_level > 0)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("TAO (%P|%t) parse_reply, ")
                    ACE_TEXT ("extracting context\n")));

      return -1;
    }

  if (cdr.length () > 0)
    {
      // Align the read pointer on an 8-byte boundary
      cdr.align_read_ptr (TAO_GIOP_MESSAGE_ALIGN_PTR);
    }




  // Steal the contents in to the reply CDR and loose ownership of the
  // data block.
  params.input_cdr_.exchange_data_blocks (cdr);

  return 0;
}

int
TAO_GIOP_Message_Generator_Parser_12::parse_locate_reply (
    TAO_InputCDR &cdr,
    TAO_Pluggable_Reply_Params &params
  )
{
  if (TAO_GIOP_Message_Generator_Parser::parse_locate_reply (cdr,
                                                             params) == -1)

    return -1;

  // Note: We dont align the pointer to an 8 byte boundary for a
  // locate reply body. This is due to an urgent issue raised by Michi
  // in the OMG. I discussed this with Michi today (09/07/2001) and I
  // learn that this has been passed. Hence the change..
  /*if (cdr.length () > 0)
    {
      // Align the read pointer on an 8-byte boundary
      cdr.align_read_ptr (TAO_GIOP_MESSAGE_ALIGN_PTR);
      }*/

  // Steal the contents in to the reply CDR and loose ownership of the
  // data block.
  params.input_cdr_.exchange_data_blocks (cdr);

  return 0;
}


CORBA::Octet
TAO_GIOP_Message_Generator_Parser_12::major_version (void)
{
  return (CORBA::Octet) 1;
}


CORBA::Octet
TAO_GIOP_Message_Generator_Parser_12::minor_version (void)
{
  return (CORBA::Octet) 2;
}

int
TAO_GIOP_Message_Generator_Parser_12::is_ready_for_bidirectional (void)
{
  // We do support bidirectional
  return 1;
}



int
TAO_GIOP_Message_Generator_Parser_12::marshall_target_spec (
    TAO_Target_Specification &spec,
    TAO_OutputCDR &msg
  )
{
  switch (spec.specifier ())
    {
    case TAO_Target_Specification::Key_Addr:
      {
        // As this is a union send in the discriminant first
        msg << GIOP::KeyAddr;

        // Get the object key
        const TAO::ObjectKey *key = spec.object_key ();

        if (key)
          {
            // Marshall in the object key
            msg << *key;
          }
        else
          {
            if (TAO_debug_level)
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("(%N |%l) Unable to handle this request \n")));
            return 0;
          }
        break;
      }
    case TAO_Target_Specification::Profile_Addr:
      {
        // As this is a union send in the discriminant first
        msg << GIOP::ProfileAddr;

        // Get the profile
        const IOP::TaggedProfile *pfile = spec.profile ();

        if (pfile)
          {
            // Marshall in the object key
            msg << *pfile;
          }
        else
          {
            if (TAO_debug_level)
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("(%N |%l) Unable to handle this request \n")));
            return 0;
          }
        break;
      }
    case TAO_Target_Specification::Reference_Addr:
      {
        // As this is a union send in the discriminant first
        msg << GIOP::ReferenceAddr;

        // Get the IOR
        IOP::IOR *ior;
        CORBA::ULong index = spec.iop_ior (ior);

        if (ior)
          {
            // This is a struct IORAddressingInfo. So, marshall each
            // member of the struct one after another in the order
            // defined.
            msg << index;
            msg << *ior;
          }
        else
          {
            if (TAO_debug_level)
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("(%N |%l) Unable to handle this request \n")));
            return 0;
          }
        break;
      }
    default:
      if (TAO_debug_level)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%N |%l) Unable to handle this request \n")));
      return 0;
    }

  return 1;
}


int
TAO_GIOP_Message_Generator_Parser_12::check_bidirectional_context (
    TAO_ServerRequest &request)
{
  TAO_Service_Context &service_context = request.request_service_context ();

  // Check whether we have the BiDir service context info available in
  // the ServiceContextList
  if (service_context.is_service_id (IOP::BI_DIR_IIOP)
      == 1)
    {
      return this->process_bidir_context (service_context,
                                          request.transport ());
    }

  return 0;
}

int
TAO_GIOP_Message_Generator_Parser_12::process_bidir_context (
    TAO_Service_Context &service_context,
    TAO_Transport *transport)
{
  // Get the context info
  IOP::ServiceContext context;
  context.context_id = IOP::BI_DIR_IIOP;

  if (service_context.get_context (context) != 1)
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%P|%t) Context info not found \n")),
                        -1);

  TAO_InputCDR cdr (ACE_reinterpret_cast
                    (const char*,
                     context.context_data.get_buffer ()),
                    context.context_data.length ());

  return transport->tear_listen_point_list (cdr);
}
