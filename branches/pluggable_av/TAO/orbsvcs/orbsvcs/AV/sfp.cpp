// $Id$

#include "sfp.h"
#include "ace/ARGV.h"

// default arguments to pass to use for the ORB
const char *TAO_SFP_Base::TAO_SFP_ORB_ARGUMENTS = "-ORBobjrefstyle URL";

// SFP magic numbers
const char *TAO_SFP_Base::TAO_SFP_MAGIC_NUMBER = "=SFP";
const char *TAO_SFP_Base::TAO_SFP_FRAGMENT_MAGIC_NUMBER = "FRAG";
const char *TAO_SFP_Base::TAO_SFP_START_MAGIC_NUMBER = "=STA";
const char *TAO_SFP_Base::TAO_SFP_CREDIT_MAGIC_NUMBER = "=CRE";
const char *TAO_SFP_Base::TAO_SFP_STARTREPLY_MAGIC_NUMBER = "=STR";

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

int
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
  ACE_DECLARE_NEW_CORBA_ENV;
  flowProtocol::frameHeader frame_header;
  flowProtocol::fragment fragment;
  flowProtocol::credit credit;
  flowProtocol::Start start;
  flowProtocol::StartReply start_reply;
  ACE_TRY
    {
      // fill in the default frameHeader fields.
      frame_header.magic_number [0] = '=';
      frame_header.magic_number [1] = 'S';
      frame_header.magic_number [2] = 'F';
      frame_header.magic_number [3] = 'P';
      frame_header.flags = TAO_ENCAP_BYTE_ORDER;
      output_cdr.reset ();
      output_cdr.encode (flowProtocol::_tc_frameHeader,
                         &frame_header,
                         0,
                         ACE_TRY_ENV);
      ACE_TRY_CHECK;
      frame_header_len = output_cdr.total_length ();
      // fill in the default fragment message fields.
      fragment.magic_number [0] = 'F';
      fragment.magic_number [1] = 'R';
      fragment.magic_number [2] = 'A';
      fragment.magic_number [3] = 'G';
      output_cdr.reset ();
      output_cdr.encode (flowProtocol::_tc_fragment,
                                &fragment,
                                0,
                                ACE_TRY_ENV);
      ACE_TRY_CHECK;
      fragment_len = output_cdr.total_length ();
      // fill in the default Start message fields.
      start.magic_number [0] = '=';
      start.magic_number [1] = 'S';
      start.magic_number [2] = 'T';
      start.magic_number [3] = 'A';
      start.major_version = TAO_SFP_Base::TAO_SFP_MAJOR_VERSION;
      start.minor_version = TAO_SFP_Base::TAO_SFP_MINOR_VERSION;
      start.flags = 0;
      output_cdr.reset ();
      output_cdr.encode (flowProtocol::_tc_Start,
                         &start,
                         0,
                         ACE_TRY_ENV);
      ACE_TRY_CHECK;
      start_len = output_cdr.total_length ();
      // fill in the default StartReply message fields.
      start_reply.magic_number [0] = '=';
      start_reply.magic_number [1] = 'S';
      start_reply.magic_number [2] = 'T';
      start_reply.magic_number [3] = 'R';
      start_reply.flags = 0;
      output_cdr.reset ();
      output_cdr.encode (flowProtocol::_tc_StartReply,
                         &start_reply,
                         0,
                         ACE_TRY_ENV);
      ACE_TRY_CHECK;
      start_reply_len = output_cdr.total_length ();

      // fill in the default Credit message fields.
      credit.magic_number [0] = '=';
      credit.magic_number [1] = 'C';
      credit.magic_number [2] = 'R';
      credit.magic_number [3] = 'E';
      output_cdr.reset ();
      output_cdr.encode (flowProtocol::_tc_credit,
                         &credit,
                         0,
                         ACE_TRY_ENV);
      ACE_TRY_CHECK;
      credit_len = output_cdr.total_length ();
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"TAO_SFP constructor");
    }
  ACE_ENDTRY;
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
        if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG,"Fragment received\n"));
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
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG,"Reading simple frame\n"));
      // Check to see what the length of the message is.
      int byte_order = frame_header.flags & 0x1;
      int message_len = frame_header.message_size;

      ACE_NEW_RETURN (message_block,
                      ACE_Message_Block (message_len),
                      0);
      int n = transport->recv (message_block->wr_ptr (),message_len);
      if (n == -1)
        ACE_ERROR_RETURN ((LM_ERROR,"SFP::handle_input -peek"),0);
      else if (n==0)
        ACE_ERROR_RETURN ((LM_ERROR,"SFP::handle_input -peek"),0);
      else if (n != message_len)
        ACE_ERROR_RETURN ((LM_ERROR,"SFP::read_simple_frame:message truncated\n"),0);
      // print the buffer.
      //      this->dump_buf (message,n);
      // skip over the frame header.
      message_block->rd_ptr (frame_header_len);
      message_block->wr_ptr (n);
      CORBA::ULong ssrc = 0;
      TAO_SFP_Fragment_Table_Entry *fragment_entry = 0;
      if (frame_header.flags & 0x2)
        {
          if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG,"fragmented frame:0th fragment\n"));
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
                frame_info_cdr.decode (flowProtocol::_tc_frame,
                                       &state.frame_,
                                       0,
                                       ACE_TRY_ENV);
                ACE_TRY_CHECK;
                if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG,"frame.timestamp = %d, frame.synchsource = %d, frame.sequence_num = %d\n",
                            state.frame_.timestamp,
                            state.frame_.synchSource,
                            state.frame_.sequence_num));
                ssrc = state.frame_.synchSource;
                // The remaining message in the CDR stream is the fragment data for frag.0
                data = frame_info_cdr.start ()->clone ();
                break;
              }
            case flowProtocol::SimpleFrame_Msg:
              {
                data = message_block;
                break;
              }
            case flowProtocol::SequencedFrame_Msg:
              break;
            case flowProtocol::SpecialFrame_Msg:
              break;
            }
          if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG,"Length of 0th fragment= %d\n",data->length ()));
          TAO_SFP_Fragment_Table *fragment_table = 0;
          result = state.fragment_table_map_.find (ssrc,fragment_table);
          if (result != 0)
            {
              ACE_NEW_RETURN (fragment_table,
                              TAO_SFP_Fragment_Table,
                              -1);
              result = state.fragment_table_map_.bind (ssrc,fragment_table);
              if (result < 0)
                ACE_ERROR_RETURN ((LM_ERROR,"TAO_SFP_Base::read_frame: fragment_table_map:bind failed\n"),-1);
            }

          TAO_SFP_Fragment_Node *new_node;
          ACE_NEW_RETURN (new_node,
                          TAO_SFP_Fragment_Node,
                          0);
          new_node->fragment_info_.frag_sz = data->length ();
          new_node->fragment_info_.frag_number = 0;
          if (state.frame_.source_ids.length () > 0)
            new_node->fragment_info_.source_id = state.frame_.source_ids [0];
          else
            new_node->fragment_info_.source_id = 0;
          new_node->data_ = data;
          //          TAO_SFP_Base::dump_buf (data->rd_ptr (),data->length ());
          if (fragment_table->find (state.frame_.sequence_num,fragment_entry) == 0)
            {
              // This case can happen where a nth (n > 0)fragment is received before the 0th fragment.
              if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG,"fragment table entry found for 0th fragment:\n"));
              result = fragment_entry->fragment_set_.insert (*new_node);
              if (result != 0)
                ACE_ERROR_RETURN ((LM_ERROR,"insert for 0th fragment failed\n"),0);
              //  enter the frame info.

              // check if all the fragments have been received.
              state.frame_block_ = TAO_SFP_Base::check_all_fragments (fragment_entry);
              if (state.frame_block_ != 0)
                state.more_fragments_ = 0;
            }
          else
            {
              if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG,"fragment table entry not found for 0th fragment\n"));
              TAO_SFP_Fragment_Table_Entry *new_entry;
              ACE_NEW_RETURN (new_entry,
                              TAO_SFP_Fragment_Table_Entry,
                              0);
              result = new_entry->fragment_set_.insert (*new_node);
              if (result != 0)
                ACE_ERROR_RETURN ((LM_ERROR,"insert for 0th fragment failed\n"),0);
              fragment_entry = new_entry;
              // not found. so bind a new entry.
              result = fragment_table->bind (state.frame_.sequence_num,new_entry);
              if (result != 0)
                ACE_ERROR_RETURN ((LM_ERROR,"fragment table bind failed\n"),0);
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
    }
  ACE_CATCHANY
    {
      ACE_TRY_ENV.print_exception ("read_simple_frame");
      return 0;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);
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

  if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG,"frag_number = %d, frag_size = %d,source_id  = %d sequnce_num = %d\n",
              fragment.frag_number,fragment.frag_sz,fragment.source_id,fragment.sequence_num));

  ACE_Message_Block *data;
  ACE_NEW_RETURN (data,
                  ACE_Message_Block(fragment.frag_sz),
                  -1);

  // Read the fragment.
  int n = transport->recv (data->wr_ptr (),fragment.frag_sz);
  if ((n == -1) || (n==0))
    ACE_ERROR_RETURN ((LM_ERROR,"TAO_SFP::read_fragment:%p",""),-1);
  // move past the fragment header.
  data->rd_ptr (fragment_len);
  data->wr_ptr (n);
  //  TAO_SFP_Base::dump_buf (data->rd_ptr (),data->length ());
  if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG,"length of %dth fragment is: %d\n",
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
        ACE_ERROR_RETURN ((LM_ERROR,"TAO_SFP_Base::read_fragment:fragment_table_map:bind failed\n"),-1);
    }
  if (fragment_table->find (fragment.sequence_num,fragment_entry) == 0)
    {
      // Already an entry exists. Traverse the list and insert it at the right place.
      result = fragment_entry->fragment_set_.insert (*new_node);
      if (result != 0)
        ACE_ERROR_RETURN ((LM_ERROR,"insert for %dth node failed\n",fragment.frag_number),-1);
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
        ACE_ERROR_RETURN ((LM_ERROR,"bind for %dth fragment failed\n",
                           fragment.frag_number),-1);
    }
  if (!(fragment.flags & 0x2))
    {
      if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG,"Last fragment received\n"));
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
  if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG,"table size: %d, num_fragments: %d\n",fragment_entry->fragment_set_.size (),fragment_entry->num_fragments_));
  // check to see if all the frames have been received.
  if (fragment_entry->fragment_set_.size () == fragment_entry->num_fragments_)
    {
      if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG,"all fragments have been received\n"));
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
  ACE_DECLARE_NEW_CORBA_ENV;
  msg.reset ();
  flowProtocol::frameHeader frame_header;
  ACE_TRY
    {

      frame_header.magic_number [0] = '=';
      frame_header.magic_number [1] = 'S';
      frame_header.magic_number [2] = 'F';
      frame_header.magic_number [3] = 'P';
      frame_header.flags = flags;
      frame_header.message_type = type;
      frame_header.message_size = 0;
      msg.encode (flowProtocol::_tc_frameHeader,
                  &frame_header,
                  0,
                  ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"TAO_SFP_Base::start_frame");
      return 0;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);
  return 1;
}

CORBA::Boolean
TAO_SFP_Base::write_start_message (TAO_OutputCDR &msg)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  flowProtocol::Start start;
  ACE_TRY
    {
      start.magic_number [0] = '=';
      start.magic_number [1] = 'S';
      start.magic_number [2] = 'T';
      start.magic_number [3] = 'A';
      start.major_version = TAO_SFP_MAJOR_VERSION;
      start.minor_version = TAO_SFP_MINOR_VERSION;
      start.flags = 0;
      msg.encode (flowProtocol::_tc_Start,
                  &start,
                  0,
                  ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"TAO_SFP_Base::write_start_message");
      return 0;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);
  return 1;
}

CORBA::Boolean
TAO_SFP_Base::write_start_reply_message (TAO_OutputCDR &msg)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  flowProtocol::StartReply start_reply;
  ACE_TRY
    {
      start_reply.magic_number [0] = '=';
      start_reply.magic_number [1] = 'S';
      start_reply.magic_number [2] = 'T';
      start_reply.magic_number [3] = 'R';
      start_reply.flags = 0;
      msg.encode (flowProtocol::_tc_StartReply,
                  &start_reply,
                  0,
                  ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"TAO_SFP_Base::write_start_reply_message");
      return 0;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);
  return 1;
}

CORBA::Boolean
TAO_SFP_Base::write_credit_message (CORBA::ULong cred_num,
                                    TAO_OutputCDR &msg)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  flowProtocol::credit credit;
  ACE_TRY
    {
      credit.magic_number [0] = '=';
      credit.magic_number [1] = 'C';
      credit.magic_number [2] = 'R';
      credit.magic_number [3] = 'E';
      credit.cred_num = cred_num;
      msg.encode (flowProtocol::_tc_credit,
                  &credit,
                  0,
                  ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"TAO_SFP_Base::write_credit_message");
      return 0;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);
  return 1;
}

CORBA::Boolean
TAO_SFP_Base::write_fragment_message (CORBA::Octet flags,
                                      CORBA::ULong fragment_number,
                                      CORBA::ULong sequence_number,
                                      CORBA::ULong source_id,
                                      TAO_OutputCDR &msg)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  msg.reset ();
  flowProtocol::fragment fragment;
  ACE_TRY
    {
      fragment.magic_number [0] = 'F';
      fragment.magic_number [1] = 'R';
      fragment.magic_number [2] = 'A';
      fragment.magic_number [3] = 'G';
      fragment.flags = flags;
      fragment.frag_number = fragment_number;
      fragment.sequence_num = sequence_number;
      fragment.source_id = source_id;
      msg.encode (flowProtocol::_tc_fragment,
                  &fragment,
                  0,
                  ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"TAO_SFP_Base::write_fragment_message");
      return 0;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);
  return 1;
}

CORBA::Boolean
TAO_SFP_Base::write_frame_message (CORBA::ULong timestamp,
                                   CORBA::ULong synchSource,
                                   flowProtocol::my_seq_ulong source_ids,
                                   CORBA::ULong sequence_num,
                                   TAO_OutputCDR &msg)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  flowProtocol::frame frame;
  ACE_TRY
    {
      frame.timestamp = timestamp;
      frame.synchSource = synchSource;
      frame.source_ids = source_ids;
      frame.sequence_num = sequence_num;
      msg.encode (flowProtocol::_tc_frame,
                  &frame,
                  0,
                  ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"TAO_SFP_Base::write_frame_message");
      return 0;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);
  return 1;
}

int
TAO_SFP_Base::send_message (TAO_AV_Transport *transport,
                            TAO_OutputCDR &stream,
                            ACE_Message_Block *mb)
{
  size_t total_len = stream.total_length ();
  if (mb != 0)
    {
      for (ACE_Message_Block *temp = mb;temp != 0;temp = temp->cont ())
        total_len += temp->length ();

      char *buf = (char *) stream.buffer ();
      size_t offset = TAO_SFP_MESSAGE_SIZE_OFFSET;
      // second character distinguished =SFP and FRAG.
      if (*(buf) == 'F')
        {
          // Fragment message.
          offset = TAO_SFP_FRAGMENT_SIZE_OFFSET;
        }
#if !defined (ACE_ENABLE_SWAP_ON_WRITE)
      *ACE_reinterpret_cast (CORBA::ULong *, buf + offset) = total_len;
#else
      if (!stream->do_byte_swap ())
        *ACE_reinterpret_cast (CORBA::ULong *,
                               buf + offset) = total_len;
      else
        ACE_CDR::swap_4 (ACE_reinterpret_cast (char *,
                                               &total_len),
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
      if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG,
                  "TAO: (%P|%t) closing conn after fault %p\n",
                  "GIOP::send_request ()"));
      return -1;
    }
  // EOF.
  if (n == 0)
    {
      if (TAO_orbdebug)
        if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG,
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
    ACE_ERROR_RETURN ((LM_ERROR,"SFP::handle_input -peek"),-1);
  else if (n==0)
    ACE_ERROR_RETURN ((LM_ERROR,"SFP::handle_input -peek"),-1);

  if (ACE_OS::strcmp (magic_number,TAO_SFP_START_MAGIC_NUMBER) == 0)
    {
      if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG,"(%P|%t)Start message received\n"));
      msg_type = flowProtocol::Start_Msg;
    }
  else if (ACE_OS::strcmp (magic_number,TAO_SFP_STARTREPLY_MAGIC_NUMBER) == 0)
    {
      if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG,"(%P|%t)StartReply message received\n"));
      msg_type = flowProtocol::StartReply_Msg;
    }
  else if (ACE_OS::strcmp (magic_number,TAO_SFP_MAGIC_NUMBER) == 0)
    {
      if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG,"(%P|%t) frameHeader received\n"));
      //      msg_type = flowProtocol::SimpleFrame;
      msg_type = (flowProtocol::MsgType)peek_buffer [TAO_SFP_MESSAGE_TYPE_OFFSET];
      if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG,"Message Type = %d\n",msg_type));
    }
  else if (ACE_OS::strcmp (magic_number,TAO_SFP_FRAGMENT_MAGIC_NUMBER) == 0)
    {
      if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG,"(%P|%t) fragment Header received\n"));
      msg_type = flowProtocol::Fragment_Msg;
    }
  else if (ACE_OS::strcmp (magic_number,TAO_SFP_CREDIT_MAGIC_NUMBER) == 0)
    {
      if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG,"(%P|%t) credit message received\n"));
      msg_type = flowProtocol::Credit_Msg;
    }
  else
    ACE_ERROR_RETURN ((LM_ERROR,"TAO_SFP:Invalid magic number\n"),-1);
  return 0;
}

int
TAO_SFP_Base::read_start_message (TAO_AV_Transport *transport,
                                  flowProtocol::Start &start,
                                  TAO_InputCDR &input)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      input.grow (start_len);
      char *buf = input.rd_ptr ();
      int n = transport->recv (buf,
                               start_len);
      if (n != ACE_static_cast (int, start_len))
        ACE_ERROR_RETURN ((LM_ERROR,"%p","TAO_SFP_Base::read_start\n"),0);
      else
        {
          input.decode (flowProtocol::_tc_Start,
                      &start,
                      0,
                      ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"TAO_SFP_Base::read_start_message");
      return -1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);
  return 0;
}


int
TAO_SFP_Base::read_start_reply_message (TAO_AV_Transport *transport,
                                        flowProtocol::StartReply &start_reply,
                                        TAO_InputCDR &input)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      input.grow (start_len);
      char *buf = input.rd_ptr ();
      int n = transport->recv (buf,
                               start_reply_len);
      if (n != ACE_static_cast (int, start_len))
        ACE_ERROR_RETURN ((LM_ERROR,"%p","TAO_SFP_Base::read_start_reply_message"),0);
      else
        {
          input.decode (flowProtocol::_tc_StartReply,
                      &start_reply,
                      0,
                      ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"TAO_SFP_Base::read_start_reply_message");
      return -1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);
  return 0;
}

int
TAO_SFP_Base::read_credit_message (TAO_AV_Transport *transport,
                                   flowProtocol::credit &credit,
                                   TAO_InputCDR &input)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      input.grow (start_len);
      char *buf = input.rd_ptr ();
      int n = transport->recv (buf,
                               credit_len);
      if (n != ACE_static_cast (int, credit_len))
        ACE_ERROR_RETURN ((LM_ERROR,"%p","TAO_SFP_Base::read_credit_message"),0);
      else
        {
          input.decode (flowProtocol::_tc_credit,
                      &credit,
                      0,
                      ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"TAO_SFP_Base::read_credit_message");
      return -1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);
  return 0;
}

int
TAO_SFP_Base::read_endofstream_message (TAO_AV_Transport *transport,
                                        flowProtocol::frameHeader &endofstream,
                                        TAO_InputCDR &input)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      input.grow (start_len);
      char *buf = input.rd_ptr ();
      int n = transport->recv (buf,
                               frame_header_len);
      if (n != ACE_static_cast (int, frame_header_len))
        ACE_ERROR_RETURN ((LM_ERROR,"%p","TAO_SFP_Base::read_endofstream_message"),0);
      else
        {
          input.decode (flowProtocol::_tc_frameHeader,
                      &endofstream,
                      0,
                      ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"TAO_SFP_Base::read_credit_message");
      return -1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);
  return 0;
}

int
TAO_SFP_Base::peek_frame_header (TAO_AV_Transport *transport,
                                 flowProtocol::frameHeader &header,
                                 TAO_InputCDR &input)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      input.grow (frame_header_len);
      char *buf = input.rd_ptr ();
      int n = transport->recv (buf,
                               frame_header_len,
                               MSG_PEEK);
      if (n != ACE_static_cast (int, frame_header_len))
        ACE_ERROR_RETURN ((LM_ERROR,"%p","TAO_SFP_Base::read_endofstream_message"),0);
      else
        {
          input.decode (flowProtocol::_tc_frameHeader,
                      &header,
                      0,
                      ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }
      //      TAO_SFP_Base::dump_buf (buf,n);
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"TAO_SFP_Base::read_credit_message");
      return -1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);
  return 0;
}

int
TAO_SFP_Base::peek_fragment_header (TAO_AV_Transport *transport,
                                    flowProtocol::fragment &fragment,
                                    TAO_InputCDR &input)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      input.grow (fragment_len);
      char *buf = input.rd_ptr ();
      int n = transport->recv (buf,
                               fragment_len,
                               MSG_PEEK);
      if (n != ACE_static_cast (int, fragment_len))
        ACE_ERROR_RETURN ((LM_ERROR,"%p","TAO_SFP_Base::read_endofstream_message"),0);
      else
        {
          input.decode (flowProtocol::_tc_fragment,
                      &fragment,
                      0,
                      ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"TAO_SFP_Base::read_credit_message");
      return -1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);
  return 0;
}

void
TAO_SFP_Base::dump_buf (char *buffer,int size)
{
  char *buf = buffer;
  if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG,"\n========================================\n"));
  for (int i=0;i<size;i++)
    if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG,"%d ",buf[i]));
  if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG,"\n========================================\n"));
}

// //------------------------------------------------------------
// //TAO_SFP_UDP_Protocol_Factory
// //------------------------------------------------------------

// TAO_SFP_UDP_Protocol_Factory::TAO_SFP_UDP_Protocol_Factory (void)
// {
// }

// TAO_SFP_UDP_Protocol_Factory::~TAO_SFP_UDP_Protocol_Factory (void)
// {
// }

// TAO_AV_Acceptor *
// TAO_SFP_UDP_Protocol_Factory::make_acceptor (void)
// {
//   TAO_SFP_UDP_Acceptor *acceptor = 0;
//   ACE_NEW_RETURN (acceptor,
//                   TAO_SFP_UDP_Acceptor,
//                   0);
//   return acceptor;
// }

// TAO_AV_Connector *
// TAO_SFP_UDP_Protocol_Factory::make_connector (void)
// {
//   TAO_SFP_UDP_Connector *connector = 0;
//   ACE_NEW_RETURN (connector,
//                   TAO_SFP_UDP_Connector,
//                   0);
//   return connector;
// }

// int
// TAO_SFP_UDP_Protocol_Factory::match_protocol (TAO_AV_Core::Protocol protocol)
// {
//   return (protocol == TAO_AV_Core::TAO_AV_SFP_UDP);
// }

// //
// // TAO_SFP_UDP_Acceptor
// TAO_SFP_UDP_Acceptor::TAO_SFP_UDP_Acceptor (void)
// {
// }

// TAO_SFP_UDP_Acceptor::~TAO_SFP_UDP_Acceptor (void)
// {
// }

// int
// TAO_SFP_UDP_Acceptor::make_svc_handler (TAO_AV_UDP_Flow_Handler *&handler)
// {
//   // @@ We should actually look at the entry and find out if we're a
//   // sink or a source and create the appropriate handler
//   if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG,"TAO_SFP_UDP_Acceptor::make_svc_handler\n"));
//   TAO_SFP_BASE::instance ();
//   TAO_SFP_Object *object;
//   TAO_AV_Callback *callback;
//   this->endpoint_->get_callback (this->entry_->flowname (),callback);
//   ACE_NEW_RETURN (object,
//                   TAO_SFP_Object (callback),
//                   -1);
//   ACE_NEW_RETURN (handler,
//                   TAO_SFP_UDP_Sender_Handler (callback,
//                                               object),
//                   -1);
//   callback->transport (handler->transport ());
//   callback->protocol_object (object);
//   object->transport (handler->transport ());
//   this->endpoint_->set_protocol_object (this->entry_->flowname (),
//                                         object);
//   this->endpoint_->set_handler (this->flowname_.c_str (),handler);
//   this->entry_->protocol_object (object);
//   return 0;
// }
// //------------------------------------------------------------
// // TAO_SFP_UDP_Connector
// //------------------------------------------------------------
// TAO_SFP_UDP_Connector::TAO_SFP_UDP_Connector (void)
// {
// }

// TAO_SFP_UDP_Connector::~TAO_SFP_UDP_Connector (void)
// {
// }

// int
// TAO_SFP_UDP_Connector::make_svc_handler (TAO_AV_UDP_Flow_Handler *&handler)
// {
//   if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG,"TAO_SFP_UDP_Connector::make_svc_handler\n"));
//   TAO_SFP_BASE::instance ();
//   // @@ We should actually look at the entry and find out if we're a
//   // sink or a source and create the appropriate handler
//   TAO_AV_Callback *callback = 0;
//   this->endpoint_->get_callback (this->entry_->flowname (),
//                                  callback);
//   ACE_NEW_RETURN (handler,
//                   TAO_SFP_UDP_Receiver_Handler (callback),
//                   -1);
//   TAO_SFP_Object *object;
//   ACE_NEW_RETURN (object,
//                   TAO_SFP_Object (callback),
//                   -1);
//   callback->transport (handler->transport ());
//   callback->protocol_object (object);
//   object->transport (handler->transport ());
//   this->endpoint_->set_protocol_object (this->entry_->flowname (),
//                                         object);
//   this->endpoint_->set_handler (this->flowname_.c_str (),handler);
//   this->entry_->protocol_object (object);
//   return 0;
// }

// //------------------------------------------------------------
// // TAO_SFP_UDP_Sender_Handler
// //------------------------------------------------------------
// TAO_SFP_UDP_Sender_Handler::TAO_SFP_UDP_Sender_Handler (TAO_AV_Callback *callback,
//                                                         TAO_SFP_Object *object)
//   :TAO_AV_Flow_Handler (callback),
//    TAO_AV_UDP_Flow_Handler (callback),
//    sfp_object_ (object)
// {
// }

// int
// TAO_SFP_UDP_Sender_Handler::handle_input (ACE_HANDLE /*fd*/)
// {
//   return -1;
// }

// int
// TAO_SFP_UDP_Sender_Handler::handle_close (ACE_HANDLE /*fd*/,
//                                           ACE_Reactor_Mask)
// {
//   return 0;
// }

// int
// TAO_SFP_UDP_Sender_Handler::start (TAO_FlowSpec_Entry::Role role)
// {
//   // @@once somehow we figure out that the stream is established we
//   // can start sending the frames.
//   if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG,"TAO_SFP_UDP_Sender_Handler::start\n"));
//   TAO_AV_Flow_Handler::start (role);
//   this->sfp_object_->start ();
//   return 0;
// }

// int
// TAO_SFP_UDP_Sender_Handler::stop (TAO_FlowSpec_Entry::Role role)
// {
//   TAO_AV_Flow_Handler::stop (role);
//   this->sfp_object_->stop ();
//   return 0;
// }

// //------------------------------------------------------------
// // TAO_SFP_UDP_Receiver_Handler
// //------------------------------------------------------------
// TAO_SFP_UDP_Receiver_Handler::TAO_SFP_UDP_Receiver_Handler (TAO_AV_Callback *callback)
//   :TAO_AV_Flow_Handler (callback),
//    TAO_AV_UDP_Flow_Handler (callback)
// {
// }

// int
// TAO_SFP_UDP_Receiver_Handler::handle_input (ACE_HANDLE /*fd*/)
// {
//   if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG,"TAO_SFP_UDP_Receiver_Handler::handle_input\n"));
//   // This is the entry point for receiving data.
//   TAO_AV_frame_info *frame_info = 0;
//   int result = TAO_SFP_Base::handle_input (this->transport_,
//                                            this->state_,
//                                            frame_info);
//   if (result < 0)
//     ACE_ERROR_RETURN ((LM_ERROR,"ERROR in TAO_SFP_UDP_Receiver_Handler::handle_input"),result);
//   if (this->state_.frame_header_.message_type == flowProtocol::EndofStream_Msg)
//     this->callback_->handle_end_stream ();
//   if (this->state_.is_complete ())
//     {
//       this->callback_->receive_frame (this->state_.frame_block_,
//                                       frame_info);
//       this->state_.reset ();
//     }
//   return 0;
// }

// int
// TAO_SFP_UDP_Receiver_Handler::handle_close (ACE_HANDLE /*fd*/,
//                                             ACE_Reactor_Mask /*mask*/)
// {
//   return 0;
// }

// //------------------------------------------------------------
// //TAO_SFP_UDP_MCast_Protocol_Factory
// //------------------------------------------------------------

// TAO_SFP_UDP_MCast_Protocol_Factory::TAO_SFP_UDP_MCast_Protocol_Factory (void)
// {
// }

// TAO_SFP_UDP_MCast_Protocol_Factory::~TAO_SFP_UDP_MCast_Protocol_Factory (void)
// {
// }

// TAO_AV_Acceptor *
// TAO_SFP_UDP_MCast_Protocol_Factory::make_acceptor (void)
// {
//   TAO_SFP_UDP_MCast_Acceptor *acceptor = 0;
//   ACE_NEW_RETURN (acceptor,
//                   TAO_SFP_UDP_MCast_Acceptor,
//                   0);
//   return acceptor;
// }

// TAO_AV_Connector *
// TAO_SFP_UDP_MCast_Protocol_Factory::make_connector (void)
// {
//   TAO_SFP_UDP_MCast_Connector *connector = 0;
//   ACE_NEW_RETURN (connector,
//                   TAO_SFP_UDP_MCast_Connector,
//                   0);
//   return connector;
// }

// int
// TAO_SFP_UDP_MCast_Protocol_Factory::match_protocol (TAO_AV_Core::Protocol protocol)
// {
//   return (protocol == TAO_AV_Core::TAO_AV_SFP_UDP_MCAST);
// }

// //
// // TAO_SFP_UDP_MCast_Acceptor
// TAO_SFP_UDP_MCast_Acceptor::TAO_SFP_UDP_MCast_Acceptor (void)
// {
// }

// TAO_SFP_UDP_MCast_Acceptor::~TAO_SFP_UDP_MCast_Acceptor (void)
// {
// }

// int
// TAO_SFP_UDP_MCast_Acceptor::make_svc_handler (TAO_AV_UDP_MCast_Flow_Handler *&handler)
// {
//   // @@ We should actually look at the entry and find out if we're a
//   // sink or a source and create the appropriate handler
//   if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG,"TAO_SFP_UDP_MCast_Acceptor::make_svc_handler\n"));
//   TAO_SFP_BASE::instance ();
//   TAO_SFP_Object *object;
//   TAO_AV_Callback *callback;
//   this->endpoint_->get_callback (this->entry_->flowname (),callback);
//   ACE_NEW_RETURN (object,
//                   TAO_SFP_Object (callback),
//                   -1);
//   ACE_NEW_RETURN (handler,
//                   TAO_SFP_UDP_MCast_Sender_Handler (callback,
//                                                     object),
//                   -1);
//   callback->transport (handler->transport ());
//   callback->protocol_object (object);
//   object->transport (handler->transport ());
//   this->endpoint_->set_protocol_object (this->entry_->flowname (),
//                                         object);
//   this->endpoint_->set_handler (this->flowname_.c_str (),handler);
//   this->entry_->protocol_object (object);
//   return 0;
// }
// //------------------------------------------------------------
// // TAO_SFP_UDP_MCast_Connector
// //------------------------------------------------------------
// TAO_SFP_UDP_MCast_Connector::TAO_SFP_UDP_MCast_Connector (void)
// {
// }

// TAO_SFP_UDP_MCast_Connector::~TAO_SFP_UDP_MCast_Connector (void)
// {
// }

// int
// TAO_SFP_UDP_MCast_Connector::make_svc_handler (TAO_AV_UDP_MCast_Flow_Handler *&handler)
// {
//   if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG,"TAO_SFP_UDP_MCast_Connector::make_svc_handler\n"));
//   TAO_SFP_BASE::instance ();
//   // @@ We should actually look at the entry and find out if we're a
//   // sink or a source and create the appropriate handler
//   TAO_AV_Callback *callback = 0;
//   this->endpoint_->get_callback (this->entry_->flowname (),
//                                  callback);
//   ACE_NEW_RETURN (handler,
//                   TAO_SFP_UDP_MCast_Receiver_Handler (callback),
//                   -1);
//   TAO_SFP_Object *object;
//   ACE_NEW_RETURN (object,
//                   TAO_SFP_Object (callback),
//                   -1);
//   callback->transport (handler->transport ());
//   callback->protocol_object (object);
//   object->transport (handler->transport ());
//   this->endpoint_->set_protocol_object (this->entry_->flowname (),
//                                         object);
//   this->endpoint_->set_handler (this->flowname_.c_str (),handler);
//   this->entry_->protocol_object (object);
//   return 0;
// }

// //------------------------------------------------------------
// // TAO_SFP_UDP_MCast_Sender_Handler
// //------------------------------------------------------------
// TAO_SFP_UDP_MCast_Sender_Handler::TAO_SFP_UDP_MCast_Sender_Handler (TAO_AV_Callback *callback,
//                                                                     TAO_SFP_Object *object)
//   :TAO_AV_Flow_Handler (callback),
//    TAO_AV_UDP_MCast_Flow_Handler (callback),
//    sfp_object_ (object)
// {
// }

// int
// TAO_SFP_UDP_MCast_Sender_Handler::handle_input (ACE_HANDLE /*fd*/)
// {
//   return -1;
// }

// int
// TAO_SFP_UDP_MCast_Sender_Handler::handle_close (ACE_HANDLE /*fd*/,
//                                           ACE_Reactor_Mask)
// {
//   return 0;
// }

// int
// TAO_SFP_UDP_MCast_Sender_Handler::start (TAO_FlowSpec_Entry::Role role)
// {
//   // @@once somehow we figure out that the stream is established we
//   // can start sending the frames.
//   if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG,"TAO_SFP_UDP_MCast_Sender_Handler::start\n"));
//   TAO_AV_Flow_Handler::start (role);
//   this->sfp_object_->start ();
//   return 0;
// }

// int
// TAO_SFP_UDP_MCast_Sender_Handler::stop (TAO_FlowSpec_Entry::Role role)
// {
//   TAO_AV_Flow_Handler::stop (role);
//   this->sfp_object_->stop ();
//   return 0;
// }

// //------------------------------------------------------------
// // TAO_SFP_UDP_MCast_Receiver_Handler
// //------------------------------------------------------------
// TAO_SFP_UDP_MCast_Receiver_Handler::TAO_SFP_UDP_MCast_Receiver_Handler (TAO_AV_Callback *callback)
//   :TAO_AV_Flow_Handler (callback),
//    TAO_AV_UDP_MCast_Flow_Handler (callback)
// {
// }

// int
// TAO_SFP_UDP_MCast_Receiver_Handler::handle_input (ACE_HANDLE /*fd*/)
// {
//   if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG,"TAO_SFP_UDP_MCast_Receiver_Handler::handle_input\n"));
//   TAO_AV_frame_info *frame_info = 0;
//   // This is the entry point for receiving data.
//   int result = TAO_SFP_Base::handle_input (this->transport_,
//                                            this->state_,
//                                            frame_info);
//   if (result < 0)
//     ACE_ERROR_RETURN ((LM_ERROR,"ERROR in TAO_SFP_UDP_MCast_Receiver_Handler::handle_input"),result);
//   if (this->state_.frame_header_.message_type == flowProtocol::EndofStream_Msg)
//     this->callback_->handle_end_stream ();
//   if (this->state_.is_complete ())
//     {
//       this->callback_->receive_frame (this->state_.frame_block_,
//                                       frame_info);
//       this->state_.reset ();
//     }
//   return 0;
// }

// int
// TAO_SFP_UDP_MCast_Receiver_Handler::handle_close (ACE_HANDLE /*fd*/,
//                                             ACE_Reactor_Mask /*mask*/)
// {
//   return 0;
// }


//------------------------------------------------------------
// TAO_SFP_Object
//------------------------------------------------------------

TAO_SFP_Object::TAO_SFP_Object (TAO_AV_Callback *callback,
                                TAO_AV_Transport *transport)
  :TAO_AV_Protocol_Object (callback,transport),
   source_id_ (10),
   credit_ (20)

{
}

TAO_SFP_Object::~TAO_SFP_Object (void)
{
  //no-op
}

int
TAO_SFP_Object::end_stream (void)
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
  return 0;
}

int
TAO_SFP_Object::send_frame (ACE_Message_Block *frame,
                            TAO_AV_frame_info *frame_info)
{
  TAO_OutputCDR out_stream;
  CORBA::Boolean result = 0;
  if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG,"TAO_SFP_Object::send_frame\n"));
  CORBA::Octet flags = TAO_ENCAP_BYTE_ORDER;
  if (this->transport_ == 0)
    ACE_ERROR_RETURN ((LM_ERROR,"TAO_SFP_Object::send_frame: transport is null\n"),-1);
   if (this->current_credit_ > 0)
    {
      // if we have enough credit then we send.
      int total_length = 0;
      for (ACE_Message_Block *temp = frame;temp != 0;temp = temp->cont ())
        total_length += temp->length ();
      if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG,"total_length of frame=%d\n",total_length));
      if (ACE_static_cast (u_int, total_length) < (TAO_SFP_MAX_PACKET_SIZE -TAO_SFP_Base::frame_header_len))
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
          int message_len = out_stream.total_length ();
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
                  if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG,"sending the last fragment\n"));
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
          this->credit--;
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
TAO_SFP_Object::send_frame (const iovec *iov,
                            int iovcnt,
                            TAO_AV_frame_info *frame_info)
{
  ACE_ERROR_RETURN ((LM_ERROR,"TAO_AV_SFP_Object::send_frame"),-1);
}

ACE_Message_Block*
TAO_SFP_Object::get_fragment (ACE_Message_Block *&mb,
                              size_t initial_len,
                              size_t &last_mb_orig_len,
                              size_t &last_mb_current_len)
{
  ACE_Message_Block *fragment_mb = 0,*temp_mb = 0;
  int prev_len,last_len = 0;
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

// TAO_SFP_Consumer_Object
int
TAO_SFP_Consumer_Object::handle_input (void)
{
  if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG,"TAO_SFP_Consumer_Object::handle_input\n"));
  // This is the entry point for receiving data.
  TAO_AV_frame_info *frame_info = 0;
  int result = TAO_SFP_Base::handle_input (this->transport_,
                                           this->state_,
                                           frame_info);
  if (result < 0)
    ACE_ERROR_RETURN ((LM_ERROR,"ERROR in TAO_SFP_Consumer_Object::handle_input"),result);
  if (this->state_.frame_header_.message_type == flowProtocol::EndofStream_Msg)
    this->callback_->handle_end_stream ();
  if (this->state_.is_complete ())
    {
      this->callback_->receive_frame (this->state_.frame_block_,
                                      frame_info);
      this->state_.reset ();
    }
  return 0;
}

TAO_SFP_Producer_Object::TAO_SFP_Producer_Object (void)
  :credit_sequence_num_ (0)
{
}

int
TAO_SFP_Producer_Object::handle_input (void)
{
  // A producer can only receive credit messages.
  int result;
  flowProtocol::MsgType msg_type;
  result = TAO_SFP_Base::peek_message_type (transport,
                                            msg_type);
  if (result < 0)
    return result;
  switch (msg_type)
    {
    case flowProtocol::Credit_Msg:
      {
        flowProtocol::credit credit;
        result = TAO_SFP_Base::read_credit_message (transport,
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

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_DNode<TAO_SFP_Fragment_Node>;
template class ACE_Equal_To<CORBA::ULong>;
template class ACE_Ordered_MultiSet<TAO_SFP_Fragment_Node>;
template class ACE_Ordered_MultiSet_Iterator<TAO_SFP_Fragment_Node>;
template class ACE_Hash_Map_Manager<CORBA::ULong,TAO_SFP_Fragment_Table_Entry*,ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager_Ex<CORBA::ULong, TAO_SFP_Fragment_Table_Entry *, ACE_Hash<CORBA::ULong>, ACE_Equal_To<CORBA::ULong>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Entry<CORBA::ULong, TAO_SFP_Fragment_Table_Entry *>;
template class ACE_Hash_Map_Iterator_Base_Ex<CORBA::ULong, TAO_SFP_Fragment_Table_Entry *, ACE_Hash<CORBA::ULong>, ACE_Equal_To<CORBA::ULong>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<CORBA::ULong,TAO_SFP_Fragment_Table_Entry*,ACE_Hash<CORBA::ULong>,ACE_Equal_To<CORBA::ULong>,ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<CORBA::ULong,TAO_SFP_Fragment_Table_Entry*,ACE_Hash<CORBA::ULong>,ACE_Equal_To<CORBA::ULong>,ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator<CORBA::ULong,TAO_SFP_Fragment_Table_Entry *,ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator<CORBA::ULong,TAO_SFP_Fragment_Table_Entry *,ACE_Null_Mutex>;
template class ACE_Singleton<TAO_SFP_Base, ACE_SYNCH_MUTEX>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_DNode<TAO_SFP_Fragment_Node>
#pragma instantiate ACE_Ordered_MultiSet<TAO_SFP_Fragment_Node>
#pragma instantiate ACE_Ordered_MultiSet_Iterator<TAO_SFP_Fragment_Node>
#pragma instantiate ACE_Hash_Map_Manager<CORBA::ULong,TAO_SFP_Fragment_Table_Entry*,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Manager_Ex<CORBA::ULong, TAO_SFP_Fragment_Table_Entry *, ACE_Hash<CORBA::ULong>, ACE_Equal_To<CORBA::ULong>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Entry<CORBA::ULong, TAO_SFP_Fragment_Table_Entry *>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<CORBA::ULong, TAO_SFP_Fragment_Table_Entry *, ACE_Hash<CORBA::ULong>, ACE_Equal_To<CORBA::ULong>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<CORBA::ULong,TAO_SFP_Fragment_Table_Entry*,ACE_Hash<CORBA::ULong>,ACE_Equal_To<CORBA::ULong>,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<CORBA::ULong,TAO_SFP_Fragment_Table_Entry*,ACE_Hash<CORBA::ULong>,ACE_Equal_To<CORBA::ULong>,ACE_Null_Mutex>
#pragma instantiate ACE_Equal_To<CORBA::ULong>
#pragma instantiate ACE_Hash_Map_Iterator<CORBA::ULong,TAO_SFP_Fragment_Table_Entry *,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<CORBA::ULong,TAO_SFP_Fragment_Table_Entry *,ACE_Null_Mutex>
#pragma instantiate ACE_Singleton<TAO_SFP_Base, ACE_SYNCH_MUTEX>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
