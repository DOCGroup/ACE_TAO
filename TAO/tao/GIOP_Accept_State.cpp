//$Id$
#include "tao/debug.h"
#include "tao/GIOP_Accept_State.h"
#include "tao/GIOP_Server_Request.h"
#include "tao/POA.h"
#include "tao/ORB_Core.h"
//#include "tao/GIOP_Assorted_Headers.h"

#if !defined (__ACE_INLINE__)
# include "tao/GIOP_Accept_State.i"
#endif /* __ACE_INLINE__ */

int
TAO_GIOP_Accept_State::parse_header (TAO_GIOP_Message_State *state)
{
  if (TAO_GIOP_Utils::parse_giop_header (state,
                                         state->cdr) == -1)
    {
      if (TAO_debug_level > 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N|%l) Error in parsing headers \n"),
                            -1);
        }
      return -1;
    }
     
  if (state->cdr.grow (this->header_length () +
                       state->message_size) == -1)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t) - %p\n",
                    "TAO_GIOP::handle_input, ACE_CDR::grow"));
      return -1;
    }

  // Growing the buffer may have reset the rd_ptr(), but we want
  // to leave it just after the GIOP header (that was parsed
  // already);
  state->cdr.skip_bytes (this->header_length ());
  
  return 1;
}


// Server sends an "I'm shutting down now, any requests you've sent me
// can be retried" message to the server.  The message is prefab, for
// simplicity.
//
// NOTE: this is IIOP-specific though it doesn't look like it is.  It
// relies on a TCP-ism: orderly disconnect, which doesn't exist in all
// transport protocols.  Versions of GIOP atop some transport that's
// lacking orderly disconnect must define some transport-specific
// handshaking (e.g. the XNS/SPP handshake convention) in order to
// know that the same transport semantics are provided when shutdown
// is begun with messages "in flight". (IIOP doesn't report false
// errors in the case of "clean shutdown", because it relies on
// orderly disconnect as provided by TCP.  This quality of service is
// required to write robust distributed systems.)

void
TAO_GIOP_Accept_State::send_close_connection (const TAO_GIOP_Version& version,
                                              TAO_Transport *transport,
                                              void *)
{
  
  // static CORBA::Octet
  // I hate  this in every method. Till the time I figure out a way
  // around  I will have them here hanging around. 
  const char close_message [TAO_GIOP_HEADER_LEN] =
  {
    // The following works on non-ASCII platforms, such as MVS (which
    // uses EBCDIC).
    0x47, // 'G'
    0x49, // 'I'
    0x4f, // 'O'
    0x50, // 'P'
    version.major,
    version.minor,
    TAO_ENCAP_BYTE_ORDER,
    TAO_GIOP_CLOSECONNECTION,
    0, 0, 0, 0
  };

  // It's important that we use a reliable shutdown after we send this
  // message, so we know it's received.
  //
  // @@ should recv and discard queued data for portability; note
  // that this won't block (long) since we never set SO_LINGER

  TAO_GIOP_Utils::dump_msg ("send_close_connection",
                            (const u_char *) close_message,
                            TAO_GIOP_HEADER_LEN);
  
  ACE_HANDLE which = transport->handle ();
  if (which == ACE_INVALID_HANDLE)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t) TAO_GIOP::send_close_connection -"
                    " connection already closed\n"));
      return;
    }

  if (transport->send ((const u_char *) close_message,
                       TAO_GIOP_HEADER_LEN) == -1)
    {
      if (TAO_orbdebug)
        ACE_ERROR ((LM_ERROR,
                    "(%P|%t) error closing connection %d\n",
                    which));
    }

  transport->close_connection ();
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) shut down transport, handle %d\n",
              which));

}


// Send an "I can't understand you" message -- again, the message is
// prefabricated for simplicity.  This implies abortive disconnect (at
// the application level, if not at the level of TCP).
//
// NOTE that IIOP will still benefit from TCP's orderly disconnect.

int
TAO_GIOP_Accept_State::send_error (TAO_Transport *transport)
{
  TAO_GIOP_Version version (this->major_version (),
                            this->minor_version ());
  const char
    error_message [TAO_GIOP_HEADER_LEN] =
  {
    // The following works on non-ASCII platforms, such as MVS (which
    // uses EBCDIC).
    0x47, // 'G'
    0x49, // 'I'
    0x4f, // 'O'
    0x50, // 'P'
    version.major,
    version.minor,
    TAO_ENCAP_BYTE_ORDER,
    TAO_GIOP_MESSAGERROR,
    0, 0, 0, 0
  };

  // @@ Q: How does this works with GIOP lite?
  //    A: It doesn't

  TAO_GIOP_Utils::dump_msg ("send_error",
                            (const u_char *) error_message,
                            TAO_GIOP_HEADER_LEN);

  ACE_HANDLE which = transport->handle ();

  int result = transport->send ((const u_char *)error_message,
                                TAO_GIOP_HEADER_LEN);
  if (result == -1)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO (%N|%l|%P|%t) error sending error to %d\n",
                    which));
    }
  
  return result;
}

int
TAO_GIOP_Accept_State::process_connector_request (TAO_Transport *transport,
                                                  TAO_ORB_Core* orb_core,
                                                  TAO_InputCDR &input,
                                                  TAO_OutputCDR &output)
{
  // Get the revision info
  TAO_GIOP_Version version (this->major_version (),
                            this->minor_version ());

  // This will extract the request header, set <response_required>
  // and <sync_with_server> as appropriate.
  TAO_GIOP_ServerRequest request (input,
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
      parse_error = this->parse_request_header (request);
     
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
      this->make_reply (version,
                        TAO_GIOP_REPLY,
                        request_id,
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
          result = this->send_reply_exception (version,
                                               transport,
                                               orb_core,
                                               request_id,
                                               &ACE_ANY_EXCEPTION);
          if (result == -1)
            {
              if (TAO_debug_level > 0)
                ACE_ERROR ((LM_ERROR,
                            "TAO: (%P|%t) %p: cannot send exception\n",
                            "TAO_GIOP::process_server_message"));
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
                      "(%P|%t) exception thrown "
                      "but client is not waiting a response\n"));
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

          result = this->send_reply_exception (version,
                                               transport,
                                               orb_core,
                                               request_id,
                                               &exception);
          if (result == -1)
            {
              if (TAO_debug_level > 0)
                ACE_ERROR ((LM_ERROR,
                            "TAO: (%P|%t) %p: cannot send exception\n",
                            "TAO_GIOP::process_server_message"));
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
                      "(%P|%t) exception thrown "
                      "but client is not waiting a response\n"));
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
      result = TAO_GIOP_Utils::send_message (transport,
                                             output,
                                             this->header_length (),
                                             this->offset_length ());
      if (result == -1)
        {
          if (TAO_debug_level > 0)
            {
              // No exception but some kind of error, yet a response
              // is required.
              ACE_ERROR ((LM_ERROR,
                          "TAO: (%P|%t) %p: cannot send reply\n",
                          "TAO_GIOP::process_server_message"));
            }
        }
    }

  return result;
}

int
TAO_GIOP_Accept_State::process_connector_locate (TAO_Transport *transport,
                                                 TAO_ORB_Core* orb_core,
                                                 TAO_InputCDR &input,
                                                 TAO_OutputCDR &output)
{
    // Get the revision info
  TAO_GIOP_Version version (this->major_version (),
                            this->minor_version ());
  
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
      int parse_error = this->parse_locate_header (locate_request);
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
                        "Simple Object key %s. Doing the Table Lookup ...\n",
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
      
      TAO_GIOP_ServerRequest server_request (req_id,
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
                        "TAO: (%P|%t) handle_locate() : found\n"));
        }
      else if (server_request.exception_type () != TAO_GIOP_NO_EXCEPTION)
        {
          status_info.forward_location_var = server_request.forward_location ();
          if (!CORBA::is_nil (status_info.forward_location_var.in ()))
            {
              status_info.status = TAO_GIOP_OBJECT_FORWARD;
              ACE_DEBUG ((LM_DEBUG,
                          "handle_locate has been called: forwarding\n"));
            }
          else
            {
              // Normal exception, so the object is not here
              status_info.status = TAO_GIOP_UNKNOWN_OBJECT;
              ACE_DEBUG ((LM_DEBUG,
                          "handle_locate has been called: not here\n"));
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
                  "handle_locate has been called: forwarding\n"));
    }
#endif /* TAO_HAS_MINIMUM_CORBA */
  ACE_CATCHANY
    {
      // Normal exception, so the object is not here
      status_info.status = TAO_GIOP_UNKNOWN_OBJECT;
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t) TAO_GIOP::process_server_locate - "
                    "CORBA exception raised\n"));
    }
#if defined (TAO_HAS_EXCEPTIONS)
  ACE_CATCHALL
    {
      // Normal exception, so the object is not here
      status_info.status = TAO_GIOP_UNKNOWN_OBJECT;
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t) TAO_GIOP::process_server_locate - "
                    "C++ exception raised\n"));
    }
#endif /* TAO_HAS_EXCEPTIONS */
  ACE_ENDTRY;


  return this->make_locate_reply (transport,
                                  version,
                                  output,
                                  locate_request,
                                  status_info);
                                  
}


int
TAO_GIOP_Accept_State::make_locate_reply (TAO_Transport *transport,
                                          const TAO_GIOP_Version &version,
                                          TAO_OutputCDR &output,
                                          TAO_GIOP_Locate_Request_Header &request,
                                          TAO_GIOP_Locate_Status_Msg &status_info)
  
                                         
{
  // Note here we are making the Locate reply header which is *QUITE*
  // different from the reply header made by the make_reply () cal.. 
  
  // Make the GIOP message header
  TAO_GIOP_Utils::start_message (version,
                                 TAO_GIOP_LOCATEREPLY,
                                 output);             
  
  // Make the header for the locate request 
  output.write_ulong (request.request_id ());
  output.write_ulong (status_info.status);

  // @@Does GIOP1.2 need a padding .. Not sure..??
  if (status_info.status == TAO_GIOP_OBJECT_FORWARD)
    {
      CORBA::Object_ptr object_ptr = status_info.forward_location_var.in ();
      if ((output << object_ptr) == 0)
        {
          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,
                        "TAO (%P|%t) TAO_GIOP::process_server_request -"
                        " cannot marshal object reference\n"));
        } 
    }

  // Get the header & offset length
  size_t header_len = this->header_length ();
  size_t offset_len = this->offset_length ();

  // Send the message
  int result = TAO_GIOP_Utils::send_message (transport,
                                             output,
                                             header_len,
                                             offset_len);
  
  // Print out message if there is an error
  if (result == -1)
    {
      if (TAO_debug_level > 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "TAO: (%P|%t) %p: cannot send reply\n",
                      "TAO_GIOP::process_server_message"));
        }
    }   
  
  return result;
}


int
TAO_GIOP_Accept_State::send_reply_exception (const TAO_GIOP_Version &version,
                                             TAO_Transport *transport,
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
  this->make_reply (version,
                    TAO_GIOP_REPLY,
                    request_id,
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
        (TAO_GIOP_Accept_State::convert_CORBA_to_GIOP_exception (extype)); 

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
                  "(%P|%t) cannot marshal exception, handle = %d: %p\n",
                  transport->handle (),
                  "TAO_GIOP::send_reply_exception"));
      return -1;
    }
  ACE_ENDTRY;

    // Get the header & offset length
  size_t header_len = this->header_length ();
  size_t offset_len = this->offset_length ();
  
  return TAO_GIOP_Utils::send_message (transport,
                                       output,
                                       header_len,
                                       offset_len);
}


TAO_GIOP_Reply_Status_Type
TAO_GIOP_Accept_State::convert_CORBA_to_GIOP_exception (
                                                        CORBA::exception_type corba_type)  
{
  switch (corba_type)
    {
    case CORBA::NO_EXCEPTION:
      return TAO_GIOP_NO_EXCEPTION;

    case CORBA::SYSTEM_EXCEPTION:
      return TAO_GIOP_SYSTEM_EXCEPTION;

    case CORBA::USER_EXCEPTION:
      return TAO_GIOP_USER_EXCEPTION;

    default:
      // Don't know what to do here??
      return TAO_GIOP_SYSTEM_EXCEPTION;
    }
}
