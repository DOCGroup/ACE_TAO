//$Id$

#include "tao/GIOP_Message_Lite.h"
#include "tao/debug.h"
#include "tao/TAOC.h"
#include "tao/ORB_Core.h"
#include "tao/GIOP_Utils.h"
#include "tao/operation_details.h"
#include "tao/TAO_Server_Request.h"
#include "tao/GIOP_Message_Headers.h"
#include "tao/target_specification.h"
#include "tao/Leader_Follower.h"

#if !defined (__ACE_INLINE__)
# include "tao/GIOP_Message_Lite.i"
#endif /* __ACE_INLINE__ */

static const size_t TAO_GIOP_LITE_HEADER_LEN = 5;
static const size_t TAO_GIOP_LITE_MESSAGE_SIZE_OFFSET = 0;
static const size_t TAO_GIOP_LITE_MESSAGE_TYPE_OFFSET = 4;

TAO_GIOP_Message_Lite::TAO_GIOP_Message_Lite (TAO_ORB_Core *orb_core)
  :message_state_ (orb_core),
   output_ (0),
   cdr_buffer_alloc_ (
        orb_core->resource_factory ()->output_cdr_buffer_allocator ()
      ),
    cdr_dblock_alloc_ (
        orb_core->resource_factory ()->output_cdr_dblock_allocator ()
      )
{
#if defined (ACE_HAS_PURIFY)
  (void) ACE_OS::memset (this->repbuf_,
                         '\0',
                         sizeof this->repbuf_);
#endif /* ACE_HAS_PURIFY */
  ACE_NEW (this->output_,
           TAO_OutputCDR (this->repbuf_,
                          sizeof this->repbuf_,
                          TAO_ENCAP_BYTE_ORDER,
                          this->cdr_buffer_alloc_,
                          this->cdr_dblock_alloc_,
                          orb_core->orb_params ()->cdr_memcpy_tradeoff (),
                          orb_core->to_iso8859 (),
                          orb_core->to_unicode ()));
}


TAO_GIOP_Message_Lite::~TAO_GIOP_Message_Lite (void)
{
  // Explicitly call the destructor of the output CDR first. They need
  // the allocators during destruction.
  delete this->output_;

  // Then call the destructor of our allocators
  if (this->cdr_dblock_alloc_ != 0)
    this->cdr_dblock_alloc_->remove ();
  //  delete this->cdr_dblock_alloc_;

  if (this->cdr_buffer_alloc_ != 0)
    this->cdr_buffer_alloc_->remove ();
  //  delete this->cdr_buffer_alloc_;
}


void
TAO_GIOP_Message_Lite::init (CORBA::Octet,
                             CORBA::Octet)
{
  return;
}

int
TAO_GIOP_Message_Lite::parse_header (void)
{
  // Get the read pointer
  char *buf = this->message_state_.cdr.rd_ptr ();

  // @@ Bala: i added the following comment, does it make sense?
  // In GIOPLite the version, byte order info, etc. are hardcoded, and
  // not transmitted over the wire.
  this->message_state_.byte_order = TAO_ENCAP_BYTE_ORDER;
  this->message_state_.giop_version.major = TAO_DEF_GIOP_MAJOR;
  this->message_state_.giop_version.minor = TAO_DEF_GIOP_MINOR;

  // Get the message type.
  this->message_state_.message_type = buf[TAO_GIOP_LITE_MESSAGE_TYPE_OFFSET];

  this->message_state_.cdr.reset_byte_order (this->message_state_.byte_order);

  // The first bytes are the length of the message.
  this->message_state_.cdr.read_ulong (this->message_state_.message_size);

  return 0;
}


void
TAO_GIOP_Message_Lite::reset (int reset_flag)
{
  // Reset the message state
  this->message_state_.reset (reset_flag);

  //What else???
}


int
TAO_GIOP_Message_Lite::generate_request_header (
    TAO_Operation_Details &op,
    TAO_Target_Specification &spec,
    TAO_OutputCDR &cdr
  )
{
  // Write the GIOPLite header first
  if (!this->write_protocol_header (TAO_GIOP_REQUEST,
                                    cdr))
    {
      if (TAO_debug_level > 3)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("(%P|%t) Error in writing GIOP header \n")),
                           -1);

    }

  // Now call the implementation for the rest of the header
  if (!this->write_request_header (op,
                                   spec,
                                   cdr))
    {
      if (TAO_debug_level > 4)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("(%P|%t) Error in writing request header \n")),
                           -1);
    }

  return 0;

}


int
TAO_GIOP_Message_Lite::generate_locate_request_header (
    TAO_Operation_Details &op,
    TAO_Target_Specification &spec,
    TAO_OutputCDR &cdr
 )
{
  // Write the GIOPLite header first
  if (!this->write_protocol_header (TAO_GIOP_LOCATEREQUEST,
                                    cdr))
    {
      if (TAO_debug_level > 3)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("(%P|%t) Error in writing GIOPLite header \n")),
                           -1);

    }

  // Now call the implementation for the rest of the header
  if (!this->write_locate_request_header (op.request_id (),
                                          spec,
                                          cdr))
    {
      if (TAO_debug_level > 4)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("(%P|%t) Error in writing locate request header \n")),
                           -1);
    }

  return 0;
}


int
TAO_GIOP_Message_Lite::generate_reply_header (
    TAO_OutputCDR &cdr,
    TAO_Pluggable_Reply_Params &params
  )
{
  // Write the GIOPLite header first
  if (!this->write_protocol_header (TAO_GIOP_REPLY,
                                    cdr))
    {
      if (TAO_debug_level > 3)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("(%P|%t) Error in writing GIOPLite header \n")),
                           -1);
    }

  // Now call the implementation for the rest of the header
  if (!this->write_reply_header (cdr,
                                 params))
    {
      if (TAO_debug_level > 4)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("(%P|%t) Error in writing reply header \n")),
                           -1);
    }

  return 0;
}


int
TAO_GIOP_Message_Lite::read_message (TAO_Transport *transport,
                                     int /*block */,
                                     ACE_Time_Value *max_wait_time)
{
  if (this->message_state_.header_received () == 0)
    {
      int retval =
        TAO_GIOP_Utils::read_bytes_input (transport,
                                          message_state_.cdr,
                                          TAO_GIOP_LITE_HEADER_LEN ,
                                          max_wait_time);
      if (retval == -1)
        {
          if (TAO_debug_level > 0)
            {
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("TAO (%P|%t) - \n")
                          ACE_TEXT ("TAO_GIOP_Message_Lite::read_message \n")));
            }

          return -1;
        }

      // Read the rest of the stuff. That should be read by the
      // corresponding states
      if (this->parse_header () == -1)
        {
          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("TAO (%P|%t|%N%l) -\n"),
                        ACE_TEXT ("TAO_GIOP_Message_Lite::handle_input \n")));
          return -1;
        }

      if (this->message_state_.cdr.grow (TAO_GIOP_LITE_HEADER_LEN  +
                                         this->message_state_.message_size) == -1)
        {
          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("TAO (%P|%t|%N|%l) - %p\n"),
                        ACE_TEXT ("ACE_CDR::grow")));
          return -1;
        }

      // Growing the buffer may have reset the rd_ptr(), but we want
      // to leave it just after the GIOP header (that was parsed
      // already);
      this->message_state_.cdr.skip_bytes (TAO_GIOP_LITE_HEADER_LEN);
    }

  size_t missing_data =
    this->message_state_.message_size - this->message_state_.current_offset;

  ssize_t n =
    TAO_GIOP_Utils::read_buffer (transport,
                                 this->message_state_.cdr.rd_ptr ()
                                 + this->message_state_.current_offset,
                                 missing_data,
                                 max_wait_time);

   if (n == -1)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO (%P|%t) - %p\n"),
                    ACE_TEXT ("TAO_GIOP_Message_Lite::handle_input, read_buffer[1] \n")));
      return -1;
    }
  else if (n == 0)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO (%P|%t) - %p\n"),
                    ACE_TEXT ("TAO_GIOP_Message_Lite::handle_input, read_buffer[2]\n")));
      return -1;
    }

   this->message_state_.current_offset += n;

  if (this->message_state_.current_offset ==
      this->message_state_.message_size)
    {
      if (TAO_debug_level >= 4)
        {
          size_t header_len = TAO_GIOP_LITE_HEADER_LEN ;

          char *buf = this->message_state_.cdr.rd_ptr ();
          buf -= header_len;
          size_t msg_len = this->message_state_.cdr.length () + header_len;
          this->dump_msg ("recv",
                          ACE_reinterpret_cast (u_char *,
                                                buf),
                          msg_len);
        }
    }

  return this->message_state_.is_complete ();
}


int
TAO_GIOP_Message_Lite::format_message (TAO_OutputCDR &stream)
{
  // Get the header length
  const size_t header_len = TAO_GIOP_LITE_HEADER_LEN ;

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

  // Strictly speaking, should not need to loop here because the
  // socket never gets set to a nonblocking mode ... some Linux
  // versions seem to need it though.  Leaving it costs little.
  if (TAO_debug_level > 2)
    {
      this->dump_msg ("send",
                      ACE_reinterpret_cast (u_char *,
                                            buf),
                      stream.length ());
    }

  return 0;
}


TAO_Pluggable_Message_Type
TAO_GIOP_Message_Lite::message_type (void)
{
  switch (this->message_state_.message_type)
    {
    case TAO_GIOP_REQUEST:
    case TAO_GIOP_LOCATEREQUEST:
      return TAO_PLUGGABLE_MESSAGE_REQUEST;

    case TAO_GIOP_LOCATEREPLY:
    case TAO_GIOP_REPLY:
      return TAO_PLUGGABLE_MESSAGE_REPLY;

    case TAO_GIOP_CLOSECONNECTION:
      return TAO_PLUGGABLE_MESSAGE_CLOSECONNECTION;

    case TAO_GIOP_CANCELREQUEST:
    case TAO_GIOP_MESSAGERROR:
    case TAO_GIOP_FRAGMENT:
      // Never happens: why??
    default:
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("TAO (%P|%t) %N:%l        message_type : ")
                    ACE_TEXT ("wrong message.\n")));
    }

  // In case of some errors
  return TAO_PLUGGABLE_MESSAGE_MESSAGERROR;
}


int
TAO_GIOP_Message_Lite::process_request_message (TAO_Transport *transport,
                                                TAO_ORB_Core *orb_core)
{
  // Set the upcall thread
  orb_core->leader_follower ().set_upcall_thread ();

  // Reset the output CDR stream.
  // @@@@Is it necessary  here?
  this->output_->reset ();

  //
  // Take out all the information from the <message_state> and reset
  // it so that nested upcall on the same transport can be handled.
  //

  // Notice that the message_state is only modified in one thread at a
  // time because the reactor does not call handle_input() for the
  // same Event_Handler in two threads at the same time.

  // Steal the input CDR from the message state.
  TAO_InputCDR input_cdr (ACE_InputCDR::Transfer_Contents (this->message_state_.cdr),
                          orb_core);

  // Send the message state for the service layer like FT to log the
  // messages
  // @@@ Needed for DOORS
  //  orb_core->services_log_msg_rcv (this->message_state_);

  // Reset the message state. Now, we are ready for the next nested
  // upcall if any.
  this->message_state_.reset (0);

  // We know we have some request message. Check whether it is a
  // GIOP_REQUEST or GIOP_LOCATE_REQUEST to take action.
  switch (this->message_state_.message_type)
    {
    case TAO_GIOP_REQUEST:
      // Should be taken care by the state specific invocations. They
      // could raise an exception or write things in the output CDR
      // stream
      return this->process_request (transport,
                                    orb_core,
                                    input_cdr);
    case TAO_GIOP_LOCATEREQUEST:
      return this->process_locate_request (transport,
                                           orb_core,
                                           input_cdr);
    default:
      return -1;
    }
}

int
TAO_GIOP_Message_Lite::process_reply_message (
    TAO_Pluggable_Reply_Params &params
  )
{
  // We know we have some reply message. Check whether it is a
  // GIOP_REPLY or GIOP_LOCATE_REPLY to take action.
  switch (this->message_state_.message_type)
    {
    case TAO_GIOP_REPLY:
      // Should be taken care by the state specific parsing
      return this->parse_reply (this->message_state_.cdr,
                                params);
    case TAO_GIOP_LOCATEREPLY:
      // We call parse_reply () here because, the message format for
      // the LOCATEREPLY & REPLY are same.
      return this->parse_reply (this->message_state_.cdr,
                                params);
    default:
      return -1;
    }
}

int
TAO_GIOP_Message_Lite::generate_exception_reply (
    TAO_OutputCDR &cdr,
    TAO_Pluggable_Reply_Params &params,
    CORBA::Exception &x
  )
{
  // A new try/catch block, but if something goes wrong now we have no
  // hope, just abort.
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      // Make the GIOP & reply header. They are version specific.
      this->write_reply_header (cdr,
                                params);
      x._tao_encode (cdr, ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (CORBA_Exception, ex)
    {
      // Now we know that while handling the error an other error
      // happened -> no hope, close connection.

      // Close the handle.
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t|%N|%l) cannot marshal exception, ")
                  ACE_TEXT ("generate_exception_reply ()")));
      return -1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);

  return 0;
}


int
TAO_GIOP_Message_Lite::write_protocol_header (
    TAO_GIOP_Message_Type t,
    TAO_OutputCDR &msg)
{
  // Reset the message type
  msg.reset ();

  // @@ Bala: this is something to think harder about:  right now we
  // leave the space to store the length, and later we set it, but the
  // way we do it is CDR specific...  Maybe the XXXStream classes
  // should support a 'save your current position' method that returns
  // a 'Mememto' (check the GoF book), later the CDR stream could be
  // restored to that state, and the size written to it.
  // @@ Then again, i don't know how would that work with fragments
  // (eventually we may want TAO to generate fragments), or protocols
  // based on chunking....
  //
  // Write a dummy <size> later it is set to the right value...  @@
  CORBA::ULong size = 0;
  msg.write_ulong (size);

  msg.write_octet ((CORBA::Octet) t);

  return 1;
}


int
TAO_GIOP_Message_Lite::process_request (TAO_Transport *transport,
                                        TAO_ORB_Core *orb_core,
                                        TAO_InputCDR &cdr)
{
  // This will extract the request header, set <response_required>
  // and <sync_with_server> as appropriate.
  TAO_ServerRequest request (this,
                             cdr,
                             *this->output_,
                             transport,
                             orb_core);

  CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ();

  CORBA::ULong request_id = 0;
  CORBA::Boolean response_required = 0;

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

      CORBA::Object_var forward_to;

      // Do this before the reply is sent.
      orb_core->adapter_registry ()->dispatch (request.object_key (),
                                               request,
                                               0,
                                               forward_to,
                                               ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (!CORBA::is_nil (forward_to.in ()))
        {
          // We should forward to another object...
          TAO_Pluggable_Reply_Params reply_params (orb_core);
          reply_params.request_id_ = request_id;
          reply_params.reply_status_ = TAO_GIOP_LOCATION_FORWARD;
          reply_params.svc_ctx_.length (0);

          // Sending back the same service context list we received in the
          // Request.  (Important for RT CORBA).
          reply_params.service_context_notowned (&request.service_info ());

          // Make the GIOP header and Reply header
          this->generate_reply_header (*this->output_,
                                       reply_params);

          *this->output_ << forward_to.in ();

          int result = transport->send_message (*this->output_);
          if (result == -1)
            {
              if (TAO_debug_level > 0)
                {
                  // No exception but some kind of error, yet a
                  // response is required.
                  ACE_ERROR ((LM_ERROR,
                              ACE_TEXT ("TAO: (%P|%t|%N|%l) %p: ")
                              ACE_TEXT ("cannot send reply\n"),
                              ACE_TEXT ("TAO_GIOP::process_server_message")));
                }
            }
          return result;
        }
    }
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
                {
                  ACE_ERROR ((LM_ERROR,
                              ACE_TEXT ("TAO: (%P|%t|%N|%l) %p: ")
                              ACE_TEXT ("cannot send exception\n"),
                              ACE_TEXT ("process_connector_request ()")));
                }

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
          CORBA::UNKNOWN exception (
              CORBA::SystemException::_tao_minor_code (
                  TAO_UNHANDLED_SERVER_CXX_EXCEPTION,
                  0
                ),
              CORBA::COMPLETED_MAYBE
            );

          result = this->send_reply_exception (transport,
                                               orb_core,
                                               request_id,
                                               &request.service_info (),
                                               &exception);
          if (result == -1)
            {
              if (TAO_debug_level > 0)
                {
                  ACE_ERROR ((LM_ERROR,
                              ACE_TEXT ("TAO: (%P|%t|%N|%l) %p: ")
                              ACE_TEXT ("cannot send exception\n"),
                              ACE_TEXT ("process_connector_request ()")));
                }

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

  return 0;
}


int
TAO_GIOP_Message_Lite::process_locate_request (TAO_Transport *transport,
                                               TAO_ORB_Core* orb_core,
                                               TAO_InputCDR &input)
{
  // This will extract the request header, set <response_required> as
  // appropriate.
  TAO_GIOP_Locate_Request_Header locate_request (input,
                                                 orb_core);

  TAO_GIOP_Locate_Status_Msg status_info;

  // Defaulting.
  status_info.status = TAO_GIOP_UNKNOWN_OBJECT;

  CORBA::Boolean response_required = 1;

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      int parse_error =
        this->parse_locate_header (locate_request);

      if (parse_error != 0)
        {
          ACE_TRY_THROW (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE,
                                         CORBA::COMPLETED_NO));
        }

      // Execute a fake request to find out if the object is there or
      // if the POA can activate it on demand...
      char repbuf[ACE_CDR::DEFAULT_BUFSIZE];
      TAO_OutputCDR dummy_output (repbuf,
                                  sizeof repbuf);
      // This output CDR is not used!

      TAO_ObjectKey tmp_key (locate_request.object_key ().length (),
                             locate_request.object_key ().length (),
                             locate_request.object_key ().get_buffer (),
                             0);

      // Set it to an error state
      parse_error = 1;
      CORBA::ULong req_id = locate_request.request_id ();

      // We will send the reply. The ServerRequest class need not send
      // the reply
      CORBA::Boolean deferred_reply = 1;
      TAO_ServerRequest server_request (this,
                                        req_id,
                                        response_required,
                                        deferred_reply,
                                        tmp_key,
                                        "_non_existent",
                                        dummy_output,
                                        transport,
                                        orb_core,
                                        parse_error);

      if (parse_error != 0)
        {
          ACE_TRY_THROW (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE,
                                         CORBA::COMPLETED_NO));
        }

      CORBA::Object_var forward_to;

      orb_core->adapter_registry ()->dispatch (server_request.object_key (),
                                               server_request,
                                               0,
                                               forward_to,
                                               ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (!CORBA::is_nil (forward_to.in ()))
        {
          status_info.status = TAO_GIOP_OBJECT_FORWARD;
          status_info.forward_location_var = forward_to;
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("handle_locate has been called: forwarding\n")));
        }
      else if (server_request.exception_type () == TAO_GIOP_NO_EXCEPTION)
        {
          // We got no exception, so the object is here.
          status_info.status = TAO_GIOP_OBJECT_HERE;
          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("TAO: (%P|%t) handle_locate() : found\n")));
        }
      else
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
TAO_GIOP_Message_Lite::make_send_locate_reply (
    TAO_Transport *transport,
    TAO_GIOP_Locate_Request_Header &request,
    TAO_GIOP_Locate_Status_Msg &status_info
  )
{
  // Note here we are making the Locate reply header which is *QUITE*
  // different from the reply header made by the make_reply () call..
  // Make the GIOP message header
  this->write_protocol_header (TAO_GIOP_LOCATEREPLY,
                               *this->output_);

  // This writes the header & body
  this->write_locate_reply_mesg (*this->output_,
                                 request.request_id (),
                                 status_info);

  // Send the message
  int result = transport->send_message (*this->output_);

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

int
TAO_GIOP_Message_Lite::parse_reply (TAO_InputCDR &cdr,
                                    TAO_Pluggable_Reply_Params &params)
{

  // Read the request id
  if (!cdr.read_ulong (params.request_id_))
    {
      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t|%N|%l) : ")
                      ACE_TEXT ("TAO_GIOP_Message_Lite::parse_reply, ")
                      ACE_TEXT ("extracting request id")));
        }

      return -1;
    }

  CORBA::ULong rep_stat = 0;

  // and the reply status type.  status can be NO_EXCEPTION,
  // SYSTEM_EXCEPTION, USER_EXCEPTION, LOCATION_FORWARD
  // CAnnot handle LOCATION_FORWARD_PERM here
  if (!cdr.read_ulong (rep_stat))
    {
      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t|%N|%l) : ")
                      ACE_TEXT ("TAO_GIOP_Message_Lite::parse_reply, ")
                      ACE_TEXT ("extracting reply status\n")));
        }

      return -1;
    }

  // Pass the right Pluggable interface code to the transport layer
  switch (rep_stat)
    {
      // Request completed successfully
    case TAO_GIOP_NO_EXCEPTION:
      params.reply_status_ = TAO_PLUGGABLE_MESSAGE_NO_EXCEPTION;
      break;

      // Request terminated with user exception
    case TAO_GIOP_USER_EXCEPTION:
      params.reply_status_ = TAO_PLUGGABLE_MESSAGE_USER_EXCEPTION;
      break;
      // Request terminated with system exception
    case TAO_GIOP_SYSTEM_EXCEPTION:
      params.reply_status_ = TAO_PLUGGABLE_MESSAGE_SYSTEM_EXCEPTION;
      break;
      // Reply is a location forward type
    case TAO_GIOP_LOCATION_FORWARD:
      params.reply_status_ = TAO_PLUGGABLE_MESSAGE_LOCATION_FORWARD;
      break;
    default:
      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%N|%l) Unknown reply status \n")));
        }
    }

  return 0;
}



int
TAO_GIOP_Message_Lite::write_reply_header (
    TAO_OutputCDR &output,
    TAO_Pluggable_Reply_Params &reply,
    CORBA::Environment & /* ACE_TRY_ENV */
  )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Write the GIOP Lite header first
  this->write_protocol_header (TAO_GIOP_REPLY,
                               output);

  // Write the request ID
  output.write_ulong (reply.request_id_);

  // Write the reply status
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

  return 1;
}

int
TAO_GIOP_Message_Lite::write_request_header (
    const TAO_Operation_Details &opdetails,
    TAO_Target_Specification &spec,
     TAO_OutputCDR &out_stream
   )
{
  out_stream << opdetails.request_id ();

  const CORBA::Octet response_flags = opdetails.response_flags ();

  // @@ (JP) Temporary hack until all of GIOP 1.2 is implemented.
  if (response_flags == TAO_TWOWAY_RESPONSE_FLAG)
    {
      out_stream << CORBA::Any::from_octet (1);
    }
  // Sync scope - ignored by server if request is not oneway.
  else if (response_flags == CORBA::Octet (TAO::SYNC_WITH_TRANSPORT)
           || response_flags == CORBA::Octet (TAO::SYNC_NONE)
           || response_flags == CORBA::Octet (TAO::SYNC_EAGER_BUFFERING)
           || response_flags == CORBA::Octet (TAO::SYNC_DELAYED_BUFFERING))
    {
      // No response required.
      out_stream << CORBA::Any::from_octet (0);
    }
  else if (response_flags == CORBA::Octet (TAO::SYNC_WITH_SERVER))
    {
      // Return before dispatching servant.  We're also setting the high
      // bit here. This is a temporary fix until the rest of GIOP 1.2 is
      // implemented in TAO.
      out_stream << CORBA::Any::from_octet (129);
    }
  else if (response_flags == CORBA::Octet (TAO::SYNC_WITH_TARGET))
    {
      // Return after dispatching servant.
      out_stream << CORBA::Any::from_octet (3);
    }
  else
    {
      // Until more flags are defined by the OMG.
      return 0;
    }

  // In this case we cannot recognise anything other than the Object
  // key as the address disposition variable. But we do a sanity check
  // anyway.
  const TAO_ObjectKey *key = spec.object_key ();

  if (key != 0)
    {
      // Put in the object key
      out_stream << *key;
    }
    else
    {
      if (TAO_debug_level)
        {
          ACE_DEBUG ((
              LM_DEBUG,
              ACE_TEXT ("(%N |%l) Unable to handle this request \n")
            ));
        }

      return 0;
    }

  out_stream.write_string (opdetails.opname_len (),
                           opdetails.opname ());

  return 1;
}

int
TAO_GIOP_Message_Lite::write_locate_request_header (
    CORBA::ULong request_id,
    TAO_Target_Specification &spec,
    TAO_OutputCDR &msg
  )
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
        {
          ACE_DEBUG ((
              LM_DEBUG,
              ACE_TEXT ("(%N |%l) Unable to handle this request \n")
            ));
        }

      return 0;
    }

  return 1;
}


int
TAO_GIOP_Message_Lite::parse_request_header (TAO_ServerRequest &request)
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
TAO_GIOP_Message_Lite::parse_locate_header (
    TAO_GIOP_Locate_Request_Header &request
  )
{
  // Get the stream.
  TAO_InputCDR &msg = request.incoming_stream ();

  CORBA::Boolean hdr_status = 1;

  // Get the request id.
  CORBA::ULong req_id = 0;
  hdr_status = msg.read_ulong (req_id);

  // Store it in the Locate request classes.
  request.request_id (req_id);

  TAO_ObjectKey &object_key =
    request.object_key ();

  // Note that here there are no unions and so no problems.
  hdr_status = hdr_status && (msg >> object_key);

  return hdr_status ? 0 : -1;
}

int
TAO_GIOP_Message_Lite::send_reply_exception (
    TAO_Transport *transport,
    TAO_ORB_Core* orb_core,
    CORBA::ULong request_id,
    IOP::ServiceContextList *svc_info,
    CORBA::Exception *x
  )
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
  TAO_Pluggable_Reply_Params reply_params (orb_core);
  reply_params.request_id_ = request_id;
  reply_params.svc_ctx_.length (0);

  // Send back the service context we received.  (RTCORBA relies on
  // this).
  reply_params.service_context_notowned (svc_info);

  // A new try/catch block, but if something goes wrong now we have no
  // hope, just abort.
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      // Write the exception.
      reply_params.reply_status_ = TAO_GIOP_USER_EXCEPTION;

      if (CORBA::SystemException::_downcast (x) != 0)
        {
          reply_params.reply_status_ = TAO_GIOP_SYSTEM_EXCEPTION;
        }

      // Make the GIOP & reply header. They are version specific.
      this->write_reply_header (output,
                                reply_params);


      x->_tao_encode (output, ACE_TRY_ENV);
      ACE_TRY_CHECK;
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

  return transport->send_message (output);
}


int
TAO_GIOP_Message_Lite::write_locate_reply_mesg (
    TAO_OutputCDR & output,
    CORBA::ULong request_id,
    TAO_GIOP_Locate_Status_Msg &status_info
  )
{
  // Note here we are making the Locate reply header which is *QUITE*
  // different from the reply header made by the make_reply () call..

  // Make the GIOP message header
  this->write_protocol_header (TAO_GIOP_LOCATEREPLY,
                               output);

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
                        ACE_TEXT ("TAO (%P|%t|%N|%l) make_locate_reply-")
                        ACE_TEXT ("cannot marshal object reference\n")));
        }
    }

  return 1;
}

// Send an "I can't understand you" message -- again, the message is
// prefabricated for simplicity.  This implies abortive disconnect (at
// the application level, if not at the level of TCP).
//
// NOTE that IIOPLite will still benefit from TCP's orderly disconnect.
int
TAO_GIOP_Message_Lite::send_error (TAO_Transport *transport)
{
  const char error_message [TAO_GIOP_LITE_HEADER_LEN] =
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
        {
          ACE_DEBUG ((
              LM_DEBUG,
              ACE_TEXT ("TAO (%N|%l|%P|%t) error sending error to %d\n"),
              which
            ));
        }
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
        {
          message_name = names [slot];
        }

      // Byte order.
      int byte_order = TAO_ENCAP_BYTE_ORDER;

      // request/reply id.
      CORBA::ULong tmp = 0;
      CORBA::ULong *id = &tmp;

      if (ptr[TAO_GIOP_LITE_MESSAGE_TYPE_OFFSET] == TAO_GIOP_REQUEST ||
          ptr[TAO_GIOP_LITE_MESSAGE_TYPE_OFFSET] == TAO_GIOP_REPLY)
        {
          // @@ Only works if ServiceContextList is empty....
          id = ACE_reinterpret_cast (CORBA::ULong *,
                                     (char * ) (ptr));

        }

      // Print.
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P | %t):%s GIOP lite msg, ")
                  ACE_TEXT ("%d data bytes, %s endian, <%s = %d> \n"),
                  label,
                  len - TAO_GIOP_LITE_HEADER_LEN,
                  (byte_order == TAO_ENCAP_BYTE_ORDER) ? "my" : "other",
                  message_name,
                  *id));
    }
}
