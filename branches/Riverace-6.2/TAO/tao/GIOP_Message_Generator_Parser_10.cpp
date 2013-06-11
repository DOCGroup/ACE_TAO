// -*- C++ -*-
// $Id$

#include "tao/GIOP_Message_Generator_Parser_10.h"
#include "tao/GIOP_Utils.h"
#include "tao/GIOP_Message_Locate_Header.h"
#include "tao/operation_details.h"
#include "tao/debug.h"
#include "tao/Pluggable_Messaging_Utils.h"
#include "tao/TAO_Server_Request.h"
#include "tao/ORB_Constants.h"
#include "tao/CDR.h"
#include "tao/SystemException.h"

#include "ace/Log_Msg.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

bool
TAO_GIOP_Message_Generator_Parser_10::write_request_header (
    const TAO_Operation_Details &opdetails,
    TAO_Target_Specification &spec,
    TAO_OutputCDR &msg)
{
  // Write the service context list
  if (!(msg << opdetails.request_service_info ()))
    return false;

  // The request ID
  if (!(msg << opdetails.request_id ()))
    return false;

  CORBA::Octet const response_flags = opdetails.response_flags ();

  // Write the response flags
  if (response_flags == TAO_TWOWAY_RESPONSE_FLAG)
    {
      msg << ACE_OutputCDR::from_octet (1);
    }
  else
    {
      msg << ACE_OutputCDR::from_octet (0);
    }

  // In this case we cannot recognise anything other than the Object
  // key as the address disposition variable. But we do a sanity check
  // anyway.
  const TAO::ObjectKey *key = spec.object_key ();

  if (key)
    {
      // Put in the object key
      msg << *key;
    }
  else
    {
      if (TAO_debug_level)
        {
          TAOLIB_ERROR ((LM_ERROR,
                      ACE_TEXT ("(%N |%l) Unable to handle this request\n")));
        }
      return false;
    }

  msg.write_string (opdetails.opname_len (), opdetails.opname ());

  // Last element of request header is the principal; no portable way
  // to get it, we just pass empty principal (convention: indicates
  // "anybody").  Steps upward in security include passing an
  // unverified user ID, and then verifying the message (i.e. a dummy
  // service context entry is set up to hold a digital signature for
  // this message, then patched shortly before it's sent).

  /***** This has been deprecated in the 2.4 spec ******/

#if defined (TAO_PEER_REQUIRES_PRINCIPAL)

  char username[BUFSIZ];
  char *result =
    ACE_OS::cuserid (username,
                     BUFSIZ);

  if (result != 0)
    {
      const CORBA::ULong username_size =
        static_cast<CORBA::ULong> (ACE_OS::strlen (username));

      CORBA::Octet *buffer =
        CORBA::OctetSeq::allocbuf (username_size + 1);

      ACE_OS::memcpy (buffer,
                      username,
                      username_size + 1);

      req_principal.replace (username_size + 1,
                             username_size + 1,
                             buffer,
                             1);
    }

#else

  CORBA::OctetSeq req_principal (0);
  req_principal.length (0);

#endif /* TAO_PEER_REQUIRES_PRINCIPAL */

  msg << req_principal;

  return true;
}

bool
TAO_GIOP_Message_Generator_Parser_10::write_locate_request_header (
    CORBA::ULong request_id,
    TAO_Target_Specification  &spec,
    TAO_OutputCDR &msg)
{
  msg << request_id;

  // In this case we cannot recognise anything other than the Object
  // key as the address disposition variable. But we do a sanity check
  // anyway.
  const TAO::ObjectKey *key = spec.object_key ();

  if (key)
    {
      // Everything is fine
      msg << *key;
    }
  else
    {
      if (TAO_debug_level)
        {
          TAOLIB_ERROR ((LM_ERROR,
                      ACE_TEXT ("(%N | %l) Unable to handle this request\n")));
        }
      return false;
    }

  return true;
}

bool
TAO_GIOP_Message_Generator_Parser_10::write_reply_header (
    TAO_OutputCDR &output,
    TAO_Pluggable_Reply_Params_Base &reply)
{
  // Write the service context list.
#if (TAO_HAS_MINIMUM_CORBA == 1)
  if (!(output << reply.service_context_notowned ()))
    return false;
#else
  if (!reply.is_dsi_)
    {
      if (!(output << reply.service_context_notowned ()))
        return false;
    }
  else
    {
      // If lazy evaluation is enabled then we are going to insert an
      // extra node at the end of the service context list, just to
      // force the appropriate padding.
      // But first we take it out any of them..
      CORBA::ULong count = 0;
      IOP::ServiceContextList &svc_ctx = reply.service_context_notowned ();
      CORBA::ULong const l = svc_ctx.length ();
      CORBA::ULong i;

      for (i = 0; i != l; ++i)
        {
          if (svc_ctx[i].context_id == TAO_SVC_CONTEXT_ALIGN)
            {
              continue;
            }

          ++count;
        }

      // Now increment it to account for the last dummy one...
      ++count;

      // Now marshal the rest of the service context objects
      if (!(output << count))
        return false;

      for (i = 0; i != l; ++i)
        {
          if (svc_ctx[i].context_id == TAO_SVC_CONTEXT_ALIGN)
            {
              continue;
            }

          if (!(output << svc_ctx[i]))
            return false;
        }

    }

  if (reply.is_dsi_ == true)
    {
      // @@ Much of this code is GIOP 1.1 specific and should be
      ptrdiff_t target = reply.dsi_nvlist_align_;

      ptrdiff_t const current =
        ptrdiff_t (output.current_alignment ()) % ACE_CDR::MAX_ALIGNMENT;

      CORBA::ULong pad = 0;

      if (target == 0)
        {
          // We want to generate adequate padding to start the request
          // id on a 8 byte boundary, two cases:
          // - If the dummy tag starts on a 4 byte boundary and the
          //   dummy sequence has 0 elements then we have:
          //   4:tag 8:sequence_length 4:sequence_body 4:request_id
          //   8:payload
          // - If the dummy tag starts on an 8 byte boundary, with 4
          //   elements we get:
          //   8:tag 4:sequence_length 8:sequence_body 4:request_id
          //   8:payload
          if (current != 0 && current <= ACE_CDR::LONG_ALIGN)
            {
              pad = 4;
            }
        }
      else if (target != ACE_CDR::LONG_ALIGN)
        {
          // The situation reverses, we want to generate adequate
          // padding to start the request id on a 4 byte boundary, two
          // cases:
          // - If the dummy tag starts on a 4 byte boundary and the
          //   dummy sequence has 4 elements then we have:
          //   4:tag 8:sequence_length 4:sequence_body 8:request_id
          //   4:payload
          // - If the dummy tag starts on an 8 byte boundary, with 0
          //   elements we get:
          //   8:tag 4:sequence_length 8:sequence_body 8:request_id
          //   4:payload
          if (current > ACE_CDR::LONG_ALIGN)
            {
              pad = 4;
            }
        }
      else if (target == ACE_CDR::MAX_ALIGNMENT)
        {
          pad = 0;
        }
      else
        {
          // <target> can only have the values above
          throw ::CORBA::MARSHAL ();
        }

      output << CORBA::ULong (TAO_SVC_CONTEXT_ALIGN);
      output << pad;

      for (CORBA::ULong j = 0; j != pad; ++j)
        {
          output << ACE_OutputCDR::from_octet(0);
        }
    }
#endif /* TAO_HAS_MINIMUM_CORBA */

  // Write the request ID
  output.write_ulong (reply.request_id_);

  // Write the reply status
  output.write_ulong (reply.reply_status ());

  return true;
}


bool
TAO_GIOP_Message_Generator_Parser_10::write_locate_reply_mesg (
    TAO_OutputCDR &output,
    CORBA::ULong request_id,
    TAO_GIOP_Locate_Status_Msg &status_info)
{
  // Make the header for the locate request
  output.write_ulong (request_id);
  output.write_ulong (status_info.status);

  if (status_info.status == GIOP::OBJECT_FORWARD)
    {
      CORBA::Object_ptr object_ptr = status_info.forward_location_var.in ();

      if (!(output << object_ptr))
        {
          if (TAO_debug_level > 0)
            {
              TAOLIB_ERROR ((
                  LM_ERROR,
                  ACE_TEXT ("TAO (%P|%t|%N|%l) write_locate_reply_mesg-")
                  ACE_TEXT (" cannot marshal object reference\n")
                ));
            }
          return false;
        }
    }

  return true;
}

bool
TAO_GIOP_Message_Generator_Parser_10::write_fragment_header (
  TAO_OutputCDR & /* cdr */,
  CORBA::ULong /* request_id */)
{
  // GIOP fragments are not supported in GIOP 1.0.
  return false;
}


int
TAO_GIOP_Message_Generator_Parser_10::parse_request_header (
    TAO_ServerRequest &request)
{
  // Tear out the service context ... we currently ignore it, but it
  // should probably be passed to each ORB service as appropriate
  // (e.g. transactions, security).
  //
  // NOTE: As security support kicks in, this is a good place to
  // verify a digital signature, if that is required in this security
  // environment.  It may be required even when using IPSEC security
  // infrastructure.

  // Get the input CDR in the request class
  TAO_InputCDR & input = *request.incoming ();

  IOP::ServiceContextList &service_info = request.request_service_info ();

  if (!(input >> service_info))
    return -1;

  CORBA::Boolean hdr_status = (CORBA::Boolean) input.good_bit ();

  CORBA::ULong req_id = 0;

  // Get the rest of the request header ...
  hdr_status = hdr_status && input.read_ulong (req_id);

  request.request_id (req_id);

  CORBA::Octet response_flags = CORBA::Octet();
  hdr_status = hdr_status && input.read_octet (response_flags);
  request.response_expected ((response_flags != 0));

  // This is not supported now in GIOP 1.1
  request.sync_with_server (0);

  // We use ad-hoc demarshalling here: there is no need to increase
  // the reference count on the CDR message block, because this key
  // will not outlive the request (or the message block).

  hdr_status =
    hdr_status && request.profile ().unmarshall_object_key (input);

  // According to the CORBA 2.6.1 (and older) specification, the operation
  // name is an IDL Identifier. Identifiers must be composed of ASCII letters,
  // numbers, and underscores, starting with a letter. Based on this, and
  // the fact that I could find no text explicitly requiring operation name
  // translation, nor could others in the CORBA community, the operation name
  // will not be translated regardless of the translation of other strings.
  //
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

  if (hdr_status)
    {
      /**** This has been deprecated in 2.4 ****/
      /*CORBA::Principal_var principal;

      input >> principal.out ();

      request.requesting_principal (principal.in ()); */

      CORBA::OctetSeq oct_seq;
      input >> oct_seq;
      request.requesting_principal (oct_seq);
      hdr_status = (CORBA::Boolean) input.good_bit ();
    }

  return hdr_status ? 0 : -1;
}

int
TAO_GIOP_Message_Generator_Parser_10::parse_locate_header (
    TAO_GIOP_Locate_Request_Header &request)
{
  // Get the stream
  TAO_InputCDR &msg = request.incoming_stream ();

  // Get the request id
  CORBA::ULong req_id = 0;
  CORBA::Boolean hdr_status = msg.read_ulong (req_id);

  // Store it in the Locate request classes
  request.request_id (req_id);

  // Get the object key
  hdr_status = hdr_status && request.profile ().unmarshall_object_key (msg);

  return hdr_status ? 0 : -1;
}

int
TAO_GIOP_Message_Generator_Parser_10::parse_reply (
    TAO_InputCDR &cdr,
    TAO_Pluggable_Reply_Params &params)
{
  // Read the service context list first
  if (!(cdr >> params.svc_ctx_))
    {
      if (TAO_debug_level > 0)
        {
          TAOLIB_ERROR ((LM_ERROR,
                      ACE_TEXT ("TAO (%P|%t) parse_reply, ")
                      ACE_TEXT ("extracting context\n")));
        }
      return -1;
    }

  // Call the base class for further processing
  if (TAO_GIOP_Message_Generator_Parser::parse_reply (cdr, params) == -1)
    return -1;

  return 0;
}


int
TAO_GIOP_Message_Generator_Parser_10::parse_locate_reply (
    TAO_InputCDR &cdr,
    TAO_Pluggable_Reply_Params &params)
{
  if (TAO_GIOP_Message_Generator_Parser::parse_locate_reply (cdr,
                                                             params) == -1)

    return -1;

  return 0;
}

CORBA::Octet
TAO_GIOP_Message_Generator_Parser_10::major_version (void) const
{
  // Any harm in hardcoding??
  return static_cast<CORBA::Octet> (1);
}

CORBA::Octet
TAO_GIOP_Message_Generator_Parser_10::minor_version (void) const
{
  // Any harm in hardcoding??
  return 0;
}

size_t
TAO_GIOP_Message_Generator_Parser_10::fragment_header_length (void) const
{
  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL
