// -*- C++ -*-

#include "GIOP_Message_Generator_Parser_10.h"
#include "GIOP_Utils.h"

#include "GIOP_Message_Locate_Header.h"

#include "operation_details.h"
#include "CDR.h"
#include "Any.h"
#include "debug.h"
#include "OctetSeqC.h"
#include "Pluggable_Messaging_Utils.h"
#include "TAO_Server_Request.h"
#include "GIOP_Message_State.h"
#include "CORBA_String.h"
#include "ORB_Core.h"

#if !defined (__ACE_INLINE__)
# include "GIOP_Message_Generator_Parser_10.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID (tao,
           GIOP_Message_Gen_Parser_10,
           "$Id$")

int
TAO_GIOP_Message_Generator_Parser_10::write_request_header (
    const TAO_Operation_Details &opdetails,
    TAO_Target_Specification &spec,
    TAO_OutputCDR &msg)
{
  // Write the service context list
  msg << opdetails.request_service_info ();

  // The request ID
  msg << opdetails.request_id ();

  const CORBA::Octet response_flags = opdetails.response_flags ();

  // Write the response flags
  if (response_flags == TAO_TWOWAY_RESPONSE_FLAG)
    msg << CORBA::Any::from_octet (1);
  else
    msg << CORBA::Any::from_octet (0);

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
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%N |%l) Unable to handle this request \n")));
      return 0;
    }

  msg.write_string (opdetails.opname_len (),
                    opdetails.opname ());

  // Last element of request header is the principal; no portable way
  // to get it, we just pass empty principal (convention: indicates
  // "anybody").  Steps upward in security include passing an
  // unverified user ID, and then verifying the message (i.e. a dummy
  // service context entry is set up to hold a digital signature for
  // this message, then patched shortly before it's sent).

  /***** This has been deprecated in the 2.4 spec ******/
  //static CORBA::Principal_ptr principal = 0;
  //msg << principal;

  // This is now introduced in 2.4 spec
  CORBA::OctetSeq req_principal (0);
  req_principal.length (0);
  msg << req_principal;

  return 1;
}




int
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
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%N | %l) Unable to handle this request \n")));
      return 0;
    }

  return 1;
}


int
TAO_GIOP_Message_Generator_Parser_10::write_reply_header (
    TAO_OutputCDR &output,
    TAO_Pluggable_Reply_Params_Base &reply
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Write the service context list.
#if (TAO_HAS_MINIMUM_CORBA == 1)
  output << reply.service_context_notowned ();
  ACE_ENV_ARG_NOT_USED; // FUZZ: ignore check_for_ace_check
#else
  if (reply.is_dsi_ == 0)
    {
      output << reply.service_context_notowned ();
    }
  else
    {
      // If lazy evaluation is enabled then we are going to insert an
      // extra node at the end of the service context list, just to
      // force the appropriate padding.
      // But first we take it out any of them..
      CORBA::ULong count = 0;
      IOP::ServiceContextList &svc_ctx =
        reply.service_context_notowned ();
      CORBA::ULong l = svc_ctx.length ();
      CORBA::ULong i;

      for (i = 0; i != l; ++i)
        {
          if (svc_ctx[i].context_id == TAO_SVC_CONTEXT_ALIGN)
            {
              continue;
            }

          count++;
        }

      // Now increment it to account for the last dummy one...
      count++;

      // Now marshal the rest of the service context objects
      output << count;
      for (i = 0; i != l; ++i)
        {
          if (svc_ctx[i].context_id == TAO_SVC_CONTEXT_ALIGN)
            {
              continue;
            }

          output << svc_ctx[i];
        }

    }

  if (reply.is_dsi_ == 1)
    {
      // @@ Much of this code is GIOP 1.1 specific and should be
      ptrdiff_t target = reply.dsi_nvlist_align_;

      ptrdiff_t current =
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
          ACE_THROW_RETURN (CORBA::MARSHAL (),
                            0);
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
  this->marshal_reply_status (output,
                              reply);

  return 1;
}


int
TAO_GIOP_Message_Generator_Parser_10::write_locate_reply_mesg (
    TAO_OutputCDR &output,
    CORBA::ULong request_id,
    TAO_GIOP_Locate_Status_Msg &status_info
  )
{
  // Make the header for the locate request
  output.write_ulong (request_id);
  output.write_ulong (status_info.status);

  if (status_info.status == TAO_GIOP_OBJECT_FORWARD)
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

  return 1;
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
  TAO_InputCDR& input = request.incoming ();

  IOP::ServiceContextList &service_info =
    request.request_service_info ();

  input >> service_info;

  CORBA::Boolean hdr_status =
    (CORBA::Boolean) input.good_bit ();

  CORBA::ULong req_id;

  // Get the rest of the request header ...
  hdr_status = hdr_status && input.read_ulong (req_id);

  request.request_id (req_id);

  CORBA::Octet response_flags;
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
    TAO_GIOP_Locate_Request_Header &request
  )
{
  // Get the stream
  TAO_InputCDR &msg = request.incoming_stream ();

  CORBA::Boolean hdr_status = 1;

  // Get the request id
  CORBA::ULong req_id = 0;
  hdr_status = msg.read_ulong (req_id);

  // Store it in the Locate request classes
  request.request_id (req_id);

  // Get the object key
  hdr_status =
    hdr_status && request.profile ().unmarshall_object_key (msg);

  return hdr_status ? 0 : -1;
}



int
TAO_GIOP_Message_Generator_Parser_10::parse_reply (
    TAO_InputCDR &cdr,
    TAO_Pluggable_Reply_Params &params)

{
  // Read the service context list first
  if ((cdr >> params.svc_ctx_) == 0)
    {
      if (TAO_debug_level >     0)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO (%P|%t) parse_reply, ")
                    ACE_TEXT ("extracting context\n")));
      return -1;
    }

  // Call the base class for further processing
  if (TAO_GIOP_Message_Generator_Parser::parse_reply (cdr,
                                                      params) == -1)
    return -1;

  // Steal rest of the contents in to the reply params and loose
  // ownership of the  data block.
  params.input_cdr_.exchange_data_blocks (cdr);

  return 0;
}


int
TAO_GIOP_Message_Generator_Parser_10::parse_locate_reply (
    TAO_InputCDR &cdr,
    TAO_Pluggable_Reply_Params &params
  )
{
  if (TAO_GIOP_Message_Generator_Parser::parse_locate_reply (cdr,
                                                             params) == -1)

    return -1;

  // Steal the contents in to the reply CDR and loose ownership of the
  // data block.
  params.input_cdr_.exchange_data_blocks (cdr);

  return 0;
}

CORBA::Octet
TAO_GIOP_Message_Generator_Parser_10::major_version (void)
{
  // Any harm in hardcoding??
  return (CORBA::Octet) 1;
}

CORBA::Octet
TAO_GIOP_Message_Generator_Parser_10::minor_version (void)
{
  // Any harm in hardcoding??
  return 0;
}
