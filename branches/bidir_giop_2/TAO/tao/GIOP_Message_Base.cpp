//$Id$

#include "tao/GIOP_Message_Base.h"
#include "tao/operation_details.h"
#include "tao/GIOP_Utils.h"
#include "tao/Pluggable.h"
#include "tao/debug.h"
#include "tao/ORB_Core.h"
#include "tao/Leader_Follower.h"
#include "tao/TAO_Server_Request.h"
#include "tao/GIOP_Message_Locate_Header.h"

#if !defined (__ACE_INLINE__)
# include "tao/GIOP_Message_Base.i"
#endif /* __ACE_INLINE__ */

// Constants for GIOP. They are declared static as that will put them
// in file scope.
static const size_t TAO_GIOP_MESSAGE_HEADER_LEN = 12;
static const size_t TAO_GIOP_MESSAGE_SIZE_OFFSET = 8;
static const size_t TAO_GIOP_VERSION_MINOR_OFFSET = 5;
static const size_t TAO_GIOP_VERSION_MAJOR_OFFSET = 4;
static const size_t TAO_GIOP_MESSAGE_FLAGS_OFFSET = 6;
static const size_t TAO_GIOP_MESSAGE_TYPE_OFFSET  = 7;


ACE_RCSID(tao, GIOP_Message_Base, "$Id$")

TAO_GIOP_Message_Base::TAO_GIOP_Message_Base (TAO_ORB_Core *orb_core)
  : message_state_ (orb_core),
    output_ (0),
    cdr_buffer_alloc_ (
        orb_core->resource_factory ()->output_cdr_buffer_allocator ()
      ),
    cdr_dblock_alloc_ (
        orb_core->resource_factory ()->output_cdr_dblock_allocator ()
      ),
    generator_parser_ (0)
{
#if defined(ACE_HAS_PURIFY)
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


TAO_GIOP_Message_Base::~TAO_GIOP_Message_Base (void)
{
  // Explicitly call the destructor of the output CDR first. They need
  // the allocators during destruction.
  delete this->output_;

  // Then call the destructor of our allocators
  if (this->cdr_dblock_alloc_ != 0)
    this->cdr_dblock_alloc_->remove ();
  delete this->cdr_dblock_alloc_;

  if (this->cdr_buffer_alloc_ != 0)
    this->cdr_buffer_alloc_->remove ();
  delete this->cdr_buffer_alloc_;
}


void
TAO_GIOP_Message_Base::init (CORBA::Octet major,
                             CORBA::Octet minor)
{
  // Set the state
  this->set_state (major,
                   minor);
}


void
TAO_GIOP_Message_Base::reset (int reset_flag)
{
  // Reset the message state
  this->message_state_.reset (reset_flag);

  //What else???
}

int
TAO_GIOP_Message_Base::generate_request_header (
    TAO_Operation_Details &op,
    TAO_Target_Specification &spec,
    TAO_OutputCDR &cdr
  )
{
  // Write the GIOP header first
  if (!this->write_protocol_header (TAO_GIOP_REQUEST,
                                    cdr))
    {
      if (TAO_debug_level > 3)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("(%P|%t) Error in writing GIOP header \n")),
                           -1);

    }

  // Now call the implementation for the rest of the header
  if (!this->generator_parser_->write_request_header (op,
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
TAO_GIOP_Message_Base::generate_locate_request_header (
    TAO_Operation_Details &op,
    TAO_Target_Specification &spec,
    TAO_OutputCDR &cdr
 )
{
  // Write the GIOP header first
  if (!this->write_protocol_header (TAO_GIOP_LOCATEREQUEST,
                                    cdr))
    {
      if (TAO_debug_level > 3)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("(%P|%t) Error in writing GIOP header \n")),
                           -1);

    }

  // Now call the implementation for the rest of the header
  if (!this->generator_parser_->write_locate_request_header
      (op.request_id (),
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
TAO_GIOP_Message_Base::generate_reply_header (
    TAO_OutputCDR &cdr,
    TAO_Pluggable_Reply_Params &params
  )
{
  // Write the GIOP header first
  if (!this->write_protocol_header (TAO_GIOP_REPLY,
                                    cdr))
    {
      if (TAO_debug_level > 3)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("(%P|%t) Error in writing GIOP header \n")),
                           -1);
    }

  // Now call the implementation for the rest of the header
  if (!this->generator_parser_->write_reply_header (cdr,
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
TAO_GIOP_Message_Base::read_message (TAO_Transport *transport,
                                     int /*block */,
                                     ACE_Time_Value *max_wait_time)
{
  if (this->message_state_.header_received () == 0)
    {
      int retval =
        TAO_GIOP_Utils::read_bytes_input (transport,
                                          message_state_.cdr,
                                          TAO_GIOP_MESSAGE_HEADER_LEN ,
                                          max_wait_time);
      if (retval == -1)
        {
          if (TAO_debug_level > 0)
            {
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("TAO (%P|%t) - \n")
                          ACE_TEXT ("TAO_GIOP_Message_Base::read_message \n")));
            }

          return -1;
        }

      if (this->parse_magic_bytes () == -1)
        {
          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("TAO (%P|%t) -"),
                        ACE_TEXT ("TAO_GIOP_Message_Base::handle_input, parse_magic_bytes \n")));
          return -1;
        }

      // Read the rest of the stuff. That should be read by the
      // corresponding states
      if (this->parse_header () == -1)
        {
          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("TAO (%P|%t|%N%l) -\n"),
                        ACE_TEXT ("TAO_GIOP_Message_Base::handle_input \n")));
          return -1;
        }

      if (this->message_state_.cdr.grow (TAO_GIOP_MESSAGE_HEADER_LEN  +
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
      this->message_state_.cdr.skip_bytes (TAO_GIOP_MESSAGE_HEADER_LEN);
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
                    ACE_TEXT ("TAO_GIOP_Message_Base::handle_input, read_buffer[1] \n")));
      return -1;
    }
  else if (n == 0)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO (%P|%t) - %p\n"),
                    ACE_TEXT ("TAO_GIOP_Message_Base::handle_input, read_buffer[2]\n")));
      return -1;
    }

   this->message_state_.current_offset += n;

  if (this->message_state_.current_offset ==
      this->message_state_.message_size)
    {
      if (TAO_debug_level >= 4)
        {
          size_t header_len = TAO_GIOP_MESSAGE_HEADER_LEN ;

          // Need to include GIOPlite too.
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
TAO_GIOP_Message_Base::format_message (TAO_OutputCDR &stream)
{
  // Get the header length
  const size_t header_len = TAO_GIOP_MESSAGE_HEADER_LEN ;

  // Get the message size offset
  const size_t offset = TAO_GIOP_MESSAGE_SIZE_OFFSET;

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
TAO_GIOP_Message_Base::message_type (void)
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

  return TAO_PLUGGABLE_MESSAGE_MESSAGERROR;
}




int
TAO_GIOP_Message_Base::process_request_message (TAO_Transport *transport,
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
TAO_GIOP_Message_Base::process_reply_message (
    TAO_Pluggable_Reply_Params &params
  )
{
  // We know we have some reply message. Check whether it is a
  // GIOP_REPLY or GIOP_LOCATE_REPLY to take action.
  switch (this->message_state_.message_type)
    {
    case TAO_GIOP_REPLY:
      // Should be taken care by the state specific parsing
      return this->generator_parser_->parse_reply (this->message_state_.cdr,
                                                   params);
    case TAO_GIOP_LOCATEREPLY:
      return this->generator_parser_->parse_locate_reply (this->message_state_.cdr,
                                                          params);
    default:
      return -1;
    }
}

int
TAO_GIOP_Message_Base::generate_exception_reply (
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
      // Make the GIOP & reply header.
      this->generate_reply_header (cdr,
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
TAO_GIOP_Message_Base::write_protocol_header (
    TAO_GIOP_Message_Type t,
    TAO_OutputCDR &msg)
{
  // Reset the message type
  msg.reset ();

  static CORBA::Octet magic[] =
  {
    // The following works on non-ASCII platforms, such as MVS (which
    // uses EBCDIC).
    0x47, // 'G'
    0x49, // 'I'
    0x4f, // 'O'
    0x50, // 'P'
  };

  static int magic_size = sizeof (magic)/sizeof (magic[0]);

  msg.write_octet_array (magic, magic_size);
  msg.write_octet (this->generator_parser_->major_version ());
  msg.write_octet (this->generator_parser_->minor_version ());

  // We are putting the byte order. But at a later date if we support
  // fragmentation and when we want to use the other 6 bits in this
  // octet we can have a virtual function do this for us as the
  // version info , Bala
  msg.write_octet (TAO_ENCAP_BYTE_ORDER);

  msg.write_octet ((CORBA::Octet) t);

  // Write a dummy <size> later it is set to the right value...
  CORBA::ULong size = 0;
  msg.write_ulong (size);

  return 1;
}



int
TAO_GIOP_Message_Base::process_request (TAO_Transport *transport,
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
        this->generator_parser_->parse_request_header (request);

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
TAO_GIOP_Message_Base::process_locate_request (TAO_Transport *transport,
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
        this->generator_parser_->parse_locate_header (locate_request);

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
TAO_GIOP_Message_Base::make_send_locate_reply (
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
  this->generator_parser_->write_locate_reply_mesg (*this->output_,
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

// Send an "I can't understand you" message -- again, the message is
// prefabricated for simplicity.  This implies abortive disconnect (at
// the application level, if not at the level of TCP).
//
// NOTE that IIOP will still benefit from TCP's orderly disconnect.
int
TAO_GIOP_Message_Base::send_error (TAO_Transport *transport)
{
  const char
    error_message [TAO_GIOP_MESSAGE_HEADER_LEN] =
  {
    // The following works on non-ASCII platforms, such as MVS (which
    // uses EBCDIC).
    0x47, // 'G'
    0x49, // 'I'
    0x4f, // 'O'
    0x50, // 'P'
    this->generator_parser_->major_version (),
    this->generator_parser_->minor_version (),
    TAO_ENCAP_BYTE_ORDER,
    TAO_GIOP_MESSAGERROR,
    0, 0, 0, 0
  };

  // @@ Q: How does this works with GIOP lite?
  //    A: It doesn't

  this->dump_msg ("send_error",
                  (const u_char *) error_message,
                  TAO_GIOP_MESSAGE_HEADER_LEN );

  ACE_HANDLE which = transport->handle ();

  int result = transport->send ((const u_char *)error_message,
                                TAO_GIOP_MESSAGE_HEADER_LEN );
  if (result == -1)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO (%N|%l|%P|%t) error sending error to %d\n"),
                    which));
    }

  return result;
}


int
TAO_GIOP_Message_Base::parse_header (void)
{
  char *buf = this->message_state_.cdr.rd_ptr ();

  // Let us be specific that it is for 1.0
  if (this->message_state_.giop_version.minor == 0 &&
      this->message_state_.giop_version.minor == 1)
    {
      this->message_state_.byte_order =
        buf[TAO_GIOP_MESSAGE_FLAGS_OFFSET];
      if (TAO_debug_level > 2
          && this->message_state_.byte_order != 0 &&
          this->message_state_.byte_order != 1)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) invalid byte order <%d>")
                      ACE_TEXT (" for version <1.0>\n"),
                      this->message_state_.byte_order));
          return -1;
        }
    }
  else
    {
      // Read teh byte ORDER
      this->message_state_.byte_order     =
        (CORBA::Octet) (buf[TAO_GIOP_MESSAGE_FLAGS_OFFSET]& 0x01);

      // Read the fragment bit
      this->message_state_.more_fragments =
        (CORBA::Octet) (buf[TAO_GIOP_MESSAGE_FLAGS_OFFSET]& 0x02);

      if (TAO_debug_level > 2
          && (buf[TAO_GIOP_MESSAGE_FLAGS_OFFSET] & ~0x3) != 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) invalid flags for <%d>")
                      ACE_TEXT (" for version <%d %d> \n"),
                      buf[TAO_GIOP_MESSAGE_FLAGS_OFFSET],
                      this->message_state_.giop_version.major,
                      this->message_state_.giop_version.minor));
          return -1;
        }
    }

  // Get the message type
  this->message_state_.message_type =
    buf[TAO_GIOP_MESSAGE_TYPE_OFFSET];

  // Reset our input CDR stream
  this->message_state_.cdr.reset_byte_order (this->message_state_.byte_order);


  this->message_state_.cdr.skip_bytes (TAO_GIOP_MESSAGE_SIZE_OFFSET);
  this->message_state_.cdr.read_ulong (this->message_state_.message_size);

  if (TAO_debug_level > 2)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) Parsed header = <%d,%d,%d,%d,%d>\n"),
                  this->message_state_.giop_version.major,
                  this->message_state_.giop_version.minor,
                  this->message_state_.byte_order,
                  this->message_state_.message_type,
                  this->message_state_.message_size));
    }

  return 1;
}


int
TAO_GIOP_Message_Base::parse_magic_bytes (void)
{
  // Grab the read pointer
  char *buf = this->message_state_.cdr.rd_ptr ();

  // The values are hard-coded to support non-ASCII platforms.
  if (!(buf [0] == 0x47      // 'G'
        && buf [1] == 0x49   // 'I'
        && buf [2] == 0x4f   // 'O'
        && buf [3] == 0x50)) // 'P'
    {
      // For the present...
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO (%P|%t) bad header, magic word [%c%c%c%c]\n"),
                    buf[0],
                    buf[1],
                    buf[2],
                    buf[3]));
      return -1;
    }

  if (this->validate_version () == -1)
    {
      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%N|%l|%P|%t) Error in validating")
                      ACE_TEXT ("revision \n")));
          return -1;
        }
    }

  return 0;
}


int
TAO_GIOP_Message_Base::validate_version (void)
{
  // Grab the read pointer
  char *buf = this->message_state_.cdr.rd_ptr ();

  CORBA::Octet incoming_major =
    buf[TAO_GIOP_VERSION_MAJOR_OFFSET];
  CORBA::Octet incoming_minor =
    buf[TAO_GIOP_VERSION_MINOR_OFFSET];

  if (this->tao_giop_impl_.check_revision (incoming_major,
                                           incoming_minor) == 0)
    {
      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t|%N|%l) bad version <%d.%d>\n"),
                      incoming_major, incoming_minor));
        }

      return -1;
    }

  // Sets the version
  this->message_state_.giop_version.minor = incoming_minor;
  this->message_state_.giop_version.major = incoming_major;

  // Sets the state
  this->set_state (incoming_major,
                   incoming_minor);

  return 0;
}


void
TAO_GIOP_Message_Base::set_state (CORBA::Octet def_major,
                                  CORBA::Octet def_minor)
{
  // @@Bala Need to find a better way
  // @@ Bala: what about a table:
  // Accept_State implementations[] = {
  //   Version_10,
  //   Version_11
  // };
  // this->accept_state_ = &implementations[def_minor];
  // @@ of course it requires range checking and the array must be in
  // some place where it is not detroyed too soon...
  // @@ I'm not sure if this implementations should be dynamically
  // loaded.
  switch (def_major)
    {
    case 1:
      switch (def_minor)
        {
        case 0:
          this->generator_parser_ =
            &this->tao_giop_impl_.tao_giop_10;
          break;
        case 1:
          this->generator_parser_ =
            &this->tao_giop_impl_.tao_giop_11;
          break;
        case 2:
          this->generator_parser_ =
            &this->tao_giop_impl_.tao_giop_12;
          break;
        default:
          break;
        }
      break;
    default:
      break;
    }
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
TAO_GIOP_Message_Base::
  send_close_connection (const TAO_GIOP_Version &version,
                         TAO_Transport *transport,
                         void *)
{

  // static CORBA::Octet
  // I hate  this in every method. Till the time I figure out a way
  // around  I will have them here hanging around.
  const char close_message [TAO_GIOP_MESSAGE_HEADER_LEN] =
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

  this->dump_msg ("send_close_connection",
                  (const u_char *) close_message,
                  TAO_GIOP_MESSAGE_HEADER_LEN);

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
                       TAO_GIOP_MESSAGE_HEADER_LEN) == -1)
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


int
TAO_GIOP_Message_Base::send_reply_exception (
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

  TAO_Pluggable_Reply_Params reply_params (orb_core);
  reply_params.request_id_ = request_id;
  reply_params.svc_ctx_.length (0);

  // We are going to send some data
  reply_params.argument_flag_ = 1;

  // Send back the service context we received.  (RTCORBA relies on
  // this).
  reply_params.service_context_notowned (svc_info);

  reply_params.reply_status_ = TAO_GIOP_USER_EXCEPTION;

  if (CORBA::SystemException::_downcast (x) != 0)
    {
      reply_params.reply_status_ = TAO_GIOP_SYSTEM_EXCEPTION;
    }

  if (this->generate_exception_reply (output,
                                      reply_params,
                                      *x) == -1)
    return -1;

  return transport->send_message (output);
}


void
TAO_GIOP_Message_Base::dump_msg (const char *label,
                                 const u_char *ptr,
                                 size_t len)
{
  static const char digits [] = "0123456789ABCD";
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
      u_long slot = ptr[TAO_GIOP_MESSAGE_TYPE_OFFSET];
      if (slot < sizeof (names)/sizeof(names[0]))
        message_name = names [slot];

      // Byte order.
      int byte_order = ptr[TAO_GIOP_MESSAGE_FLAGS_OFFSET] & 0x01;

      // request/reply id.
      CORBA::ULong tmp = 0;
      CORBA::ULong *id = &tmp;

      if (ptr[TAO_GIOP_MESSAGE_TYPE_OFFSET] == TAO_GIOP_REQUEST ||
          ptr[TAO_GIOP_MESSAGE_TYPE_OFFSET] == TAO_GIOP_REPLY)
        {
          // @@ Only works if ServiceContextList is empty....
          if (this->generator_parser_->minor_version () < 2)
            {
              id = ACE_reinterpret_cast (CORBA::ULong *,
                                         (char * ) (ptr + TAO_GIOP_MESSAGE_HEADER_LEN  + 4));

            }
          else
            {
              id = ACE_reinterpret_cast (CORBA::ULong *,
                                         (char * ) (ptr + TAO_GIOP_MESSAGE_HEADER_LEN));
            }
        }

      // Print.
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P | %t):%s GIOP v%c.%c msg, ")
                  ACE_TEXT ("%d data bytes, %s endian, %s = %d\n"),
                  label,
                  digits[ptr[TAO_GIOP_VERSION_MAJOR_OFFSET]],
                  digits[ptr[TAO_GIOP_VERSION_MINOR_OFFSET]],
                  len - TAO_GIOP_MESSAGE_HEADER_LEN ,
                  (byte_order == TAO_ENCAP_BYTE_ORDER) ? "my" : "other",
                  message_name,
                  *id));

      if (TAO_debug_level >= 10)
        ACE_HEX_DUMP ((LM_DEBUG,
                       (const char *) ptr,
                       len,
                       ACE_TEXT ("GIOP message")));


    }
}


int
TAO_GIOP_Message_Base::generate_locate_reply_header (
    TAO_OutputCDR & /*cdr*/,
    TAO_Pluggable_Reply_Params & /*params*/)
{
  return 0;
}
