// -*- C++ -*-
//
//$Id$

#include "tao/GIOP_Message_Lite.h"
#include "tao/debug.h"
#include "tao/TAOC.h"
#include "tao/ORB_Core.h"
#include "tao/GIOP_Utils.h"
#include "tao/operation_details.h"
#include "tao/TAO_Server_Request.h"
#include "tao/GIOP_Message_Locate_Header.h"
#include "tao/target_specification.h"
#include "tao/Leader_Follower.h"
#include "tao/LF_Strategy.h"
#include "tao/Transport.h"
#include "tao/Codeset_Manager.h"

#if !defined (__ACE_INLINE__)
# include "tao/GIOP_Message_Lite.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID (tao,
           GIOP_Message_Lite,
           "$Id$")

static const size_t TAO_GIOP_LITE_HEADER_LEN = 5;
static const size_t TAO_GIOP_LITE_MESSAGE_SIZE_OFFSET = 0;
static const size_t TAO_GIOP_LITE_MESSAGE_TYPE_OFFSET = 4;

TAO_GIOP_Message_Lite::TAO_GIOP_Message_Lite (TAO_ORB_Core *orb_core,
                                              size_t /*input_cdr_size*/)
  : orb_core_ (orb_core),
    message_type_ (0),
    message_size_ (0),
    byte_order_ (ACE_CDR_BYTE_ORDER)
{
}


TAO_GIOP_Message_Lite::~TAO_GIOP_Message_Lite (void)
{
}


void
TAO_GIOP_Message_Lite::init (CORBA::Octet,
                             CORBA::Octet)
{
  return;
}

TAO_OutputCDR &
TAO_GIOP_Message_Lite::out_stream (void)
{
  return this->cdr_;
}

void
TAO_GIOP_Message_Lite::reset (void)
{
  this->message_type_ = 0;
  this->message_size_ = 0;
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
      if (TAO_debug_level)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("(%P|%t) Error in writing GIOP header \n")));


      return -1;
    }

  // Now call the implementation for the rest of the header
  if (!this->write_request_header (op,
                                   spec,
                                   cdr))
    {
      if (TAO_debug_level)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("(%P|%t) Error in writing request header \n")));

      return -1;
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
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("(%P|%t) Error in writing GIOPLite header \n")));

      return -1;
    }

  // Now call the implementation for the rest of the header
  if (!this->write_locate_request_header (op.request_id (),
                                          spec,
                                          cdr))
    {
      if (TAO_debug_level > 4)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("(%P|%t) Error in writing locate request header \n")));

      return -1;
    }

  return 0;
}


int
TAO_GIOP_Message_Lite::generate_reply_header (
    TAO_OutputCDR &cdr,
    TAO_Pluggable_Reply_Params_Base &params
  )
{
  // Write the GIOPLite header first
  if (!this->write_protocol_header (TAO_GIOP_REPLY,
                                    cdr))
    {
      if (TAO_debug_level > 3)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("(%P|%t) Error in writing GIOPLite header \n")));

      return -1;
    }

  // Now call the implementation for the rest of the header
  if (!this->write_reply_header (cdr,
                                 params))
    {
      if (TAO_debug_level > 4)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("(%P|%t) Error in writing reply header \n")));

      return -1;
    }

  return 0;
}


int
TAO_GIOP_Message_Lite::read_message (TAO_Transport * /*transport*/,
                                     int /*block */,
                                     ACE_Time_Value * /*max_wait_time*/)
{
  return 1;
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

  CORBA::ULong bodylen = ACE_static_cast (CORBA::ULong,
                                          total_len - header_len);

#if !defined (ACE_ENABLE_SWAP_ON_WRITE)
  *ACE_reinterpret_cast (CORBA::ULong *, buf + offset) = bodylen;
#else
  if (!stream.do_byte_swap ())
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
      // Check whether the output cdr stream is build up of multiple
      // messageblocks. If so, consolidate them to one block that can be
      // dumped
      ACE_Message_Block* consolidated_block = 0;
      if (stream.begin()->cont() != 0)
        {
          consolidated_block = new ACE_Message_Block;
          ACE_CDR::consolidate(consolidated_block, stream.begin());
          buf = (char *) (consolidated_block->rd_ptr ());
        }
      ///

      this->dump_msg ("send",
                      ACE_reinterpret_cast (u_char *,
                                            buf),
                      stream.length ());

      //
      delete consolidated_block;
      consolidated_block = 0;
      //
    }

  return 0;
}


int
TAO_GIOP_Message_Lite::parse_incoming_messages (ACE_Message_Block &block)
{
    // Get the read pointer
  char *rd_ptr = block.rd_ptr ();

  // We dont need to do this sort of copy. But some compilers (read it
  // as solaris ones) have a problem in deferencing from the
  // reinterpret_cast pointer of the <rd_ptr>, as the <rd_ptr> can be
  // on stack. So let us go ahead with this copying...
  char buf [4];
  buf[0] = *rd_ptr;
  buf[1] = *(rd_ptr + 1);
  buf[2] = *(rd_ptr + 2);
  buf[3] = *(rd_ptr + 3);

  CORBA::ULong x = 0;
#if !defined (ACE_DISABLE_SWAP_ON_READ)
  if (!(this->byte_order_ != ACE_CDR_BYTE_ORDER))
    {
      x = *ACE_reinterpret_cast (ACE_CDR::ULong*, buf);
    }
  else
    {
      ACE_CDR::swap_4 (buf, ACE_reinterpret_cast (char*, &x));
    }
#else
  x = *ACE_reinterpret_cast (ACE_CDR::ULong*, buf);
#endif /* ACE_DISABLE_SWAP_ON_READ */

  this->message_size_ = x;

  // Get the message type.
  this->message_type_ = rd_ptr[TAO_GIOP_LITE_MESSAGE_TYPE_OFFSET];

  return 0;
}

TAO_Pluggable_Message_Type
TAO_GIOP_Message_Lite::message_type (void)
{
  switch (this->message_type_)
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


ssize_t
TAO_GIOP_Message_Lite::missing_data (ACE_Message_Block &block)
{
    // Actual message size including the header..
  CORBA::ULong msg_size =
    this->message_size_ + TAO_GIOP_LITE_HEADER_LEN;

  size_t len = block.length ();

  if (len > msg_size)
    {
      return -1;
     }
  else if (len == msg_size)
     return 0;

   return msg_size - len;
}


int
TAO_GIOP_Message_Lite::extract_next_message (ACE_Message_Block &incoming,
                                             TAO_Queued_Data *&qd)
{
  if (incoming.length () < TAO_GIOP_LITE_HEADER_LEN)
    {
      if (incoming.length () > 0)
        {
          // Make a node which has a message block of the size of
          // MESSAGE_HEADER_LEN.
          qd =
            this->make_queued_data (TAO_GIOP_LITE_HEADER_LEN);

            qd->msg_block_->copy (incoming.rd_ptr (),
                                  incoming.length ());
          qd->missing_data_ = -1;
        }
      return 0;
    }

  if (this->parse_incoming_messages (incoming) == -1)
    {
      return -1;
    }

  size_t copying_len =
    this->message_size_ + TAO_GIOP_LITE_HEADER_LEN;

  qd = this->make_queued_data (copying_len);

  if (copying_len > incoming.length ())
    {
      qd->missing_data_ =
        copying_len - incoming.length ();

      copying_len = incoming.length ();
    }

  qd->msg_block_->copy (incoming.rd_ptr (),
                        copying_len);

  incoming.rd_ptr (copying_len);
  qd->byte_order_ = TAO_ENCAP_BYTE_ORDER;
  qd->major_version_ = TAO_DEF_GIOP_MAJOR;
  qd->minor_version_ = TAO_DEF_GIOP_MINOR;
  qd->msg_type_ = this->message_type ();
  return 1;
}

int
TAO_GIOP_Message_Lite::consolidate_node (TAO_Queued_Data *qd,
                                         ACE_Message_Block &incoming)
{
  // Look to see whether we had atleast parsed the GIOP header ...
  if (qd->missing_data_ == -1)
    {
      // The data length that has been stuck in there during the last
      // read ....
      size_t len =
        qd->msg_block_->length ();

      // We know that we would have space for
      // TAO_GIOP_MESSAGE_HEADER_LEN here.  So copy that much of data
      // from the <incoming> into the message block in <qd>
      qd->msg_block_->copy (incoming.rd_ptr (),
                            TAO_GIOP_LITE_HEADER_LEN - len);

      // Move the rd_ptr () in the incoming message block..
      incoming.rd_ptr (TAO_GIOP_LITE_HEADER_LEN - len);

      // Parse the message header now...
      if (this->parse_incoming_messages (*qd->msg_block_) == -1)
        return -1;

      // Now grow the message block so that we can copy the rest of
      // the data...
      ACE_CDR::grow (qd->msg_block_,
                     this->message_size_ + TAO_GIOP_LITE_HEADER_LEN);

      // Copy the pay load..

      // Calculate the bytes that needs to be copied in the queue...
      size_t copy_len = this->message_size_;

      // If teh data that needs to be copied is more than that is
      // available to us ..
      if (copy_len > incoming.length ())
        {
          // Calculate the missing data..
          qd->missing_data_ =
            copy_len - incoming.length ();

          // Set the actual possible copy_len that is available...
          copy_len = incoming.length ();
        }
      else
        {
          qd->missing_data_ = 0;
        }

      // ..now we are set to copy the right amount of data to the
      // node..
      qd->msg_block_->copy (incoming.rd_ptr (),
                            copy_len);

      // Set the <rd_ptr> of the <incoming>..
      incoming.rd_ptr (copy_len);

      // Get the other details...
      qd->byte_order_ = TAO_ENCAP_BYTE_ORDER;
      qd->major_version_ = TAO_DEF_GIOP_MAJOR;
      qd->minor_version_ = TAO_DEF_GIOP_MINOR;
      qd->msg_type_ = this->message_type ();
    }
  else
    {
      // @@todo: Need to abstract this out to a seperate method...
      size_t copy_len = qd->missing_data_;

      if (copy_len > incoming.length ())
        {
          // Calculate the missing data..
          qd->missing_data_ =
            copy_len - incoming.length ();

          // Set the actual possible copy_len that is available...
          copy_len = incoming.length ();
        }

      // Copy the right amount of data in to the node...
      // node..
      qd->msg_block_->copy (incoming.rd_ptr (),
                            copy_len);

      // Set the <rd_ptr> of the <incoming>..
      qd->msg_block_->rd_ptr (copy_len);

    }

  return 0;
}


void
TAO_GIOP_Message_Lite::get_message_data (TAO_Queued_Data *qd)
{
  // Get the message information
  qd->byte_order_ =
    ACE_CDR_BYTE_ORDER;
  qd->major_version_ =
    TAO_DEF_GIOP_MAJOR;
  qd->minor_version_ =
    TAO_DEF_GIOP_MINOR;

  qd->msg_type_=
    this->message_type ();

  this->reset ();
}

int
TAO_GIOP_Message_Lite::consolidate_fragments (TAO_Queued_Data * /*dqd*/,
                                              const TAO_Queued_Data * /*sqd*/)
{
  // We dont know what fragments are???
  return -1;
}

int
TAO_GIOP_Message_Lite::process_request_message (TAO_Transport *transport,
                                                TAO_Queued_Data *qd)
{
  // Set the upcall thread
  this->orb_core_->lf_strategy ().set_upcall_thread (
      this->orb_core_->leader_follower ());

  // A buffer that we will use to initialise the CDR stream
  char repbuf[ACE_CDR::DEFAULT_BUFSIZE];

#if defined(ACE_HAS_PURIFY)
  (void) ACE_OS::memset (repbuf,
                         '\0',
                         sizeof repbuf);
#endif /* ACE_HAS_PURIFY */

  // Initialze an output CDR on the stack
  TAO_OutputCDR output (repbuf,
                        sizeof repbuf,
                        TAO_ENCAP_BYTE_ORDER,
                        this->orb_core_->output_cdr_buffer_allocator (),
                        this->orb_core_->output_cdr_dblock_allocator (),
                        this->orb_core_->output_cdr_msgblock_allocator (),
                        this->orb_core_->orb_params ()->cdr_memcpy_tradeoff (),
                        qd->major_version_,
                        qd->minor_version_);

  transport->assign_translators(0,&output);

  // Get the read and write positions before we steal data.
  size_t rd_pos = qd->msg_block_->rd_ptr () - qd->msg_block_->base ();
  size_t wr_pos = qd->msg_block_->wr_ptr () - qd->msg_block_->base ();
  rd_pos += TAO_GIOP_LITE_HEADER_LEN;

  this->dump_msg ("recv",
                  ACE_reinterpret_cast (u_char *,
                                        qd->msg_block_->rd_ptr ()),
                  qd->msg_block_->length ());


  // Create a input CDR stream.
  // NOTE: We use the same data block in which we read the message and
  // we pass it on to the higher layers of the ORB. So we dont to any
  // copies at all here. The same is also done in the higher layers.

  TAO_InputCDR input_cdr (qd->msg_block_->data_block (),
                          ACE_Message_Block::DONT_DELETE,
                          rd_pos,
                          wr_pos,
                          qd->byte_order_,
                          qd->major_version_,
                          qd->minor_version_,
                          this->orb_core_);


  // We know we have some request message. Check whether it is a
  // GIOP_REQUEST or GIOP_LOCATE_REQUEST to take action.
  switch (qd->msg_type_)
    {
    case TAO_PLUGGABLE_MESSAGE_REQUEST:
      // Should be taken care by the state specific invocations. They
      // could raise an exception or write things in the output CDR
      // stream
      return this->process_request (transport,
                                    input_cdr,
                                    output);
    case TAO_PLUGGABLE_MESSAGE_LOCATEREQUEST:
      return this->process_locate_request (transport,
                                           input_cdr,
                                           output);
    default:
      return -1;
    }
}

int
TAO_GIOP_Message_Lite::process_reply_message (
    TAO_Pluggable_Reply_Params &params,
    TAO_Queued_Data *qd
  )
{


  // Get the read and write positions before we steal data.
  size_t rd_pos = qd->msg_block_->rd_ptr () - qd->msg_block_->base ();
  size_t wr_pos = qd->msg_block_->wr_ptr () - qd->msg_block_->base ();
  rd_pos += TAO_GIOP_LITE_HEADER_LEN;

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

  // We know we have some reply message. Check whether it is a
  // GIOP_REPLY or GIOP_LOCATE_REPLY to take action.
  switch (qd->msg_type_)
    {
    case TAO_GIOP_REPLY:
      // Should be taken care by the state specific parsing
      return this->parse_reply (input_cdr,
                                params);
    case TAO_GIOP_LOCATEREPLY:
      // We call parse_reply () here because, the message format for
      // the LOCATEREPLY & REPLY are same.
      return this->parse_reply (input_cdr,
                                params);
    default:
      return -1;
    }
}

int
TAO_GIOP_Message_Lite::generate_exception_reply (
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
      // Make the GIOP & reply header. They are version specific.
      this->write_reply_header (cdr,
                                params);
      x._tao_encode (cdr ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (CORBA::Exception, ex)
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
                                        TAO_InputCDR &cdr,
                                        TAO_OutputCDR &output)
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
        this->parse_request_header (request);

      request.orb_core()->codeset_manager()->process_service_context(request);
      transport->assign_translators(&cdr,&output);

      // Throw an exception if the
      if (parse_error != 0)
        ACE_TRY_THROW (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE,
                                       CORBA::COMPLETED_NO));
      request_id = request.request_id ();

      response_required = request.response_expected ();

      CORBA::Object_var forward_to;

      // Do this before the reply is sent.
      this->orb_core_->adapter_registry ()->dispatch (request.object_key (),
                                                      request,
                                                      forward_to
                                                       ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (!CORBA::is_nil (forward_to.in ()))
        {
          // We should forward to another object...
          TAO_Pluggable_Reply_Params reply_params (this->orb_core_);
          reply_params.request_id_ = request_id;
          reply_params.reply_status_ = TAO_GIOP_LOCATION_FORWARD;
          reply_params.svc_ctx_.length (0);

          // Send back the reply service context list.
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

          int result = transport->send_message (output);
          if (result == -1)
            {
              if (TAO_debug_level > 0)
                {
                  // No exception but some kind of error, yet a
                  // response is required.
                  ACE_ERROR ((LM_ERROR,
                              ACE_TEXT ("TAO: (%P|%t|%N|%l) %p: ")
                              ACE_TEXT ("cannot send reply\n"),
                              ACE_TEXT ("TAO_GIOP_Message_Lite::process_request")));
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
                                               this->orb_core_,
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
                              ACE_TEXT ("process_request ()")));
                  ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                                       "TAO: ");
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
                                               this->orb_core_,
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
                  ACE_PRINT_EXCEPTION (exception, "TAO: ");
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
TAO_GIOP_Message_Lite::process_locate_request (TAO_Transport *transport,
                                               TAO_InputCDR &input,
                                               TAO_OutputCDR &output)
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

      TAO::ObjectKey tmp_key (locate_request.object_key ().length (),
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
                                        this->orb_core_,
                                        parse_error);

      if (parse_error != 0)
        {
          ACE_TRY_THROW (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE,
                                         CORBA::COMPLETED_NO));
        }

      CORBA::Object_var forward_to;

      this->orb_core_->adapter_registry ()->dispatch (server_request.object_key (),
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
                    ACE_TEXT ("TAO (%P|%t) TAO_GIOP_Message_Lite::process_locate_request - ")
                    ACE_TEXT ("CORBA exception raised\n")));
    }
#if defined (TAO_HAS_EXCEPTIONS)
  ACE_CATCHALL
    {
      // Normal exception, so the object is not here
      status_info.status = TAO_GIOP_UNKNOWN_OBJECT;
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO (%P|%t) TAO_GIOP_Message_Lite::process_locate_request - ")
                    ACE_TEXT ("C++ exception raised\n")));
    }
#endif /* TAO_HAS_EXCEPTIONS */
  ACE_ENDTRY;

  return this->make_send_locate_reply (transport,
                                       output,
                                       locate_request,
                                       status_info);
}


int
TAO_GIOP_Message_Lite::make_send_locate_reply (
    TAO_Transport *transport,
    TAO_OutputCDR &output,
    TAO_GIOP_Locate_Request_Header &request,
    TAO_GIOP_Locate_Status_Msg &status_info
  )
{
  // Note here we are making the Locate reply header which is *QUITE*
  // different from the reply header made by the make_reply () call..
  // Make the GIOP message header
  this->write_protocol_header (TAO_GIOP_LOCATEREPLY,
                               output);

  // This writes the header & body
  this->write_locate_reply_mesg (output,
                                 request.request_id (),
                                 status_info);

  // Send the message
  int result = transport->send_message (output);

  // Print out message if there is an error
  if (result == -1)
    {
      if (TAO_debug_level > 0)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("TAO: (%P|%t) %p: cannot send reply\n"),
                      ACE_TEXT ("TAO_GIOP_Message_Lite::make_send_locate_reply")));
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

  // Steal the contents in to the reply CDR and loose ownership of the
  // data block.
  params.input_cdr_.exchange_data_blocks (cdr);

  return 0;
}



int
TAO_GIOP_Message_Lite::write_reply_header (
    TAO_OutputCDR &output,
    TAO_Pluggable_Reply_Params_Base &reply
    ACE_ENV_ARG_DECL_NOT_USED /* ACE_ENV_SINGLE_ARG_PARAMETER */
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
  else if (response_flags == CORBA::Octet (Messaging::SYNC_WITH_TRANSPORT)
           || response_flags == CORBA::Octet (Messaging::SYNC_NONE)
           || response_flags == CORBA::Octet (TAO::SYNC_EAGER_BUFFERING)
           || response_flags == CORBA::Octet (TAO::SYNC_DELAYED_BUFFERING))
    {
      // No response required.
      out_stream << CORBA::Any::from_octet (0);
    }
  else if (response_flags == CORBA::Octet (Messaging::SYNC_WITH_SERVER))
    {
      // Return before dispatching servant.  We're also setting the high
      // bit here. This is a temporary fix until the rest of GIOP 1.2 is
      // implemented in TAO.
      out_stream << CORBA::Any::from_octet (129);
    }
  else if (response_flags == CORBA::Octet (Messaging::SYNC_WITH_TARGET))
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
  const TAO::ObjectKey *key = spec.object_key ();

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
  const TAO::ObjectKey *key = spec.object_key ();

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

  if (input.char_translator () == 0)
    {
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

      request.operation (tmp._retn (),
                         0,
                         1 /* TAO_ServerRequest owns string */);
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

  TAO::ObjectKey &object_key =
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
                        orb_core->output_cdr_msgblock_allocator (),
                        orb_core->orb_params ()->cdr_memcpy_tradeoff (),
                        TAO_DEF_GIOP_MAJOR,
                        TAO_DEF_GIOP_MINOR);

  transport->assign_translators(0,&output);

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


      x->_tao_encode (output ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (CORBA::Exception, ex)
    {
      // Now we know that while handling the error an other error
      // happened -> no hope, close connection.

      // Close the handle.
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t|%N|%l) cannot marshal exception on transport %lu: %p\n"),
                  transport->id (),
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

  ACE_Data_Block data_block (TAO_GIOP_LITE_HEADER_LEN,
                             ACE_Message_Block::MB_DATA,
                             error_message,
                             0,
                             0,
                             ACE_Message_Block::DONT_DELETE,
                             0);
  ACE_Message_Block message_block(&data_block);
  message_block.wr_ptr (TAO_GIOP_LITE_HEADER_LEN);

  size_t bt;
  int result = transport->send_message_block_chain (&message_block,
                                                    bt);
  if (result == -1)
    {
      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((
              LM_DEBUG,
              ACE_TEXT ("TAO (%N|%l|%P|%t) error sending error to transport %lu, errno = %d\n"),
              transport->id (), errno
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
          // We are not sure where the read pointer is pointing
          // to. So, try to align teh pointer to a 4 byte boundary.
          char *buf = ACE_ptr_align_binary (ptr + TAO_GIOP_LITE_HEADER_LEN, 4);

          id = ACE_reinterpret_cast (CORBA::ULong *,
                                     (char * ) (buf));
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

      if (TAO_debug_level >= 10)
        ACE_HEX_DUMP ((LM_DEBUG,
                       (const char *) ptr,
                       len,
                       ACE_TEXT ("GIOP lite message")));
    }
}

TAO_Queued_Data *
TAO_GIOP_Message_Lite::make_queued_data (size_t sz)
{
  // Get a node for the queue..
  TAO_Queued_Data *qd =
    TAO_Queued_Data::get_queued_data ();

  // Make a datablock for the size requested + something. The
  // "something" is required because we are going to align the data
  // block in the message block. During alignment we could loose some
  // bytes. As we may not know how many bytes will be lost, we will
  // allocate ACE_CDR::MAX_ALIGNMENT extra.
  ACE_Data_Block *db =
    this->orb_core_->create_input_cdr_data_block (sz +
                                                  ACE_CDR::MAX_ALIGNMENT);

  ACE_Allocator *alloc =
    this->orb_core_->input_cdr_msgblock_allocator ();

  ACE_Message_Block mb (db,
                        0,
                        alloc);

  ACE_Message_Block *new_mb = mb.duplicate ();

  ACE_CDR::mb_align (new_mb);

  qd->msg_block_ = new_mb;

  return qd;
}

int
TAO_GIOP_Message_Lite::generate_locate_reply_header (
    TAO_OutputCDR & /*cdr*/,
    TAO_Pluggable_Reply_Params_Base & /*params*/)
{
  return 0;
}


int
TAO_GIOP_Message_Lite::is_ready_for_bidirectional (TAO_OutputCDR & )
{
  // No we dont support..
  return 0;
}

size_t
TAO_GIOP_Message_Lite::header_length (void) const
{
  return TAO_GIOP_LITE_HEADER_LEN;
}
