// -*- C++ -*-
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
#include "tao/ZIOP_Adapter.h"
#include "ace/Min_Max.h"

/*
 * Hook to add additional include files during specializations.
 */
//@@ GIOP_MESSAGE_BASE_INCLUDE_ADD_HOOK

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_GIOP_Message_Base::TAO_GIOP_Message_Base (TAO_ORB_Core *orb_core,
                                              TAO_Transport *transport,
                                              size_t input_cdr_size)
  : orb_core_ (orb_core)
  , fragmentation_strategy_ (orb_core->fragmentation_strategy (transport))
  , out_stream_ (0,
                 input_cdr_size,
                 TAO_ENCAP_BYTE_ORDER,
                 orb_core->output_cdr_buffer_allocator (),
                 orb_core->output_cdr_dblock_allocator (),
                 orb_core->output_cdr_msgblock_allocator (),
                 orb_core->orb_params ()->cdr_memcpy_tradeoff (),
                 fragmentation_strategy_.get (),
                 TAO_DEF_GIOP_MAJOR,
                 TAO_DEF_GIOP_MINOR)
{
#if defined (TAO_HAS_MONITOR_POINTS) && (TAO_HAS_MONITOR_POINTS == 1)
  const int nibbles = 2 * sizeof (size_t);
  char hex_string[nibbles + 1];
  ACE_OS::sprintf (hex_string,
                   "%8.8X",
                   transport->id ());
  hex_string[nibbles] = '\0';
  ACE_CString monitor_name ("OutputCDR_");
  monitor_name += hex_string;
  this->out_stream_.register_monitor (monitor_name.c_str ());
#endif /* TAO_HAS_MONITOR_POINTS==1 */
}


TAO_GIOP_Message_Base::~TAO_GIOP_Message_Base (void)
{
#if defined (TAO_HAS_MONITOR_POINTS) && (TAO_HAS_MONITOR_POINTS == 1)
  this->out_stream_.unregister_monitor ();
#endif /* TAO_HAS_MONITOR_POINTS==1 */
}

void
TAO_GIOP_Message_Base::init (CORBA::Octet major, CORBA::Octet minor)
{
  // Set the giop version of the out stream
  this->out_stream_.set_version (major, minor);
}

TAO_OutputCDR &
TAO_GIOP_Message_Base::out_stream (void)
{
  return this->out_stream_;
}

int
TAO_GIOP_Message_Base::generate_request_header (
    TAO_Operation_Details &op,
    TAO_Target_Specification &spec,
    TAO_OutputCDR &cdr)
{
  // Get a parser for us
  TAO_GIOP_Message_Version giop_version;

  cdr.get_version (giop_version);

  // Write the GIOP header first
  if (!this->write_protocol_header (GIOP::Request, giop_version, cdr))
    {
      if (TAO_debug_level)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("(%P|%t) Error in writing GIOP header\n")));
        }

      return -1;
    }

  // Get the parser we need to use
  TAO_GIOP_Message_Generator_Parser *generator_parser =
    this->get_parser (giop_version);

  // Now call the implementation for the rest of the header
  if (!generator_parser->write_request_header (op, spec, cdr))
    {
      if (TAO_debug_level)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("(%P|%t) Error in writing request header\n")));

      return -1;
    }

  return 0;
}

int
TAO_GIOP_Message_Base::generate_locate_request_header (
    TAO_Operation_Details &op,
    TAO_Target_Specification &spec,
    TAO_OutputCDR &cdr)
{
  TAO_GIOP_Message_Version giop_version;

  cdr.get_version (giop_version);

  // Get the parser we need to use
  TAO_GIOP_Message_Generator_Parser *generator_parser =
    this->get_parser (giop_version);

  // Write the GIOP header first
  if (!this->write_protocol_header (GIOP::LocateRequest, giop_version, cdr))
    {
      if (TAO_debug_level)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("(%P|%t) Error in writing GIOP header\n")));

      return -1;
    }

  // Now call the implementation for the rest of the header
  if (!generator_parser->write_locate_request_header
      (op.request_id (), spec, cdr))
    {
      if (TAO_debug_level)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("(%P|%t) Error in writing locate request header\n")));


      return -1;

    }

  return 0;
}

int
TAO_GIOP_Message_Base::generate_reply_header (
    TAO_OutputCDR &cdr,
    TAO_Pluggable_Reply_Params_Base &params)
{
  TAO_GIOP_Message_Version giop_version;

  cdr.get_version (giop_version);

  // Write the GIOP header first
  if (!this->write_protocol_header (GIOP::Reply, giop_version, cdr))
    {
      if (TAO_debug_level)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("(%P|%t) Error in writing GIOP header\n")));

      return -1;
    }

  try
    {
      // Get the parser we need to use
      TAO_GIOP_Message_Generator_Parser *generator_parser =
        this->get_parser (giop_version);

      // Now call the implementation for the rest of the header
      if (!generator_parser->write_reply_header (cdr, params))
        {
          if (TAO_debug_level > 4)
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("(%P|%t) Error in writing reply ")
                        ACE_TEXT ("header\n")));

          return -1;
        }
    }
  catch (const ::CORBA::Exception& ex)
    {
      if (TAO_debug_level > 4)
        ex._tao_print_exception (
          ACE_TEXT ("TAO_GIOP_Message_Base::generate_reply_header"));

      return -1;
    }

  return 0;
}

int
TAO_GIOP_Message_Base::generate_fragment_header (TAO_OutputCDR & cdr,
                                                 CORBA::ULong request_id)
{
  TAO_GIOP_Message_Version giop_version;

  cdr.get_version (giop_version);

  // GIOP fragments are supported in GIOP 1.1 and better, but TAO only
  // supports them in 1.2 or better since GIOP 1.1 fragments do not
  // have a fragment message header.
  if (giop_version.major == 1 && giop_version.minor < 2)
    return -1;

  // Get the parser we need to use
  TAO_GIOP_Message_Generator_Parser *generator_parser =
    this->get_parser (giop_version);

  // Write the GIOP header first
  if (!this->write_protocol_header (GIOP::Fragment, giop_version, cdr)
      || !generator_parser->write_fragment_header (cdr, request_id))
    {
      if (TAO_debug_level)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("(%P|%t) Error in writing GIOP header\n")));

      return -1;
    }

  return 0;
}

int
TAO_GIOP_Message_Base::dump_consolidated_msg (TAO_OutputCDR &stream)
{
  // Check whether the output cdr stream is build up of multiple
  // messageblocks. If so, consolidate them to one block that can be
  // dumped
  ACE_Message_Block* consolidated_block = 0;
  char *buf = const_cast <char*> (stream.buffer ());
  size_t const total_len = stream.total_length ();
  if (stream.begin()->cont () != 0)
    {
      ACE_NEW_RETURN (consolidated_block, ACE_Message_Block, 0);
      ACE_CDR::consolidate (consolidated_block, stream.begin ());
      buf = (char *) (consolidated_block->rd_ptr ());
    }

  this->dump_msg ("send", reinterpret_cast <u_char *> (buf), total_len);

  delete consolidated_block;
  consolidated_block = 0;

  return 0;
}

int
TAO_GIOP_Message_Base::format_message (TAO_OutputCDR &stream, TAO_Stub *stub, TAO_ServerRequest *request)
{
  this->set_giop_flags (stream);

  bool log_msg = TAO_debug_level > 9;
  if (stub || request)
    {
#if defined (TAO_HAS_ZIOP) && TAO_HAS_ZIOP != 0
      TAO_ZIOP_Adapter* ziop_adapter = this->orb_core_->ziop_adapter ();
      if (ziop_adapter)
        {
          if (log_msg)
            {
              this->dump_consolidated_msg (stream);
            }

          const bool compressed=
            stub ?
            ziop_adapter->marshal_data (stream, *stub) :
            ziop_adapter->marshal_data (stream, *this->orb_core_, request);

          if (log_msg && !compressed)
            {
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("TAO (%P|%t) - ")
                          ACE_TEXT ("TAO_GIOP_Message_Base::format_message, ")
                          ACE_TEXT ("GIOP message not compressed\n")));

              // no need to log. If compressed->ZIOP library dumps message
              // if not compressed (due to failure or policy settings)
              // message hasn't changed and was allready dumped
              // prior to compression...
              log_msg = false;
            }
        }
#endif /* TAO_HAS_ZIOP */
    }

  // Length of all buffers.
  size_t const total_len = stream.total_length ();

  // NOTE: Here would also be a fine place to calculate a digital
  // signature for the message and place it into a preallocated slot
  // in the "ServiceContext".  Similarly, this is a good spot to
  // encrypt messages (or just the message bodies) if that's needed in
  // this particular environment and that isn't handled by the
  // networking infrastructure (e.g., IPSEC).

  char *buf = const_cast <char*> (stream.buffer ());

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

  if (log_msg)
    {
      this->dump_consolidated_msg (stream);
    }

  return 0;
}

int
TAO_GIOP_Message_Base::parse_next_message (TAO_Queued_Data &qd,
                                           size_t &mesg_length)
{
  if (qd.msg_block ()->length () < TAO_GIOP_MESSAGE_HEADER_LEN)
    {
      qd.missing_data (TAO_MISSING_DATA_UNDEFINED);

      return 0; /* incomplete header */
    }
  else
    {
      TAO_GIOP_Message_State state;

      if (state.parse_message_header (*(qd.msg_block ())) == -1)
        {
          return -1;
        }

      size_t const message_size = state.message_size (); /* Header + Payload */

      if (message_size > qd.msg_block ()->length ())
        {
          qd.missing_data (message_size - qd.msg_block ()->length ());
        }
      else
        {
          qd.missing_data (0);
        }

      /* init out-parameters */
      qd.state (state);
      mesg_length = message_size;

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

          qd->msg_block ()->copy (incoming.rd_ptr (), incoming.length ());

          incoming.rd_ptr (incoming.length ()); // consume all available data

          qd->missing_data (TAO_MISSING_DATA_UNDEFINED);
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
      qd->missing_data (copying_len - incoming.length ());
      copying_len = incoming.length ();
    }
  else
    {
      qd->missing_data (0);
    }

  qd->msg_block ()->copy (incoming.rd_ptr (), copying_len);

  incoming.rd_ptr (copying_len);
  qd->state (state);

  return 1;
}

int
TAO_GIOP_Message_Base::consolidate_node (TAO_Queued_Data *qd,
                                         ACE_Message_Block &incoming)
{
  // Look to see whether we had atleast parsed the GIOP header ...
  if (qd->missing_data () == TAO_MISSING_DATA_UNDEFINED)
    {
      // The data length that has been stuck in there during the last
      // read ....
      size_t const len = qd->msg_block ()->length ();

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

      if (qd->msg_block ()->copy (incoming.rd_ptr (), n_copy) == -1)
        {
           return -1;
        }

      // Move the rd_ptr () in the incoming message block..
      incoming.rd_ptr (n_copy);

      // verify sufficient data to parse GIOP header
      if (qd->msg_block ()->length () < TAO_GIOP_MESSAGE_HEADER_LEN)
        {
          return 0; /* continue */
        }

      TAO_GIOP_Message_State state;

      // Parse the message header now...
      if (state.parse_message_header (*qd->msg_block ()) == -1)
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
       if (ACE_CDR::grow (qd->msg_block (),
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
          qd->missing_data (copy_len - incoming.length ());

          // Set the actual possible copy_len that is available...
          copy_len = incoming.length ();
        }
      else
        {
          qd->missing_data (0);
        }

      // ..now we are set to copy the right amount of data to the
      // node..
      if (qd->msg_block ()->copy (incoming.rd_ptr (), copy_len) == -1)
        {
          return -1;
        }

      // Set the <rd_ptr> of the <incoming>..
      incoming.rd_ptr (copy_len);

      // Get the other details...
      qd->state (state);
    }
  else
    {
      // @todo: Need to abstract this out to a seperate method...
      size_t copy_len = qd->missing_data ();

      if (copy_len > incoming.length ())
        {
          // Calculate the missing data..
          qd->missing_data (copy_len - incoming.length ());

          // Set the actual possible copy_len that is available...
          copy_len = incoming.length ();
        }

      // paranoid check for endless-event-looping
      if (copy_len == 0)
        {
          return -1;
        }

      // Copy the right amount of data in to the node...
      if (qd->msg_block ()->copy (incoming.rd_ptr (), copy_len) == -1)
        {
          return -1;
        }

      // Set the <rd_ptr> of the <incoming>..
      qd->msg_block ()->rd_ptr (copy_len);

    }

  return 0;
}

int
TAO_GIOP_Message_Base::process_request_message (TAO_Transport *transport,
                                                TAO_Queued_Data *qd)
{
  // Set the upcall thread
  this->orb_core_->lf_strategy ().set_upcall_thread (this->orb_core_->leader_follower ());

  // Get the parser we need to use
  TAO_GIOP_Message_Generator_Parser *generator_parser =
    this->get_parser (qd->giop_version ());

  // A buffer that we will use to initialise the CDR stream.  Since we're
  // allocating the buffer on the stack, we may as well allocate the data
  // block on the stack too and avoid an allocation inside the message
  // block of the CDR.
#if defined (ACE_INITIALIZE_MEMORY_BEFORE_USE)
  char repbuf[ACE_CDR::DEFAULT_BUFSIZE] = { 0 };
#else
  char repbuf[ACE_CDR::DEFAULT_BUFSIZE];
#endif /* ACE_INITIALIZE_MEMORY_BEFORE_USE */
  ACE_Data_Block out_db (sizeof (repbuf),
                         ACE_Message_Block::MB_DATA,
                         repbuf,
                         this->orb_core_->input_cdr_buffer_allocator (),
                         0,
                         ACE_Message_Block::DONT_DELETE,
                         this->orb_core_->input_cdr_dblock_allocator ());

  // Initialize an output CDR on the stack
  // NOTE: Don't jump to a conclusion as to why we are using the
  // input_cdr and hence the  global pool here. These pools will move
  // to the lanes anyway at some point of time. Further, it would have
  // been awesome to have this in TSS. But for some reason the cloning
  // that happens when the ORB gets flow controlled while writing a
  // reply is messing things up. We crash horribly. Doing this adds a
  // lock, we need to set things like this -- put stuff in TSS here
  // and transfer to global memory when we get flow controlled. We
  // need to work on the message block to get it right!
  TAO_OutputCDR output (&out_db,
                        TAO_ENCAP_BYTE_ORDER,
                        this->orb_core_->input_cdr_msgblock_allocator (),
                        this->orb_core_->orb_params ()->cdr_memcpy_tradeoff (),
                        this->fragmentation_strategy_.get (),
                        qd->giop_version ().major_version (),
                        qd->giop_version ().minor_version ());

  // Get the read and write positions and header before we steal data.
  size_t rd_pos = qd->msg_block ()->rd_ptr () - qd->msg_block ()->base ();
  size_t wr_pos = qd->msg_block ()->wr_ptr () - qd->msg_block ()->base ();
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

  ACE_Data_Block *db = 0;

  // Get the flag in the message block
  ACE_Message_Block::Message_Flags flg = qd->msg_block ()->self_flags ();

  if (ACE_BIT_ENABLED (flg, ACE_Message_Block::DONT_DELETE))
    {
      // Use the same datablock
      db = qd->msg_block ()->data_block ();
    }
  else
    {
      // Use a duplicated datablock as the datablock has come off the
      // heap.
      db = qd->msg_block ()->data_block ()->duplicate ();
    }
  db->size (qd->msg_block ()->length ());

#if defined (TAO_HAS_ZIOP) && TAO_HAS_ZIOP ==1
  if (qd->state ().compressed ())
    {
      if (!this->decompress (&db, *qd, rd_pos, wr_pos))
        return -1;
    }
#endif
  if (TAO_debug_level > 9)
    {
      char buf[48];
      ACE_OS::sprintf (buf, "Transport[" ACE_SIZE_T_FORMAT_SPECIFIER_ASCII "] recv",
                       transport->id ());
      //due to alignment data block has an offset which needs to be corrected
      this->dump_msg (buf,
                      reinterpret_cast <u_char *> (db->base () + rd_pos - TAO_GIOP_MESSAGE_HEADER_LEN),
                      db->size ()  + rd_pos - TAO_GIOP_MESSAGE_HEADER_LEN);
    }

  TAO_InputCDR input_cdr (db,
                          db->flags (),
                          rd_pos,
                          wr_pos,
                          qd->byte_order (),
                          qd->giop_version ().major_version (),
                          qd->giop_version ().minor_version (),
                          this->orb_core_);

  transport->assign_translators(&input_cdr,&output);

  // We know we have some request message. Check whether it is a
  // GIOP_REQUEST or GIOP_LOCATE_REQUEST to take action.

  // Once we send the InputCDR stream we need to just forget about
  // the stream and never touch that again for anything. We basically
  // loose ownership of the data_block.

  switch (qd->msg_type ())
    {
    case GIOP::Request:
      // Should be taken care by the state specific invocations. They
      // could raise an exception or write things in the output CDR
      // stream
      return this->process_request (transport,
                                    input_cdr,
                                    output,
                                    generator_parser);

    case GIOP::LocateRequest:
      return this->process_locate_request (transport,
                                           input_cdr,
                                           output,
                                           generator_parser);
    default:
      return -1;
    }
}

#if defined (TAO_HAS_ZIOP) && TAO_HAS_ZIOP ==1
bool
TAO_GIOP_Message_Base::decompress (ACE_Data_Block **db, TAO_Queued_Data& qd,
                                   size_t& rd_pos, size_t& wr_pos)
{
  TAO_ZIOP_Adapter* adapter = this->orb_core_->ziop_adapter ();
  if (adapter)
    {
      if (!adapter->decompress (db, qd, *this->orb_core_))
        return false;
      rd_pos = TAO_GIOP_MESSAGE_HEADER_LEN;
      wr_pos = (*db)->size();
    }
  else
    {
      if (TAO_debug_level > 0)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("TAO (%P|%t) ERROR: Unable to decompress ")
                    ACE_TEXT ("data.\n")));

      return false;
    }
  return true;
}
#endif

int
TAO_GIOP_Message_Base::process_reply_message (
    TAO_Pluggable_Reply_Params &params,
    TAO_Queued_Data *qd)
{
  // Get the parser we need to use
  TAO_GIOP_Message_Generator_Parser *generator_parser =
    this->get_parser (qd->giop_version ());

  // Get the read and write positions before we steal data.
  size_t rd_pos = qd->msg_block ()->rd_ptr () - qd->msg_block ()->base ();
  size_t wr_pos = qd->msg_block ()->wr_ptr () - qd->msg_block ()->base ();
  rd_pos += TAO_GIOP_MESSAGE_HEADER_LEN;

  ACE_Data_Block *db = 0;

  // Get the flag in the message block
  ACE_Message_Block::Message_Flags flg = qd->msg_block ()->self_flags ();

  if (ACE_BIT_ENABLED (flg, ACE_Message_Block::DONT_DELETE))
    {
      // Use the same datablock
      db = qd->msg_block ()->data_block ();
    }
  else
    {
      // Use a duplicated datablock as the datablock has come off the
      // heap.
      db = qd->msg_block ()->data_block ()->duplicate ();
    }
  db->size (qd->msg_block ()->length ());

#if defined (TAO_HAS_ZIOP) && TAO_HAS_ZIOP ==1
   if (qd->state ().compressed ())
    {
      if (!this->decompress (&db, *qd, rd_pos, wr_pos))
        return -1;
    }
#endif
  if (TAO_debug_level > 9)
    {
      char buf[48];
      ACE_OS::sprintf (buf, "Transport[" ACE_SIZE_T_FORMAT_SPECIFIER_ASCII "] recv",
                       params.transport_->id ());
      this->dump_msg (buf,
                      reinterpret_cast <u_char *> (db->base () + rd_pos - TAO_GIOP_MESSAGE_HEADER_LEN),
                      db->size ()  + rd_pos - TAO_GIOP_MESSAGE_HEADER_LEN);
    }

  // Create a empty buffer on stack
  // NOTE: We use the same data block in which we read the message and
  // we pass it on to the higher layers of the ORB. So we dont to any
  // copies at all here.
  TAO_InputCDR input_cdr (db,
                          db->flags (),
                          rd_pos,
                          wr_pos,
                          qd->byte_order (),
                          qd->giop_version ().major_version (),
                          qd->giop_version ().minor_version (),
                          this->orb_core_);

  // We know we have some reply message. Check whether it is a
  // GIOP_REPLY or GIOP_LOCATE_REPLY to take action.

  // Once we send the InputCDR stream we need to just forget about
  // the stream and never touch that again for anything. We basically
  // loose ownership of the data_block.
  int retval = 0;

  switch (qd->msg_type ())
    {
    case GIOP::Reply:
      // Should be taken care by the state specific parsing
      retval = generator_parser->parse_reply (input_cdr, params);
      break;
    case GIOP::LocateReply:
      retval = generator_parser->parse_locate_reply (input_cdr, params);
      break;
    default:
      retval = -1;
    }

  if (retval == -1)
    return retval;

  params.input_cdr_ = &input_cdr;
  params.transport_->assign_translators (params.input_cdr_, 0);

  retval = params.transport_->tms ()->dispatch_reply (params);

  if (retval == -1)
    {
      // Something really critical happened, we will forget about
      // every reply on this connection.
      if (TAO_debug_level > 0)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("TAO (%P|%t) - GIOP_Message_Base[%d]::process_reply_message, ")
                    ACE_TEXT ("dispatch reply failed\n"),
                    params.transport_->id ()));
    }
  return retval;
}

int
TAO_GIOP_Message_Base::generate_exception_reply (
    TAO_OutputCDR &cdr,
    TAO_Pluggable_Reply_Params_Base &params,
    const CORBA::Exception &x)
{
  // A new try/catch block, but if something goes wrong now we have no
  // hope, just abort.

  try
    {
      // Make the GIOP & reply header.
      this->generate_reply_header (cdr, params);
      x._tao_encode (cdr);
    }
  catch (const ::CORBA::BAD_PARAM&)
    {
      throw;
    }
  catch (const ::CORBA::Exception&)
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

  return 0;
}

int
TAO_GIOP_Message_Base::write_protocol_header (GIOP::MsgType type,
                                              const TAO_GIOP_Message_Version &version,
                                              TAO_OutputCDR &msg)
{
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

  header[4] = version.major;
  header[5] = version.minor;

  // "flags" octet, i.e. header[6] will be set up later when message
  // is formatted by the transport.

  header[7] = static_cast <CORBA::Octet> (type);  // Message type

  static ACE_CDR::ULong const header_size =
    sizeof (header) / sizeof (header[0]);

  // Fragmentation should not occur at this point since there are only
  // 12 bytes in the stream, and fragmentation may only occur when
  // the stream length >= 16.
  msg.write_octet_array (header, header_size);

  return msg.good_bit ();
}

int
TAO_GIOP_Message_Base::process_request (
  TAO_Transport * transport,
  TAO_InputCDR & cdr,
  TAO_OutputCDR & output,
  TAO_GIOP_Message_Generator_Parser * parser)
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

  try
    {
      int const parse_error = parser->parse_request_header (request);

      // Throw an exception if the
      if (parse_error != 0)
        throw ::CORBA::MARSHAL (0, CORBA::COMPLETED_NO);

      TAO_Codeset_Manager *csm = request.orb_core ()->codeset_manager ();
      if (csm)
        {
          csm->process_service_context (request);
          transport->assign_translators (&cdr, &output);
        }

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
          forward_to);

//@@ TAO_DISPATCH_RESOLUTION_OPT_COMMENT_HOOK_END

      if (request.is_forwarded ())
        {
          CORBA::Boolean const permanent_forward_condition =
              this->orb_core_->is_permanent_forward_condition
              (forward_to.in (),
               request.request_service_context ());

          // We should forward to another object...
          TAO_Pluggable_Reply_Params_Base reply_params;
          reply_params.request_id_ = request_id;
          reply_params.reply_status (
              permanent_forward_condition
              ? GIOP::LOCATION_FORWARD_PERM
              : GIOP::LOCATION_FORWARD);
          reply_params.svc_ctx_.length (0);

          // Send back the reply service context.
          reply_params.service_context_notowned (
            &request.reply_service_info ());

          output.message_attributes (request_id,
                                     0,
                                     TAO_Message_Semantics (TAO_Message_Semantics::TAO_REPLY),
                                     0);

          // Make the GIOP header and Reply header
          this->generate_reply_header (output, reply_params);

          if (!(output << forward_to.in ()))
            {
              if (TAO_debug_level > 0)
                ACE_ERROR ((LM_ERROR,
                            ACE_TEXT ("TAO (%P|%t) ERROR: Unable to marshal ")
                            ACE_TEXT ("forward reference.\n")));

              return -1;
            }

          output.more_fragments (false);

          int const result = transport->send_message (output,
                                                      0,
                                                      &request,
                                                      TAO_Message_Semantics (TAO_Message_Semantics::TAO_REPLY));
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
  catch ( ::CORBA::Exception& ex)
    {
      int result = 0;

      if (response_required)
        {
          result = this->send_reply_exception (transport,
                                               output,
                                               request_id,
                                               &request.reply_service_info (),
                                               &ex);
          if (result == -1)
            {
              if (TAO_debug_level > 0)
                {
                  ACE_ERROR ((LM_ERROR,
                              ACE_TEXT ("TAO: (%P|%t|%N|%l) %p: ")
                              ACE_TEXT ("cannot send exception\n"),
                              ACE_TEXT ("process_connector_request ()")));

                  ex._tao_print_exception (
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

          ex._tao_print_exception (
            "TAO_GIOP_Message_Base::process_request[2]");
        }

      return result;
    }
  catch (...)
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

          if (this->send_reply_exception (transport,
                                          output,
                                          request_id,
                                          &request.reply_service_info (),
                                          &exception) == -1
              && TAO_debug_level > 0)
            {
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT ("TAO (%P|%t) - TAO_GIOP_Message_Base::process_request[3], ")
                          ACE_TEXT ("%p: ")
                          ACE_TEXT ("cannot send exception\n"),
                          ACE_TEXT ("process_request ()")));
              exception._tao_print_exception (
                "TAO_GIOP_Message_Base::process_request[3]");
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
                      ACE_TEXT ("TAO (%P|%t) exception thrown ")
                      ACE_TEXT ("but client is not waiting a response\n")));
        }

      return result;
    }

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
  TAO_GIOP_Locate_Request_Header locate_request (input, this->orb_core_);

  TAO_GIOP_Locate_Status_Msg status_info;

  // Defaulting.
  status_info.status = GIOP::UNKNOWN_OBJECT;

  CORBA::Boolean response_required = true;

  try
    {
      int parse_error = parser->parse_locate_header (locate_request);

      if (parse_error != 0)
        {
          throw ::CORBA::MARSHAL (0, CORBA::COMPLETED_NO);
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
          throw ::CORBA::MARSHAL (0, CORBA::COMPLETED_NO);
        }

      CORBA::Object_var forward_to;

      this->orb_core_->request_dispatcher ()->dispatch (
          this->orb_core_,
          server_request,
          forward_to);

      if (server_request.is_forwarded ())
        {
          status_info.status = GIOP::OBJECT_FORWARD;
          status_info.forward_location_var = forward_to;
          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("TAO (%P|%t) - TAO_GIOP_Message_Base::process_locate_request, ")
                        ACE_TEXT ("called: forwarding\n")));
        }
      else if (server_request.reply_status () == GIOP::NO_EXCEPTION)
        {
          // We got no exception, so the object is here.
          status_info.status = GIOP::OBJECT_HERE;
          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("TAO (%P|%t) - TAO_GIOP_Message_Base::process_locate_request, ")
                        ACE_TEXT ("found\n")));
        }
      else
        {
          // Normal exception, so the object is not here
          status_info.status = GIOP::UNKNOWN_OBJECT;
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) - TAO_GIOP_Message_Base::process_locate_request, ")
                      ACE_TEXT ("not here\n")));
        }
    }

  catch (const ::CORBA::Exception&)
    {
      // Normal exception, so the object is not here
      status_info.status = GIOP::UNKNOWN_OBJECT;
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO (%P|%t) - TAO_GIOP_Message_Base::process_locate_request, ")
                    ACE_TEXT ("CORBA exception raised\n")));
    }
  catch (...)
    {
      // Normal exception, so the object is not here
      status_info.status = GIOP::UNKNOWN_OBJECT;
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO (%P|%t) TAO_GIOP_Message_Base::process_locate_request - ")
                    ACE_TEXT ("C++ exception raised\n")));
    }

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
  TAO_GIOP_Message_Version giop_version;
  output.get_version (giop_version);

  // Note here we are making the Locate reply header which is *QUITE*
  // different from the reply header made by the make_reply () call..
  // Make the GIOP message header
  this->write_protocol_header (GIOP::LocateReply, giop_version, output);

  // This writes the header & body
  parser->write_locate_reply_mesg (output,
                                   request.request_id (),
                                   status_info);

  output.more_fragments (false);

  // Send the message
  int const result = transport->send_message (output,
                                              0,
                                              0,
                                              TAO_Message_Semantics (TAO_Message_Semantics::TAO_REPLY));

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
  const char error_message [TAO_GIOP_MESSAGE_HEADER_LEN] =
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
    GIOP::MessageError,
    0, 0, 0, 0
  };

  if (TAO_debug_level > 9)
    {
      this->dump_msg ("send_error",
                      reinterpret_cast <const u_char *> (error_message),
                      TAO_GIOP_MESSAGE_HEADER_LEN);
    }

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
  int const result = transport->send_message_block_chain (&message_block, bt);
  if (result == -1)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO (%N|%l|%P|%t) error sending error to transport %u\n"),
                    transport->id ()));
    }

  return result;
}

TAO_GIOP_Message_Generator_Parser*
TAO_GIOP_Message_Base::get_parser (
    const TAO_GIOP_Message_Version &version) const
{
  switch (version.major)
    {
    case 1:
      switch (version.minor)
        {
        case 0:
          return
            const_cast<TAO_GIOP_Message_Generator_Parser_10 *> (
                                     &this->tao_giop_impl_.tao_giop_10);
          break;
        case 1:
          return
            const_cast<TAO_GIOP_Message_Generator_Parser_11 *> (
                                     &this->tao_giop_impl_.tao_giop_11);
          break;
        case 2:
          return
            const_cast<TAO_GIOP_Message_Generator_Parser_12 *> (
                                     &this->tao_giop_impl_.tao_giop_12);
          break;
        default:
          throw ::CORBA::INTERNAL (0, CORBA::COMPLETED_NO);
          break;
        }
      break;
    default:
      throw ::CORBA::INTERNAL (0, CORBA::COMPLETED_NO);
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
                         TAO_Transport *transport)
{
  // static CORBA::Octet
  // I hate this in every method. Till the time I figure out a way
  // around I will have them here hanging around.
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
    GIOP::CloseConnection,
    0, 0, 0, 0
  };

  // It's important that we use a reliable shutdown after we send this
  // message, so we know it's received.
  //
  // @@ should recv and discard queued data for portability; note
  // that this won't block (long) since we never set SO_LINGER

  if (TAO_debug_level > 9)
    {
      this->dump_msg ("send_close_connection",
                      reinterpret_cast <const u_char *> (close_message),
                      TAO_GIOP_MESSAGE_HEADER_LEN);
    }

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
  int const result = transport->send_message_block_chain (&message_block, bt);
  if (result == -1)
    {
      if (TAO_debug_level > 0)
        ACE_ERROR ((LM_ERROR,
           ACE_TEXT ("(%P|%t) error closing connection %u, errno = %d\n"),
           transport->id (), ACE_ERRNO_GET));
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
  reply_params.argument_flag_ = true;

  // Send back the service context we received.  (RTCORBA relies on
  // this).
  reply_params.service_context_notowned (svc_info);

  if (CORBA::SystemException::_downcast (x) != 0)
    {
      reply_params.reply_status (GIOP::SYSTEM_EXCEPTION);
    }
  else
    {
      reply_params.reply_status (GIOP::USER_EXCEPTION);
    }

  if (this->generate_exception_reply (output, reply_params, *x) == -1)
    return -1;

  output.more_fragments (false);

  return transport->send_message (output, 0, 0, TAO_Message_Semantics (TAO_Message_Semantics::TAO_REPLY));
}

void
TAO_GIOP_Message_Base::dump_msg (const char *label,
                                 const u_char *ptr,
                                 size_t len)
{
    if (TAO_debug_level < 10)
      {
        return;
      }

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
    int const byte_order = ptr[TAO_GIOP_MESSAGE_FLAGS_OFFSET] & 0x01;

    // Get the version info
    CORBA::Octet const major = ptr[TAO_GIOP_VERSION_MAJOR_OFFSET];
    CORBA::Octet const minor = ptr[TAO_GIOP_VERSION_MINOR_OFFSET];

    // request/reply id.
    CORBA::ULong tmp = 0;
    CORBA::ULong *id = &tmp;
    char *tmp_id = 0;

    if (ptr[TAO_GIOP_MESSAGE_TYPE_OFFSET] == GIOP::Request ||
        ptr[TAO_GIOP_MESSAGE_TYPE_OFFSET] == GIOP::Reply ||
        ptr[TAO_GIOP_MESSAGE_TYPE_OFFSET] == GIOP::Fragment)
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
    else if (ptr[TAO_GIOP_MESSAGE_TYPE_OFFSET] == GIOP::CancelRequest ||
             ptr[TAO_GIOP_MESSAGE_TYPE_OFFSET] == GIOP::LocateRequest ||
             ptr[TAO_GIOP_MESSAGE_TYPE_OFFSET] == GIOP::LocateReply)
      {
        tmp_id = (char * ) (ptr + TAO_GIOP_MESSAGE_HEADER_LEN);
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
                ACE_TEXT("TAO (%P|%t) - GIOP_Message_Base::dump_msg, ")
                ACE_TEXT("%C GIOP message v%c.%c, %d data bytes, %s endian, ")
                ACE_TEXT("Type %C[%u]\n"),
                label,
                digits[ptr[TAO_GIOP_VERSION_MAJOR_OFFSET]],
                digits[ptr[TAO_GIOP_VERSION_MINOR_OFFSET]],
                len - TAO_GIOP_MESSAGE_HEADER_LEN ,
                (byte_order == TAO_ENCAP_BYTE_ORDER) ? ACE_TEXT("my") : ACE_TEXT("other"),
                message_name,
                *id));
    ACE_HEX_DUMP ((LM_DEBUG,
                   (const char *) ptr,
                   len,
                   ACE_TEXT ("GIOP message")));
}

int
TAO_GIOP_Message_Base::generate_locate_reply_header (
    TAO_OutputCDR & /*cdr*/,
    TAO_Pluggable_Reply_Params_Base & /*params*/)
{
  return 0;
}

bool
TAO_GIOP_Message_Base::is_ready_for_bidirectional (TAO_OutputCDR &msg) const
{
  TAO_GIOP_Message_Version giop_version;

  msg.get_version (giop_version);

  // Get the parser we need to use
  TAO_GIOP_Message_Generator_Parser *generator_parser =
    this->get_parser (giop_version);

  // We dont really know.. So ask the generator and parser objects that
  // we know.
  // @@ TODO: Need to make this faster, instead of making virtual
  // call, try todo the check within this class
  return generator_parser->is_ready_for_bidirectional ();
}


TAO_Queued_Data *
TAO_GIOP_Message_Base::make_queued_data (size_t sz)
{
  // Make a datablock for the size requested + something. The
  // "something" is required because we are going to align the data
  // block in the message block. During alignment we could loose some
  // bytes. As we may not know how many bytes will be lost, we will
  // allocate ACE_CDR::MAX_ALIGNMENT extra.
  ACE_Data_Block *db =
    this->orb_core_->create_input_cdr_data_block (sz +
                                                  ACE_CDR::MAX_ALIGNMENT);

  TAO_Queued_Data *qd =
    TAO_Queued_Data::make_queued_data (
                 this->orb_core_->transport_message_buffer_allocator (),
                 this->orb_core_->input_cdr_msgblock_allocator (),
                 db);

  if (qd == 0)
    {
      if (TAO_debug_level > 0)
        {
          ACE_ERROR ((LM_ERROR,
            ACE_TEXT ("TAO (%P|%t) - TAO_GIOP_Message_Base::make_queued_data, ")
            ACE_TEXT ("out of memory, failed to allocate queued data object\n")));
        }
      db->release ();
      return 0; // NULL pointer
    }

  return qd;
}

size_t
TAO_GIOP_Message_Base::header_length (void) const
{
  return TAO_GIOP_MESSAGE_HEADER_LEN;
}

size_t
TAO_GIOP_Message_Base::fragment_header_length (
  const TAO_GIOP_Message_Version& giop_version) const
{
  // Get the parser we need to use
  TAO_GIOP_Message_Generator_Parser *generator_parser =
    this->get_parser (giop_version);

  return generator_parser->fragment_header_length ();
}

int
TAO_GIOP_Message_Base::parse_request_id (const TAO_Queued_Data *qd,
                                         CORBA::ULong &request_id) const
{
  // Get the read and write positions before we steal data.
  size_t rd_pos = qd->msg_block ()->rd_ptr () - qd->msg_block ()->base ();
  size_t wr_pos = qd->msg_block ()->wr_ptr () - qd->msg_block ()->base ();
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
  flg = qd->msg_block ()->self_flags ();

  if (ACE_BIT_ENABLED (flg, ACE_Message_Block::DONT_DELETE))
    {
      // Use the same datablock
      db = qd->msg_block ()->data_block ();
    }
  else
    {
      // Use a duplicated datablock as the datablock has come off the
      // heap.
      db = qd->msg_block ()->data_block ()->duplicate ();
    }

  TAO_InputCDR input_cdr (db,
                          flg,
                          rd_pos,
                          wr_pos,
                          qd->byte_order (),
                          qd->giop_version ().major_version (),
                          qd->giop_version ().minor_version (),
                          this->orb_core_);

  if (qd->giop_version ().major == 1 &&
      (qd->giop_version ().minor == 0 || qd->giop_version ().minor == 1))
    {
      switch (qd->msg_type ())
      {
        case GIOP::Request:
        case GIOP::Reply:
          {
            IOP::ServiceContextList service_context;

            if ((input_cdr >> service_context)
                && (input_cdr >> request_id))
              {
                return 0;
              }
          }
          break;
        case GIOP::CancelRequest:
        case GIOP::LocateRequest:
        case GIOP::LocateReply:
          {
            if ((input_cdr >> request_id))
              {
                return 0;
              }
          }
          break;
        default:
          break;
      }
    }
  else
    {
      switch (qd->msg_type ())
      {
        case GIOP::Request:
        case GIOP::Reply:
        case GIOP::Fragment:
        case GIOP::CancelRequest:
        case GIOP::LocateRequest:
        case GIOP::LocateReply:
          {
            // Dealing with GIOP-1.2, the request-id is located directly
            // behind the GIOP-Header.  This is true for all message
            // types that might be sent in form of fragments or
            // cancel-requests.
            if ((input_cdr >> request_id))
              {
                return 0;
              }
          }
          break;
        default:
          break;
      }
    }

  return -1;
}

/* @return -1 error, 0 ok, +1 outstanding fragments */
int
TAO_GIOP_Message_Base::consolidate_fragmented_message (
  TAO_Queued_Data * qd,
  TAO_Queued_Data *& msg)
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

  if (qd->giop_version ().major == 1 && qd->giop_version ().minor == 0)
    {
      TAO_Queued_Data::release (qd);
      return -1; // error: GIOP-1.0 does not support fragments
    }

  // If this is not the last fragment, push it onto stack for later processing
  if (qd->more_fragments ())
    {
      this->fragment_stack_.push (qd);

      msg = 0;   // no consolidated message available yet
      return 1;  // status: more messages expected.
    }

  tail = qd;  // init

  // Add the current message block to the end of the chain
  // after adjusting the read pointer to skip the header(s)
  size_t const header_adjustment =
    this->header_length () +
    this->fragment_header_length (tail->giop_version ().major_version ());

  if (tail->msg_block ()->length () < header_adjustment)
    {
      // buffer length not sufficient
      TAO_Queued_Data::release (qd);
      return -1;
    }

  // duplicate code to speed up both processes, for GIOP-1.1 and GIOP-1.2
  if (tail->giop_version ().major_version () == 1 && tail->giop_version ().minor_version () == 1)
    {
      // GIOP-1.1

      while (this->fragment_stack_.pop (head) != -1)
        {
          if (head->more_fragments () &&
              head->giop_version ().major_version () == 1 &&
              head->giop_version ().minor_version () == 1 &&
              head->msg_block ()->length () >= header_adjustment)
            {
              // adjust the read-pointer, skip the fragment header
              tail->msg_block ()->rd_ptr(header_adjustment);

              head->msg_block ()->cont (tail->msg_block ());

              tail->msg_block (0);

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

      CORBA::ULong const request_id = tmp_request_id;

      while (this->fragment_stack_.pop (head) != -1)
        {
          CORBA::ULong head_request_id = 0;
          int parse_status = 0;

          if (head->more_fragments () &&
              head->giop_version ().major_version () >= 1 &&
              head->giop_version ().minor_version () >= 2 &&
              head->msg_block ()->length () >= header_adjustment &&
              (parse_status = this->parse_request_id (head, head_request_id)) != -1 &&
              request_id == head_request_id)
            {
              // adjust the read-pointer, skip the fragment header
              tail->msg_block ()->rd_ptr(header_adjustment);

              head->msg_block ()->cont (tail->msg_block ());

              tail->msg_block (0);

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

  TAO_Queued_Data *head = 0;

  // Revert stack
  while (this->fragment_stack_.pop (head) != -1)
    {
      reverse_stack.push (head);
    }

  bool discard_all_GIOP11_messages = false;

  // Now we are able to process message in order they have arrived.
  // If the cancel_request_id matches to GIOP-1.1 message, all succeeding
  // fragments belong to this message and must be discarded.
  // Note: GIOP-1.1 fragment header dont have any request-id encoded. If the
  // cancel_request_id matches GIOP-1.2 messages, all GIOP-1.2 fragments
  // having encoded the request id will be discarded.
  while (reverse_stack.pop (head) != -1)
    {
      CORBA::ULong head_request_id;

      if (head->giop_version ().major_version () == 1 &&
          head->giop_version ().minor_version () <= 1 &&
          head->msg_type () != GIOP::Fragment && // GIOP11 fragment does not provide request id
          this->parse_request_id (head, head_request_id) >= 0 &&
          cancel_request_id == head_request_id)
        {
          TAO_Queued_Data::release (head);
          discard_all_GIOP11_messages = true;
        }
      else if (head->giop_version ().major_version () == 1 &&
               head->giop_version ().minor_version () <= 1 &&
               discard_all_GIOP11_messages)
        {
          TAO_Queued_Data::release (head);
        }
      else if (head->giop_version ().major_version () >= 1 &&
               head->giop_version ().minor_version () >= 2 &&
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

TAO_GIOP_Fragmentation_Strategy *
TAO_GIOP_Message_Base::fragmentation_strategy (void)
{
  return this->fragmentation_strategy_.get ();
}

void
TAO_GIOP_Message_Base::set_giop_flags (TAO_OutputCDR & msg) const
{
  CORBA::Octet * const buf =
    reinterpret_cast<CORBA::Octet *> (const_cast<char *> (msg.buffer ()));

  CORBA::Octet const & major = buf[TAO_GIOP_VERSION_MAJOR_OFFSET];
  CORBA::Octet const & minor = buf[TAO_GIOP_VERSION_MINOR_OFFSET];

  // Flags for the GIOP protocol header "flags" field.
  CORBA::Octet & flags = buf[TAO_GIOP_MESSAGE_FLAGS_OFFSET];

  // Least significant bit:        Byte order
  ACE_SET_BITS (flags, TAO_ENCAP_BYTE_ORDER ^ msg.do_byte_swap ());

  // Second least significant bit: More fragments
  //
  // Only supported in GIOP 1.1 or better.
  if (!(major <= 1 && minor == 0))
    ACE_SET_BITS (flags, msg.more_fragments () << 1);
}

TAO_END_VERSIONED_NAMESPACE_DECL
