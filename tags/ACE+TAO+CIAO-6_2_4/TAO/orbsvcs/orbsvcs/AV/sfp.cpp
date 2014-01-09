// $Id$

#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/AV/sfp.h"
#include "tao/debug.h"
#include "ace/ARGV.h"
#include "ace/OS_NS_strings.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// default arguments to pass to use for the ORB
const char TAO_SFP_Base::TAO_SFP_ORB_ARGUMENTS[] = "-ORBObjRefStyle URL";

// SFP magic numbers
const char TAO_SFP_Base::TAO_SFP_MAGIC_NUMBER[] = "=SFP";
const char TAO_SFP_Base::TAO_SFP_FRAGMENT_MAGIC_NUMBER[] = "FRAG";
const char TAO_SFP_Base::TAO_SFP_START_MAGIC_NUMBER[] = "=STA";
const char TAO_SFP_Base::TAO_SFP_CREDIT_MAGIC_NUMBER[] = "=CRE";
const char TAO_SFP_Base::TAO_SFP_STARTREPLY_MAGIC_NUMBER[] = "=STR";

// SFP version 1.0
const unsigned char TAO_SFP_Base::TAO_SFP_MAJOR_VERSION = 1;
const unsigned char TAO_SFP_Base::TAO_SFP_MINOR_VERSION = 0;

// lengths of various SFP headers
const unsigned char TAO_SFP_Base::TAO_SFP_FRAME_HEADER_LEN = 12;
const unsigned char TAO_SFP_Base::TAO_SFP_MESSAGE_SIZE_OFFSET = 8;
const unsigned char TAO_SFP_Base::TAO_SFP_FRAGMENT_SIZE_OFFSET = 16;

u_int TAO_SFP_Base::frame_header_len;
u_int TAO_SFP_Base::start_reply_len;
u_int TAO_SFP_Base::start_len;
u_int TAO_SFP_Base::credit_len;
u_int TAO_SFP_Base::fragment_len;

bool
operator< (const TAO_SFP_Fragment_Node& left,
           const TAO_SFP_Fragment_Node& right)
{
  return left.fragment_info_.frag_number < right.fragment_info_.frag_number;
}


//------------------------------------------------------------
// TAO_SFP_Base
//------------------------------------------------------------

TAO_SFP_Base::TAO_SFP_Base (void)
{
  TAO_OutputCDR output_cdr;
  flowProtocol::frameHeader frame_header;
  flowProtocol::fragment fragment;
  flowProtocol::credit credit;
  flowProtocol::Start start;
  flowProtocol::StartReply start_reply;

  // fill in the default frameHeader fields.
  frame_header.magic_number [0] = '=';
  frame_header.magic_number [1] = 'S';
  frame_header.magic_number [2] = 'F';
  frame_header.magic_number [3] = 'P';
  frame_header.flags = TAO_ENCAP_BYTE_ORDER;
  output_cdr.reset ();
  if (!(output_cdr << frame_header))
    {
      ORBSVCS_ERROR ((LM_ERROR, "TAO_SFP constructor\n"));
      return;
    }

  frame_header_len = static_cast<u_int> (output_cdr.total_length ());
  // fill in the default fragment message fields.
  fragment.magic_number [0] = 'F';
  fragment.magic_number [1] = 'R';
  fragment.magic_number [2] = 'A';
  fragment.magic_number [3] = 'G';
  output_cdr.reset ();
  if (!(output_cdr << fragment))
    {
      ORBSVCS_ERROR ((LM_ERROR, "TAO_SFP constructor\n"));
      return;
    }

  fragment_len = static_cast<u_int> (output_cdr.total_length ());
  // fill in the default Start message fields.
  start.magic_number [0] = '=';
  start.magic_number [1] = 'S';
  start.magic_number [2] = 'T';
  start.magic_number [3] = 'A';
  start.major_version = TAO_SFP_Base::TAO_SFP_MAJOR_VERSION;
  start.minor_version = TAO_SFP_Base::TAO_SFP_MINOR_VERSION;
  start.flags = 0;
  output_cdr.reset ();
  if (!(output_cdr << start))
    {
      ORBSVCS_ERROR ((LM_ERROR, "TAO_SFP constructor\n"));
      return;
    }

  start_len = static_cast<u_int> (output_cdr.total_length ());
  // fill in the default StartReply message fields.
  start_reply.magic_number [0] = '=';
  start_reply.magic_number [1] = 'S';
  start_reply.magic_number [2] = 'T';
  start_reply.magic_number [3] = 'R';
  start_reply.flags = 0;
  output_cdr.reset ();
  if (!(output_cdr << start_reply))
    {
      ORBSVCS_ERROR ((LM_ERROR, "TAO_SFP constructor\n"));
      return;
    }

  start_reply_len = static_cast<u_int> (output_cdr.total_length ());

  // fill in the default Credit message fields.
  credit.magic_number [0] = '=';
  credit.magic_number [1] = 'C';
  credit.magic_number [2] = 'R';
  credit.magic_number [3] = 'E';
  output_cdr.reset ();

  if (!(output_cdr << credit))
    {
      ORBSVCS_ERROR ((LM_ERROR, "TAO_SFP constructor\n"));
      return;
    }
  credit_len = static_cast<u_int> (output_cdr.total_length ());
}

int
TAO_SFP_Base::handle_input (TAO_AV_Transport *transport,
                            TAO_SFP_Frame_State &state,
                            TAO_AV_frame_info *&frame_info)
{
  int result;
  flowProtocol::MsgType msg_type;
  result = TAO_SFP_Base::peek_message_type (transport,
                                            msg_type);
  if (result < 0)
    return result;
  //  TAO_InputCDR &input = state.cdr;
  switch (msg_type)
    {
    case flowProtocol::SimpleFrame_Msg:
    case flowProtocol::Frame_Msg:
      {
        result = TAO_SFP_Base::peek_frame_header (transport,
                                                  state.frame_header_,
                                                  state.cdr);
        if (result < 0)
          return result;
        int result =TAO_SFP_Base::read_frame (transport,
                                              state.frame_header_,
                                              state,
                                              frame_info);
        if (result < 0)
          return result;
        break;
      }
    case flowProtocol::Fragment_Msg:
      {
        result = TAO_SFP_Base::peek_fragment_header (transport,
                                                     state.fragment_,
                                                     state.cdr);
        if (result < 0)
          return result;
        if (TAO_debug_level > 0) ORBSVCS_DEBUG ((LM_DEBUG,"Fragment received\n"));
        result = TAO_SFP_Base::read_fragment (transport,
                                              state.fragment_,
                                              state,
                                              frame_info);
        if (result < 0)
          return result;
        break;
      }
    case flowProtocol::EndofStream_Msg:
      {
        result = TAO_SFP_Base::read_endofstream_message (transport,
                                                         state.frame_header_,
                                                         state.cdr);
        if (result < 0)
          return result;
        break;
      }
    default:
      break;
    }
  return 0;
}

int
TAO_SFP_Base::read_frame (TAO_AV_Transport *transport,
                          flowProtocol::frameHeader &frame_header,
                          TAO_SFP_Frame_State &state,
                          TAO_AV_frame_info *&frame_info)
{
  ACE_Message_Block *message_block = 0;
  int result = -1;

  if (TAO_debug_level > 0)
    ORBSVCS_DEBUG ((LM_DEBUG,"Reading simple frame\n"));
  // Check to see what the length of the message is.
  int byte_order = frame_header.flags & 0x1;
  int message_len = frame_header.message_size;

//       ACE_NEW_RETURN (message_block,
//                       ACE_Message_Block (message_len),
//                       0);
  state.static_frame_.rd_ptr (state.static_frame_.base ());
  state.static_frame_.wr_ptr (state.static_frame_.base ());
  int n = transport->recv (state.static_frame_.rd_ptr (),message_len);
  if (n == -1)
    ORBSVCS_ERROR_RETURN ((LM_ERROR,"SFP::handle_input -peek"),0);
  else if (n==0)
    ORBSVCS_ERROR_RETURN ((LM_ERROR,"SFP::handle_input -peek"),0);
  else if (n != message_len)
    ORBSVCS_ERROR_RETURN ((LM_ERROR,"SFP::read_simple_frame:message truncated\n"),0);
  message_block = &state.static_frame_;
  // print the buffer.
  //      this->dump_buf (message,n);
  // skip over the frame header.
  message_block->rd_ptr (frame_header_len);
  message_block->wr_ptr (n);
  CORBA::ULong ssrc = 0;
  TAO_SFP_Fragment_Table_Entry *fragment_entry = 0;
  if (frame_header.flags & 0x2)
    {
      if (TAO_debug_level > 0)
        ORBSVCS_DEBUG ((LM_DEBUG,"fragmented frame:0th fragment\n"));
      state.more_fragments_ = 1;
      ACE_Message_Block *data = 0;
      switch (frame_header.message_type)
        {
        case flowProtocol::Frame_Msg:
          {
            // read the frame info.
            ACE_Message_Block frame_info_mb (message_len-frame_header_len+ACE_CDR::MAX_ALIGNMENT);
            ACE_CDR::mb_align (&frame_info_mb);
            frame_info_mb.copy (message_block->rd_ptr (),
                                message_block->length ());
            // print the buffer.
            //          this->dump_buf (message_block->rd_ptr (),16);
            TAO_InputCDR frame_info_cdr (&frame_info_mb,byte_order);
            frame_info_cdr >> state.frame_;
            if (TAO_debug_level > 0)
              ORBSVCS_DEBUG ((LM_DEBUG,
                          "frame.timestamp = %d, "
                          "frame.synchsource = %d, "
                          "frame.sequence_num = %d\n",
                          state.frame_.timestamp,
                          state.frame_.synchSource,
                          state.frame_.sequence_num));
            ssrc = state.frame_.synchSource;
            // The remaining message in the CDR stream is the fragment
            // data for frag.0
            data = frame_info_cdr.start ()->clone ();
            break;
          }
        case flowProtocol::SimpleFrame_Msg:
          {
            data = message_block->clone ();
            break;
          }
        case flowProtocol::SequencedFrame_Msg:
          break;
        case flowProtocol::SpecialFrame_Msg:
          break;
        }
      if (TAO_debug_level > 0)
        ORBSVCS_DEBUG ((LM_DEBUG,"Length of 0th fragment= %d\n",data->length ()));
      TAO_SFP_Fragment_Table *fragment_table = 0;
      result = state.fragment_table_map_.find (ssrc,fragment_table);
      if (result != 0)
        {
          ACE_NEW_RETURN (fragment_table,
                          TAO_SFP_Fragment_Table,
                          -1);
          result = state.fragment_table_map_.bind (ssrc,fragment_table);
          if (result < 0)
            ORBSVCS_ERROR_RETURN ((LM_ERROR,
                               "TAO_SFP_Base::read_frame: "
                               "fragment_table_map:bind failed\n"),-1);
        }

      TAO_SFP_Fragment_Node *new_node;
      ACE_NEW_RETURN (new_node,
                      TAO_SFP_Fragment_Node,
                      0);
      new_node->fragment_info_.frag_sz = static_cast<CORBA::ULong> (data->length ());
      new_node->fragment_info_.frag_number = 0;
      if (state.frame_.source_ids.length () > 0)
        new_node->fragment_info_.source_id = state.frame_.source_ids [0];
      else
        new_node->fragment_info_.source_id = 0;
      new_node->data_ = data;
      //          TAO_SFP_Base::dump_buf (data->rd_ptr (),data->length ());
      if (fragment_table->find (state.frame_.sequence_num,fragment_entry) == 0)
        {
          // This case can happen where a nth (n > 0)fragment is
          // received before the 0th fragment.
          if (TAO_debug_level > 0)
            ORBSVCS_DEBUG ((LM_DEBUG,
                        "fragment table entry found for 0th fragment:\n"));
          result = fragment_entry->fragment_set_.insert (*new_node);
          if (result != 0)
            ORBSVCS_ERROR_RETURN ((LM_ERROR,
                               "insert for 0th fragment failed\n"),0);
          //  enter the frame info.

          // check if all the fragments have been received.
          state.frame_block_ =
            TAO_SFP_Base::check_all_fragments (fragment_entry);
          if (state.frame_block_ != 0)
            state.more_fragments_ = 0;
        }
      else
        {
          if (TAO_debug_level > 0)
            ORBSVCS_DEBUG ((LM_DEBUG,
                        "fragment table entry not found for 0th fragment\n"));
          TAO_SFP_Fragment_Table_Entry *new_entry;
          ACE_NEW_RETURN (new_entry,
                          TAO_SFP_Fragment_Table_Entry,
                          0);
          result = new_entry->fragment_set_.insert (*new_node);
          if (result != 0)
            ORBSVCS_ERROR_RETURN ((LM_ERROR,"insert for 0th fragment failed\n"),0);
          fragment_entry = new_entry;
          // not found. so bind a new entry.
          result = fragment_table->bind (state.frame_.sequence_num,new_entry);
          if (result != 0)
            ORBSVCS_ERROR_RETURN ((LM_ERROR,"fragment table bind failed\n"),0);
          if (frame_header.message_type & 4 )
            fragment_entry->frame_info.boundary_marker = 1;
          switch (frame_header.message_type)
            {
            case flowProtocol::Frame_Msg:
              fragment_entry->frame_info.ssrc = state.frame_.synchSource;
              fragment_entry->frame_info.timestamp = state.frame_.timestamp;
              fragment_entry->frame_info.sequence_num = state.frame_.sequence_num;
              break;
            case flowProtocol::SimpleFrame_Msg:
              fragment_entry->frame_info.ssrc =
                fragment_entry->frame_info.timestamp =
                fragment_entry->frame_info.sequence_num = 0;
              break;
            }
          return 0;
        }
    }
  else
    {
      state.more_fragments_ = 0;
      state.frame_block_ = message_block;
    }
  if (state.more_fragments_ == 0)
    {
      if (fragment_entry != 0)
        {
          ACE_NEW_RETURN (frame_info,
                          TAO_AV_frame_info,
                          -1);
          *frame_info = fragment_entry->frame_info;
        }
    }
  return 0;
}

int
TAO_SFP_Base::read_fragment (TAO_AV_Transport *transport,
                             flowProtocol::fragment &fragment,
                             TAO_SFP_Frame_State &state,
                             TAO_AV_frame_info *&frame_info)
{
  TAO_SFP_Fragment_Table_Entry *fragment_entry = 0;
  int result = -1;

  if (TAO_debug_level > 0) ORBSVCS_DEBUG ((LM_DEBUG,"frag_number = %d, frag_size = %d,source_id  = %d sequnce_num = %d\n",
              fragment.frag_number,fragment.frag_sz,fragment.source_id,fragment.sequence_num));

  ACE_Message_Block *data;
  ACE_NEW_RETURN (data,
                  ACE_Message_Block(fragment.frag_sz),
                  -1);

  // Read the fragment.
  int n = transport->recv (data->wr_ptr (),fragment.frag_sz);
  if ((n == -1) || (n==0))
    ORBSVCS_ERROR_RETURN ((LM_ERROR,"TAO_SFP::read_fragment:%p",""),-1);
  // move past the fragment header.
  data->rd_ptr (fragment_len);
  data->wr_ptr (n);
  //  TAO_SFP_Base::dump_buf (data->rd_ptr (),data->length ());
  if (TAO_debug_level > 0) ORBSVCS_DEBUG ((LM_DEBUG,"length of %dth fragment is: %d\n",
              fragment.frag_number,
              data->length ()));

  TAO_SFP_Fragment_Node *new_node;
  ACE_NEW_RETURN (new_node,
                  TAO_SFP_Fragment_Node,
                  -1);
  new_node->fragment_info_ = fragment;
  new_node->data_ = data;
  TAO_SFP_Fragment_Table *fragment_table = 0;
  result = state.fragment_table_map_.find (fragment.source_id,fragment_table);
  if (result != 0)
    {
      ACE_NEW_RETURN (fragment_table,
                      TAO_SFP_Fragment_Table,
                      -1);
      result = state.fragment_table_map_.bind (fragment.source_id,fragment_table);
      if (result < 0)
        ORBSVCS_ERROR_RETURN ((LM_ERROR,"TAO_SFP_Base::read_fragment:fragment_table_map:bind failed\n"),-1);
    }
  if (fragment_table->find (fragment.sequence_num,fragment_entry) == 0)
    {
      // Already an entry exists. Traverse the list and insert it at the right place.
      result = fragment_entry->fragment_set_.insert (*new_node);
      if (result != 0)
        ORBSVCS_ERROR_RETURN ((LM_ERROR,"insert for %dth node failed\n",fragment.frag_number),-1);
      // check if all the fragments have been received.
    }
  else
    {
      ACE_NEW_RETURN (fragment_entry,
                      TAO_SFP_Fragment_Table_Entry,
                      -1);
      fragment_entry->fragment_set_.insert (*new_node);
      // bind a new entry for this sequence number.
      result = fragment_table->bind (fragment.sequence_num,fragment_entry);
      if (result != 0)
        ORBSVCS_ERROR_RETURN ((LM_ERROR,"bind for %dth fragment failed\n",
                           fragment.frag_number),-1);
    }
  if (!(fragment.flags & 0x2))
    {
      if (TAO_debug_level > 0) ORBSVCS_DEBUG ((LM_DEBUG,"Last fragment received\n"));
      // if bit 1 is not set then there are
      // no more fragments.
      fragment_entry->last_received_ = 1;
      // since fragment number starts from 0 to n-1 we add 1.
      fragment_entry->num_fragments_ = fragment.frag_number + 1;
    }


  state.frame_block_ = check_all_fragments (fragment_entry);
  if (state.frame_block_ != 0)
    {
      state.more_fragments_ = 0;
      ACE_NEW_RETURN (frame_info,
                      TAO_AV_frame_info,
                      -1);
      *frame_info = fragment_entry->frame_info;
    }
  return 0;
}

ACE_Message_Block*
TAO_SFP_Base::check_all_fragments (TAO_SFP_Fragment_Table_Entry *fragment_entry)
{
  if (TAO_debug_level > 0) ORBSVCS_DEBUG ((LM_DEBUG,"table size: %d, num_fragments: %d\n",fragment_entry->fragment_set_.size (),fragment_entry->num_fragments_));
  // check to see if all the frames have been received.
  if (fragment_entry->fragment_set_.size () == fragment_entry->num_fragments_)
    {
      if (TAO_debug_level > 0) ORBSVCS_DEBUG ((LM_DEBUG,"all fragments have been received\n"));
      // all the fragments have been received
      // we can now chain the ACE_Message_Blocks in the fragment_set_ and then return them
      // back.
      ACE_Message_Block *frame = 0,*head = 0;
      FRAGMENT_SET_ITERATOR frag_iterator (fragment_entry->fragment_set_);
      TAO_SFP_Fragment_Node *node;
      for (;frag_iterator.next (node) != 0;frag_iterator.advance ())
        {
          if (!head)
            head = frame = node->data_;
          else
            {
              frame->cont (node->data_);
              frame = node->data_;
            }
        }
      return head;
    }
  return 0;
}

CORBA::Boolean
TAO_SFP_Base::start_frame (CORBA::Octet flags,
                           flowProtocol::MsgType type,
                           TAO_OutputCDR &msg)
{
  msg.reset ();
  flowProtocol::frameHeader frame_header;

  frame_header.magic_number [0] = '=';
  frame_header.magic_number [1] = 'S';
  frame_header.magic_number [2] = 'F';
  frame_header.magic_number [3] = 'P';
  frame_header.flags = flags;
  frame_header.message_type = static_cast<CORBA::Octet> (type);
  frame_header.message_size = 0;
  if (!(msg << frame_header))
    return 0;
  return 1;
}

CORBA::Boolean
TAO_SFP_Base::write_start_message (TAO_OutputCDR &msg)
{
  flowProtocol::Start start;

  start.magic_number [0] = '=';
  start.magic_number [1] = 'S';
  start.magic_number [2] = 'T';
  start.magic_number [3] = 'A';
  start.major_version = TAO_SFP_MAJOR_VERSION;
  start.minor_version = TAO_SFP_MINOR_VERSION;
  start.flags = 0;
  if (!(msg << start))
    return 0;
  return 1;
}

CORBA::Boolean
TAO_SFP_Base::write_start_reply_message (TAO_OutputCDR &msg)
{
  flowProtocol::StartReply start_reply;

  start_reply.magic_number [0] = '=';
  start_reply.magic_number [1] = 'S';
  start_reply.magic_number [2] = 'T';
  start_reply.magic_number [3] = 'R';
  start_reply.flags = 0;
  if (!(msg << start_reply))
    return 0;
  return 1;
}

CORBA::Boolean
TAO_SFP_Base::write_credit_message (CORBA::ULong cred_num,
                                    TAO_OutputCDR &msg)
{
  flowProtocol::credit credit;

  credit.magic_number [0] = '=';
  credit.magic_number [1] = 'C';
  credit.magic_number [2] = 'R';
  credit.magic_number [3] = 'E';
  credit.cred_num = cred_num;
  if (!(msg << credit))
    return 0;
  return 1;
}

CORBA::Boolean
TAO_SFP_Base::write_fragment_message (CORBA::Octet flags,
                                      CORBA::ULong fragment_number,
                                      CORBA::ULong sequence_number,
                                      CORBA::ULong source_id,
                                      TAO_OutputCDR &msg)
{
  msg.reset ();
  flowProtocol::fragment fragment;

  fragment.magic_number [0] = 'F';
  fragment.magic_number [1] = 'R';
  fragment.magic_number [2] = 'A';
  fragment.magic_number [3] = 'G';
  fragment.flags = flags;
  fragment.frag_number = fragment_number;
  fragment.sequence_num = sequence_number;
  fragment.source_id = source_id;
  if (!(msg << fragment))
    return 0;
  return 1;
}

CORBA::Boolean
TAO_SFP_Base::write_frame_message (CORBA::ULong timestamp,
                                   CORBA::ULong synchSource,
                                   flowProtocol::my_seq_ulong source_ids,
                                   CORBA::ULong sequence_num,
                                   TAO_OutputCDR &msg)
{
  flowProtocol::frame frame;

  frame.timestamp = timestamp;
  frame.synchSource = synchSource;
  frame.source_ids = source_ids;
  frame.sequence_num = sequence_num;
  if (!(msg << frame))
    return 0;
  return 1;
}

int
TAO_SFP_Base::send_message (TAO_AV_Transport *transport,
                            TAO_OutputCDR &stream,
                            ACE_Message_Block *mb)
{
  CORBA::ULong total_len = static_cast<CORBA::ULong> (stream.total_length ());
  if (mb != 0)
    {
      for (ACE_Message_Block *temp = mb;temp != 0;temp = temp->cont ())
        total_len += static_cast<CORBA::ULong> (temp->length ());

      char *buf = (char *) stream.buffer ();
      size_t offset = TAO_SFP_MESSAGE_SIZE_OFFSET;
      // second character distinguished =SFP and FRAG.
      if (*(buf) == 'F')
        {
          // Fragment message.
          offset = TAO_SFP_FRAGMENT_SIZE_OFFSET;
        }
#if !defined (ACE_ENABLE_SWAP_ON_WRITE)
      *reinterpret_cast<CORBA::ULong *> (buf + offset) = total_len;
#else
      if (!stream.do_byte_swap ())
        *reinterpret_cast<CORBA::ULong *> (buf + offset) = total_len;
      else
        ACE_CDR::swap_4 (reinterpret_cast<char *> (&total_len),
                         buf + offset);
#endif /* ACE_ENABLE_SWAP_ON_WRITE */
    }
  // we join the data block with the cdr block.
  ACE_Message_Block *end = (ACE_Message_Block *)stream.end ();
  if (end == 0)
    {
      // There is only one message block.
      end = (ACE_Message_Block *)stream.begin ();
      //      TAO_SFP_Base::dump_buf (end->rd_ptr (),end->length ());
    }
  end->cont (mb);
  ssize_t n = transport->send (stream.begin ());
  if (n == -1)
    {
      if (TAO_debug_level > 0) ORBSVCS_DEBUG ((LM_DEBUG,
                  "TAO: (%P|%t) closing conn after fault %p\n",
                  "GIOP::send_request ()"));
      return -1;
    }
  // EOF.
  if (n == 0)
    {
      if (TAO_debug_level > 0)
        ORBSVCS_DEBUG ((LM_DEBUG,
                    "TAO: (%P|%t) GIOP::send_request () "
                    "EOF, closing conn:\n"));
      return -1;
    }
  return 1;

}

int
TAO_SFP_Base::peek_message_type (TAO_AV_Transport *transport,
                                 flowProtocol::MsgType &msg_type)
{
  char peek_buffer [TAO_SFP_MAGIC_NUMBER_LEN+2];// 2 is for flags + message_type.
  int peek_len = TAO_SFP_MAGIC_NUMBER_LEN +2;
  char magic_number [TAO_SFP_MAGIC_NUMBER_LEN+1];
  ssize_t n =transport->recv (peek_buffer,
                              peek_len,
                              MSG_PEEK);
  ACE_OS::strncpy (magic_number,
                   peek_buffer,
                   TAO_SFP_MAGIC_NUMBER_LEN);
  magic_number [TAO_SFP_MAGIC_NUMBER_LEN] = 0;
  if (n == -1)
    ORBSVCS_ERROR_RETURN ((LM_ERROR,"SFP::handle_input -peek"),-1);
  else if (n==0)
    ORBSVCS_ERROR_RETURN ((LM_ERROR,"SFP::handle_input -peek"),-1);

  if (ACE_OS::strcmp (magic_number,TAO_SFP_START_MAGIC_NUMBER) == 0)
    {
      if (TAO_debug_level > 0) ORBSVCS_DEBUG ((LM_DEBUG,"(%P|%t)Start message received\n"));
      msg_type = flowProtocol::Start_Msg;
    }
  else if (ACE_OS::strcmp (magic_number,TAO_SFP_STARTREPLY_MAGIC_NUMBER) == 0)
    {
      if (TAO_debug_level > 0) ORBSVCS_DEBUG ((LM_DEBUG,"(%P|%t)StartReply message received\n"));
      msg_type = flowProtocol::StartReply_Msg;
    }
  else if (ACE_OS::strcmp (magic_number,TAO_SFP_MAGIC_NUMBER) == 0)
    {
      if (TAO_debug_level > 0) ORBSVCS_DEBUG ((LM_DEBUG,"(%P|%t) frameHeader received\n"));
      //      msg_type = flowProtocol::SimpleFrame;
      msg_type = (flowProtocol::MsgType)peek_buffer [TAO_SFP_MESSAGE_TYPE_OFFSET];
      if (TAO_debug_level > 0) ORBSVCS_DEBUG ((LM_DEBUG,"Message Type = %d\n",msg_type));
    }
  else if (ACE_OS::strcmp (magic_number,TAO_SFP_FRAGMENT_MAGIC_NUMBER) == 0)
    {
      if (TAO_debug_level > 0) ORBSVCS_DEBUG ((LM_DEBUG,"(%P|%t) fragment Header received\n"));
      msg_type = flowProtocol::Fragment_Msg;
    }
  else if (ACE_OS::strcmp (magic_number,TAO_SFP_CREDIT_MAGIC_NUMBER) == 0)
    {
      if (TAO_debug_level > 0) ORBSVCS_DEBUG ((LM_DEBUG,"(%P|%t) credit message received\n"));
      msg_type = flowProtocol::Credit_Msg;
    }
  else
    ORBSVCS_ERROR_RETURN ((LM_ERROR,"TAO_SFP:Invalid magic number\n"),-1);
  return 0;
}

int
TAO_SFP_Base::read_start_message (TAO_AV_Transport *transport,
                                  flowProtocol::Start &start,
                                  TAO_InputCDR &input)
{
  input.grow (start_len);
  char *buf = input.rd_ptr ();
  int n = transport->recv (buf,
                           start_len);
  if (n != static_cast<int> (start_len))
    ORBSVCS_ERROR_RETURN ((LM_ERROR,"%p","TAO_SFP_Base::read_start\n"),0);
  else
    {
      if (!(input >> start))
        return -1;
    }
  return 0;
}


int
TAO_SFP_Base::read_start_reply_message (TAO_AV_Transport *transport,
                                        flowProtocol::StartReply &start_reply,
                                        TAO_InputCDR &input)
{
  input.grow (start_len);
  char *buf = input.rd_ptr ();
  int n = transport->recv (buf,
                           start_reply_len);
  if (n != static_cast<int> (start_len))
    ORBSVCS_ERROR_RETURN ((LM_ERROR,"%p","TAO_SFP_Base::read_start_reply_message"),0);
  else
    {
      if (!(input >> start_reply))
        return -1;
    }
  return 0;
}

int
TAO_SFP_Base::read_credit_message (TAO_AV_Transport *transport,
                                   flowProtocol::credit &credit,
                                   TAO_InputCDR &input)
{
  input.grow (start_len);
  char *buf = input.rd_ptr ();
  int n = transport->recv (buf,
                           credit_len);
  if (n != static_cast<int> (credit_len))
    ORBSVCS_ERROR_RETURN ((LM_ERROR,"%p","TAO_SFP_Base::read_credit_message"),0);
  else
    {
      if (!(input >> credit))
        return -1;
    }
  return 0;
}

int
TAO_SFP_Base::read_endofstream_message (TAO_AV_Transport *transport,
                                        flowProtocol::frameHeader &endofstream,
                                        TAO_InputCDR &input)
{
  input.grow (start_len);
  char *buf = input.rd_ptr ();
  int n = transport->recv (buf,
                           frame_header_len);
  if (n != static_cast<int> (frame_header_len))
    ORBSVCS_ERROR_RETURN ((LM_ERROR,"%p","TAO_SFP_Base::read_endofstream_message"),0);
  else
    {
      if (!(input >> endofstream))
        return -1;
    }
  return 0;
}

int
TAO_SFP_Base::peek_frame_header (TAO_AV_Transport *transport,
                                 flowProtocol::frameHeader &header,
                                 TAO_InputCDR &input)
{
  input.grow (frame_header_len);
  char *buf = input.rd_ptr ();
  int n = transport->recv (buf,
                           frame_header_len,
                           MSG_PEEK);
  if (n != static_cast<int> (frame_header_len))
    ORBSVCS_ERROR_RETURN ((LM_ERROR,"%p","TAO_SFP_Base::read_endofstream_message"),0);
  else
    {
      if (!(input >> header))
        return -1;
    }
  return 0;
}

int
TAO_SFP_Base::peek_fragment_header (TAO_AV_Transport *transport,
                                    flowProtocol::fragment &fragment,
                                    TAO_InputCDR &input)
{
  input.grow (fragment_len);
  char *buf = input.rd_ptr ();
  int n = transport->recv (buf,
                           fragment_len,
                           MSG_PEEK);
  if (n != static_cast<int> (fragment_len))
    ORBSVCS_ERROR_RETURN ((LM_ERROR,"%p","TAO_SFP_Base::read_endofstream_message"),0);
  else
    {
      if (!(input >> fragment))
        return -1;
    }
  return 0;
}

void
TAO_SFP_Base::dump_buf (char *buffer,int size)
{
  char *buf = buffer;
  if (TAO_debug_level > 0) ORBSVCS_DEBUG ((LM_DEBUG,"\n========================================\n"));
  for (int i=0;i<size;i++)
    if (TAO_debug_level > 0) ORBSVCS_DEBUG ((LM_DEBUG,"%d ",buf[i]));
  if (TAO_debug_level > 0) ORBSVCS_DEBUG ((LM_DEBUG,"\n========================================\n"));
}

//------------------------------------------------------------
// TAO_SFP_Object
//------------------------------------------------------------

TAO_SFP_Object::TAO_SFP_Object (TAO_AV_Callback *callback,
                                TAO_AV_Transport *transport)
  :TAO_AV_Protocol_Object (callback,transport),
   source_id_ (10),
   max_credit_ (-1),
   current_credit_ (-1)
{
  TAO_SFP_BASE::instance ();
  this->state_.static_frame_.size (2* this->transport_->mtu ());
}

TAO_SFP_Object::~TAO_SFP_Object (void)
{
  //no-op
}

int
TAO_SFP_Object::destroy (void)
{
  int result = -1;
  TAO_OutputCDR out_stream;
  result = TAO_SFP_Base::start_frame (TAO_ENCAP_BYTE_ORDER,
                                      flowProtocol::EndofStream_Msg,
                                      out_stream);
  if (result < 0)
    return result;
  result = TAO_SFP_Base::send_message (this->transport_,
                                       out_stream);
  if (result < 0)
    return result;
  this->callback_->handle_destroy ();
  return 0;
}

int
TAO_SFP_Object::send_frame (ACE_Message_Block *frame,
                            TAO_AV_frame_info *frame_info)
{
  TAO_OutputCDR out_stream;
  CORBA::Boolean result = 0;
  if (TAO_debug_level > 0) ORBSVCS_DEBUG ((LM_DEBUG,"TAO_SFP_Object::send_frame\n"));
  CORBA::Octet flags = TAO_ENCAP_BYTE_ORDER;
  if (this->transport_ == 0)
    ORBSVCS_ERROR_RETURN ((LM_ERROR,"TAO_SFP_Object::send_frame: transport is null\n"),-1);
   if (this->current_credit_ != 0)
    {
      // if we have enough credit then we send.
      size_t total_length = 0;
      for (ACE_Message_Block *temp = frame;temp != 0;temp = temp->cont ())
        total_length += temp->length ();
      if (TAO_debug_level > 0) ORBSVCS_DEBUG ((LM_DEBUG,"total_length of frame=%d\n",total_length));
      if (total_length < (TAO_SFP_MAX_PACKET_SIZE -TAO_SFP_Base::frame_header_len))
        {
          if (frame_info != 0)
            {
              if (frame_info->boundary_marker)
                flags |= 4;
              CORBA::Boolean result = TAO_SFP_Base::start_frame (flags,
                                                                 flowProtocol::Frame_Msg,
                                                                 out_stream);
              if (result == 0)
                return 0;
              flowProtocol::my_seq_ulong source_ids;
              source_ids.length (1);
              source_ids [0] = 0;
              TAO_SFP_Base::write_frame_message (frame_info->timestamp,
                                                 frame_info->ssrc,
                                                 source_ids,
                                                 this->sequence_num_,
                                                 out_stream);
            }
          else
            {
              CORBA::Boolean result = TAO_SFP_Base::start_frame (flags,
                                                                 flowProtocol::SimpleFrame_Msg,
                                                                 out_stream);
              if (result == 0)
                return 0;
            }
          TAO_SFP_Base::send_message (this->transport_,
                                      out_stream,
                                      frame);
        }
      else // larger frame,fragment and send it.
        {
          flags = flags | 2;
          if (frame_info != 0)
            {
              if (frame_info->boundary_marker)
                flags |= 4;
              result = TAO_SFP_Base::start_frame (flags,
                                                  flowProtocol::Frame_Msg,
                                                  out_stream);
              if (result == 0)
                return result;
              flowProtocol::my_seq_ulong source_ids;
              source_ids.length (1);
              source_ids [0] = 0;
              TAO_SFP_Base::write_frame_message (frame_info->timestamp,
                                                 frame_info->ssrc,
                                                 source_ids,
                                                 this->sequence_num_,
                                                 out_stream);
            }
          else
            {
              CORBA::Boolean result = TAO_SFP_Base::start_frame (flags,
                                                                 flowProtocol::SimpleFrame_Msg,
                                                                 out_stream);
              if (result == 0)
                return 0;
            }
          size_t last_len,current_len;
          int message_len = static_cast<int> (out_stream.total_length ());
          ACE_Message_Block *mb = frame;
          ACE_Message_Block *fragment_mb =
            this->get_fragment (mb,
                                message_len,
                                last_len,
                                current_len);
          //  This can be either a simpleframe or a sequenced frame,other types of frames.
          TAO_SFP_Base::send_message (this->transport_,
                                      out_stream,
                                      fragment_mb);
          out_stream.reset ();
          int frag_number = 1;
          mb->length (last_len);
          mb->rd_ptr (current_len);
          // If there is any more data send those as fragments.
          while (mb != 0)
            {
              message_len = TAO_SFP_Base::fragment_len;
              fragment_mb = this->get_fragment (mb,
                                                message_len,
                                                last_len,
                                                current_len);
              if (mb == 0)
                {
                  if (TAO_debug_level > 0) ORBSVCS_DEBUG ((LM_DEBUG,"sending the last fragment\n"));
                  // This is the last fragment so clear the fragments bit.
                  flags = TAO_ENCAP_BYTE_ORDER;
                }
              if (fragment_mb == 0)
                break;
              if (frame_info != 0)
                {
                  TAO_SFP_Base::write_fragment_message (flags,
                                                        frag_number++,
                                                        this->sequence_num_,
                                                        frame_info->ssrc,
                                                        out_stream);
                }
              else
                {
                  TAO_SFP_Base::write_fragment_message (flags,
                                                        frag_number++,
                                                        this->sequence_num_,
                                                        0,
                                                        out_stream);
                }
              //   send the fragment now.
              // without the sleep the fragments gets lost!
              // probably because the UDP buffer queue on the sender side
              // is overflown it drops the packets.
              // XXX: This is a hack.
              ACE_OS::sleep (1);
              result = TAO_SFP_Base::send_message (this->transport_,
                                                   out_stream,
                                                   fragment_mb);
              if (mb != 0)
                {
                  mb->length (last_len);
                  mb->rd_ptr (current_len);
                }
            }
          // Increment the sequence_num after sending the message.
          this->sequence_num_++;
          // Also reduce the number of credits.
          if (this->max_credit_ > 0)
            this->current_credit_--;
        }
    }
  else
    {
      // flow controlled so wait.
      // A greater than 0 value indicates that flow control is being exercised.
      return 1;
    }
   return 0;
}

int
TAO_SFP_Object::send_frame (const iovec * /*iov*/,
                            int /*iovcnt*/,
                            TAO_AV_frame_info * /*frame_info*/)
{
  ORBSVCS_ERROR_RETURN ((LM_ERROR,"TAO_AV_SFP_Object::send_frame"),-1);
}

int
TAO_SFP_Object::send_frame (const char* /*buf*/,
                               size_t /*len*/)
{
  return 0;
}


ACE_Message_Block*
TAO_SFP_Object::get_fragment (ACE_Message_Block *&mb,
                              size_t initial_len,
                              size_t &last_mb_orig_len,
                              size_t &last_mb_current_len)
{
  ACE_Message_Block *fragment_mb = 0,*temp_mb = 0;
  size_t prev_len,last_len = 0;
  size_t current_len = 0;
  size_t message_len = initial_len;
  while (mb != 0)
    {
      prev_len = message_len;
      message_len += mb->length ();
      if (fragment_mb == 0)
        fragment_mb = temp_mb = mb->duplicate ();
      if (message_len > TAO_SFP_MAX_PACKET_SIZE)
        {
          // get only the length that we can accomodate.
          current_len = TAO_SFP_MAX_PACKET_SIZE - prev_len;
          if (current_len < mb->length ())
            {
              // The above condition is an assertion.
              message_len += (current_len-mb->length ());
              last_len = mb->length ();
              mb->length (current_len);
              temp_mb->length (current_len);
            }
          break;
        }
      else
        {
          // we can accomodate this message block
          message_len += mb->length ();
          mb = mb->cont ();
          temp_mb = temp_mb->cont ();
        }
    }
  last_mb_orig_len = last_len;
  last_mb_current_len = current_len;
  return fragment_mb;
}

int
TAO_SFP_Object::set_policies (const TAO_AV_PolicyList& policies)
{
  TAO_AV_Policy *policy = 0;
  for (CORBA::ULong i=0;i<policies.length ();i++)
    {
      policy = policies[i];
      switch (policies[i]->type ())
        {

        case TAO_AV_SFP_CREDIT_POLICY:
          {
            TAO_AV_SFP_Credit_Policy *credit_policy =
              reinterpret_cast<TAO_AV_SFP_Credit_Policy*> (policy);
            this->max_credit_ = credit_policy->value ();
          }
        default:
          break;
        }
    }
  return 0;
}

// TAO_SFP_Consumer_Object
TAO_SFP_Consumer_Object::TAO_SFP_Consumer_Object (TAO_AV_Callback *callback,
                                                  TAO_AV_Transport *transport,
                                                  ACE_CString& sfp_options)
  :TAO_SFP_Object (callback,transport)
{
  TAO_AV_PolicyList policies = callback->get_policies ();
  if (policies.length () == 0)
    return;
  this->set_policies (policies);
  if (this->max_credit_ > 0)
    {
      sfp_options = "sfp:1.0:credit=";
      char buf[10];
      ACE_OS::sprintf(buf, "%d", this->max_credit_);
      sfp_options += buf;
    }
}

int
TAO_SFP_Consumer_Object::handle_input (void)
{
  if (TAO_debug_level > 0) ORBSVCS_DEBUG ((LM_DEBUG,"TAO_SFP_Consumer_Object::handle_input\n"));
  // This is the entry point for receiving data.
  TAO_AV_frame_info *frame_info = 0;
  int result = TAO_SFP_Base::handle_input (this->transport_,
                                           this->state_,
                                           frame_info);
  if (result < 0)
    ORBSVCS_ERROR_RETURN ((LM_ERROR,"ERROR in TAO_SFP_Consumer_Object::handle_input"),result);
  if (this->state_.frame_header_.message_type == flowProtocol::EndofStream_Msg)
    this->callback_->handle_destroy ();
  if (this->state_.is_complete ())
    {
      this->callback_->receive_frame (this->state_.frame_block_,
                                      frame_info);
      // Now release the memory for the frame.
      if (this->state_.frame_block_ != &this->state_.static_frame_)
        {
          ACE_Message_Block *temp = 0;
          for (temp = this->state_.frame_block_;
               temp != 0;
               temp = temp->cont ())
            {
              temp->release ();
              delete temp;
            }
        }
      this->state_.reset ();
    }
  return 0;
}

TAO_SFP_Producer_Object::TAO_SFP_Producer_Object (TAO_AV_Callback *callback,
                                                  TAO_AV_Transport *transport,
                                                  const char *sfp_options)
  :TAO_SFP_Object (callback,transport),
   credit_sequence_num_ (0)

{
  TAO_Tokenizer flow_string (sfp_options,':');
  if (flow_string [2] != 0)
    {
      TAO_Tokenizer options (flow_string[2],'=');
      if (options [1] != 0)
        this->max_credit_ = ACE_OS::atoi (options[1]);
    }
}

int
TAO_SFP_Producer_Object::handle_input (void)
{
  // A producer can only receive credit messages.
  int result;
  flowProtocol::MsgType msg_type = flowProtocol::Start_Msg;
  result = TAO_SFP_Base::peek_message_type (this->transport_,
                                            msg_type);
  if (result < 0)
    return result;
  switch (msg_type)
    {
    case flowProtocol::Credit_Msg:
      {
        flowProtocol::credit credit;
        result = TAO_SFP_Base::read_credit_message (this->transport_,
                                                    credit,
                                                    this->state_.cdr);
        if (result < 0)
          return result;
      if (!this->credit_sequence_num_)
        this->credit_sequence_num_ = credit.cred_num;
      else
        {
          // check that the sequence number is above the last sequence number
          // else its a duplicate credit message so we can ignore it.
          if (credit.cred_num <= this->credit_sequence_num_)
            return 0;
          else      // Update our credit now.
            this->current_credit_ = this->max_credit_;
        }
      }
      break;
    default:
      {
        ACE_Message_Block mb (2*this->transport_->mtu ());

      // Ignore the rest of the message by just reading.
        this->transport_->recv (mb.rd_ptr (),
                                mb.size ());
        break;
      }
    }
  return 0;
}

// TAO_AV_SFP_Factory
TAO_AV_SFP_Factory::TAO_AV_SFP_Factory (void)
{
}

TAO_AV_SFP_Factory::~TAO_AV_SFP_Factory (void)
{
}

// Initialization hook from service configurator.
int
TAO_AV_SFP_Factory::init (int /*argc*/, ACE_TCHAR ** /*argv*/)
{
  return 0;
}

int
TAO_AV_SFP_Factory::match_protocol (const char *flow_string)
{
  if (ACE_OS::strncasecmp (flow_string,"sfp",3) == 0)
    return 1;
  return 0;
}

TAO_AV_Protocol_Object*
TAO_AV_SFP_Factory::make_protocol_object (TAO_FlowSpec_Entry *entry,
                                          TAO_Base_StreamEndPoint *endpoint,
                                          TAO_AV_Flow_Handler *handler,
                                          TAO_AV_Transport *transport)
{
  TAO_AV_Protocol_Object *object = 0;
  TAO_AV_Callback *callback = 0;
  endpoint->get_callback (entry->flowname (),
                       callback);
  ACE_CString flow_string( entry->flow_protocol_str () );
  switch (entry->role ())
    {
    case TAO_FlowSpec_Entry::TAO_AV_PRODUCER:
      {
        ACE_NEW_RETURN (object,
                        TAO_SFP_Producer_Object (callback,
                                                 transport,
                                                 flow_string.c_str() ),
                        0);
      }
      break;
    case TAO_FlowSpec_Entry::TAO_AV_CONSUMER:
      {

        ACE_NEW_RETURN (object,
                        TAO_SFP_Consumer_Object (callback,
                                                 transport,
                                                 flow_string),
                        0);
        entry->flow_protocol_str( flow_string.c_str() );
      }
      break;
    case TAO_FlowSpec_Entry::TAO_AV_INVALID_ROLE:
      return 0;
    }
  callback->open (object,
                  handler);
  endpoint->set_protocol_object (entry->flowname (),
                                 object);
  return object;
}

//------------------------------------------------------------
// TAO_SFP_Frame_State
//------------------------------------------------------------

TAO_SFP_Frame_State::TAO_SFP_Frame_State (void)
  :cdr (new ACE_Data_Block (ACE_CDR::DEFAULT_BUFSIZE,
                            ACE_Message_Block::MB_DATA,
                            0,
                            0,
                            0,
                            0,
                            0),
        0,
        TAO_ENCAP_BYTE_ORDER),
   more_fragments_ (0),
   frame_block_ (0)
{
}

CORBA::Boolean
TAO_SFP_Frame_State::is_complete (void)
{
  return (!this->more_fragments_) &&  (this->frame_block_);
}

int
TAO_SFP_Frame_State::reset (void)
{
  this->frame_block_ = 0;
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_STATIC_TEMPLATE_MEMBER_INSTANTIATION)
template ACE_Singleton<TAO_SFP_Base, TAO_SYNCH_MUTEX> *ACE_Singleton<TAO_SFP_Base, TAO_SYNCH_MUTEX>::singleton_;
#endif /* ACE_HAS_EXPLICIT_STATIC_TEMPLATE_MEMBER_INSTANTIATION */

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_FACTORY_DEFINE (TAO_AV, TAO_AV_SFP_Factory)
ACE_STATIC_SVC_DEFINE (TAO_AV_SFP_Factory,
                       ACE_TEXT ("SFP_Factory"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_AV_SFP_Factory),
                       ACE_Service_Type::DELETE_THIS |
                       ACE_Service_Type::DELETE_OBJ,
                       0)
