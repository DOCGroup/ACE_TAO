//$Id$
#include "tao/GIOP_Message_Accept_State.h"
#include "tao/debug.h"

#if !defined (__ACE_INLINE__)
# include "tao/GIOP_Message_Accept_State.i"
#endif /* __ACE_INLINE__ */

////////////////////////////////////////////////////////////////////////////////
// TAO_GIOP_Message_Accept_State_11 methods 
////////////////////////////////////////////////////////////////////////////////
int
TAO_GIOP_Message_Accept_State_11::
  parse_request_header (TAO_GIOP_ServerRequest &request)
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
  
  IOP::ServiceContextList service_info;
  input >> service_info;
  
  // This method is going to a copy?? Data copy?? Need to figure out a
  // way to avoid this 
  request.service_info (service_info);
  
  
  CORBA::Boolean hdr_status = (CORBA::Boolean) input.good_bit ();

  CORBA::ULong req_id; 
  // Get the rest of the request header ...
  hdr_status = hdr_status && input.read_ulong (req_id);
  
  request.request_id (req_id);
  
  CORBA::Octet response_flags;
  hdr_status = hdr_status && input.read_octet (response_flags);
  request.response_expected ((response_flags != 0));

  // The high bit of the octet has been set if the SyncScope policy
  // value is SYNC_WITH_SERVER. This is a temporary hack until all
  // of GIOP 1.2 is in place. Then we can check the version in the
  // message header instead.
  request.sync_with_server ((response_flags == 129));

  // We use ad-hoc demarshalling here: there is no need to increase
  // the reference count on the CDR message block, because this key
  // will not outlive the request (or the message block).

  CORBA::Long key_length = 0;
  hdr_status = hdr_status && input.read_long (key_length);
  if (hdr_status)
    {
      request.object_key ().replace (key_length, key_length,
                                     (CORBA::Octet*)input.rd_ptr (),
                                     0);
      input.skip_bytes (key_length);
    }

  ACE_CString operation_name;
  if (input.char_translator () == 0)
    {
      CORBA::ULong length = 0;
      hdr_status = hdr_status && input.read_ulong (length);
      if (hdr_status)
        {
          // Do not include NULL character at the end.
          // @@ This is not getting demarshaled using the codeset
          //    translators!
          operation_name.set (input.rd_ptr (),
                              length - 1,
                              0);
          request.operation (operation_name);
          hdr_status = input.skip_bytes (length);
        }
    }
  else
    {
      // @@ We could optimize for this case too, i.e. do in-place
      //    demarshaling of the string... But there is an issue
      //    pending on the OMG as to whether the operation should be
      //    sent in the connection negotiated codeset or always in
      //    ISO8859-1.
      CORBA::String_var tmp;
      hdr_status = hdr_status && input.read_string (tmp.inout ());
      operation_name.set (tmp._retn (), 1);
      request.operation (operation_name);
    }
  
  if (hdr_status)
    {
      CORBA::Principal_var principal;
      
      // Beware extra data copying
      input >> principal.out ();

      request.requesting_principal (principal.in ());
      
      hdr_status = (CORBA::Boolean) input.good_bit ();
    }

  // Set the header length info and offset info
  // request.header_length (this->header_length ());
  // request.message_size_offset (this->offset_length ());
  
  return hdr_status ? 0 : -1;
}


CORBA::Boolean
TAO_GIOP_Message_Accept_State_11::
  write_reply_header (TAO_OutputCDR &output,
                      CORBA::ULong request_id)
{
  // create and write a dummy context
  IOP::ServiceContextList resp_ctx;
  resp_ctx.length (0);
  output << resp_ctx;

  // Write the request ID
  output.write_ulong (request_id);



  return 1;
}

int
TAO_GIOP_Message_Accept_State_11::
  parse_locate_header (TAO_GIOP_Locate_Request_Header &request)
{
  // Get the stream 
  TAO_InputCDR &msg = request.incoming_stream ();

  CORBA::Boolean hdr_status = 1;
  
  // Get the request id
  CORBA::ULong req_id = 0;
  hdr_status = msg.read_ulong (req_id);
  
  // Store it in the Locate request classes
  request.request_id (req_id);

  TAO_ObjectKey object_key;
  
  // Note that here there are no unions and so no problems
  hdr_status = hdr_status && (msg >> object_key);
  
  // Get the underlying TargetAddress from the request class
  GIOP::TargetAddress &target = request.target_address ();

  // Put this object key in the target_adderss
  target.object_key (object_key);

  return hdr_status ? 0 : -1;
}

CORBA::Boolean
TAO_GIOP_Message_Accept_State_11::
write_locate_reply_mesg (TAO_OutputCDR &output,
                         CORBA::ULong request_id,
                         TAO_GIOP_Locate_Status_Msg &status_info)
{
  // Make the header for the locate request 
  output.write_ulong (request_id);
  output.write_ulong (status_info.status);
  
  if (status_info.status == TAO_GIOP_OBJECT_FORWARD)
    {
      CORBA::Object_ptr object_ptr = status_info.forward_location_var.in ();
      if ((output << object_ptr) == 0)
        {
          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,
                        ASYS_TEXT ("TAO (%P|%t|%N|%l) write_locate_reply_mesg-")
                        ASYS_TEXT (" cannot marshal object reference\n")));
        } 
    }
  
  return 1;
}

/*int
TAO_GIOP_Message_Accept_State_11::
  make_reply (const TAO_GIOP_Version &version,
              TAO_GIOP_Message_Type t,
              CORBA::ULong request_id,
              TAO_OutputCDR &);
{
  
}*/

///////////////////////////////////////////////////////////////
// TAO_GIOP_Message_Accept_Impl methods
//////////////////////////////////////////////////////////////
  
CORBA::Boolean
TAO_GIOP_Message_Accept_Impl::
  check_revision (CORBA::Octet incoming_major,
                  CORBA::Octet incoming_minor)
{
  // Need a better way to do this. @@Bala
  if (incoming_major > 1 ||
      incoming_minor > 2)
    return 0;

  return 1;
}
