//$Id$
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
TAO_GIOP::send_close_connection (const TAO_GIOP_Version& version,
                                 TAO_Transport *transport,
                                 void *)
{
  // static CORBA::Octet
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
    TAO_GIOP::CloseConnection,
    0, 0, 0, 0
  };

  // It's important that we use a reliable shutdown after we send this
  // message, so we know it's received.
  //
  // @@ should recv and discard queued data for portability; note
  // that this won't block (long) since we never set SO_LINGER

  TAO_GIOP::dump_msg ("send_close_connection",
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
TAO_GIOP::send_error (const TAO_GIOP_Version &version,
                      TAO_Transport *transport)
{
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
    TAO_GIOP::MessageError,
    0, 0, 0, 0
  };

  // @@ Q: How does this works with GIOP lite?
  //    A: It doesn't

  TAO_GIOP::dump_msg ("send_error",
                      (const u_char *) error_message,
                      TAO_GIOP_HEADER_LEN);

  ACE_HANDLE which = transport->handle ();

  int result = transport->send ((const u_char *)error_message,
                                TAO_GIOP_HEADER_LEN);
  if (result == -1)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t) error sending error to %d\n",
                    which));
    }

  return result;
}

ssize_t
TAO_GIOP::read_buffer (TAO_Transport *transport,
                       char *buf,
                       size_t len,
                       ACE_Time_Value *max_wait_time)
{
  ACE_FUNCTION_TIMEPROBE (TAO_GIOP_READ_BUFFER_START);

  ssize_t bytes_read = transport->recv (buf, len, max_wait_time);

  if (bytes_read <= 0 && TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "TAO (%P|%t) - %p,\n"
                "              transport = %d, "
                "bytes = %d, len = %d\n",
                "TAO_GIOP::read_buffer",
                transport->handle (),
                bytes_read,
                len));

  if (bytes_read == -1 && errno == ECONNRESET)
    {
      // @@ Is this OK??

      // We got a connection reset (TCP RSET) from the other side,
      // i.e., they didn't initiate a proper shutdown.
      //
      // Make it look like things are OK to the upper layer.
      bytes_read = 0;
      errno = 0;
    }

  return bytes_read;
}

void
TAO_GIOP_LocateRequestHeader::init (TAO_InputCDR &msg,
                                    CORBA::Environment &ACE_TRY_ENV)
{
  ACE_FUNCTION_TIMEPROBE (TAO_GIOP_LOCATE_REQUEST_HEADER_INIT_START);

  if (msg.read_ulong (this->request_id) == 0
      || (msg >> this->object_key) == 0)
    ACE_THROW (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE,
                               CORBA::COMPLETED_NO));
}

TAO_GIOP_ReplyStatusType
TAO_GIOP::convert_CORBA_to_GIOP_exception (CORBA::exception_type corba_type)
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

int
TAO_GIOP::read_header (TAO_Transport *transport,
                       TAO_ORB_Core *orb_core,
                       TAO_GIOP_Message_State &state,
                       CORBA::ULong &header_size,
                       TAO_InputCDR &input,
                       ACE_Time_Value *max_wait_time)
{
  // Default header length.
  header_size = TAO_GIOP_HEADER_LEN;
  if (orb_core->orb_params ()->use_lite_protocol ())
    header_size = TAO_GIOP_LITE_HEADER_LEN;

  if (input.grow (header_size) == -1)
    return -1;

  // Read until all the header is received.  There should be no
  // problems with locking, the header is only a few bytes so they
  // should all be available on the socket, otherwise there is a
  // problem with the underlying transport, in which case we have more
  // problems than just this small loop.

  char *buf = input.rd_ptr ();
  ssize_t n;

  for (int t = header_size;
       t != 0;
       t -= n)
    {
      n = transport->recv (buf, t, max_wait_time);
      if (n == -1)
        return -1;
      else if (n == 0) // @@ TODO && errno != EWOULDBLOCK)
        return -1;
      buf += n;
    }

  if (TAO_GIOP::parse_header (orb_core,
                              input,
                              state) == -1)
    {
      TAO_GIOP::send_error (state.giop_version, transport);
      return -1;
    }
  return header_size;
}

int
TAO_GIOP::handle_input (TAO_Transport *transport,
                        TAO_ORB_Core *orb_core,
                        TAO_GIOP_Message_State &state,
                        ACE_Time_Value *max_wait_time)
{
  if (state.header_received () == 0)
    {
      CORBA::ULong header_size;
      if (TAO_GIOP::read_header (transport,
                                 orb_core,
                                 state,
                                 header_size,
                                 state.cdr,
                                 max_wait_time) == -1)
        {
          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,
                        "TAO (%P|%t) - %p\n",
                        "TAO_GIOP::handle_input, read_header"));
          return -1;
        }

      if (state.cdr.grow (header_size +
                          state.message_size) == -1)
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
      state.cdr.skip_bytes (header_size);
    }

  size_t missing_data =
    state.message_size - state.current_offset;
  ssize_t n =
    TAO_GIOP::read_buffer (transport,
                           state.cdr.rd_ptr () + state.current_offset,
                           missing_data,
                           max_wait_time);
  if (n == -1)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t) - %p\n",
                    "TAO_GIOP::handle_input, read_buffer[1]"));
      return -1;
    }
  else if (n == 0)
    {
      if (errno == EWOULDBLOCK)
        return 0;
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t) - %p\n",
                    "TAO_GIOP::handle_input, read_buffer[2]"));
      return -1;
    }

  state.current_offset += n;

  if (state.current_offset == state.message_size)
    {
      if (TAO_debug_level >= 5)
        {
          size_t header_len = TAO_GIOP_HEADER_LEN;
          if (orb_core->orb_params ()->use_lite_protocol ())
            header_len = TAO_GIOP_LITE_HEADER_LEN;

          char *buf = state.cdr.rd_ptr ();
          buf -= header_len;
          size_t msg_len = state.cdr.length () + header_len;
          TAO_GIOP::dump_msg ("recv",
                              ACE_reinterpret_cast (u_char *,
                                                    buf),
                              msg_len);
        }
    }

  return state.is_complete ();
}

int
TAO_GIOP::parse_reply (TAO_Transport *,
                       TAO_ORB_Core *,
                       TAO_GIOP_Message_State &state,
                       IOP::ServiceContextList &reply_ctx,
                       CORBA::ULong &request_id,
                       CORBA::ULong &reply_status)
{
  switch (state.message_type)
    {
    case TAO_GIOP::Request:
      // In GIOP 1.0 and GIOP 1.1 this is an error, but it is
      // *possible* to receive requests in GIOP 1.2.  Don't handle
      // this on the firt iteration, leave it for the nearby future...
      // ERROR too.  @@ this->reply_handler_->error ();
      ACE_ERROR_RETURN ((LM_ERROR,
                         "TAO (%P|%t) %N:%l TAO_GIOP::parse_reply: "
                         "request.\n"),
                        -1);

    case TAO_GIOP::CancelRequest:
    case TAO_GIOP::LocateRequest:
    case TAO_GIOP::CloseConnection:
    default:
      // @@ Errors for the time being.
      // @@ this->reply_handler_->error ();
      ACE_ERROR_RETURN ((LM_ERROR,
                         "TAO (%P|%t) %N:%l TAO_GIOP::parse_reply: "
                         "wrong message.\n"),
                        -1);

    case TAO_GIOP::LocateReply:
    case TAO_GIOP::Reply:
      // Handle after the switch.
      break;

    case TAO_GIOP::Fragment:
      // Never happens:
      break;
    }

  // For GIOP 1.0 and 1.1 the reply_ctx comes first:
  // @@ Use <header.giop_version> to make this work with GIOP 1.2
  if (state.message_type != TAO_GIOP::LocateReply)
    {
      if ((state.cdr >> reply_ctx) == 0)
        {
          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,
                        "TAO (%P|%t) TAO_GIOP::parse_reply, "
                        "extracting context\n"));
          return -1;
        }
    }

  // Read the request id
  if (!state.cdr.read_ulong (request_id))
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t) : TAO_GIOP::parse_reply, "
                    "extracting request id"));
      return -1;
    }

  // and the reply status type.  status can be NO_EXCEPTION,
  // SYSTEM_EXCEPTION, USER_EXCEPTION, LOCATION_FORWARD or (on GIOP
  // 1.2) LOCATION_FORWARD_PERM
  if (!state.cdr.read_ulong (reply_status))
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t) : TAO_GIOP::parse_reply, "
                    "extracting reply status\n"));
      return -1;
    }
  return 0;
}

int
TAO_GIOP::process_server_message (TAO_Transport *transport,
                                  TAO_ORB_Core *orb_core,
                                  TAO_InputCDR &input,
                                  CORBA::Octet message_type,
                                  const TAO_GIOP_Version &giop_version)
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

  TAO_MINIMAL_TIMEPROBE (TAO_SERVER_CONNECTION_HANDLER_RECEIVE_REQUEST_END);

  switch (message_type)
    {
    case TAO_GIOP::Request:
      // The following two routines will either raise an exception
      // or successfully write the response into <output>
      return TAO_GIOP::process_server_request (transport,
                                               orb_core,
                                               input,
                                               output,
                                               giop_version);

    case TAO_GIOP::LocateRequest:
      return TAO_GIOP::process_server_locate (transport,
                                              orb_core,
                                              input,
                                              output,
                                              giop_version);

    case TAO_GIOP::MessageError:
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t) MessageError received by server\n"));
      break;

      // These messages should never be sent to the server; it's an
      // error if the peer tries.  Set the environment accordingly,
      // as it's not yet been reported as an error.
    case TAO_GIOP::Reply:
    case TAO_GIOP::LocateReply:
    case TAO_GIOP::CloseConnection:
    default:   // Unknown message
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t) Illegal message received by server\n"));
      return TAO_GIOP::send_error (giop_version, transport);
    }

  TAO_MINIMAL_TIMEPROBE (TAO_SERVER_CONNECTION_HANDLER_HANDLE_INPUT_END);

  return 0;
}

int
TAO_GIOP::process_server_request (TAO_Transport *transport,
                                  TAO_ORB_Core *orb_core,
                                  TAO_InputCDR &input,
                                  TAO_OutputCDR &output,
                                  const TAO_GIOP_Version &version)
{
  CORBA::ULong request_id = 0;
  CORBA::Boolean response_required = 0;
  CORBA::Boolean sync_with_server = 0;
  CORBA::Boolean location_forward = 0;
  CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ();
  int parse_error;

  // This will extract the request header, set <response_required>
  // and <sync_with_server> as appropriate.

  TAO_GIOP_ServerRequest request (input,
                                  output,
                                  orb_core,
                                  version,
                                  parse_error);

  ACE_TRY
    {
      request_id = request.request_id ();
      response_required = request.response_expected ();
      sync_with_server = request.sync_with_server ();

      if (parse_error != 0)
        ACE_TRY_THROW (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE,
                                       CORBA::COMPLETED_NO));

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
      TAO_GIOP::start_message (version,
                               TAO_GIOP::Reply,
                               output,
                               orb_core);
      IOP::ServiceContextList resp_ctx;
      resp_ctx.length (0);
      output << resp_ctx;

      output.write_ulong (request_id);

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
          result = TAO_GIOP::send_reply_exception (version,
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

          result = TAO_GIOP::send_reply_exception (version,
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
      result = TAO_GIOP::send_message (transport,
                                       output,
                                       orb_core);
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
TAO_GIOP::process_server_locate (TAO_Transport *transport,
                                 TAO_ORB_Core* orb_core,
                                 TAO_InputCDR &input,
                                 TAO_OutputCDR &output,
                                 const TAO_GIOP_Version& version)
{
  // This will extract the request header, set <response_required> as
  // appropriate.
  TAO_GIOP_LocateRequestHeader locateRequestHeader;

  TAO_GIOP_LocateStatusType status = TAO_GIOP_UNKNOWN_OBJECT;
  CORBA::Object_var forward_location_var;

  CORBA::Boolean response_required = 1;

  CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ();
  ACE_TRY
    {
      locateRequestHeader.init (input, ACE_TRY_ENV);
      ACE_TRY_CHECK;

#if !defined (TAO_NO_IOR_TABLE)

      const CORBA::Octet *object_key =
        locateRequestHeader.object_key.get_buffer ();

      if (ACE_OS::memcmp (object_key,
                          &TAO_POA::objectkey_prefix[0],
                          TAO_POA::TAO_OBJECTKEY_PREFIX_SIZE) != 0)
        {
          ACE_CString object_id (ACE_reinterpret_cast (const char *,
                                                       object_key),
                                 locateRequestHeader.object_key.length (),
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

      TAO_ObjectKey tmp_key (locateRequestHeader.object_key.length (),
                             locateRequestHeader.object_key.length (),
                             locateRequestHeader.object_key.get_buffer (),
                             0);

      int parse_error;
      TAO_GIOP_ServerRequest serverRequest (locateRequestHeader.request_id,
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

      orb_core->object_adapter ()->dispatch_servant (serverRequest.object_key (),
                                                     serverRequest,
                                                     transport,
                                                     0,
                                                     ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (serverRequest.exception_type () == TAO_GIOP_NO_EXCEPTION)
        {
          // We got no exception, so the object is here.
          status = TAO_GIOP_OBJECT_HERE;
          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,
                        "TAO: (%P|%t) handle_locate() : found\n"));
        }
      else if (serverRequest.exception_type () != TAO_GIOP_NO_EXCEPTION)
        {
          forward_location_var = serverRequest.forward_location ();
          if (!CORBA::is_nil (forward_location_var.in ()))
            {
              status = TAO_GIOP_OBJECT_FORWARD;
              ACE_DEBUG ((LM_DEBUG,
                          "handle_locate has been called: forwarding\n"));
            }
          else
            {
              // Normal exception, so the object is not here
              status = TAO_GIOP_UNKNOWN_OBJECT;
              ACE_DEBUG ((LM_DEBUG,
                          "handle_locate has been called: not here\n"));
            }
        }

    }
#if !defined (TAO_HAS_MINIMUM_CORBA)
  ACE_CATCH (PortableServer::ForwardRequest, forward_request)
    {
      status = TAO_GIOP_OBJECT_FORWARD;
      forward_location_var =
        forward_request.forward_reference;
      ACE_DEBUG ((LM_DEBUG,
                  "handle_locate has been called: forwarding\n"));
    }
#endif /* TAO_HAS_MINIMUM_CORBA */
  ACE_CATCHANY
    {
      // Normal exception, so the object is not here
      status = TAO_GIOP_UNKNOWN_OBJECT;
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t) TAO_GIOP::process_server_locate - "
                    "CORBA exception raised\n"));
    }
#if defined (TAO_HAS_EXCEPTIONS)
  ACE_CATCHALL
    {
      // Normal exception, so the object is not here
      status = TAO_GIOP_UNKNOWN_OBJECT;
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t) TAO_GIOP::process_server_locate - "
                    "C++ exception raised\n"));
    }
#endif /* TAO_HAS_EXCEPTIONS */
  ACE_ENDTRY;

  // Create the response.
  TAO_GIOP::start_message (version,
                           TAO_GIOP::LocateReply,
                           output,
                           orb_core);
  output.write_ulong (locateRequestHeader.request_id);
  output.write_ulong (status);

  if (status == TAO_GIOP_OBJECT_FORWARD)
    {
      CORBA::Object_ptr object_ptr = forward_location_var.in ();
      if ((output << object_ptr) == 0)
        {
          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,
                        "TAO (%P|%t) TAO_GIOP::process_server_request -"
                        " cannot marshal object reference\n"));
        }
    }

  int result = TAO_GIOP::send_message (transport,
                                       output,
                                       orb_core);

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
TAO_GIOP::send_reply_exception (const TAO_GIOP_Version &version,
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

  // Construct a REPLY header.
  TAO_GIOP::start_message (version,
                           TAO_GIOP::Reply,
                           output,
                           orb_core);

  // A new try/catch block, but if something goes wrong now we have no
  // hope, just abort.
  ACE_TRY_NEW_ENV
    {
      // create and write a dummy context
      IOP::ServiceContextList resp_ctx;
      resp_ctx.length (0);
      output << resp_ctx;

      // Write the request ID
      output.write_ulong (request_id);

      // Write the exception
      CORBA::TypeCode_ptr except_tc = x->_type ();

      CORBA::exception_type extype =
        CORBA::USER_EXCEPTION;

      if (CORBA::SystemException::_narrow (x) != 0)
        extype = CORBA::SYSTEM_EXCEPTION;

      // write the reply_status
      output.write_ulong (TAO_GIOP::convert_CORBA_to_GIOP_exception (extype));

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

  return TAO_GIOP::send_message (transport,
                                 output,
                                 orb_core);
}


CORBA::Boolean
TAO_GIOP::start_message_lite (const TAO_GIOP_Version &,
                              TAO_GIOP::Message_Type type,
                              TAO_OutputCDR &msg)
{
  msg.reset ();

  // Write a dummy <size> later it is set to the right value...  @@
  // TODO Maybe we should store the OutputCDR status in
  CORBA::ULong size = 0;
  msg.write_ulong (size);

  msg.write_octet ((CORBA::Octet) type);

  return 1;
}

CORBA::Boolean
TAO_GIOP::write_request_header_lite (const IOP::ServiceContextList&,
                                     CORBA::ULong request_id,
                                     CORBA::Octet response_flags,
                                     const TAO_opaque &key,
                                     const char* opname,
                                     CORBA::Principal_ptr,
                                     TAO_OutputCDR &out_stream)
{
  out_stream << request_id;

  // Sync scope - ignored by server if request is not oneway.
  switch (response_flags)
    {
      case 0:
      case 1:
        // No response required.
        out_stream << CORBA::Any::from_octet (0);
        break;
      case 2:
        // Return before dispatching servant.
        out_stream << CORBA::Any::from_octet (1);
        break;
      case 3:
        // Return after dispatching servant.
        out_stream << CORBA::Any::from_octet (3);
        break;
      default:
        // Until more flags are defined by the OMG.
        return 0;
    }

  out_stream << key;
  out_stream << opname;
  return 1;
}

int
TAO_GIOP::parse_header (TAO_ORB_Core *orb_core,
                        TAO_InputCDR &input,
                        TAO_GIOP_Message_State& state)
{
  if (orb_core->orb_params ()->use_lite_protocol ())
    return TAO_GIOP::parse_header_lite (input, state);
  else
    return TAO_GIOP::parse_header_std (input, state);
}

int
TAO_GIOP::parse_header_std (TAO_InputCDR &input,
                            TAO_GIOP_Message_State &state)
{
  char *buf = input.rd_ptr ();

  // The values are hard-coded to support non-ASCII platforms.
  if (!(buf [0] == 0x47      // 'G'
        && buf [1] == 0x49   // 'I'
        && buf [2] == 0x4f   // 'O'
        && buf [3] == 0x50)) // 'P'
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t) bad header, magic word [%c%c%c%c]\n",
                    buf[0],
                    buf[1],
                    buf[2],
                    buf[3]));
      return -1;
    }

  state.giop_version.major = buf[TAO_GIOP_VERSION_MAJOR_OFFSET];
  state.giop_version.minor = buf[TAO_GIOP_VERSION_MINOR_OFFSET];

  if (state.giop_version.major != TAO_DEF_GIOP_MAJOR
      || state.giop_version.minor > TAO_DEF_GIOP_MINOR)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t) bad version <%d.%d>\n",
                    state.giop_version.major,
                    state.giop_version.minor));
      return -1;
    }

  if (state.giop_version.minor == 0)
    {
      state.byte_order = buf[TAO_GIOP_MESSAGE_FLAGS_OFFSET];
      if (TAO_debug_level > 2
          && state.byte_order != 0 && state.byte_order != 1)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "TAO (%P|%t) invalid byte order <%d>"
                      " for version <1.0>\n",
                      state.byte_order));
          return -1;
        }
    }
  else
    {
      state.byte_order     = (CORBA::Octet) (buf[TAO_GIOP_MESSAGE_FLAGS_OFFSET] & 0x01);
      state.more_fragments = (CORBA::Octet) (buf[TAO_GIOP_MESSAGE_FLAGS_OFFSET] & 0x02);
      if (TAO_debug_level > 2
          && state.giop_version.minor == 1
          && (buf[TAO_GIOP_MESSAGE_FLAGS_OFFSET] & ~0x3) != 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "TAO (%P|%t) invalid flags for <%d>"
                      " for version <1.1>\n",
                      buf[TAO_GIOP_MESSAGE_FLAGS_OFFSET]));
          return -1;
        }

    }

  state.message_type = buf[TAO_GIOP_MESSAGE_TYPE_OFFSET];

  input.reset_byte_order (state.byte_order);
  input.skip_bytes (TAO_GIOP_MESSAGE_SIZE_OFFSET);
  input.read_ulong (state.message_size);

  if (TAO_debug_level > 2)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "TAO (%P|%t) Parsed header = <%d,%d,%d,%d,%d>\n",
                  state.giop_version.major,
                  state.giop_version.minor,
                  state.byte_order,
                  state.message_type,
                  state.message_size));
    }
  return 0;
}

int
TAO_GIOP::parse_header_lite (TAO_InputCDR &input,
                             TAO_GIOP_Message_State &state)
{
  char *buf = input.rd_ptr ();

  state.giop_version.major = TAO_DEF_GIOP_MAJOR;
  state.giop_version.minor = TAO_DEF_GIOP_MINOR;
  state.byte_order = TAO_ENCAP_BYTE_ORDER;
  state.message_type = buf[TAO_GIOP_LITE_MESSAGE_TYPE_OFFSET];

  input.reset_byte_order (state.byte_order);
  input.read_ulong (state.message_size);
  return 0;
}
