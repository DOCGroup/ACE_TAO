// -*- C++ -*-
//$Id$

#include "tao/GIOP_Message_Acceptors.h"
#include "tao/POA.h"
#include "tao/debug.h"
#include "tao/GIOP_Utils.h"
#include "tao/GIOP_Server_Request.h"
#include "tao/Leader_Follower.h"

#if !defined (__ACE_INLINE__)
# include "tao/GIOP_Message_Acceptors.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(tao, GIOP_Message_Acceptors, "$Id$")

int
TAO_GIOP_Message_Acceptors::
  process_client_message (TAO_Transport *transport,
                          TAO_ORB_Core *orb_core,
                          TAO_InputCDR &input,
                          CORBA::Octet message_type)
{
  orb_core->leader_follower ().set_upcall_thread ();

  this->output_->reset ();
  switch (message_type)
    {
    case TAO_GIOP_REQUEST:
      // Should be taken care by the state specific invocations. They
      // could raise an exception or write things in the output CDR
      // stream
      this->process_client_request (transport,
                                    orb_core,
                                    input);
      break;
    case TAO_GIOP_LOCATEREQUEST:
      this->process_client_locate (transport,
                                   orb_core,
                                   input);
      break;
    case TAO_GIOP_MESSAGERROR:
    case TAO_GIOP_REPLY:
    case TAO_GIOP_LOCATEREPLY:
    case TAO_GIOP_CLOSECONNECTION:
    default:
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO (%P|%t) Illegal message received by server\n")));
      return this->send_error (transport);
    }

  return 0;
}

CORBA::Boolean
TAO_GIOP_Message_Acceptors::write_reply_header (TAO_OutputCDR &output,
                                                TAO_Pluggable_Reply_Params &params,
                                                CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Write the GIOP header first
  this->write_protocol_header (TAO_PLUGGABLE_MESSAGE_REPLY,
                               output);

  // Write the reply header
  this->accept_state_->write_reply_header (output,
                                           params,
                                           ACE_TRY_ENV);

  return 0;
}


int
TAO_GIOP_Message_Acceptors::
  process_client_request (TAO_Transport *transport,
                          TAO_ORB_Core* orb_core,
                          TAO_InputCDR &input)
{
  // Get the revision info
  TAO_GIOP_Version version (this->major_version (),
                            this->minor_version ());



  // This will extract the request header, set <response_required>
  // and <sync_with_server> as appropriate.
  TAO_GIOP_ServerRequest request (this,
                                  input,
                                  *this->output_,
                                  transport,
                                  orb_core,
                                  version);

  CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ();

  CORBA::ULong request_id = 0;
  CORBA::Boolean response_required = 0;

  int parse_error = 0;

  ACE_TRY
    {
      parse_error =
        this->accept_state_->parse_request_header (request);

      // Throw an exception if the
      if (parse_error != 0)
        ACE_TRY_THROW (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE,
                                       CORBA::COMPLETED_NO));
      request_id = request.request_id ();

      response_required = request.response_expected ();

#if (TAO_NO_IOR_TABLE == 0)
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

          //CORBA::Object_ptr object_reference =
          CORBA::Object_var object_reference =
            CORBA::Object::_nil ();

          // Do the Table Lookup.
          int status =
            orb_core->orb ()->_tao_find_in_IOR_table (object_id,
                                                      object_reference.out ());

          // If ObjectID not in table or reference is nil raise
          // OBJECT_NOT_EXIST.

          if (status == -1 || CORBA::is_nil (object_reference.in ()))
            ACE_TRY_THROW (CORBA::OBJECT_NOT_EXIST ());

          // ObjectID present in the table with an associated NON-NULL
          // reference.  Throw a forward request exception.

          /*CORBA::Object_ptr dup =
            CORBA::Object::_duplicate (object_reference);*/

          // @@ We could simply write the response at this point...
          ACE_TRY_THROW (PortableServer::ForwardRequest (object_reference.in ()));
        }
#endif /* TAO_NO_IOR_TABLE */

      // Do this before the reply is sent.
      orb_core->object_adapter ()->dispatch_servant (
                                                     request.object_key (),
                                                     request,
                                                     0,
                                                     ACE_TRY_ENV
                                                     );
      ACE_TRY_CHECK;
    }
#if (TAO_HAS_MINIMUM_CORBA == 0)
  ACE_CATCH (PortableServer::ForwardRequest, forward_request)
    {
      TAO_Pluggable_Reply_Params reply_params;
      reply_params.request_id_ = request_id;
      reply_params.reply_status_ = TAO_GIOP_LOCATION_FORWARD;
      reply_params.svc_ctx_.length (0);
      // Sending back the same service context list we received in the
      // Request.  (Important for RT CORBA).
      reply_params.service_context_notowned (&request.service_info ());
      reply_params.params_ = 0;

      // Make the GIOP header and Reply header
      this->write_reply_header (*this->output_,
                                reply_params);

      CORBA::Object_ptr object_ptr =
        forward_request.forward_reference.in();

      *this->output_ << object_ptr;

      int result = this->send_message (transport,
                                       *this->output_);

      if (result == -1)
      {
         if (TAO_debug_level > 0)
         {
            // No exception but some kind of error, yet a response
            // is required.
            ACE_ERROR ((LM_ERROR,
                          ACE_TEXT ("TAO: (%P|%t|%N|%l) %p: cannot send reply\n"),
                          ACE_TEXT ("TAO_GIOP::process_server_message")));
         }
      }
      return result;

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
                                               &request.service_info (),
                                               &ACE_ANY_EXCEPTION);
          if (result == -1)
            {
              if (TAO_debug_level > 0)
                ACE_ERROR ((LM_ERROR,
                            ACE_TEXT ("TAO: (%P|%t|%N|%l) %p: cannot send exception\n"),
                            ACE_TEXT ("process_connector_request ()")));
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
                      ACE_TEXT ("(%P|%t) exception thrown ")
                      ACE_TEXT ("but client is not waiting a response\n")));
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
                                               &request.service_info (),
                                               &exception);
          if (result == -1)
            {
              if (TAO_debug_level > 0)
                ACE_ERROR ((LM_ERROR,
                            ACE_TEXT ("TAO: (%P|%t|%N|%l) %p: cannot send exception\n"),
                            ACE_TEXT ("process_connector_request ()")));
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
                      ACE_TEXT ("(%P|%t|%N|%l) exception thrown ")
                      ACE_TEXT ("but client is not waiting a response\n")));
        }

      return result;
    }
#endif /* TAO_HAS_EXCEPTIONS */
  ACE_ENDTRY;

  int result = 0;

  return result;
}

int
TAO_GIOP_Message_Acceptors::
  process_client_locate (TAO_Transport *transport,
                         TAO_ORB_Core* orb_core,
                         TAO_InputCDR &input)
{
  // Get the revision info
  TAO_GIOP_Version version (this->major_version (),
                            this->minor_version ());

  // This will extract the request header, set <response_required> as
  // appropriate.
  TAO_GIOP_Locate_Request_Header locate_request (input,
                                                 orb_core);

  TAO_GIOP_Locate_Status_Msg status_info;

  // Defaulting
  status_info.status = TAO_GIOP_UNKNOWN_OBJECT;

  CORBA::Boolean response_required = 1;

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      int parse_error =
        this->accept_state_->parse_locate_header (locate_request);
      if (parse_error != 0)
        ACE_TRY_THROW (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE,
                                       CORBA::COMPLETED_NO));
#if (TAO_NO_IOR_TABLE == 0)

      const CORBA::Octet *object_key =
        locate_request.object_key ().get_buffer ();

      if (ACE_OS::memcmp (object_key,
                          &TAO_POA::objectkey_prefix[0],
                          TAO_POA::TAO_OBJECTKEY_PREFIX_SIZE) != 0)
        {
          CORBA::ULong len =
            locate_request.object_key ().length ();

          ACE_CString object_id (ACE_reinterpret_cast (const char *,
                                                       object_key),
                                 len,
                                 0,
                                 0);

          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("Simple Object key %s. Doing the Table Lookup ...\n"),
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
      TAO_ObjectKey tmp_key (locate_request.object_key ().length (),
                             locate_request.object_key ().length (),
                             locate_request.object_key ().get_buffer (),
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
                                             transport,
                                             orb_core,
                                             version,
                                             parse_error);
      if (parse_error != 0)
        ACE_TRY_THROW (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE,
                                       CORBA::COMPLETED_NO));

      orb_core->object_adapter ()->dispatch_servant
        (server_request.object_key (),
         server_request,
         0,
         ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (server_request.exception_type () == TAO_GIOP_NO_EXCEPTION)
        {
          // We got no exception, so the object is here.
          status_info.status = TAO_GIOP_OBJECT_HERE;
          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("TAO: (%P|%t) handle_locate() : found\n")));
        }
      else if (server_request.exception_type () != TAO_GIOP_NO_EXCEPTION)
        {
          status_info.forward_location_var = server_request.forward_location ();
          if (!CORBA::is_nil (status_info.forward_location_var.in ()))
            {
              status_info.status = TAO_GIOP_OBJECT_FORWARD;
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("handle_locate has been called: forwarding\n")));
            }
          else
            {
              // Normal exception, so the object is not here
              status_info.status = TAO_GIOP_UNKNOWN_OBJECT;
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("handle_locate has been called: not here\n")));
            }
        }
    }

#if (TAO_HAS_MINIMUM_CORBA == 0)
  ACE_CATCH (PortableServer::ForwardRequest, forward_request)
    {
      status_info.status = TAO_GIOP_OBJECT_FORWARD;
      status_info.forward_location_var =
        forward_request.forward_reference;
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("handle_locate has been called: forwarding\n")));
    }
#endif /* TAO_HAS_MINIMUM_CORBA */
  ACE_CATCHANY
    {
      // Normal exception, so the object is not here
      status_info.status = TAO_GIOP_UNKNOWN_OBJECT;
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO (%P|%t) TAO_GIOP::process_server_locate - ")
                    ACE_TEXT ("CORBA exception raised\n")));
    }
#if defined (TAO_HAS_EXCEPTIONS)
  ACE_CATCHALL
    {
      // Normal exception, so the object is not here
      status_info.status = TAO_GIOP_UNKNOWN_OBJECT;
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO (%P|%t) TAO_GIOP::process_server_locate - ")
                    ACE_TEXT ("C++ exception raised\n")));
    }
#endif /* TAO_HAS_EXCEPTIONS */
  ACE_ENDTRY;

  return this->make_send_locate_reply (transport,
                                       locate_request,
                                       status_info);
}


int
TAO_GIOP_Message_Acceptors::
  send_reply_exception (TAO_Transport *transport,
                        TAO_ORB_Core* orb_core,
                        CORBA::ULong request_id,
                        IOP::ServiceContextList *svc_info,
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

  TAO_Pluggable_Reply_Params reply_params;
  reply_params.request_id_ = request_id;
  reply_params.svc_ctx_.length (0);

  // Send back the service context we received.  (RTCORBA relies on
  // this).
  reply_params.service_context_notowned (svc_info);

#if (TAO_HAS_MINIMUM_CORBA == 0)
  reply_params.params_ = 0;
#endif /*TAO_HAS_MINIMUM_CORBA*/

  // A new try/catch block, but if something goes wrong now we have no
  // hope, just abort.
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      // Write the exception
      // CORBA::TypeCode_ptr except_tc = x->_type ();

      CORBA::exception_type extype =
        CORBA::USER_EXCEPTION;

      if (CORBA::SystemException::_downcast (x) != 0)
        extype = CORBA::SYSTEM_EXCEPTION;

      // write the reply_status
      reply_params.reply_status_ =
        TAO_GIOP_Utils::convert_CORBA_to_GIOP_exception (extype);


      // Make the GIOP & reply header. They are version specific.
      this->write_reply_header (output,
                                reply_params);

      // @@ Any way to implement this without interpretive
      //    marshaling???
      x->_tao_encode (output, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      /*output.encode (except_tc,
                     x,
                     0,
                     ACE_TRY_ENV);
                     ACE_TRY_CHECK;*/
    }
  ACE_CATCH (CORBA_Exception, ex)
    {
      // Now we know that while handling the error an other error
      // happened -> no hope, close connection.

      // Close the handle.
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t|%N|%l) cannot marshal exception, handle = %d: %p\n"),
                  transport->handle (),
                  ACE_TEXT ("send_reply_exception ()")));
      return -1;
    }
  ACE_ENDTRY;

  return this->send_message (transport,
                             output);
}


CORBA::Octet
TAO_GIOP_Message_Acceptors::major_version (void)
{
  return this->accept_state_->major_version ();
}

CORBA::Octet
TAO_GIOP_Message_Acceptors::minor_version (void)
{
  return this->accept_state_->minor_version ();
}

int
TAO_GIOP_Message_Acceptors::
  validate_version (TAO_GIOP_Message_State *state)
{
  char *buf = state->cdr.rd_ptr ();
  CORBA::Octet incoming_major =
    buf[TAO_GIOP_VERSION_MAJOR_OFFSET];
  CORBA::Octet incoming_minor =
    buf[TAO_GIOP_VERSION_MINOR_OFFSET];

  if (this->implementations_.check_revision (incoming_major,
                                             incoming_minor) == 0)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO (%P|%t|%N|%l) bad version <%d.%d>\n"),
                    incoming_major, incoming_minor));
      return -1;
    }

  // Sets the version
  // @@Bala Need to remove this as redundant data
  state->giop_version.minor = incoming_minor;
  state->giop_version.major = incoming_major;

  // Sets the state
  this->set_state (incoming_major,
                   incoming_minor);

  return 0;
}

int
TAO_GIOP_Message_Acceptors::
  make_send_locate_reply (TAO_Transport *transport,
                          TAO_GIOP_Locate_Request_Header &request,
                          TAO_GIOP_Locate_Status_Msg &status_info)
{
  // Note here we are making the Locate reply header which is *QUITE*
  // different from the reply header made by the make_reply () call..
  // Make the GIOP message header
  this->write_protocol_header (TAO_PLUGGABLE_MESSAGE_LOCATEREPLY,
                               *this->output_);

  // This writes the header & body
  this->accept_state_->write_locate_reply_mesg (*this->output_,
                                                request.request_id (),
                                                status_info);

  // Send the message
  int result = this->send_message (transport,
                                   *this->output_);

  // Print out message if there is an error
  if (result == -1)
    {
      if (TAO_debug_level > 0)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("TAO: (%P|%t) %p: cannot send reply\n"),
                      ACE_TEXT ("TAO_GIOP::process_server_message")));
        }
    }

  return result;
}

////////////////////////////////////////////////////////////
// Methods that should not be called from the acceptor side
////////////////////////////////////////////////////////////

CORBA::Boolean
TAO_GIOP_Message_Acceptors::
write_request_header (const TAO_Operation_Details & /**/,
                      TAO_Target_Specification & /*spec*/,
                      TAO_OutputCDR & /*msg*/)
{
   ACE_NOTSUP_RETURN (0);
}

CORBA::Boolean
TAO_GIOP_Message_Acceptors::
  write_locate_request_header (CORBA::ULong /*request_id*/,
                               TAO_Target_Specification & /*spec*/,
                               TAO_OutputCDR & /*msg*/)
{
  ACE_NOTSUP_RETURN (0);
}

int
TAO_GIOP_Message_Acceptors::
parse_reply (TAO_Message_State_Factory & /*mesg_state*/,
             TAO_Pluggable_Reply_Params & /*params*/)
{
  ACE_NOTSUP_RETURN (-1);
}
