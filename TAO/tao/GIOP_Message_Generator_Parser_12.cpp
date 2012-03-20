// -*- C++ -*-
// $Id$

#include "tao/GIOP_Message_Generator_Parser_12.h"
#include "tao/GIOPC.h"
#include "tao/GIOP_Utils.h"
#include "tao/GIOP_Message_Locate_Header.h"
#include "tao/operation_details.h"
#include "tao/debug.h"
#include "tao/Pluggable_Messaging_Utils.h"
#include "tao/GIOP_Message_State.h"
#include "tao/TAO_Server_Request.h"
#include "tao/TAOC.h"
#include "tao/ORB_Core.h"
#include "tao/Transport.h"
#include "tao/CDR.h"

// This is used by GIOP1.2. This is to align the message body on a
// 8-octet boundary. This is declared static so that it is in file
// scope.
static const size_t TAO_GIOP_MESSAGE_ALIGN_PTR = 8;

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

bool
TAO_GIOP_Message_Generator_Parser_12::write_request_header (
    const TAO_Operation_Details &opdetails,
    TAO_Target_Specification &spec,
    TAO_OutputCDR &msg)
{
  // First the request id
  if (!(msg << opdetails.request_id ()))
    return false;

  CORBA::Octet const response_flags = opdetails.response_flags ();

  // Here are the Octet values for different policies. See the meaning
  // of response_flags of RequestHeader_1_2 in the CORBA specification as
  // to why the values below are used.
  // '00000000' for SYNC_NONE
  // '00000000' for SYNC_WITH_TRANSPORT
  // '00000001' for SYNC_WITH_SERVER
  // '00000011' for SYNC_WITH_TARGET
  // '00000011' for regular two ways, but if they are invoked via a
  // DII with INV_NO_RESPONSE flag set then we need to send '00000001'
  //
  // We have not implemented the policy INV_NO_RESPONSE for DII.
  if (response_flags == TAO_TWOWAY_RESPONSE_FLAG)
    msg << ACE_OutputCDR::from_octet (3);
  // Second the response flags
  // Sync scope - ignored by server if request is not oneway.
  else if (response_flags == CORBA::Octet (Messaging::SYNC_NONE)
           || response_flags == CORBA::Octet (Messaging::SYNC_WITH_TRANSPORT)
           || response_flags == CORBA::Octet (TAO::SYNC_DELAYED_BUFFERING))
    // No response required.
    msg << ACE_OutputCDR::from_octet (0);

  else if (response_flags == CORBA::Octet (Messaging::SYNC_WITH_SERVER))
    // Return before dispatching to the servant
    msg << ACE_OutputCDR::from_octet (1);

  else if (response_flags == CORBA::Octet (Messaging::SYNC_WITH_TARGET))
    // Return after dispatching servant.
    msg << ACE_OutputCDR::from_octet (3);
  else
    // Until more flags are defined by the OMG.
    return false;

  // The reserved field
  CORBA::Octet reserved[3] = {0, 0, 0};

  msg.write_octet_array (reserved, 3);

  if (!this->marshall_target_spec (spec, msg))
    return false;

  // Write the operation name
  msg.write_string (opdetails.opname_len (),
                    opdetails.opname ());

  // Write the service context list
  if (!(msg << opdetails.request_service_info ()))
    return false;

  // We align the pointer only if the operation has arguments.
  if (opdetails.argument_flag ()
      && msg.align_write_ptr (TAO_GIOP_MESSAGE_ALIGN_PTR) == -1)
    {
      return false;
    }

  return true;
}

bool
TAO_GIOP_Message_Generator_Parser_12::write_locate_request_header (
    CORBA::ULong request_id,
    TAO_Target_Specification &spec,
    TAO_OutputCDR &msg)
{
  // Write the request id
  if (!(msg << request_id))
    return false;

  // Write the target address
  if (!(this->marshall_target_spec (spec, msg)))
    return false;

  // I dont think we need to align the pointer to an 8 byte boundary
  // here.
  // We need to align the pointer
  //  if (msg.align_write_ptr (TAO_GIOP_MESSAGE_ALIGN_PTR) == -1)
  //  return 0;

  // Return success
  return true;
}

bool
TAO_GIOP_Message_Generator_Parser_12::write_reply_header (
    TAO_OutputCDR & output,
    TAO_Pluggable_Reply_Params_Base &reply)
{
  // Write the request ID
  if (!(output.write_ulong (reply.request_id_)))
    return false;

  // Write the reply status
  if (!(output.write_ulong (reply.reply_status ())))
    return false;

  if (!(output << reply.service_context_notowned ()))
    return false;

  if (reply.argument_flag_)
    {
      // If we have some data to be marshalled, then we align the
      // pointer to a 8 byte boundary.  Else, we just leave it throu
      if (output.align_write_ptr (TAO_GIOP_MESSAGE_ALIGN_PTR) == -1)
        {
          return false;
        }
    }
  return true;
}

bool
TAO_GIOP_Message_Generator_Parser_12::write_locate_reply_mesg (
    TAO_OutputCDR & output,
    CORBA::ULong request_id,
    TAO_GIOP_Locate_Status_Msg &status_info)
{
  if (!(output.write_ulong (request_id)))
    return false;

  // Make the header for the locate request
  if (!(output.write_ulong (status_info.status)))
    return false;

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
          return false;
        }
    }
  */
  switch (status_info.status)
    {
    case GIOP::OBJECT_FORWARD:

      // More likely than not we will not have this in TAO
    case GIOP::OBJECT_FORWARD_PERM:
      {
        CORBA::Object_ptr object_ptr =
          status_info.forward_location_var.in ();

        if (!(output << object_ptr))
        {
          if (TAO_debug_level > 0)
            {
              ACE_ERROR ((
                  LM_ERROR,
                  ACE_TEXT ("TAO (%P|%t|%N|%l) write_locate_reply_mesg-")
                  ACE_TEXT (" cannot marshal object reference\n")
                ));
            }
        }
      }
      break;
    case GIOP::LOC_SYSTEM_EXCEPTION:
    case GIOP::LOC_NEEDS_ADDRESSING_MODE:
      // Do we do these in TAO??
      // What to do here???? I dont really know. I have to do a survey
      // of the specifications that uses this.
      break;
    default:
      break;
    }

  return true;
}

bool
TAO_GIOP_Message_Generator_Parser_12::write_fragment_header (
  TAO_OutputCDR & cdr,
  CORBA::ULong request_id)
{
  return (cdr << request_id);

  // No need to align write pointer to an 8 byte boundary since it
  // should already be aligned (12 for GIOP messager + 4 for fragment
  // header = 16 -- a multiple of 8)
}

int
TAO_GIOP_Message_Generator_Parser_12::parse_request_header (
    TAO_ServerRequest &request)
{
  // Get the input CDR in the request class
  TAO_InputCDR & input = *request.incoming ();

  CORBA::Boolean hdr_status = (CORBA::Boolean) input.good_bit ();

  CORBA::ULong req_id = 0;
  // Get the rest of the request header ...
  hdr_status = hdr_status && input.read_ulong (req_id);

  request.request_id (req_id);

  CORBA::Octet response_flags = CORBA::Octet();
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
  IOP::ServiceContextList &req_service_info = request.request_service_info ();

  if (!(input >> req_service_info))
    {
      if (TAO_debug_level)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("TAO (%P|%t) parse_request_header, ")
                      ACE_TEXT ("extracting context\n")));
        }

      return -1;
    }

  if (req_service_info.length() > 0)
    {
      request.orb_core ()->service_context_registry ().
        process_service_contexts (req_service_info, *(request.transport ()), &request);
    }

  if (input.length () > 0)
    {
      // Reset the read_ptr to an 8-byte boundary.
      input.align_read_ptr (TAO_GIOP_MESSAGE_ALIGN_PTR);
    }

  return hdr_status ? 0 : -1;
}


int
TAO_GIOP_Message_Generator_Parser_12::parse_locate_header (
    TAO_GIOP_Locate_Request_Header &request)
{
  // Get the stream .
  TAO_InputCDR &msg = request.incoming_stream ();

  // Get the request id.
  CORBA::ULong req_id = 0;
  CORBA::Boolean hdr_status = msg.read_ulong (req_id);

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
    TAO_Pluggable_Reply_Params &params)
{
  if (TAO_GIOP_Message_Generator_Parser::parse_reply (cdr, params) == -1)
    return -1;

  if (!(cdr >> params.svc_ctx_))
    {
      if (TAO_debug_level)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("TAO (%P|%t) parse_reply, ")
                      ACE_TEXT ("extracting context\n")));
        }

      return -1;
    }

  if (cdr.length () > 0)
    {
      // Align the read pointer on an 8-byte boundary
      cdr.align_read_ptr (TAO_GIOP_MESSAGE_ALIGN_PTR);
    }

  return 0;
}

int
TAO_GIOP_Message_Generator_Parser_12::parse_locate_reply (
    TAO_InputCDR &cdr,
    TAO_Pluggable_Reply_Params &params)
{
  if (TAO_GIOP_Message_Generator_Parser::parse_locate_reply (cdr, params) == -1)
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

  return 0;
}


CORBA::Octet
TAO_GIOP_Message_Generator_Parser_12::major_version (void) const
{
  return static_cast<CORBA::Octet> (1);
}


CORBA::Octet
TAO_GIOP_Message_Generator_Parser_12::minor_version (void) const
{
  return static_cast<CORBA::Octet> (2);
}

bool
TAO_GIOP_Message_Generator_Parser_12::is_ready_for_bidirectional (void) const
{
  // We do support bidirectional
  return true;
}

bool
TAO_GIOP_Message_Generator_Parser_12::marshall_target_spec (
    TAO_Target_Specification &spec,
    TAO_OutputCDR &msg)
{
  switch (spec.specifier ())
    {
    case TAO_Target_Specification::Key_Addr:
      {
        // As this is a union send in the discriminant first
        if (!(msg << GIOP::KeyAddr))
          return false;

        // Get the object key
        const TAO::ObjectKey *key = spec.object_key ();

        if (key)
          {
            // Marshall in the object key
            if (!(msg << *key))
              return false;
          }
        else
          {
            if (TAO_debug_level)
              {
                ACE_DEBUG ((LM_DEBUG,
                            ACE_TEXT ("(%N |%l) Unable to handle this request\n")));
              }
            return false;
          }
        break;
      }
    case TAO_Target_Specification::Profile_Addr:
      {
        // As this is a union send in the discriminant first
        if (!(msg << GIOP::ProfileAddr))
          return false;

        // Get the profile
        const IOP::TaggedProfile *pfile = spec.profile ();

        if (pfile)
          {
            // Marshall in the object key
            if (!(msg << *pfile))
              return false;
          }
        else
          {
            if (TAO_debug_level)
              {
                ACE_DEBUG ((LM_DEBUG,
                            ACE_TEXT ("(%N |%l) Unable to handle this request\n")));
              }
            return false;
          }
        break;
      }
    case TAO_Target_Specification::Reference_Addr:
      {
        // As this is a union send in the discriminant first
        if (!(msg << GIOP::ReferenceAddr))
          return false;

        // Get the IOR
        IOP::IOR *ior = 0;
        CORBA::ULong const index = spec.iop_ior (ior);

        if (ior)
          {
            // This is a struct IORAddressingInfo. So, marshall each
            // member of the struct one after another in the order
            // defined.
            if (!(msg << index))
              return false;
            if (!(msg << *ior))
              return false;;
          }
        else
          {
            if (TAO_debug_level)
              {
                ACE_DEBUG ((LM_DEBUG,
                            ACE_TEXT ("(%N |%l) Unable to handle this request\n")));
              }
            return false;
          }
        break;
      }
    default:
      if (TAO_debug_level)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%N |%l) Unable to handle this request\n")));
        }
      return false;
    }

  return true;
}

size_t
TAO_GIOP_Message_Generator_Parser_12::fragment_header_length (void) const
{
  return TAO_GIOP_MESSAGE_FRAGMENT_HEADER;
}

TAO_END_VERSIONED_NAMESPACE_DECL
