// -*- C++ -*-
//$Id$

#include "tao/GIOP_Message_Accept_State.h"
#include "tao/debug.h"
#include "tao/GIOP_Server_Request.h"
#include "tao/GIOP_Utils.h"
//#include "tao/CDR.h"
#include "tao/Pluggable_Messaging_Utils.h"
#include "tao/NVList.h"


ACE_RCSID(tao, GIOP_Message_Accept_State, "$Id$")


#if !defined (__ACE_INLINE__)
# include "tao/GIOP_Message_Accept_State.i"
#endif /* __ACE_INLINE__ */


CORBA::Boolean 
TAO_GIOP_Message_Accept_State::marshal_svc_ctx (TAO_OutputCDR &output,
                                                TAO_Pluggable_Reply_Params &reply)
{
  // Write the service context list
#if (TAO_HAS_MINIMUM_CORBA == 1)
  output << reply.service_context_notowned ();
#else
  if (reply.params_ == 0)
    {
      output << reply.service_context_notowned ();
    }
  else
    {
      // If lazy evaluation is enabled then we are going to insert an
      // extra node at the end of the service context list, just to
      // force the appropiate padding.
      // But first we take it out any of them..
      CORBA::ULong count = 0;
      IOP::ServiceContextList &svc_ctx =
        reply.service_context_notowned ();
      CORBA::ULong l = svc_ctx.length ();
      CORBA::ULong i;
      for (i = 0; i != l; ++i)
        {
          if (svc_ctx[i].context_id == TAO_SVC_CONTEXT_ALIGN)
            continue;
          count++;
        }
      // Now increment it to account for the last dummy one...
      count++;
      
      // Now marshal the rest of the service context objects
      output << count;
      for (i = 0; i != l; ++i)
        {
          if (svc_ctx[i].context_id == TAO_SVC_CONTEXT_ALIGN)
            continue;
          output << svc_ctx[i];
        }
    }
#endif /* TAO_HAS_MINIMUM_CORBA */
  return 1;
}


void
TAO_GIOP_Message_Accept_State::marshal_reply_status (TAO_OutputCDR &output,
                                                     TAO_Pluggable_Reply_Params &reply)
{
  switch (reply.reply_status_)
    {
    case TAO_PLUGGABLE_MESSAGE_NO_EXCEPTION:
      output.write_ulong (TAO_GIOP_NO_EXCEPTION);
      break;
    case TAO_PLUGGABLE_MESSAGE_LOCATION_FORWARD:
      output.write_ulong (TAO_GIOP_LOCATION_FORWARD);
      break;
    case TAO_PLUGGABLE_MESSAGE_SYSTEM_EXCEPTION:
      output.write_ulong (TAO_GIOP_SYSTEM_EXCEPTION);
      break;
    case TAO_PLUGGABLE_MESSAGE_USER_EXCEPTION:
      output.write_ulong (TAO_GIOP_USER_EXCEPTION);
      break;
    default:
      // Some other specifc exception
      output.write_ulong (reply.reply_status_);
      break;
    }
}

CORBA::Boolean 
TAO_GIOP_Message_Accept_State::
  unmarshall_object_key (TAO_ObjectKey &object_key,
                         TAO_InputCDR &input)
{
  CORBA::Boolean hdr_status = 
    (CORBA::Boolean) input.good_bit ();
  
  CORBA::Long key_length = 0;
  hdr_status = hdr_status && input.read_long (key_length);
  if (hdr_status)
    {
      object_key.replace (key_length, 
                          key_length,
                          (CORBA::Octet*)input.rd_ptr (),
                          0);
      input.skip_bytes (key_length);
    }
  
  return hdr_status;
}



CORBA::Boolean 
TAO_GIOP_Message_Accept_State::
unmarshall_iop_profile (TAO_ObjectKey & /*object_key*/, 
                        IOP::TaggedProfile & /*profile*/,
                        TAO_InputCDR & /*cdr*/)
{
  return 0;
}


CORBA::Boolean 
TAO_GIOP_Message_Accept_State::
unmarshall_ref_addr (TAO_ObjectKey & /*object_key*/, 
                     GIOP::IORAddressingInfo & /*profile*/,
                     TAO_InputCDR & /*cdr*/)
{
  return 0;
}


////////////////////////////////////////////////////////////////////////////////
// TAO_GIOP_Message_Accept_State_10 methods 
////////////////////////////////////////////////////////////////////////////////
int
TAO_GIOP_Message_Accept_State_10::
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

  IOP::ServiceContextList &service_info = 
    request.service_info ();
  
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

  // The high bit of the octet has been set if the SyncScope policy
  // value is SYNC_WITH_SERVER. This is a temporary hack until all
  // of GIOP 1.2 is in place. Then we can check the version in the
  // message header instead.
  request.sync_with_server ((response_flags == 129));

  // We use ad-hoc demarshalling here: there is no need to increase
  // the reference count on the CDR message block, because this key
  // will not outlive the request (or the message block).

  hdr_status = this->unmarshall_object_key (request.object_key (),
                                            input);

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
TAO_GIOP_Message_Accept_State_10::
  write_reply_header (TAO_OutputCDR &output,
                      TAO_Pluggable_Reply_Params &reply,
                      CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{

  this->marshal_svc_ctx (output,
                         reply);

#if (TAO_HAS_MINIMUM_CORBA == 0)  
  if (reply.params_ != 0)
    {
      // @@ Much of this code is GIOP 1.1 specific and should be
      ptr_arith_t target =
        reply.params_->_tao_target_alignment ();
      
      ptr_arith_t current =
        ptr_arith_t (output.current_alignment ()) % ACE_CDR::MAX_ALIGNMENT;
      
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

  ACE_UNUSED_ARG (ACE_TRY_ENV);
  return 1;
}



int
TAO_GIOP_Message_Accept_State_10::
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

  // Get the object key
  hdr_status = 
    this->unmarshall_object_key (request.object_key (),
                                 msg);

  return hdr_status ? 0 : -1;
}



CORBA::Boolean
TAO_GIOP_Message_Accept_State_10::
write_locate_reply_mesg (TAO_OutputCDR &output,
                         CORBA::ULong request_id,
                         TAO_GIOP_Locate_Status_Msg &status_info)
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
            ACE_DEBUG ((LM_DEBUG,
                        ASYS_TEXT ("TAO (%P|%t|%N|%l) write_locate_reply_mesg-")
                        ASYS_TEXT (" cannot marshal object reference\n")));
        }
    }

  return 1;
}


CORBA::Octet
TAO_GIOP_Message_Accept_State_10::major_version (void)
{
  return (CORBA::Octet) 1;
}



CORBA::Octet
TAO_GIOP_Message_Accept_State_10::minor_version (void)
{
  return (CORBA::Octet) 0;
}



/////////////////////////////////////////////////////
//  TAO_GIOP_Message_Accept_State_11
/////////////////////////////////////////////////////

CORBA::Octet
TAO_GIOP_Message_Accept_State_11::minor_version (void)
{
  return 1;
}



////////////////////////////////////////////////////////////////////////////////
// TAO_GIOP_Message_Accept_State_12 methods 
////////////////////////////////////////////////////////////////////////////////

// The methods below are not complete
int
TAO_GIOP_Message_Accept_State_12::
  parse_request_header (TAO_GIOP_ServerRequest &request)
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
  
  // Need to work around the hacks
  request.response_expected ((response_flags != 0));

  // The high bit of the octet has been set if the SyncScope policy
  // value is SYNC_WITH_SERVER. This is a temporary hack until all
  // of GIOP 1.2 is in place. Then we can check the version in the
  // message header instead.
  request.sync_with_server ((response_flags == 129));

  // Read the discriminant of the union.
  CORBA::Short disc = 0;
  hdr_status = hdr_status && input.read_short (disc);

  if (hdr_status)
    {
      if (disc == GIOP::KeyAddr)
          {
            hdr_status = 
              this->unmarshall_object_key (request.object_key (),
                                           input);
          }
       else if (disc == GIOP::ProfileAddr)
          {
            hdr_status = 
              this->unmarshall_iop_profile (request.object_key (),
                                            request.tagged_profile (),
                                            input);
          }
      else if (disc == GIOP::ReferenceAddr)
        {
             hdr_status = 
               this->unmarshall_ref_addr (request.object_key (),
                                          request.addressing_info (),
                                          input);
        }
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

  // Tear out the service context ... we currently ignore it, but it
  // should probably be passed to each ORB service as appropriate
  // (e.g. transactions, security).
  //
  // NOTE: As security support kicks in, this is a good place to
  // verify a digital signature, if that is required in this security
  // environment.  It may be required even when using IPSEC security
  // infrastructure.
  IOP::ServiceContextList &service_info = 
    request.service_info ();
  
  input >> service_info;

  // Reset the read_ptr to an 8-byte boundary
  input.align_read_ptr (TAO_GIOP_MESSAGE_ALIGN_PTR);
  
  return hdr_status ? 0 : -1;
}


int
TAO_GIOP_Message_Accept_State_12::
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

    // Read the discriminant of the union.
  CORBA::Short disc = 0;
  hdr_status = 
    hdr_status && msg.read_short (disc);

  if (hdr_status)
    {
      if (disc == GIOP::KeyAddr)
          {
            hdr_status = 
              this->unmarshall_object_key (request.object_key (),
                                           msg);
          }
       else if (disc == GIOP::ProfileAddr)
          {
            hdr_status = 
              this->unmarshall_iop_profile (request.object_key (),
                                            request.tagged_profile (),
                                            msg);
          }
      else if (disc == GIOP::ReferenceAddr)
        {
             hdr_status = 
               this->unmarshall_ref_addr (request.object_key (),
                                          request.addressing_info (),
                                          msg);
        }
    }
  
  // Reset the pointer to an 8-byte bouns]dary
  msg.align_read_ptr (TAO_GIOP_MESSAGE_ALIGN_PTR);

  return hdr_status ? 0 : -1;
}



CORBA::Boolean 
TAO_GIOP_Message_Accept_State_12::
  unmarshall_iop_profile (TAO_ObjectKey &object_key,
                          IOP::TaggedProfile &profile_addr,
                          TAO_InputCDR &input)
{
  CORBA::Boolean hdr_status = 
    (CORBA::Boolean) input.good_bit ();
  
  // Extract the TaggedProfile
  // @@We can also look in to the CDR stream to extract the
  // members of the struct (TaggedProfile) directly. A place
  // for optimzation. Once we have this working we can implement
  // this 
  hdr_status &= input >> profile_addr;
  
  // Extract the object key from the TaggedProfile
  if (hdr_status)
    {
      object_key.replace (profile_addr.profile_data.length (),
                          profile_addr.profile_data.length (),
                          profile_addr.profile_data.get_buffer ());
                          
    }

  return hdr_status;
}


CORBA::Boolean 
TAO_GIOP_Message_Accept_State_12::
  unmarshall_ref_addr (TAO_ObjectKey &object_key,
                       GIOP::IORAddressingInfo &addr_info,
                       TAO_InputCDR &input)
{
  CORBA::Boolean hdr_status = 
    (CORBA::Boolean) input.good_bit ();

  //Extract the Addressing info
  // @@We can also look in to the CDR stream to extract the
  // members of the struct (AddressingInfo) directly. A place
  // for optimzation. Once we have this working we can implement
  // this 
  hdr_status &= input>> addr_info;
  
  // Extract the object key
  if (hdr_status)
    {
      // Get the IOP::TaggedProfile
      IOP::TaggedProfile &tag = 
        addr_info.ior.profiles [addr_info.selected_profile_index];
      
      // Replace the object key
      object_key.replace (tag.profile_data.length (),
                          tag.profile_data.length (),
                          tag.profile_data.get_buffer ());
    }
  
  return hdr_status;
}




CORBA::Boolean
TAO_GIOP_Message_Accept_State_12::
write_reply_header (TAO_OutputCDR & output,
                    TAO_Pluggable_Reply_Params &reply,
                    CORBA::Environment & /*ACE_TRY_ENV*/)
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
           TAO_PLUGABLE_MESSAGE_NEEDS_ADDRESSING_MODE)
    {
      // Not sure when we will use this.
      output.write_ulong (TAO_GIOP_LOC_NEEDS_ADDRESSING_MODE);
    }
  else
    {
      this->marshal_reply_status (output, 
                                  reply);
    }

  // Service context list
  this->marshal_svc_ctx (output,
                         reply);
  
  if (output.align_write_ptr (TAO_GIOP_MESSAGE_ALIGN_PTR) == -1)
    return 0;

  return 1;
}


CORBA::Boolean 
TAO_GIOP_Message_Accept_State_12::
write_locate_reply_mesg (TAO_OutputCDR & output,
                         CORBA::ULong request_id,
                         TAO_GIOP_Locate_Status_Msg &status_info)
{
  // Make the header for the locate request
  output.write_ulong (request_id);
  output.write_ulong (status_info.status);

  if (output.align_write_ptr (TAO_GIOP_MESSAGE_ALIGN_PTR) == -1)
    return 0;

  switch (status_info.status)
    {

      // More likely than not we will not have this in TAO
    case TAO_GIOP_OBJECT_FORWARD:
    case TAO_GIOP_OBJECT_FORWARD_PERM:
      {
        CORBA::Object_ptr object_ptr = 
          status_info.forward_location_var.in ();
        if ((output << object_ptr) == 0)
        {
          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,
                        ASYS_TEXT ("TAO (%P|%t|%N|%l) write_locate_reply_mesg-")
                        ASYS_TEXT (" cannot marshal object reference\n")));
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



CORBA::Octet
TAO_GIOP_Message_Accept_State_12::major_version (void)
{
  return (CORBA::Octet) 1;
}


CORBA::Octet
TAO_GIOP_Message_Accept_State_12::minor_version (void)
{
  return (CORBA::Octet) 2;
}

