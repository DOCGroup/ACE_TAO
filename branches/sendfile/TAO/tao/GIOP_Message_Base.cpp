// $Id$

#include "tao/GIOP_Message_Base.h"
#include "tao/operation_details.h"
#include "tao/debug.h"
#include "tao/ORB_Core.h"
#include "tao/TAO_Server_Request.h"
#include "tao/GIOP_Message_Locate_Header.h"
#include "tao/Transport.h"
#include "tao/Transport_Mux_Strategy.h"
#include "tao/LF_Strategy.h"
#include "tao/Request_Dispatcher.h"
#include "tao/Codeset_Manager.h"
#include "tao/SystemException.h"

/*
 * Hook to add additional include files during specializations.
 */
//@@ GIOP_MESSAGE_BASE_INCLUDE_ADD_HOOK

ACE_RCSID (tao,
           GIOP_Message_Base,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_GIOP_Message_Base::TAO_GIOP_Message_Base (TAO_ORB_Core * orb_core,
                                              size_t input_cdr_size)
  : orb_core_ (orb_core)
  , message_state_ ()
  , out_stream_ (0,
                 input_cdr_size,
                 TAO_ENCAP_BYTE_ORDER,
                 orb_core->output_cdr_buffer_allocator (),
                 orb_core->output_cdr_dblock_allocator (),
                 orb_core->output_cdr_msgblock_allocator (),
                 orb_core->orb_params ()->cdr_memcpy_tradeoff (),
                 TAO_DEF_GIOP_MAJOR,
                 TAO_DEF_GIOP_MINOR)
{
}


TAO_GIOP_Message_Base::~TAO_GIOP_Message_Base (void)
{
}


void
TAO_GIOP_Message_Base::init (CORBA::Octet major,
                             CORBA::Octet minor)
{
  // Set the giop version of the out stream
  this->out_stream_.set_version (major,
                                 minor);
}

TAO_OutputCDR &
TAO_GIOP_Message_Base::out_stream (void)
{
  return this->out_stream_;
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
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("(%P|%t) Error in writing GIOP header \n")));
        }

      return -1;
    }

  // Now call the implementation for the rest of the header
  if (!generator_parser->write_request_header (op,
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
                    ACE_TEXT ("(%P|%t) Error in writing GIOP header \n")));

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
                    ACE_TEXT ("(%P|%t) Error in writing locate request header \n")));


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
                    ACE_TEXT ("(%P|%t) Error in writing GIOP header \n")));

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
                             ACE_TEXT ("TAO_GIOP_Message_Base::generate_reply_header"));

      return -1;
    }
  ACE_ENDTRY;

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

  CORBA::ULong bodylen = static_cast <CORBA::ULong>
                           (total_len - TAO_GIOP_MESSAGE_HEADER_LEN);

#if !defined (ACE_ENABLE_SWAP_ON_WRITE)
  *(reinterpret_cast <CORBA::ULong *> (buf +
                         TAO_GIOP_MESSAGE_SIZE_OFFSET)) = bodylen;
#else
  if (!stream.do_byte_swap ())
    *(reinterpret_cast <CORBA::ULong *>
                           (buf + TAO_GIOP_MESSAGE_SIZE_OFFSET)) = bodylen;
  else
    ACE_CDR::swap_4 (reinterpret_cast <char *> (&bodylen),
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
                      reinterpret_cast <u_char *> (buf),
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
                         const TAO_GIOP_Message_State &msg_state) const
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
      return TAO_PLUGGABLE_MESSAGE_MESSAGERROR;

    case TAO_GIOP_CANCELREQUEST:
      return TAO_PLUGGABLE_MESSAGE_CANCELREQUEST;

    default:
        if (TAO_debug_level > 0)
          {
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("TAO (%P|%t) %N:%l        message_type : ")
                    ACE_TEXT ("wrong message.\n")));
    }
    }

  return TAO_PLUGGABLE_MESSAGE_MESSAGERROR;
}

int
TAO_GIOP_Message_Base::parse_incoming_messages (ACE_Message_Block &incoming)
{
  this->message_state_.reset ();

  return this->message_state_.parse_message_header (incoming);
}

int
TAO_GIOP_Message_Base::parse_next_message (ACE_Message_Block &incoming,
                                           TAO_Queued_Data &qd,
                                           size_t &mesg_length)
{
  if (incoming.length () < TAO_GIOP_MESSAGE_HEADER_LEN)
    {
      qd.missing_data_ = TAO_MISSING_DATA_UNDEFINED;

      return 0; /* incomplete header */
    }
  else
    {
      TAO_GIOP_Message_State state;

      if (state.parse_message_header (incoming) == -1)
        {
          return -1;
        }

      const size_t message_size = state.message_size (); /* Header + Payload */

      if (message_size > incoming.length ())
        {
          qd.missing_data_ = message_size - incoming.length ();
        }
      else
        {
          qd.missing_data_ = 0;
        }

      /* init out-parameters */
      this->init_queued_data (&qd, state);
      mesg_length  = TAO_GIOP_MESSAGE_HEADER_LEN
                   + state.payload_size ();

      return 1; /* complete header */
    }
}

int
TAO_GIOP_Message_Base::extract_next_message (ACE_Message_Block &incoming,
                                             TAO_Queued_Data *&qd)
{
  if (incoming.length () < TAO_GIOP_MESSAGE_HEADER_LEN)
    {
      if (incoming.length () > 0)
        {
          // Optimize memory usage, we dont know actual message size
          // so far, but allocate enough space to hold small GIOP
          // messages. This way we avoid expensive "grow" operation
          // for small messages.
          size_t const default_buf_size = ACE_CDR::DEFAULT_BUFSIZE;

          // Make a node which has at least message block of the size
          // of MESSAGE_HEADER_LEN.
          size_t const buf_size = ace_max (TAO_GIOP_MESSAGE_HEADER_LEN,
                                           default_buf_size);

          // POST: buf_size >= TAO_GIOP_MESSAGE_HEADER_LEN

          qd = this->make_queued_data (buf_size);

          if (qd == 0)
            {
              if (TAO_debug_level > 0)
                {
                  ACE_ERROR((LM_ERROR,
                             ACE_TEXT ("TAO (%P|%t) - TAO_GIOP_Message_Base::extract_next_message, ")
                             ACE_TEXT ("out of memory\n")));
                }
              return -1;
            }

          qd->msg_block_->copy (incoming.rd_ptr (),
                                incoming.length ());

          incoming.rd_ptr (incoming.length ()); // consume all available data

          qd->missing_data_ = TAO_MISSING_DATA_UNDEFINED;
        }
      else
        {
          // handle not initialized variables
          qd = 0;  // reset
        }

      return 0;
    }

  TAO_GIOP_Message_State state;
  if (state.parse_message_header (incoming) == -1)
    {
      return -1;
    }

  size_t copying_len = state.message_size ();

  qd = this->make_queued_data (copying_len);

  if (qd == 0)
    {
      if (TAO_debug_level > 0)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("TAO (%P|%t) - TAO_GIOP_Message_Base::extract_next_message, ")
                      ACE_TEXT ("out of memory\n")));
        }
      return -1;
    }

  if (copying_len > incoming.length ())
    {
      qd->missing_data_ = copying_len - incoming.length ();
      copying_len = incoming.length ();
    }
  else
    {
      qd->missing_data_ = 0;
    }

  qd->msg_block_->copy (incoming.rd_ptr (),
                        copying_len);

  incoming.rd_ptr (copying_len);
  this->init_queued_data (qd, state);

  return 1;
}

int
TAO_GIOP_Message_Base::consolidate_node (TAO_Queued_Data *qd,
                                         ACE_Message_Block &incoming)
{
  // Look to see whether we had atleast parsed the GIOP header ...
  if (qd->missing_data_ == TAO_MISSING_DATA_UNDEFINED)
    {
      // The data length that has been stuck in there during the last
      // read ....
      size_t len =
        qd->msg_block_->length ();

      // paranoid check
      if (len >= TAO_GIOP_MESSAGE_HEADER_LEN)
        {
          // inconsistency - this code should have parsed the header
          // so far
          return -1;
        }

      // We know that we would have space for
      // TAO_GIOP_MESSAGE_HEADER_LEN here.  So copy that much of data
      // from the <incoming> into the message block in <qd>
      size_t const available = incoming.length ();
      size_t const desired   = TAO_GIOP_MESSAGE_HEADER_LEN - len;
      size_t const n_copy    = ace_min (available, desired);

      // paranoid check, but would cause endless looping
      if (n_copy == 0)
        {
          return -1;
        }

      if (qd->msg_block_->copy (incoming.rd_ptr (),
                                n_copy) == -1)
        {
           return -1;
        }

      // Move the rd_ptr () in the incoming message block..
      incoming.rd_ptr (n_copy);

      // verify sufficient data to parse GIOP header
      if (qd->msg_block_->length () < TAO_GIOP_MESSAGE_HEADER_LEN)
        {
          return 0; /* continue */
        }

      TAO_GIOP_Message_State state;

      // Parse the message header now...
      if (state.parse_message_header (*qd->msg_block_) == -1)
        {
          if (TAO_debug_level > 0)
            {
              ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("TAO (%P|%t) - TAO_GIOP_Message_Base::consolidate_node, ")
                  ACE_TEXT ("error parsing header\n") ));
            }
          return -1;
        }
      // Now grow the message block so that we can copy the rest of
      // the data, the message_block must be able to hold complete message
       if (ACE_CDR::grow (qd->msg_block_,
                          state.message_size ()) == -1)  /* GIOP_Header + Payload */
         {
           // on mem-error get rid of context silently, try to avoid
           // system calls that might allocate additional memory
           return -1;
         }

      // Copy the pay load..
      // Calculate the bytes that needs to be copied in the queue...
      size_t copy_len = state.payload_size ();

      // If the data that needs to be copied is more than that is
      // available to us ..
      if (copy_len > incoming.length ())
        {
          // Calculate the missing data..
          qd->missing_data_ = copy_len - incoming.length ();

          // Set the actual possible copy_len that is available...
          copy_len = incoming.length ();
        }
      else
        {
          qd->missing_data_ = 0;
        }

      // ..now we are set to copy the right amount of data to the
      // node..
      if (qd->msg_block_->copy (incoming.rd_ptr (),
                                copy_len) == -1)
        {
          return -1;
        }

      // Set the <rd_ptr> of the <incoming>..
      incoming.rd_ptr (copy_len);

      // Get the other details...
      this->init_queued_data (qd, state);
    }
  else
    {
      // @@todo: Need to abstract this out to a seperate method...
      size_t copy_len = qd->missing_data_;

      if (copy_len > incoming.length ())
        {
          // Calculate the missing data..
          qd->missing_data_ = copy_len - incoming.length ();

          // Set the actual possible copy_len that is available...
          copy_len = incoming.length ();
        }

      // paranoid check for  endless-event-looping
      if (copy_len == 0)
        {
          return -1;
        }

      // Copy the right amount of data in to the node...
      // node..
      if (qd->msg_block_->copy (incoming.rd_ptr (),
                                copy_len) == -1)
        {
          return -1;
        }

      // Set the <rd_ptr> of the <incoming>..
      qd->msg_block_->rd_ptr (copy_len);

    }

  return 0;
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
#if defined (ACE_INITIALIZE_MEMORY_BEFORE_USE)
  char repbuf[ACE_CDR::DEFAULT_BUFSIZE] = { 0 };
#else
  char repbuf[ACE_CDR::DEFAULT_BUFSIZE];
#endif /* ACE_INITIALIZE_MEMORY_BEFORE_USE */

  // Initialze an output CDR on the stack
  // NOTE: Don't jump to a conclusion as to why we are using the
  // input_cdr and hence the  global pool here. These pools will move
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
                        qd->minor_version_);

  // Get the read and write positions before we steal data.
  size_t rd_pos = qd->msg_block_->rd_ptr () - qd->msg_block_->base ();
  size_t wr_pos = qd->msg_block_->wr_ptr () - qd->msg_block_->base ();
  rd_pos += TAO_GIOP_MESSAGE_HEADER_LEN;

  if (TAO_debug_level > 0)
    this->dump_msg ("recv",
                    reinterpret_cast <u_char *> (qd->msg_block_->rd_ptr ()),
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

  transport->assign_translators(&input_cdr,&output);

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
                    reinterpret_cast <u_char *> (qd->msg_block_->rd_ptr ()),
                    qd->msg_block_->length ());


  // Create a empty buffer on stack
  // NOTE: We use the same data block in which we read the message and
  // we pass it on to the higher layers of the ORB. So we dont to any
  // copies at all here.
  TAO_InputCDR input_cdr (qd->msg_block_->data_block (),
                          ACE_Message_Block::DONT_DELETE,
                          rd_pos,
                          wr_pos,
                          qd->byte_order_,
                          qd->major_version_,
                          qd->minor_version_,
                          this->orb_core_);

  // We know we have some reply message. Check whether it is a
  // GIOP_REPLY or GIOP_LOCATE_REPLY to take action.

  // Once we send the InputCDR stream we need to just forget about
  // the stream and never touch that again for anything. We basically
  // loose ownership of the data_block.
  int retval = 0;

  switch (qd->msg_type_)
    {
    case TAO_PLUGGABLE_MESSAGE_REPLY:
      // Should be taken care by the state specific parsing
      retval =
        generator_parser->parse_reply (input_cdr,
                                       params);

      break;
    case TAO_PLUGGABLE_MESSAGE_LOCATEREPLY:
      retval =
        generator_parser->parse_locate_reply (input_cdr,
                                              params);
      break;
    default:
      retval = -1;
    }

  if (retval == -1)
    return retval;

  params.input_cdr_ = &input_cdr;

  retval =
    params.transport_->tms ()->dispatch_reply (params);

  if (retval == -1)
    {
      // Something really critical happened, we will forget about
      // every reply on this connection.
      if (TAO_debug_level > 0)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("TAO (%P|%t) - GIOP_Message_Base[%d]::process_parsed_messages, ")
                    ACE_TEXT ("dispatch reply failed\n"),
                    params.transport_->id ()));
    }

  return retval;
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
      x._tao_encode (cdr
                     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (CORBA::Exception, ex)
    {
      // Now we know that while handling the error an other error
      // happened -> no hope, close connection.

      // Close the handle.
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t|%N|%l) cannot marshal exception, ")
                  ACE_TEXT ("generate_exception_reply ()\n")));
      return -1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);

  return 0;
}

int
TAO_GIOP_Message_Base::write_protocol_header (TAO_GIOP_Message_Type type,
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

  header[7] = CORBA::Octet(type);

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
  CORBA::Boolean response_required = false;

  int parse_error = 0;

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      parse_error =
        parser->parse_request_header (request);

      TAO_Codeset_Manager *csm = request.orb_core()->codeset_manager();
      if (csm)
        {
          csm->process_service_context(request);
          transport->assign_translators(&cdr,&output);
        }

      // Throw an exception if the
      if (parse_error != 0)
        ACE_TRY_THROW (CORBA::MARSHAL (0,
                                       CORBA::COMPLETED_NO));
      request_id = request.request_id ();

      response_required = request.response_expected ();

      CORBA::Object_var forward_to;

/*
 * Hook to specialize request processing within TAO
 * This hook will be replaced by specialized request
 * processing implementation.
 */
//@@ TAO_DISPATCH_RESOLUTION_OPT_COMMENT_HOOK_START

      // Do this before the reply is sent.
      this->orb_core_->request_dispatcher ()->dispatch (
          this->orb_core_,
          request,
          forward_to
          ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

//@@ TAO_DISPATCH_RESOLUTION_OPT_COMMENT_HOOK_END

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
          // It is unfortunate that an exception (probably a system
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
                              ACE_TEXT ("TAO (%P|%t) - TAO_GIOP_Message_Base::process_request[3], ")
                              ACE_TEXT ("%p: ")
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
          ACE_TRY_THROW (CORBA::MARSHAL (0,
                                         CORBA::COMPLETED_NO));
        }

      TAO::ObjectKey tmp_key (locate_request.object_key ().length (),
                              locate_request.object_key ().length (),
                              locate_request.object_key ().get_buffer (),
                              0);

      // Set it to an error state
      parse_error = 1;
      CORBA::ULong req_id = locate_request.request_id ();

      // We will send the reply. The ServerRequest class need not send
      // the reply
      CORBA::Boolean deferred_reply = true;
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
          ACE_TRY_THROW (CORBA::MARSHAL (0,
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
          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("TAO (%P|%t) - TAO_GIOP_Message_Base::process_locate_request, ")
                        ACE_TEXT ("called: forwarding\n")));
        }
      else if (server_request.exception_type () == TAO_GIOP_NO_EXCEPTION)
        {
          // We got no exception, so the object is here.
          status_info.status = TAO_GIOP_OBJECT_HERE;
          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("TAO (%P|%t) - TAO_GIOP_Message_Base::process_locate_request, ")
                        ACE_TEXT ("found\n")));
        }
      else
        {
          status_info.forward_location_var = server_request.forward_location ();

          if (!CORBA::is_nil (status_info.forward_location_var.in ()))
            {
              status_info.status = TAO_GIOP_OBJECT_FORWARD;
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("TAO (%P|%t) - TAO_GIOP_Message_Base::process_locate_request, ")
                          ACE_TEXT ("forwarding\n")));
            }
          else
            {
              // Normal exception, so the object is not here
              status_info.status = TAO_GIOP_UNKNOWN_OBJECT;
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("TAO (%P|%t) - TAO_GIOP_Message_Base::process_locate_request, ")
                          ACE_TEXT ("not here\n")));
            }
        }
    }

  ACE_CATCHANY
    {
      // Normal exception, so the object is not here
      status_info.status = TAO_GIOP_UNKNOWN_OBJECT;
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO (%P|%t) - TAO_GIOP_Message_Base::process_locate_request, ")
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
                    ACE_TEXT ("TAO (%N|%l|%P|%t) error sending error to transport %u\n"),
                    transport->id ()));
    }

  return result;
}

void
TAO_GIOP_Message_Base::set_state (
    CORBA::Octet def_major,
    CORBA::Octet def_minor,
    TAO_GIOP_Message_Generator_Parser *&gen_parser) const
{
  switch (def_major)
    {
    case 1:
      switch (def_minor)
        {
        case 0:
          gen_parser =
            const_cast<TAO_GIOP_Message_Generator_Parser_10 *> (
                                     &this->tao_giop_impl_.tao_giop_10);
          break;
        case 1:
          gen_parser =
            const_cast<TAO_GIOP_Message_Generator_Parser_11 *> (
                                     &this->tao_giop_impl_.tao_giop_11);
          break;
        case 2:
          gen_parser =
            const_cast<TAO_GIOP_Message_Generator_Parser_12 *> (
                                     &this->tao_giop_impl_.tao_giop_12);
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
           ACE_TEXT ("TAO (%P|%t) TAO_GIOP_Message_Base::send_close_connection -")
           ACE_TEXT (" connection already closed\n")));
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
           ACE_TEXT ("(%P|%t) error closing connection %u, errno = %d\n"),
           transport->id (), errno));
    }

  transport->close_connection ();
  ACE_DEBUG ((LM_DEBUG,
      ACE_TEXT ("(%P|%t) shut down transport, handle %d\n"),
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
      CORBA::Octet major = ptr[TAO_GIOP_VERSION_MAJOR_OFFSET];
      CORBA::Octet minor = ptr[TAO_GIOP_VERSION_MINOR_OFFSET];

      // request/reply id.
      CORBA::ULong tmp = 0;
      CORBA::ULong *id = &tmp;
      char *tmp_id = 0;

      if (ptr[TAO_GIOP_MESSAGE_TYPE_OFFSET] == TAO_GIOP_REQUEST ||
          ptr[TAO_GIOP_MESSAGE_TYPE_OFFSET] == TAO_GIOP_REPLY)
        {
          if (major == 1 && minor < 2)
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
          id = reinterpret_cast <ACE_CDR::ULong*> (tmp_id);
        }
      else
        {
          ACE_CDR::swap_4 (tmp_id, reinterpret_cast <char*> (id));
        }
#else
      id = reinterpret_cast <ACE_CDR::ULong*> (tmp_id);
#endif /* ACE_DISABLE_SWAP_ON_READ */

        }

      // Print.
      ACE_DEBUG ((LM_DEBUG,
                  "TAO (%P|%t) - GIOP_Message_Base::dump_msg, "
                  "%s GIOP v%c.%c msg, %d data bytes, %s endian, "
                  "Type %s[%u]\n",
                  ACE_TEXT_CHAR_TO_TCHAR (label),
                  digits[ptr[TAO_GIOP_VERSION_MAJOR_OFFSET]],
                  digits[ptr[TAO_GIOP_VERSION_MINOR_OFFSET]],
                  len - TAO_GIOP_MESSAGE_HEADER_LEN ,
                  (byte_order == TAO_ENCAP_BYTE_ORDER) ? ACE_TEXT("my") : ACE_TEXT("other"),
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


TAO_Queued_Data *
TAO_GIOP_Message_Base::make_queued_data (size_t sz)
{
  // Get a node for the queue..
  TAO_Queued_Data *qd =
    TAO_Queued_Data::make_queued_data (
      this->orb_core_->transport_message_buffer_allocator ());

  if (qd == 0)
    {
      if (TAO_debug_level > 0)
        {
          ACE_ERROR ((LM_ERROR,
            ACE_TEXT ("TAO (%P|%t) - TAO_GIOP_Message_Base::make_queued_data, ")
            ACE_TEXT ("our of memory, failed to allocate queued data object\n")));
        }
      return 0; // NULL pointer
    }

  // @@todo: We have a similar method in Transport.cpp. Need to see how
  // we can factor them out..
  // Make a datablock for the size requested + something. The
  // "something" is required because we are going to align the data
  // block in the message block. During alignment we could loose some
  // bytes. As we may not know how many bytes will be lost, we will
  // allocate ACE_CDR::MAX_ALIGNMENT extra.
  ACE_Data_Block *db =
    this->orb_core_->create_input_cdr_data_block (sz +
                                                  ACE_CDR::MAX_ALIGNMENT);

  if (db == 0)
    {
      TAO_Queued_Data::release (qd);

      if (TAO_debug_level > 0)
        {
          ACE_ERROR ((LM_ERROR,
            ACE_TEXT ("TAO (%P|%t) - TAO_GIOP_Message_Base::make_queued_data, ")
            ACE_TEXT ("out of memory, failed to allocate input data block of size %u\n"),
            sz));
        }
      return 0; // NULL pointer
    }

  ACE_Allocator *alloc =
    this->orb_core_->input_cdr_msgblock_allocator ();

  if (alloc == 0)
    {
      if (TAO_debug_level >= 8)
        {
          ACE_DEBUG ((LM_DEBUG,
             ACE_TEXT ("(%P|%t) - TAO_GIOP_Message_Base::make_queued_data,")
             ACE_TEXT (" no ACE_Allocator defined\n")));
        }
    }


  ACE_Message_Block mb (db,
                        0,
                        alloc);

  ACE_Message_Block *new_mb = mb.duplicate ();

  if (new_mb == 0)
    {
      TAO_Queued_Data::release (qd);
      db->release();

      if (TAO_debug_level > 0)
        {
          ACE_ERROR ((LM_ERROR,
            ACE_TEXT ("TAO (%P|%t) - TAO_GIOP_Message_Base::make_queued_data, ")
            ACE_TEXT ("out of memory, failed to allocate message block\n")));
        }
      return 0;
    }

  ACE_CDR::mb_align (new_mb);

  qd->msg_block_ = new_mb;

  return qd;
}

size_t
TAO_GIOP_Message_Base::header_length (void) const
{
  return TAO_GIOP_MESSAGE_HEADER_LEN;
}

size_t
TAO_GIOP_Message_Base::fragment_header_length (CORBA::Octet major,
                                               CORBA::Octet minor) const
{
  TAO_GIOP_Message_Generator_Parser *generator_parser = 0;

  // Get the state information that we need to use
  this->set_state (major,
                   minor,
                   generator_parser);

  return generator_parser->fragment_header_length ();
}

void
TAO_GIOP_Message_Base::init_queued_data (
  TAO_Queued_Data* qd,
  const TAO_GIOP_Message_State& state) const
{
  qd->byte_order_     = state.byte_order_;
  qd->major_version_  = state.giop_version_.major;
  qd->minor_version_  = state.giop_version_.minor;
  qd->more_fragments_ = state.more_fragments_;
  qd->msg_type_       = this->message_type (state);
}

int
TAO_GIOP_Message_Base::parse_request_id (const TAO_Queued_Data *qd, CORBA::ULong &request_id) const
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

  if (qd->major_version_ >= 1 &&
      (qd->minor_version_ == 0 || qd->minor_version_ == 1))
    {
      if (qd->msg_type_ == TAO_PLUGGABLE_MESSAGE_REQUEST ||
          qd->msg_type_ == TAO_PLUGGABLE_MESSAGE_REPLY)
        {
          IOP::ServiceContextList service_context;

          if ( ! (input_cdr >> service_context &&
                  input_cdr >> request_id) )
            {
              return -1;
            }

          return 0;
        }
      else if (qd->msg_type_ == TAO_PLUGGABLE_MESSAGE_CANCELREQUEST ||
               qd->msg_type_ == TAO_PLUGGABLE_MESSAGE_LOCATEREQUEST ||
               qd->msg_type_ == TAO_PLUGGABLE_MESSAGE_LOCATEREPLY)
        {
          if ( ! (input_cdr >> request_id) )
            {
              return -1;
            }

          return 0;
        }
      else
        {
          return -1;
        }
    }
  else
    {
      if (qd->msg_type_ == TAO_PLUGGABLE_MESSAGE_REQUEST  ||
          qd->msg_type_ == TAO_PLUGGABLE_MESSAGE_REPLY    ||
          qd->msg_type_ == TAO_PLUGGABLE_MESSAGE_FRAGMENT ||
          qd->msg_type_ == TAO_PLUGGABLE_MESSAGE_CANCELREQUEST ||
          qd->msg_type_ == TAO_PLUGGABLE_MESSAGE_LOCATEREQUEST ||
          qd->msg_type_ == TAO_PLUGGABLE_MESSAGE_LOCATEREPLY)
        {
          // Dealing with GIOP-1.2, the request-id is located directly behind the GIOP-Header.
          // This is true for all message types that might be sent in form of fragments or cancel-requests.
          if ( ! (input_cdr >> request_id) )
            {
              return -1;
            }

          return 0;
        }
      else
        {
          return -1;
        }
    }
}

/* @return -1 error, 0 ok, +1 outstanding fragments */
int
TAO_GIOP_Message_Base::consolidate_fragmented_message (TAO_Queued_Data *qd, TAO_Queued_Data *&msg)
{
  TAO::Incoming_Message_Stack reverse_stack;

  TAO_Queued_Data *tail = 0;
  TAO_Queued_Data *head = 0;

  //
  // CONSOLIDATE FRAGMENTED MESSAGE
  //

  // check for error-condition
  if (qd == 0)
    {
      return -1;
    }

  if (qd->major_version_ == 1 && qd->minor_version_ == 0)
    {
      TAO_Queued_Data::release (qd);
      return -1; // error: GIOP-1.0 does not support fragments
    }

  // If this is not the last fragment, push it onto stack for later processing
  if (qd->more_fragments_)
    {
      this->fragment_stack_.push (qd);

      msg = 0;   // no consolidated message available yet
      return 1;  // status: more messages expected.
    }

  tail = qd;  // init

  // Add the current message block to the end of the chain
  // after adjusting the read pointer to skip the header(s)
  const size_t header_adjustment =
    this->header_length () +
    this->fragment_header_length (tail->major_version_,
                                  tail->minor_version_);

  if (tail->msg_block_->length () < header_adjustment)
    {
      // buffer length not sufficient
      TAO_Queued_Data::release (qd);
      return -1;
    }

  // duplicate code to speed up both processes, for GIOP-1.1 and GIOP-1.2
  if (tail->major_version_ == 1 && tail->minor_version_ == 1)
    {
      // GIOP-1.1

      while (this->fragment_stack_.pop (head) != -1)
        {
          if (head->more_fragments_ &&
              head->major_version_ == 1 &&
              head->minor_version_ == 1 &&
              head->msg_block_->length () >= header_adjustment)
            {
              // adjust the read-pointer, skip the fragment header
              tail->msg_block_->rd_ptr(header_adjustment);

              head->msg_block_->cont (tail->msg_block_);

              tail->msg_block_ = 0;

              TAO_Queued_Data::release (tail);

              tail = head;
            }
          else
            {
              reverse_stack.push (head);
            }
        }
    }
  else
    {
      // > GIOP-1.2

      CORBA::ULong tmp_request_id = 0;
      if (this->parse_request_id (tail, tmp_request_id) == -1)
        {
          return -1;
        }

      const CORBA::ULong request_id = tmp_request_id;

      while (this->fragment_stack_.pop (head) != -1)
        {
          CORBA::ULong head_request_id = 0;
          int parse_status = 0;

          if (head->more_fragments_ &&
              head->major_version_ >= 1 &&
              head->minor_version_ >= 2 &&
              head->msg_block_->length () >= header_adjustment &&
              (parse_status = this->parse_request_id (head, head_request_id)) != -1 &&
              request_id == head_request_id)
            {
              // adjust the read-pointer, skip the fragment header
              tail->msg_block_->rd_ptr(header_adjustment);

              head->msg_block_->cont (tail->msg_block_);

              tail->msg_block_ = 0;

              TAO_Queued_Data::release (tail);

              tail = head;
            }
          else
            {
              if (parse_status == -1)
                {
                  TAO_Queued_Data::release (head);
                  return -1;
                }

              reverse_stack.push (head);
            }
        }
    }

  // restore stack
  while (reverse_stack.pop (head) != -1)
    {
      this->fragment_stack_.push (head);
    }

  if (tail->consolidate () == -1)
    {
      // memory allocation failed
      TAO_Queued_Data::release (tail);
      return -1;
    }

  // set out value
  msg = tail;

  return 0;
}


int
TAO_GIOP_Message_Base::discard_fragmented_message (const TAO_Queued_Data *cancel_request)
{
  // We must extract the specific request-id from message-buffer
  // and remove all fragments from stack that match this request-id.

  TAO::Incoming_Message_Stack reverse_stack;

  CORBA::ULong cancel_request_id;

  if (this->parse_request_id (cancel_request, cancel_request_id) == -1)
    {
      return -1;
    }

  TAO_Queued_Data *head;

  // Revert stack
  while (this->fragment_stack_.pop (head) != -1)
    {
      reverse_stack.push (head);
    }

  int discard_all_GIOP11_messages = 0; // false

  // Now we are able to process message in order they have arrived.
  // If the cancel_request_id matches to GIOP-1.1 message, all succeeding
  // fragments belong to this message and must be discarded.
  // Note: GIOP-1.1 fragment header dont have any request-id encoded. If the
  // cancel_request_id matches GIOP-1.2 messages, all GIOP-1.2 fragments
  // having encoded the request id will be discarded.
  while (reverse_stack.pop (head) != -1)
    {
      CORBA::ULong head_request_id;

      if (head->major_version_ == 1 &&
          head->minor_version_ <= 1 &&
          head->msg_type_ != TAO_PLUGGABLE_MESSAGE_FRAGMENT && // GIOP11 fragment does not provide request id
          this->parse_request_id (head, head_request_id) >= 0 &&
          cancel_request_id == head_request_id)
        {
          TAO_Queued_Data::release (head);

          discard_all_GIOP11_messages = 1; // true
        }
      else if (head->major_version_ == 1 &&
               head->minor_version_ <= 1 &&
               discard_all_GIOP11_messages)
        {
          TAO_Queued_Data::release (head);
        }
      else if (head->major_version_ >= 1 &&
               head->minor_version_ >= 2 &&
               this->parse_request_id (head, head_request_id) >= 0 &&
               cancel_request_id == head_request_id)
        {
          TAO_Queued_Data::release (head);
        }
      else
        {
          this->fragment_stack_.push (head);
        }
    }

  return 0;
}


TAO_END_VERSIONED_NAMESPACE_DECL
