//$Id$

#include "tao/GIOP_Message_Lite.h"
#include "debug.h"
#include "tao/TAOC.h"
#include "tao/ORB_Core.h"
#include "tao/POA.h"
#include "tao/GIOP_Utils.h"

#if !defined (__ACE_INLINE__)
# include "tao/GIOP_Message_Lite.i"
#endif /* __ACE_INLINE__ */

TAO_GIOP_Message_Lite::TAO_GIOP_Message_Lite (void)
{
  //no-op
}

TAO_GIOP_Message_Lite::~TAO_GIOP_Message_Lite (void)
{
  //no-op
}

CORBA::Boolean
TAO_GIOP_Message_Lite::
  write_protocol_header (TAO_Pluggable_Message_Type t,
                         TAO_OutputCDR &msg)
{
  // Reset the message type
  msg.reset ();
  
  // Write a dummy <size> later it is set to the right value...  @@
  // TODO Maybe we should store the OutputCDR status in
  CORBA::ULong size = 0;
  msg.write_ulong (size);
  
  TAO_GIOP_Message_Type type = TAO_GIOP_MESSAGERROR;
  
  // First convert the Pluggable type to the GIOP specific type. 
  switch (t)
    {
    case (TAO_PLUGGABLE_MESSAGE_REQUEST):
      type = TAO_GIOP_REQUEST;
      break;
    case (TAO_PLUGGABLE_MESSAGE_REPLY):
      type = TAO_GIOP_REPLY;
      break;
    case (TAO_PLUGGABLE_MESSAGE_CANCELREQUEST):
      type = TAO_GIOP_CANCELREQUEST;
      break;
    case (TAO_PLUGGABLE_MESSAGE_LOCATEREQUEST):
      type = TAO_GIOP_LOCATEREQUEST;
      break;
    case (TAO_PLUGGABLE_MESSAGE_LOCATEREPLY):
      type = TAO_GIOP_LOCATEREPLY;
      break;
    case (TAO_PLUGGABLE_MESSAGE_CLOSECONNECTION):
      type = TAO_GIOP_CLOSECONNECTION;
      break;
    case (TAO_PLUGGABLE_MESSAGE_MESSAGERROR):
      type = TAO_GIOP_MESSAGERROR;
      break;
    case (TAO_PLUGGABLE_MESSAGE_FRAGMENT):
      type = TAO_GIOP_FRAGMENT;
      break;
    }

  // Write the message type
  msg.write_octet ((CORBA::Octet) type);
  return 1;
}


int
TAO_GIOP_Message_Lite::
  handle_input (TAO_Transport *transport,
                TAO_ORB_Core * /*orb_core*/,
                TAO_Message_State_Factory &mesg_state,
                ACE_Time_Value *max_wait_time)
{
  TAO_GIOP_Message_State *state =
    ACE_dynamic_cast (TAO_GIOP_Message_State *,
                      &mesg_state);
  
  if (state->header_received () == 0)
    {
      int retval = 
        TAO_GIOP_Utils::read_bytes_input (transport,
                                          state->cdr,
                                          TAO_GIOP_LITE_HEADER_LEN,
                                          max_wait_time); 
      if (retval == -1 && TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ASYS_TEXT ("TAO (%P|%t) - %p\n")
                      ASYS_TEXT ("TAO_GIOP_Message_Lite::handle_input")));
          return -1;
        }
      
      // After we read, we assume that everything is fine. We dont do
      // any sanity check of the incoming header.
      
      // Read the rest of the stuff. That should be read by the
      // corresponding states
      if (this->parse_header (state) == -1)
        {
          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,
                        "TAO (%P|%t|%N%l) - %p\n"
                          "TAO_GIOP_Message_Lite::handle_input"));
          return -1;
        }
      
      if (state->cdr.grow (TAO_GIOP_LITE_HEADER_LEN +
                           state->message_size) == -1)
        {
          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,
                        ASYS_TEXT ("TAO (%P|%t|%N|%l) - %p\n"),
                        ASYS_TEXT ("ACE_CDR::grow")));
          return -1;
        }
      
      // Growing the buffer may have reset the rd_ptr(), but we want
      // to leave it just after the GIOP header (that was parsed
      // already);
      state->cdr.skip_bytes (TAO_GIOP_LITE_HEADER_LEN);
    }

  size_t missing_data =
    state->message_size - state->current_offset;
    
  ssize_t n =
    TAO_GIOP_Utils::read_buffer (transport,
                                 state->cdr.rd_ptr () + state->current_offset,
                                 missing_data,
                                 max_wait_time);
  if (n == -1)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    ASYS_TEXT ("TAO (%P|%t) - %p\n"),
                    ASYS_TEXT ("TAO_GIOP_Message_Lite::handle_input, read_buffer[1]"))); 
      return -1;
    }
  else if (n == 0)
    {
      if (errno == EWOULDBLOCK)
        return 0;
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    ASYS_TEXT ("TAO (%P|%t) - %p\n"),
                    ASYS_TEXT ("TAO_GIOP_Message_Lite::handle_input, read_buffer[2]")));
      return -1;
    }

  
  state->current_offset += n;
  
  if (state->current_offset == state->message_size)
    {
      if (TAO_debug_level >= 4)
        {

          size_t header_len = TAO_GIOP_LITE_HEADER_LEN;

          // Need to include GIOPlite too.
          char *buf = state->cdr.rd_ptr ();
          buf -= header_len;
          size_t msg_len = state->cdr.length () + header_len;
          this->dump_msg ("recv",
                          ACE_reinterpret_cast (u_char *,
                                                buf),
                          msg_len);
        }
    }
  return state->is_complete ();
}

CORBA::Boolean
TAO_GIOP_Message_Lite::
  write_message_header (const TAO_Pluggable_Connector_Params &params, 
                        TAO_Pluggable_Header_Type header_type,
                        TAO_Target_Specification &spec,
                        TAO_OutputCDR &cdr)
{
  switch (header_type)
    {
    case TAO_PLUGGABLE_MESSAGE_REQUEST_HEADER:
      return this->write_request_header (params.svc_ctx,
                                         params.request_id,
                                         params.response_flags,
                                         spec,
                                         params.operation_name,
                                         cdr);
      break;
    case TAO_PLUGGABLE_MESSAGE_LOCATE_REQUEST_HEADER:
      return this->write_locate_request_header (params.request_id,
                                                spec,
                                                cdr);
      break;
    default:
      if (TAO_debug_level > 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ASYS_TEXT ("(%P|%t|%N|%l) Wrong header type \n")),
                          0);
      
    }
  
  return 1;
}

int
TAO_GIOP_Message_Lite::
  send_message (TAO_Transport *transport,
                TAO_OutputCDR &stream,
                ACE_Time_Value *max_wait_time = 0,
                TAO_Stub *stub = 0,
                int two_way = 1)
{
  // Get the header length
  const size_t header_len = TAO_GIOP_LITE_HEADER_LEN;

  // Get the message size offset
  const size_t offset = TAO_GIOP_LITE_MESSAGE_SIZE_OFFSET;

  // Ptr to first buffer.
  char *buf = (char *) stream.buffer ();

  // Length of all buffers.
  size_t total_len =
    stream.total_length ();

  // NOTE: Here would also be a fine place to calculate a digital
  // signature for the message and place it into a preallocated slot
  // in the "ServiceContext".  Similarly, this is a good spot to
  // encrypt messages (or just the message bodies) if that's needed in
  // this particular environment and that isn't handled by the
  // networking infrastructure (e.g., IPSEC).

  CORBA::ULong bodylen = total_len - header_len;

#if !defined (ACE_ENABLE_SWAP_ON_WRITE)
  *ACE_reinterpret_cast (CORBA::ULong *, buf + offset) = bodylen;
#else
  if (!stream->do_byte_swap ())
    *ACE_reinterpret_cast (CORBA::ULong *,
                           buf + offset) = bodylen;
  else
    ACE_CDR::swap_4 (ACE_reinterpret_cast (char *,
                                           &bodylen),
                     buf + offset);
#endif /* ACE_ENABLE_SWAP_ON_WRITE */
  
  this->dump_msg ("send",
                  ACE_reinterpret_cast (u_char *,
                                        buf),
                  stream.length ());
  
  return this->transport_message (transport,
                                  stream,
                                  two_way,
                                  stub,
                                  max_wait_time); 
}


int
TAO_GIOP_Message_Lite::
  parse_reply (TAO_Message_State_Factory &mesg_state,
               TAO_Pluggable_Connector_Params &params,
               CORBA::ULong &reply_status)
{
  // Cast to the GIOP Message state
  TAO_GIOP_Message_State *state = 
    ACE_dynamic_cast (TAO_GIOP_Message_State *,
                      &mesg_state);
  
  switch (state->message_type)
    {
    case TAO_GIOP_REQUEST:
      // In GIOP 1.0 and GIOP 1.1 this is an error,
      ACE_ERROR_RETURN ((LM_ERROR,
                         ASYS_TEXT ("TAO (%P|%t) %N:%l TAO_GIOP_Message_Lite::parse_reply: ")  
                         ASYS_TEXT ("request.\n")),
                        -1);

    case TAO_GIOP_CANCELREQUEST:
    case TAO_GIOP_LOCATEREQUEST:
      // Errors
    case TAO_GIOP_CLOSECONNECTION:
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ASYS_TEXT ("TAO (%P|%t) %N:%l parse_reply: ")
                           ASYS_TEXT ("wrong message.\n")),
                          -1);
        break;
    case TAO_GIOP_LOCATEREPLY:
      // Handle after the switch
      break;
    case TAO_GIOP_REPLY:
      if ((state->cdr >> params.svc_ctx) == 0)
        {
          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,
                        ASYS_TEXT ("TAO (%P|%t) parse_reply, ")
                        ASYS_TEXT ("extracting context\n")));
          return -1;
        }
      // Rest of the stuff after the switch
      break;
    case TAO_GIOP_FRAGMENT:
      // Never happens: why??
      break;
    }
  
  // Read the request id
  if (!state->cdr.read_ulong (params.request_id))
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    ASYS_TEXT ("TAO (%P|%t|%N|%l) : TAO_GIOP_Message_Lite::parse_reply, ")
                    ASYS_TEXT ("extracting request id")));
      return -1;
    }

  // and the reply status type.  status can be NO_EXCEPTION,
  // SYSTEM_EXCEPTION, USER_EXCEPTION, LOCATION_FORWARD
  // CAnnot handle LOCATION_FORWARD_PERM here
  if (!state->cdr.read_ulong (reply_status))
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    ASYS_TEXT ("TAO (%P|%t|%N|%l) : TAO_GIOP_Message_Lite::parse_reply, ")
                    ASYS_TEXT ("extracting reply status\n")));
      return -1;
    }

  return 0;
}


int
TAO_GIOP_Message_Lite::
  process_connector_messages (TAO_Transport *transport,
                              TAO_ORB_Core *orb_core,
                              TAO_InputCDR &input,
                              CORBA::Octet message_type)
{
  char repbuf[ACE_CDR::DEFAULT_BUFSIZE];

#if defined(ACE_HAS_PURIFY)
  (void) ACE_OS::memset (repbuf,
                         '\0',
                         sizeof repbuf);
#endif /* ACE_HAS_PURIFY */
  TAO_OutputCDR output (repbuf,
                        sizeof repbuf,
                        TAO_ENCAP_BYTE_ORDER,
                        orb_core->output_cdr_buffer_allocator (),
                        orb_core->output_cdr_dblock_allocator (),
                        orb_core->orb_params ()->cdr_memcpy_tradeoff (),
                        orb_core->to_iso8859 (),
                        orb_core->to_unicode ());

  switch (message_type)
    {
    case TAO_GIOP_REQUEST:
      // Should be taken care by the state specific invocations. They
      // could raise an exception or write things in the output CDR
      // stream
      this->process_connector_request (transport,
                                       orb_core,
                                       input,
                                       output);
      break;
    case TAO_GIOP_LOCATEREQUEST:
      this->process_connector_locate (transport,
                                      orb_core,
                                      input,
                                      output);
      break;
    case TAO_GIOP_MESSAGERROR:
    case TAO_GIOP_REPLY:
    case TAO_GIOP_LOCATEREPLY:
    case TAO_GIOP_CLOSECONNECTION:
    default:
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    ASYS_TEXT ("TAO (%P|%t) Illegal message received by server\n")));
      return this->send_error (transport);
      break;
    }

  return 0;
}


CORBA::Boolean
TAO_GIOP_Message_Lite::
  write_request_header (const IOP::ServiceContextList& /*svc_ctx*/,
                        CORBA::ULong request_id,
                        CORBA::Octet response_flags,
                        TAO_Target_Specification & spec,
                        const char * opname,
                        TAO_OutputCDR &out_stream)
{
  out_stream << request_id;

  // @@ (JP) Temporary hack until all of GIOP 1.2 is implemented.
  if (response_flags == 131)
    out_stream << CORBA::Any::from_octet (1);

  // Sync scope - ignored by server if request is not oneway.
  else if (response_flags == CORBA::Octet (TAO::SYNC_WITH_TRANSPORT) ||
           response_flags == CORBA::Octet (TAO::SYNC_NONE) ||
           response_flags == CORBA::Octet (TAO::SYNC_EAGER_BUFFERING) ||
           response_flags == CORBA::Octet (TAO::SYNC_DELAYED_BUFFERING))
    // No response required.
    out_stream << CORBA::Any::from_octet (0);

  else if (response_flags == CORBA::Octet (TAO::SYNC_WITH_SERVER))
    // Return before dispatching servant.  We're also setting the high
    // bit here. This is a temporary fix until the rest of GIOP 1.2 is
    // implemented in TAO.
    out_stream << CORBA::Any::from_octet (129);

  else if (response_flags == CORBA::Octet (TAO::SYNC_WITH_TARGET))
    // Return after dispatching servant.
    out_stream << CORBA::Any::from_octet (3);

  else
    // Until more flags are defined by the OMG.
    return 0;

  // In this case we cannot recognise anything other than the Object
  // key as the address disposition variable. But we do a sanity check
  // anyway.
  const TAO_ObjectKey *key = spec.object_key ();
  
  if (key)
    {
      // Put in the object key
      out_stream << *key;
    }
    else
    {
      if (TAO_debug_level)
        ACE_DEBUG ((LM_DEBUG,
                    ASYS_TEXT ("(%N |%l) Unable to handle this request \n")));
      return 0;
    }
  
  out_stream << opname;

  return 1;
}                        


CORBA::Boolean
TAO_GIOP_Message_Lite::
  write_locate_request_header (CORBA::ULong request_id,
                               TAO_Target_Specification &spec,
                               TAO_OutputCDR &msg)
{
  msg << request_id;

  // In this case we cannot recognise anything other than the Object
  // key as the address disposition variable. But we do a sanity check
  // anyway.
  const TAO_ObjectKey *key = spec.object_key ();
  if (key)
    {
      // Put in the object key
      msg << *key;
    }
    else
    {
      if (TAO_debug_level)
        ACE_DEBUG ((LM_DEBUG,
                    ASYS_TEXT ("(%N |%l) Unable to handle this request \n")));
      return 0;
    }


  return 1;
}


int
TAO_GIOP_Message_Lite::parse_header (TAO_GIOP_Message_State *state)
{
  // Get the read pointer
  char *buf = state->cdr.rd_ptr ();
  
  state->byte_order = TAO_ENCAP_BYTE_ORDER;
  state->giop_version.major = TAO_DEF_GIOP_MAJOR;
  state->giop_version.minor = TAO_DEF_GIOP_MINOR;
  state->message_type = buf[TAO_GIOP_LITE_MESSAGE_TYPE_OFFSET];
  
  state->cdr.reset_byte_order (state->byte_order);
  state->cdr.read_ulong (state->message_size);

  return 0;
}

int
TAO_GIOP_Message_Lite::
  process_connector_request (TAO_Transport *transport, 
                             TAO_ORB_Core* orb_core,
                             TAO_InputCDR &input,
                             TAO_OutputCDR &output)
{
   // Get the revision info
  TAO_GIOP_Version version (TAO_DEF_GIOP_MAJOR,
                            TAO_DEF_GIOP_MINOR);
                           
  // This will extract the request header, set <response_required>
  // and <sync_with_server> as appropriate.
  TAO_GIOP_ServerRequest request (this,
                                  input,
                                  output,
                                  orb_core,
                                  version);
  
  CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ();
  
  CORBA::ULong request_id = 0;
  CORBA::Boolean response_required = 0;
  CORBA::Boolean sync_with_server = 0;
  CORBA::Boolean location_forward = 0;
  int parse_error = 0;
  
  ACE_TRY
    {
      parse_error = 
        this->parse_request_header (request);

      // Throw an exception if the 
      if (parse_error != 0)
        ACE_TRY_THROW (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE,
                                       CORBA::COMPLETED_NO));
      request_id = request.request_id ();

      response_required = request.response_expected ();
      sync_with_server = request.sync_with_server ();
      
#if !defined (TAO_NO_IOR_TABLE)
      const CORBA::Octet *object_key =
        request.object_key ().get_buffer ();
      
      if (ACE_OS::memcmp (object_key,
                          &TAO_POA::objectkey_prefix[0],
                          TAO_POA::TAO_OBJECTKEY_PREFIX_SIZE) != 0)
        {
          ACE_CString object_id (ACE_reinterpret_cast (const char *,
                                                       object_key),
                                 request.object_key ().length (),
                                 0,
                                 0);
          
          // @@ This debugging output should *NOT* be used since the
          //    object key string is not null terminated, nor can it
          //    be null terminated without copying.  No copying should
          //    be done since performance is somewhat important here.
          //    So, just remove the debugging output entirely.
          //
          //           if (TAO_debug_level > 0)
          //             ACE_DEBUG ((LM_DEBUG,
          //                         "Simple Object key %s. "
          //                         "Doing the Table Lookup ...\n",
          //                         object_id.c_str ()));

          CORBA::Object_ptr object_reference =
            CORBA::Object::_nil ();

          // Do the Table Lookup.
          int status =
            orb_core->orb ()->_tao_find_in_IOR_table (object_id,
                                                      object_reference);

          // If ObjectID not in table or reference is nil raise
          // OBJECT_NOT_EXIST.
          
          if (status == -1 || CORBA::is_nil (object_reference))
            ACE_TRY_THROW (CORBA::OBJECT_NOT_EXIST ());
          
          // ObjectID present in the table with an associated NON-NULL
          // reference.  Throw a forward request exception.

          CORBA::Object_ptr dup =
            CORBA::Object::_duplicate (object_reference);
          
          // @@ We could simply write the response at this point...
          ACE_TRY_THROW (PortableServer::ForwardRequest (dup));
        }
#endif /* TAO_NO_IOR_TABLE */
  
      // Do this before the reply is sent.
      orb_core->object_adapter ()->dispatch_servant (
                                                     request.object_key (),
                                                     request,
                                                     transport,
                                                     0,
                                                     ACE_TRY_ENV
                                                     );
      ACE_TRY_CHECK;
    }
#if !defined (TAO_HAS_MINIMUM_CORBA)
  ACE_CATCH (PortableServer::ForwardRequest, forward_request)
    {
      // Make the GIOP header and Reply header
      this->make_reply (request_id,
                        output);
      
      output.write_ulong (TAO_GIOP_LOCATION_FORWARD);

      CORBA::Object_ptr object_ptr =
        forward_request.forward_reference.in();

      output << object_ptr;

      // Flag for code below catch blocks.
      location_forward = 1;
    }
#else
  ACE_UNUSED_ARG (request_id);
#endif /* TAO_HAS_MINIMUM_CORBA */
  // Only CORBA exceptions are caught here.
   ACE_CATCHANY
    {
      int result = 0;
      if (response_required)
        {
          result = this->send_reply_exception (transport,
                                               orb_core,
                                               request_id,
                                               &ACE_ANY_EXCEPTION);
          if (result == -1)
            {
              if (TAO_debug_level > 0)
                ACE_ERROR ((LM_ERROR,
                            ASYS_TEXT ("TAO: (%P|%t|%N|%l) %p: cannot send exception\n"),
                            ASYS_TEXT ("process_connector_request ()")));
              ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                                   "TAO: ");
            }

        }
      else if (TAO_debug_level > 0)
        {
          // It is unfotunate that an exception (probably a system
          // exception) was thrown by the upcall code (even by the
          // user) when the client was not expecting a response.
          // However, in this case, we cannot close the connection
          // down, since it really isn't the client's fault.

          ACE_ERROR ((LM_ERROR,
                      ASYS_TEXT ("(%P|%t) exception thrown ")
                      ASYS_TEXT ("but client is not waiting a response\n")));
          ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                               "TAO: ");
        }

      return result;
    }
#if defined (TAO_HAS_EXCEPTIONS)
  ACE_CATCHALL
    {
      // @@ TODO some c++ exception or another, but what do we do with
      //    it?
      // We are supposed to map it into a CORBA::UNKNOWN exception.
      // BTW, this cannot be detected if using the <env> mapping.  If
      // we have native exceptions but no support for them in the ORB
      // we should still be able to catch it.  If we don't have native
      // exceptions it couldn't have been raised in the first place!
      int result = 0;
      if (response_required)
        {
          CORBA::UNKNOWN exception
            (CORBA::SystemException::_tao_minor_code
             (TAO_UNHANDLED_SERVER_CXX_EXCEPTION, 0),
             CORBA::COMPLETED_MAYBE);

          result = this->send_reply_exception (transport,
                                               orb_core,
                                               request_id,
                                               &exception);
          if (result == -1)
            {
              if (TAO_debug_level > 0)
                ACE_ERROR ((LM_ERROR,
                            ASYS_TEXT ("TAO: (%P|%t|%N|%l) %p: cannot send exception\n"),
                            ASYS_TEXT ("process_connector_request ()")));
              ACE_PRINT_EXCEPTION (exception, "TAO: ");
            }
        }
      else if (TAO_debug_level > 0)
        {
          // It is unfotunate that an exception (probably a system
          // exception) was thrown by the upcall code (even by the
          // user) when the client was not expecting a response.
          // However, in this case, we cannot close the connection
          // down, since it really isn't the client's fault.
          ACE_ERROR ((LM_ERROR,
                      ASYS_TEXT ("(%P|%t|%N|%l) exception thrown ")
                      ASYS_TEXT ("but client is not waiting a response\n")));
        }

      return result;
    }
#endif /* TAO_HAS_EXCEPTIONS */
  ACE_ENDTRY;

  int result = 0;

  // Do we have a twoway request, a oneway SYNC_WITH_TARGET,
  // or a oneway SYNC_WITH_SERVER with a location forward reply?
  if ((response_required && !sync_with_server)
      || (sync_with_server && location_forward))
    {
      result = this->send_message (transport,
                                   output);
                                   
      if (result == -1)
        {
          if (TAO_debug_level > 0)
            {
              // No exception but some kind of error, yet a response
              // is required.
              ACE_ERROR ((LM_ERROR,
                          ASYS_TEXT ("TAO: (%P|%t|%N|%l) %p: cannot send reply\n"),
                          ASYS_TEXT ("TAO_GIOP::process_server_message")));
            }
        }
    }

  return result;   
   
}

int
TAO_GIOP_Message_Lite::
  process_connector_locate (TAO_Transport *transport,
                            TAO_ORB_Core* orb_core,
                            TAO_InputCDR &input,
                            TAO_OutputCDR &output)
{
  // Get the revision info
  TAO_GIOP_Version version (TAO_DEF_GIOP_MAJOR,
                            TAO_DEF_GIOP_MINOR);
  
  // This will extract the request header, set <response_required> as
  // appropriate.
  TAO_GIOP_Locate_Request_Header locate_request (input);

  TAO_GIOP_Locate_Status_Msg status_info;

  // Defaulting
  status_info.status = TAO_GIOP_UNKNOWN_OBJECT;

  CORBA::Boolean response_required = 1;

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      int parse_error = 
        this->parse_locate_header (locate_request);
      if (parse_error != 0)
        ACE_TRY_THROW (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE,
                                       CORBA::COMPLETED_NO));
#if !defined (TAO_NO_IOR_TABLE)

      const CORBA::Octet *object_key =
        locate_request.target_address ().object_key ().get_buffer ();
      
      if (ACE_OS::memcmp (object_key,
                          &TAO_POA::objectkey_prefix[0],
                          TAO_POA::TAO_OBJECTKEY_PREFIX_SIZE) != 0)
        {
          CORBA::ULong len = 
            locate_request.target_address ().object_key ().length ();
          
          ACE_CString object_id (ACE_reinterpret_cast (const char *,
                                                       object_key),
                                 len,
                                 0,
                                 0);

          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,
                        ASYS_TEXT ("Simple Object key %s. Doing the Table Lookup ...\n"), 
                        object_id.c_str ()));

          CORBA::Object_ptr object_reference;

          // Do the Table Lookup.
          int find_status =
            orb_core->orb ()->_tao_find_in_IOR_table (object_id,
                                                      object_reference);

          // If ObjectID not in table or reference is nil raise
          // OBJECT_NOT_EXIST.

          if (CORBA::is_nil (object_reference)
              || find_status == -1)
            ACE_TRY_THROW (CORBA::OBJECT_NOT_EXIST ());

          // ObjectID present in the table with an associated NON-NULL
          // reference.  Throw a forward request exception.

          CORBA::Object_ptr dup =
            CORBA::Object::_duplicate (object_reference);

          // @@ We could simply write the response at this point...
          ACE_TRY_THROW (PortableServer::ForwardRequest (dup));
        }
#endif /* TAO_NO_IOR_TABLE */

      // Execute a fake request to find out if the object is there or
      // if the POA can activate it on demand...
      char repbuf[ACE_CDR::DEFAULT_BUFSIZE];
      TAO_OutputCDR dummy_output (repbuf,
                                  sizeof repbuf);
      // This output CDR is not used!

      // This could be tricky if the target_address does not have the
      // object key. Till then .. Bala
      TAO_ObjectKey tmp_key (locate_request.target_address ().object_key ().length (),
                             locate_request.target_address ().object_key ().length (),
                             locate_request.target_address ().object_key ().get_buffer (),
                             0);

      // Set it to an error state
      parse_error = 1;
      CORBA::ULong req_id = locate_request.request_id ();
      
      TAO_GIOP_ServerRequest server_request (this,
                                             req_id,
                                             response_required,
                                             tmp_key,
                                             "_non_existent",
                                             dummy_output,
                                             orb_core,
                                             version,
                                             parse_error);
      if (parse_error != 0)
        ACE_TRY_THROW (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE,
                                       CORBA::COMPLETED_NO));
      
      orb_core->object_adapter ()->dispatch_servant
        (server_request.object_key (), 
         server_request,
         transport,
         0,
         ACE_TRY_ENV);
      
      ACE_TRY_CHECK;

      if (server_request.exception_type () == TAO_GIOP_NO_EXCEPTION)
        {
          // We got no exception, so the object is here.
          status_info.status = TAO_GIOP_OBJECT_HERE;
          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,
                        ASYS_TEXT ("TAO: (%P|%t) handle_locate() : found\n")));
        }
      else if (server_request.exception_type () != TAO_GIOP_NO_EXCEPTION)
        {
          status_info.forward_location_var = server_request.forward_location ();
          if (!CORBA::is_nil (status_info.forward_location_var.in ()))
            {
              status_info.status = TAO_GIOP_OBJECT_FORWARD;
              ACE_DEBUG ((LM_DEBUG,
                          ASYS_TEXT ("handle_locate has been called: forwarding\n")));
            }
          else
            {
              // Normal exception, so the object is not here
              status_info.status = TAO_GIOP_UNKNOWN_OBJECT;
              ACE_DEBUG ((LM_DEBUG,
                          ASYS_TEXT ("handle_locate has been called: not here\n")));
            }
        }

    }

#if !defined (TAO_HAS_MINIMUM_CORBA)
  ACE_CATCH (PortableServer::ForwardRequest, forward_request)
    {
      status_info.status = TAO_GIOP_OBJECT_FORWARD;
      status_info.forward_location_var =
        forward_request.forward_reference;
      ACE_DEBUG ((LM_DEBUG,
                  ASYS_TEXT ("handle_locate has been called: forwarding\n")));
    }
#endif /* TAO_HAS_MINIMUM_CORBA */
  ACE_CATCHANY
    {
      // Normal exception, so the object is not here
      status_info.status = TAO_GIOP_UNKNOWN_OBJECT;
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    ASYS_TEXT ("TAO (%P|%t) TAO_GIOP::process_server_locate - ")
                    ASYS_TEXT ("CORBA exception raised\n")));
    }
#if defined (TAO_HAS_EXCEPTIONS)
  ACE_CATCHALL
    {
      // Normal exception, so the object is not here
      status_info.status = TAO_GIOP_UNKNOWN_OBJECT;
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    ASYS_TEXT ("TAO (%P|%t) TAO_GIOP::process_server_locate - ")
                    ASYS_TEXT ("C++ exception raised\n")));
    }
#endif /* TAO_HAS_EXCEPTIONS */
  ACE_ENDTRY;


  return this->make_locate_reply (transport,
                                  output,
                                  locate_request,
                                  status_info);
}




int
TAO_GIOP_Message_Lite::
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
  return hdr_status ? 0 : -1;

}

int 
TAO_GIOP_Message_Lite::
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
TAO_GIOP_Message_Lite::
  make_reply (CORBA::ULong request_id,
              TAO_OutputCDR &output)
{
  // Write the GIOP header first
  this->write_protocol_header (TAO_PLUGGABLE_MESSAGE_REPLY,
                               output);
  
  // create and write a dummy context
  IOP::ServiceContextList resp_ctx;
  resp_ctx.length (0);
  output << resp_ctx;

  // Write the request ID
  output.write_ulong (request_id);
  
  return 0;
}

int
TAO_GIOP_Message_Lite::
  send_reply_exception (TAO_Transport *transport,
                        TAO_ORB_Core* orb_core,
                        CORBA::ULong request_id,
                        CORBA::Exception *x)
{
  // Create a new output CDR stream
  char repbuf[ACE_CDR::DEFAULT_BUFSIZE];

#if defined(ACE_HAS_PURIFY)
  (void) ACE_OS::memset (repbuf,
                         '\0',
                         sizeof repbuf);
#endif /* ACE_HAS_PURIFY */
  TAO_OutputCDR output (repbuf,
                        sizeof repbuf,
                        TAO_ENCAP_BYTE_ORDER,
                        orb_core->output_cdr_buffer_allocator (),
                        orb_core->output_cdr_dblock_allocator (),
                        orb_core->orb_params ()->cdr_memcpy_tradeoff (),
                        orb_core->to_iso8859 (),
                        orb_core->to_unicode ());

  // Make the GIOP & reply header. They are version specific.
  this->make_reply (request_id,
                    output);
  
  // A new try/catch block, but if something goes wrong now we have no
  // hope, just abort.
  ACE_DECLARE_NEW_CORBA_ENV;
  
  ACE_TRY
    {
      // Write the exception
      CORBA::TypeCode_ptr except_tc = x->_type ();

      CORBA::exception_type extype =
        CORBA::USER_EXCEPTION;

      if (CORBA::SystemException::_narrow (x) != 0)
        extype = CORBA::SYSTEM_EXCEPTION;

      // write the reply_status
      output.write_ulong
        (TAO_GIOP_Utils::convert_CORBA_to_GIOP_exception (extype));  

      // @@ Any way to implement this without interpretive
      //    marshaling???
      output.encode (except_tc,
                     x,
                     0,
                     ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (CORBA_Exception, ex)
    {
      // Now we know that while handling the error an other error
      // happened -> no hope, close connection.

      // Close the handle.
      ACE_DEBUG ((LM_DEBUG,
                  ASYS_TEXT ("(%P|%t|%N|%l) cannot marshal exception, handle = %d: %p\n"),
                  transport->handle (),
                  ASYS_TEXT ("send_reply_exception ()")));
      return -1;
    }
  ACE_ENDTRY;

  return this->send_message (transport,
                             output);
}


int
TAO_GIOP_Message_Lite::
  make_locate_reply (TAO_Transport *transport,
                     TAO_OutputCDR & output,
                     TAO_GIOP_Locate_Request_Header &request,
                     TAO_GIOP_Locate_Status_Msg &status_info)
{
  // Note here we are making the Locate reply header which is *QUITE*
  // different from the reply header made by the make_reply () call..

  // Make the GIOP message header
  this->write_protocol_header (TAO_PLUGGABLE_MESSAGE_LOCATEREPLY,
                               output);

  // Make the header for the locate request
  output.write_ulong (request.request_id ());
  output.write_ulong (status_info.status);
  
  if (status_info.status == TAO_GIOP_OBJECT_FORWARD)
    {
      CORBA::Object_ptr object_ptr = status_info.forward_location_var.in ();
      if ((output << object_ptr) == 0)
        {
          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,
                        ASYS_TEXT ("TAO (%P|%t|%N|%l) make_locate_reply-")
                        ASYS_TEXT ("cannot marshal object reference\n")));
        }
    }



  // Send the message
  int result = this->send_message (transport,
                                   output);

  // Print out message if there is an error
  if (result == -1)
    {
      if (TAO_debug_level > 0)
        {
          ACE_ERROR ((LM_ERROR,
                      ASYS_TEXT ("TAO: (%P|%t) %p: cannot send reply\n"),
                      ASYS_TEXT ("TAO_GIOP::process_server_message")));
        }
    }

  return result;
}

// Send an "I can't understand you" message -- again, the message is
// prefabricated for simplicity.  This implies abortive disconnect (at
// the application level, if not at the level of TCP).
//
// NOTE that IIOPLite will still benefit from TCP's orderly disconnect.
int
TAO_GIOP_Message_Lite::send_error (TAO_Transport *transport)
{
  const char
    error_message [TAO_GIOP_LITE_HEADER_LEN] =
  {
    // The following works on non-ASCII platforms, such as MVS (which
    // uses EBCDIC).
    TAO_GIOP_MESSAGERROR,
    0, 0, 0, 0
  };
  
  this->dump_msg ("send_error",
                  (const u_char *) error_message,
                  TAO_GIOP_LITE_HEADER_LEN);
 
  ACE_HANDLE which = transport->handle ();

  int result = transport->send ((const u_char *)error_message,
                                TAO_GIOP_LITE_HEADER_LEN);
  if (result == -1)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    ASYS_TEXT ("TAO (%N|%l|%P|%t) error sending error to %d\n"),
                    which));
    }
 
  return result;
}

void
TAO_GIOP_Message_Lite::dump_msg (const char *label,
                                 const u_char *ptr,
                                 size_t len)
{
  static const char *names [] =
  {
    "Request",
    "Reply",
    "CancelRequest",
    "LocateRequest",
    "LocateReply",
    "CloseConnection",
    "MessageError"
    "Fragment"
  };
  
  if (TAO_debug_level >= 5)
    {
      // Message name.
      const char *message_name = "UNKNOWN MESSAGE";
      u_long slot = ptr[TAO_GIOP_LITE_MESSAGE_TYPE_OFFSET];
      if (slot < sizeof (names)/sizeof(names[0]))
        message_name = names [slot];
      
      ACE_DEBUG ((LM_DEBUG,
                  "(%N|%l) \n"));
      // Byte order.
      int byte_order = TAO_ENCAP_BYTE_ORDER;

      // request/reply id.
      CORBA::ULong tmp = 0;
      CORBA::ULong *id = &tmp;
      
      if (ptr[TAO_GIOP_LITE_MESSAGE_TYPE_OFFSET] == TAO_GIOP_REQUEST ||
          ptr[TAO_GIOP_LITE_MESSAGE_TYPE_OFFSET] == TAO_GIOP_REPLY)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "(%N|%l) \n"));
          // @@ Only works if ServiceContextList is empty....
          id = ACE_reinterpret_cast (CORBA::ULong *,
                                     (char * ) (ptr));
                               
          ACE_DEBUG ((LM_DEBUG,
                      "(%N|%l) \n"));
        }

      // Print.
      ACE_DEBUG ((LM_DEBUG,
                  ASYS_TEXT ("(%P | %t):%s GIOP lite msg, ")
                  ASYS_TEXT ("%d data bytes, %s endian, <%s = %d> \n"),
                  label,
                  len - TAO_GIOP_LITE_HEADER_LEN,
                  (byte_order == TAO_ENCAP_BYTE_ORDER) ? "my" : "other",
                  message_name,
                  *id));
    }
}


