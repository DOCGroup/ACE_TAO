// $Id$

#include "GIOP_Message_Base.h"
#include "operation_details.h"
#include "GIOP_Utils.h"
#include "debug.h"
#include "ORB_Core.h"
#include "Leader_Follower.h"
#include "TAO_Server_Request.h"
#include "GIOP_Message_Locate_Header.h"
#include "Transport.h"
#include "LF_Strategy.h"
#include "Request_Dispatcher.h"

#if !defined (__ACE_INLINE__)
# include "GIOP_Message_Base.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID (tao, GIOP_Message_Base, "$Id$")


TAO_GIOP_Message_Base::TAO_GIOP_Message_Base (TAO_ORB_Core *orb_core,
                                              size_t /*input_cdr_size*/)
  : orb_core_ (orb_core),
    message_state_ (orb_core,
                    this)
{

}


TAO_GIOP_Message_Base::~TAO_GIOP_Message_Base (void)
{

}


void
TAO_GIOP_Message_Base::init (CORBA::Octet /*major*/,
                             CORBA::Octet /*minor*/)
{
  // Set the state
  // this->set_state (major, minor);
}


void
TAO_GIOP_Message_Base::reset (void)
{
  // no-op
}

int
TAO_GIOP_Message_Base::generate_request_header (
    TAO_Operation_Details &op,
    TAO_Target_Specification &spec,
    TAO_OutputCDR &cdr
  )
{
  // Get a parser for us
  TAO_GIOP_Message_Generator_Parser *generator_parser = 0;

  CORBA::Octet major, minor;

  cdr.get_version (major, minor);

  // Get the state information that we need to use
  this->set_state (major,
                   minor,
                   generator_parser);

  // Write the GIOP header first
  if (!this->write_protocol_header (TAO_GIOP_REQUEST,
                                    cdr))
    {
      if (TAO_debug_level)
        ACE_ERROR ((LM_ERROR,
                    ACE_LIB_TEXT ("(%P|%t) Error in writing GIOP header \n")));

      return -1;
    }

  // Now call the implementation for the rest of the header
  if (!generator_parser->write_request_header (op,
                                               spec,
                                               cdr))
    {
      if (TAO_debug_level)
        ACE_ERROR ((LM_ERROR,
                    ACE_LIB_TEXT ("(%P|%t) Error in writing request header \n")));

      return -1;
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
  // Get a parser for us
  TAO_GIOP_Message_Generator_Parser *generator_parser = 0;

  CORBA::Octet major, minor;

  cdr.get_version (major, minor);

  // Get the state information that we need to use
  this->set_state (major,
                   minor,
                   generator_parser);

  // Write the GIOP header first
  if (!this->write_protocol_header (TAO_GIOP_LOCATEREQUEST,
                                    cdr))
    {
      if (TAO_debug_level)
        ACE_ERROR ((LM_ERROR,
                    ACE_LIB_TEXT ("(%P|%t) Error in writing GIOP header \n")));

      return -1;
    }

  // Now call the implementation for the rest of the header
  if (!generator_parser->write_locate_request_header
      (op.request_id (),
       spec,
       cdr))
    {
      if (TAO_debug_level)
        ACE_ERROR ((LM_ERROR,
                    ACE_LIB_TEXT ("(%P|%t) Error in writing locate request header \n")));


      return -1;

    }

  return 0;
}

int
TAO_GIOP_Message_Base::generate_reply_header (
    TAO_OutputCDR &cdr,
    TAO_Pluggable_Reply_Params_Base &params
  )
{
  // Get a parser for us
  TAO_GIOP_Message_Generator_Parser *generator_parser = 0;

  CORBA::Octet major, minor;

  cdr.get_version (major, minor);

  // Get the state information that we need to use
  this->set_state (major,
                   minor,
                   generator_parser);

  // Write the GIOP header first
  if (!this->write_protocol_header (TAO_GIOP_REPLY,
                                    cdr))
    {
      if (TAO_debug_level)
        ACE_ERROR ((LM_ERROR,
                    ACE_LIB_TEXT ("(%P|%t) Error in writing GIOP header \n")));

      return -1;
    }

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // Now call the implementation for the rest of the header
      int result =
        generator_parser->write_reply_header (cdr,
                                              params
                                              ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (!result)
        {
          if (TAO_debug_level > 4)
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("(%P|%t) Error in writing reply ")
                        ACE_TEXT ("header\n")));

          return -1;
        }
    }
  ACE_CATCHANY
    {
      if (TAO_debug_level > 4)
        ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                             "TAO_GIOP_Message_Base::generate_reply_header");

      return -1;
    }
  ACE_ENDTRY;

  return 0;
}


int
TAO_GIOP_Message_Base::read_message (TAO_Transport * /*transport*/,
                                     int /*block */,
                                     ACE_Time_Value * /*max_wait_time*/)
{
  return 0;
}

int
TAO_GIOP_Message_Base::format_message (TAO_OutputCDR &stream)
{
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

  CORBA::ULong bodylen = total_len - TAO_GIOP_MESSAGE_HEADER_LEN;

#if !defined (ACE_ENABLE_SWAP_ON_WRITE)
  *ACE_reinterpret_cast (CORBA::ULong *, buf +
                         TAO_GIOP_MESSAGE_SIZE_OFFSET) = bodylen;
#else
  if (!stream.do_byte_swap ())
    *ACE_reinterpret_cast (CORBA::ULong *,
                           buf + TAO_GIOP_MESSAGE_SIZE_OFFSET) = bodylen;
  else
    ACE_CDR::swap_4 (ACE_reinterpret_cast (char *,
                                           &bodylen),
                     buf + TAO_GIOP_MESSAGE_SIZE_OFFSET);
#endif /* ACE_ENABLE_SWAP_ON_WRITE */

  if (TAO_debug_level > 2)
    {
      // Check whether the output cdr stream is build up of multiple
      // messageblocks. If so, consolidate them to one block that can be
      // dumped
      ACE_Message_Block* consolidated_block = 0;
      if (stream.begin()->cont () != 0)
        {
          consolidated_block = new ACE_Message_Block;
          ACE_CDR::consolidate (consolidated_block, stream.begin ());
          buf = (char *) (consolidated_block->rd_ptr ());
        }
      ///
      this->dump_msg ("send",
                      ACE_reinterpret_cast (u_char *,
                                            buf),
                      total_len);

      //
      delete consolidated_block;
      consolidated_block = 0;
      //
    }

  return 0;
}

TAO_Pluggable_Message_Type
TAO_GIOP_Message_Base::message_type (
    TAO_GIOP_Message_State &msg_state)
{
  // Convert to the right type of Pluggable Messaging message type.

  switch (msg_state.message_type_)
    {
    case TAO_GIOP_REQUEST:
      return TAO_PLUGGABLE_MESSAGE_REQUEST;
    case TAO_GIOP_LOCATEREQUEST:
      return TAO_PLUGGABLE_MESSAGE_LOCATEREQUEST;

    case TAO_GIOP_LOCATEREPLY:
      return TAO_PLUGGABLE_MESSAGE_LOCATEREPLY;
    case TAO_GIOP_REPLY:
      return TAO_PLUGGABLE_MESSAGE_REPLY;

    case TAO_GIOP_CLOSECONNECTION:
      return TAO_PLUGGABLE_MESSAGE_CLOSECONNECTION;

    case TAO_GIOP_FRAGMENT:
      return TAO_PLUGGABLE_MESSAGE_FRAGMENT;

    case TAO_GIOP_MESSAGERROR:
    case TAO_GIOP_CANCELREQUEST:
      // Does it happen?  why??
    default:
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("TAO (%P|%t) %N:%l        message_type : ")
                    ACE_TEXT ("wrong message.\n")));
    }

  return TAO_PLUGGABLE_MESSAGE_MESSAGERROR;
}

int
TAO_GIOP_Message_Base::process_request_message (TAO_Transport *transport,
                                                TAO_Queued_Data *qd)

{
  // Set the upcall thread
  this->orb_core_->lf_strategy ().set_upcall_thread (this->orb_core_->leader_follower ());

  // Get a parser for us
  TAO_GIOP_Message_Generator_Parser *generator_parser = 0;

  // Get the state information that we need to use
  this->set_state (qd->major_version_,
                   qd->minor_version_,
                   generator_parser);

  // A buffer that we will use to initialise the CDR stream
  char repbuf[ACE_CDR::DEFAULT_BUFSIZE];

#if defined(ACE_HAS_PURIFY)
  (void) ACE_OS::memset (repbuf,
                         '\0',
                         sizeof repbuf);
#endif /* ACE_HAS_PURIFY */

  // Initialze an output CDR on the stack
  // NOTE: Dont jump to a conclusion as to why we are using the
  // inpout_cdr and hence the  global pool here. These pools will move
  // to the lanes anyway at some point of time. Further, it would have
  // been awesome to have this in TSS. But for some reason the cloning
  // that happens when the ORB gets flow controlled while writing a
  // reply is messing things up. We crash horribly. Doing this adds a
  // lock, we need to set things like this -- put stuff in TSS here
  // and transfer to global memory when we get flow controlled. We
  // need to work on the message block to get it right!
  TAO_OutputCDR output (repbuf,
                        sizeof repbuf,
                        TAO_ENCAP_BYTE_ORDER,
                        this->orb_core_->input_cdr_buffer_allocator (),
                        this->orb_core_->input_cdr_dblock_allocator (),
                        this->orb_core_->input_cdr_msgblock_allocator (),
                        this->orb_core_->orb_params ()->cdr_memcpy_tradeoff (),
                        qd->major_version_,
                        qd->minor_version_,
                        this->orb_core_->to_iso8859 (),
                        this->orb_core_->to_unicode ());

  // Get the read and write positions before we steal data.
  size_t rd_pos = qd->msg_block_->rd_ptr () - qd->msg_block_->base ();
  size_t wr_pos = qd->msg_block_->wr_ptr () - qd->msg_block_->base ();
  rd_pos += TAO_GIOP_MESSAGE_HEADER_LEN;

  if (TAO_debug_level > 0)
    this->dump_msg ("recv",
                    ACE_reinterpret_cast (u_char *,
                                          qd->msg_block_->rd_ptr ()),
                    qd->msg_block_->length ());


  // Create a input CDR stream. We do the following
  //  1 - If the incoming message block has a data block with a flag
  //      DONT_DELETE  (for the data block) we create an input CDR
  //      stream the same way.
  //  2 - If the incoming message block had a datablock from heap just
  //      use it by duplicating it and make the flag 0.
  // NOTE: We use the same data block in which we read the message and
  // we pass it on to the higher layers of the ORB. So we dont to any
  // copies at all here. The same is also done in the higher layers.

  ACE_Message_Block::Message_Flags flg = 0;
  ACE_Data_Block *db = 0;

  // Get the flag in the message block
  flg = qd->msg_block_->self_flags ();

  if (ACE_BIT_ENABLED (flg,
                       ACE_Message_Block::DONT_DELETE))
    {
      // Use the same datablock
      db = qd->msg_block_->data_block ();
    }
  else
    {
      // Use a duplicated datablock as the datablock has come off the
      // heap.
      db = qd->msg_block_->data_block ()->duplicate ();
    }


  TAO_InputCDR input_cdr (db,
                          flg,
                          rd_pos,
                          wr_pos,
                          qd->byte_order_,
                          qd->major_version_,
                          qd->minor_version_,
                          this->orb_core_);


  // We know we have some request message. Check whether it is a
  // GIOP_REQUEST or GIOP_LOCATE_REQUEST to take action.

  // Once we send the InputCDR stream we need to just forget about
  // the stream and never touch that again for anything. We basically
  // loose ownership of the data_block.

  switch (qd->msg_type_)
    {
    case TAO_PLUGGABLE_MESSAGE_REQUEST:
      // Should be taken care by the state specific invocations. They
      // could raise an exception or write things in the output CDR
      // stream
      return this->process_request (transport,
                                    input_cdr,
                                    output,
                                    generator_parser);

    case TAO_PLUGGABLE_MESSAGE_LOCATEREQUEST:
      return this->process_locate_request (transport,
                                           input_cdr,
                                           output,
                                           generator_parser);
    default:
      return -1;
    }
}

int
TAO_GIOP_Message_Base::process_reply_message (
    TAO_Pluggable_Reply_Params &params,
    TAO_Queued_Data *qd)
{
  // Get a parser for us
  TAO_GIOP_Message_Generator_Parser *generator_parser = 0;

  // Get the state information that we need to use
  this->set_state (qd->major_version_,
                   qd->minor_version_,
                   generator_parser);

  // Get the read and write positions before we steal data.
  size_t rd_pos = qd->msg_block_->rd_ptr () - qd->msg_block_->base ();
  size_t wr_pos = qd->msg_block_->wr_ptr () - qd->msg_block_->base ();
  rd_pos += TAO_GIOP_MESSAGE_HEADER_LEN;

  if (TAO_debug_level > 0)
    this->dump_msg ("recv",
                    ACE_reinterpret_cast (u_char *,
                                          qd->msg_block_->rd_ptr ()),
                    qd->msg_block_->length ());


  // Create a empty buffer on stack
  // NOTE: We use the same data block in which we read the message and
  // we pass it on to the higher layers of the ORB. So we dont to any
  // copies at all here. The same is alos done in the higher layers.
  TAO_InputCDR input_cdr (qd->msg_block_->data_block (),
                          ACE_Message_Block::DONT_DELETE,
                          rd_pos,
                          wr_pos,
                          qd->byte_order_,
                          qd->major_version_,
                          qd->minor_version_,
                          this->orb_core_);

  // Reset the message state. Now, we are ready for the next nested
  // upcall if any.
  // this->message_handler_.reset (0);

  // We know we have some reply message. Check whether it is a
  // GIOP_REPLY or GIOP_LOCATE_REPLY to take action.

  // Once we send the InputCDR stream we need to just forget about
  // the stream and never touch that again for anything. We basically
  // loose ownership of the data_block.

  switch (qd->msg_type_)
    {
    case TAO_PLUGGABLE_MESSAGE_REPLY:
      // Should be taken care by the state specific parsing
      return generator_parser->parse_reply (input_cdr,
                                            params);

    case TAO_PLUGGABLE_MESSAGE_LOCATEREPLY:
      return generator_parser->parse_locate_reply (input_cdr,
                                                   params);
    default:
        return -1;
    }
}

int
TAO_GIOP_Message_Base::generate_exception_reply (
    TAO_OutputCDR &cdr,
    TAO_Pluggable_Reply_Params_Base &params,
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
      x._tao_encode (cdr ACE_ENV_ARG_PARAMETER);
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
TAO_GIOP_Message_Base::write_protocol_header (TAO_GIOP_Message_Type t,
                                              TAO_OutputCDR &msg)
{
  // Reset the message type
  // Reset the message type
  msg.reset ();

  CORBA::Octet header[12] =
  {
    // The following works on non-ASCII platforms, such as MVS (which
    // uses EBCDIC).
    0x47, // 'G'
    0x49, // 'I'
    0x4f, // 'O'
    0x50  // 'P'
  };

  CORBA::Octet major, minor = 0;
  msg.get_version (major, minor);

  header[4] = major;
  header[5] = minor;

  // We are putting the byte order. But at a later date if we support
  // fragmentation and when we want to use the other 6 bits in this
  // octet we can have a virtual function do this for us as the
  // version info , Bala
  header[6] = (TAO_ENCAP_BYTE_ORDER ^ msg.do_byte_swap ());

  header[7] = CORBA::Octet(t);

  static int header_size = sizeof (header) / sizeof (header[0]);
  msg.write_octet_array (header, header_size);

  return msg.good_bit ();
}

int
TAO_GIOP_Message_Base::process_request (TAO_Transport *transport,
                                        TAO_InputCDR &cdr,
                                        TAO_OutputCDR &output,
                                        TAO_GIOP_Message_Generator_Parser *parser)
{
  // This will extract the request header, set <response_required>
  // and <sync_with_server> as appropriate.
  TAO_ServerRequest request (this,
                             cdr,
                             output,
                             transport,
                             this->orb_core_);

  CORBA::ULong request_id = 0;
  CORBA::Boolean response_required = 0;

  int parse_error = 0;

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      parse_error =
        parser->parse_request_header (request);

      // Throw an exception if the
      if (parse_error != 0)
        ACE_TRY_THROW (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE,
                                       CORBA::COMPLETED_NO));
      request_id = request.request_id ();

      response_required = request.response_expected ();

      CORBA::Object_var forward_to;

      // Do this before the reply is sent.
      this->orb_core_->request_dispatcher ()->dispatch (
          this->orb_core_,
          request,
          forward_to
          ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (!CORBA::is_nil (forward_to.in ()))
        {
          // We should forward to another object...
          TAO_Pluggable_Reply_Params_Base reply_params;
          reply_params.request_id_ = request_id;
          reply_params.reply_status_ = TAO_GIOP_LOCATION_FORWARD;
          reply_params.svc_ctx_.length (0);

          // Send back the reply service context.
          reply_params.service_context_notowned (&request.reply_service_info ());

          // Make the GIOP header and Reply header
          this->generate_reply_header (output,
                                       reply_params);

          if (!(output << forward_to.in ()))
            {
              if (TAO_debug_level > 0)
                ACE_ERROR ((LM_ERROR,
                            ACE_TEXT ("TAO (%P|%t) ERROR: Unable to marshal ")
                            ACE_TEXT ("forward reference.\n")));

              return -1;
            }

          int result = transport->send_message (output,
                                                0,
                                                TAO_Transport::TAO_REPLY);
          if (result == -1)
            {
              if (TAO_debug_level > 0)
                {
                  // No exception but some kind of error, yet a
                  // response is required.
                  ACE_ERROR ((LM_ERROR,
                              ACE_TEXT ("TAO: (%P|%t|%N|%l) %p: ")
                              ACE_TEXT ("cannot send reply\n"),
                              ACE_TEXT ("TAO_GIOP_Message_Base::process_request")));
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
                                               output,
                                               request_id,
                                               &request.reply_service_info (),
                                               &ACE_ANY_EXCEPTION);
          if (result == -1)
            {
              if (TAO_debug_level > 0)
                {
                  ACE_ERROR ((LM_ERROR,
                              ACE_TEXT ("TAO: (%P|%t|%N|%l) %p: ")
                              ACE_TEXT ("cannot send exception\n"),
                              ACE_TEXT ("process_connector_request ()")));

                  ACE_PRINT_EXCEPTION (
                      ACE_ANY_EXCEPTION,
                      "TAO_GIOP_Message_Base::process_request[1]");
                }
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

          ACE_PRINT_EXCEPTION (
              ACE_ANY_EXCEPTION,
              "TAO_GIOP_Message_Base::process_request[2]");
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
          CORBA::UNKNOWN exception (CORBA::SystemException::_tao_minor_code
                                    (TAO_UNHANDLED_SERVER_CXX_EXCEPTION, 0),
                                    CORBA::COMPLETED_MAYBE);

          result = this->send_reply_exception (transport,
                                               output,
                                               request_id,
                                               &request.reply_service_info (),
                                               &exception);
          if (result == -1)
            {
              if (TAO_debug_level > 0)
                {
                  ACE_ERROR ((LM_ERROR,
                              ACE_TEXT ("TAO: (%P|%t|%N|%l) %p: ")
                              ACE_TEXT ("cannot send exception\n"),
                              ACE_TEXT ("process_request ()")));
                  ACE_PRINT_EXCEPTION (
                      exception,
                      "TAO_GIOP_Message_Base::process_request[3]");
                }
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
                                               TAO_InputCDR &input,
                                               TAO_OutputCDR &output,
                                               TAO_GIOP_Message_Generator_Parser *parser)
{
  // This will extract the request header, set <response_required> as
  // appropriate.
  TAO_GIOP_Locate_Request_Header locate_request (input,
                                                 this->orb_core_);

  TAO_GIOP_Locate_Status_Msg status_info;

  // Defaulting.
  status_info.status = TAO_GIOP_UNKNOWN_OBJECT;

  CORBA::Boolean response_required = 1;

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      int parse_error =
        parser->parse_locate_header (locate_request);

      if (parse_error != 0)
        {
          ACE_TRY_THROW (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE,
                                         CORBA::COMPLETED_NO));
        }

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
                                        output,
                                        transport,
                                        this->orb_core_,
                                        parse_error);

      if (parse_error != 0)
        {
          ACE_TRY_THROW (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE,
                                         CORBA::COMPLETED_NO));
        }

      CORBA::Object_var forward_to;

      this->orb_core_->request_dispatcher ()->dispatch (
          this->orb_core_,
          server_request,
          forward_to
           ACE_ENV_ARG_PARAMETER);
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
                    ACE_TEXT ("TAO (%P|%t) TAO_GIOP_Message_Base::process_locate_request - ")
                    ACE_TEXT ("CORBA exception raised\n")));
    }
#if defined (TAO_HAS_EXCEPTIONS)
  ACE_CATCHALL
    {
      // Normal exception, so the object is not here
      status_info.status = TAO_GIOP_UNKNOWN_OBJECT;
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO (%P|%t) TAO_GIOP_Message_Base::process_locate_request - ")
                    ACE_TEXT ("C++ exception raised\n")));
    }
#endif /* TAO_HAS_EXCEPTIONS */
  ACE_ENDTRY;

  return this->make_send_locate_reply (transport,
                                       locate_request,
                                       status_info,
                                       output,
                                       parser);
}

int
TAO_GIOP_Message_Base::make_send_locate_reply (TAO_Transport *transport,
                                               TAO_GIOP_Locate_Request_Header &request,
                                               TAO_GIOP_Locate_Status_Msg &status_info,
                                               TAO_OutputCDR &output,
                                               TAO_GIOP_Message_Generator_Parser *parser)
{
  // Note here we are making the Locate reply header which is *QUITE*
  // different from the reply header made by the make_reply () call..
  // Make the GIOP message header
  this->write_protocol_header (TAO_GIOP_LOCATEREPLY,
                               output);

  // This writes the header & body
  parser->write_locate_reply_mesg (output,
                                   request.request_id (),
                                   status_info);

  // Send the message
  int result = transport->send_message (output,
                                        0,
                                        TAO_Transport::TAO_REPLY);

  // Print out message if there is an error
  if (result == -1)
    {
      if (TAO_debug_level > 0)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("TAO: (%P|%t) %p: cannot send reply\n"),
                      ACE_TEXT ("TAO_GIOP_Message_Base::make_send_locate_reply")));
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
    (CORBA::Octet) 1, // Use the lowest GIOP version
    (CORBA::Octet) 0,
    TAO_ENCAP_BYTE_ORDER,
    TAO_GIOP_MESSAGERROR,
    0, 0, 0, 0
  };

  // @@ Q: How does this works with GIOP lite?
  //    A: It doesn't

  this->dump_msg ("send_error",
                  (const u_char *) error_message,
                  TAO_GIOP_MESSAGE_HEADER_LEN);

  ACE_Data_Block data_block (TAO_GIOP_MESSAGE_HEADER_LEN,
                             ACE_Message_Block::MB_DATA,
                             error_message,
                             0,
                             0,
                             ACE_Message_Block::DONT_DELETE,
                             0);
  ACE_Message_Block message_block(&data_block,
                                  ACE_Message_Block::DONT_DELETE);
  message_block.wr_ptr (TAO_GIOP_MESSAGE_HEADER_LEN);

  size_t bt;
  int result = transport->send_message_block_chain (&message_block, bt);
  if (result == -1)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO (%N|%l|%P|%t) error sending error to transport %lu\n"),
                    transport->id ()));
    }

  return result;
}

void
TAO_GIOP_Message_Base::set_state (
    CORBA::Octet def_major,
    CORBA::Octet def_minor,
    TAO_GIOP_Message_Generator_Parser *&gen_parser)
{
  switch (def_major)
    {
    case 1:
      switch (def_minor)
        {
        case 0:
          gen_parser =
            &this->tao_giop_impl_.tao_giop_10;
          break;
        case 1:
          gen_parser =
            &this->tao_giop_impl_.tao_giop_11;
          break;
        case 2:
          gen_parser =
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
  send_close_connection (const TAO_GIOP_Message_Version &version,
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

#if 0
  // @@CJC I don't think we need this check b/c the transport's send()
  // will simply return -1.  However, I guess we could create something
  // like TAO_Tranport::is_closed() that returns whether the connection
  // is already closed.  The problem with that, however, is that it's
  // entirely possible that is_closed() could return TRUE, and then the
  // transport could get closed down btw. the time it gets called and the
  // time that the send actually occurs.
  ACE_HANDLE which = transport->handle ();
  if (which == ACE_INVALID_HANDLE)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t) TAO_GIOP_Message_Base::send_close_connection -"
                    " connection already closed\n"));
      return;
    }
#endif

  ACE_Data_Block data_block (TAO_GIOP_MESSAGE_HEADER_LEN,
                             ACE_Message_Block::MB_DATA,
                             close_message,
                             0,
                             0,
                             ACE_Message_Block::DONT_DELETE,
                             0);
  ACE_Message_Block message_block(&data_block);
  message_block.wr_ptr (TAO_GIOP_MESSAGE_HEADER_LEN);

  size_t bt;
  int result = transport->send_message_block_chain (&message_block, bt);
  if (result == -1)
    {
      if (TAO_debug_level > 0)
        ACE_ERROR ((LM_ERROR,
                    "(%P|%t) error closing connection %lu, errno = %d\n",
                    transport->id (), errno));
    }

  transport->close_connection ();
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) shut down transport, handle %d\n",
              transport-> id ()));

}


int
TAO_GIOP_Message_Base::send_reply_exception (
    TAO_Transport *transport,
    TAO_OutputCDR &output,
    CORBA::ULong request_id,
    IOP::ServiceContextList *svc_info,
    CORBA::Exception *x
  )
{
  TAO_Pluggable_Reply_Params_Base reply_params;
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

  return transport->send_message (output,
                                  0,
                                  TAO_Transport::TAO_REPLY);
}

void
TAO_GIOP_Message_Base::dump_msg (const char *label,
                                 const u_char *ptr,
                                 size_t len)
{

  if (TAO_debug_level >= 5)
    {
      static const char digits[] = "0123456789ABCD";
      static const char *names[] =
      {
        "Request",
        "Reply",
        "CancelRequest",
        "LocateRequest",
        "LocateReply",
        "CloseConnection",
        "MessageError",
        "Fragment"
      };

      // Message name.
      const char *message_name = "UNKNOWN MESSAGE";
      u_long slot = ptr[TAO_GIOP_MESSAGE_TYPE_OFFSET];
      if (slot < sizeof (names) / sizeof (names[0]))
        message_name = names[slot];

      // Byte order.
      int byte_order = ptr[TAO_GIOP_MESSAGE_FLAGS_OFFSET] & 0x01;

      // Get the version info
      // CORBA::Octet major = ptr[TAO_GIOP_VERSION_MAJOR_OFFSET];
      CORBA::Octet minor = ptr[TAO_GIOP_VERSION_MINOR_OFFSET];

      // request/reply id.
      CORBA::ULong tmp = 0;
      CORBA::ULong *id = &tmp;
      char *tmp_id = 0;

      if (ptr[TAO_GIOP_MESSAGE_TYPE_OFFSET] == TAO_GIOP_REQUEST ||
          ptr[TAO_GIOP_MESSAGE_TYPE_OFFSET] == TAO_GIOP_REPLY)
        {
          if (minor < 2)
            {
              // @@ Only works if ServiceContextList is empty....
              tmp_id = (char * ) (ptr + TAO_GIOP_MESSAGE_HEADER_LEN  + 4);
            }
          else
            {
              tmp_id = (char * ) (ptr + TAO_GIOP_MESSAGE_HEADER_LEN);
            }
#if !defined (ACE_DISABLE_SWAP_ON_READ)
      if (byte_order == TAO_ENCAP_BYTE_ORDER)
        {
          id = ACE_reinterpret_cast (ACE_CDR::ULong*, tmp_id);
        }
      else
        {
          ACE_CDR::swap_4 (tmp_id, ACE_reinterpret_cast (char*,id));
        }
#else
      id = ACE_reinterpret_cast(ACE_CDR::ULong*, tmp_id);
#endif /* ACE_DISABLE_SWAP_ON_READ */

        }

      // Print.
      ACE_DEBUG ((LM_DEBUG,
                  "TAO (%P|%t) - GIOP_Message_Base::dump_msg, "
                  "%s GIOP v%c.%c msg, %d data bytes, %s endian, "
                  "Type %s[%u]\n",
                  label,
                  digits[ptr[TAO_GIOP_VERSION_MAJOR_OFFSET]],
                  digits[ptr[TAO_GIOP_VERSION_MINOR_OFFSET]],
                  len - TAO_GIOP_MESSAGE_HEADER_LEN ,
                  (byte_order == TAO_ENCAP_BYTE_ORDER) ? ACE_LIB_TEXT("my") : ACE_LIB_TEXT("other"),
                  ACE_TEXT_CHAR_TO_TCHAR(message_name),
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
    TAO_Pluggable_Reply_Params_Base & /*params*/)
{
  return 0;
}

int
TAO_GIOP_Message_Base::is_ready_for_bidirectional (TAO_OutputCDR &msg)
{
  // Get a parser for us
  TAO_GIOP_Message_Generator_Parser *parser = 0;

  CORBA::Octet major, minor = 0;

  msg.get_version (major, minor);

  // Get the state information that we need to use
  this->set_state (major,
                   minor,
                   parser);

  // We dont really know.. So ask the generator and parser objects that
  // we know.
  // @@ TODO: Need to make this faster, instead of making virtual
  // call, try todo the check within this class
  return parser->is_ready_for_bidirectional ();
}


void
TAO_GIOP_Message_Base::set_queued_data_from_message_header (
  TAO_Queued_Data *qd,
  const ACE_Message_Block &mb
  ) const
{
  // @@CJC: Try leaving out the declaration for this->message_state_
  // and see what pukes.  I don't think we need it any more.
  TAO_GIOP_Message_State state;
  if (state.take_values_from_message_block (mb) == -1)
    {
      // what the heck do we do here?!
      qd->current_state_ = TAO_Queued_Data::INVALID;
      return;
    }

  // It'd be nice to have an abstract base for GIOP_Message_State
  // so that there could just be a line like:
  //   qd->take_values_from (state);
  // Get the message information
  qd->byte_order_ = state.byte_order ();
  qd->major_version_ = state.giop_version ().major;
  qd->minor_version_ = state.giop_version ().minor;
  qd->more_fragments_ = state.more_fragments () ? 1 : 0;
  qd->msg_type_= message_type (state);
  qd->missing_data_bytes_ = state.payload_size ();
}

int
TAO_GIOP_Message_Base::check_for_valid_header (
  const ACE_Message_Block &mb
  ) const
{
  // NOTE!  We don't hardcode the length of the header b/c header_length should
  // be eligible for inlining by pretty much any compiler, and it should return
  // a constant.  The rest of this method is hard-coded and hand-optimized because
  // this method gets called A LOT.
  if (mb.length () < this->header_length ())
    return 0;

  // Is finding that it's the right length and the magic bytes present
  // enough to declare it a valid header?  I think so...
  register const char* h = mb.rd_ptr ();
  return (h[0] == 'G' && h[1] == 'I' && h[2] == 'O' && h[3] == 'P');
}
