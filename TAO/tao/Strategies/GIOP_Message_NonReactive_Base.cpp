#include "GIOP_Message_NonReactive_Base.h"
#include "tao/Leader_Follower.h"
#include "tao/debug.h"

#if !defined (__ACE_INLINE__)
# include "GIOP_Message_NonReactive_Base.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID (Strategies, GIOP_Message_NonReactive_Base, "$Id$")

TAO_GIOP_Message_NonReactive_Base::TAO_GIOP_Message_NonReactive_Base (TAO_ORB_Core *orb_core,
                                                                      size_t buf_size)
  : TAO_GIOP_Message_Base (orb_core, buf_size),
    message_handler_ (orb_core, this, buf_size)
{

}


int
TAO_GIOP_Message_NonReactive_Base::read_message (TAO_Transport *transport,
                                                 int /*block*/,
                                                 ACE_Time_Value *max_wait_time)
{
  // Call the handler to read and do a simple parse of the header of
  // the message.
  int retval =
    this->message_handler_.read_parse_message (transport,
                                               max_wait_time);


  // Error in the message that was received
  if (retval <= 0)
    return retval;

  // Get the message state
  TAO_GIOP_Message_State &state =
    this->message_handler_.message_state ();

  // Set the state internally for parsing and generating messages
  this->set_state (state.giop_version.major,
                   state.giop_version.minor);

  if (TAO_debug_level > 2)
    {
      TAO_InputCDR &cdr =
        this->message_handler_.input_cdr ();

      char *base = cdr.start ()->base ();

      size_t len = cdr.length () + TAO_GIOP_MESSAGE_HEADER_LEN;

      this->dump_msg ("Recv",
                      ACE_reinterpret_cast (u_char *,
                                            base),
                      len);

    }
  // We return 2, it is ugly. But the reactor semantics has made us to
  // limp :(
  return 2;
}

TAO_Pluggable_Message_Type
TAO_GIOP_Message_NonReactive_Base::message_type (void)
{
  // @@ maybe we should refactor this code and put it in
  // GIOP_Message_Base.- NB
  switch (this->message_handler_.message_state ().message_type)
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
TAO_GIOP_Message_NonReactive_Base::process_request_message (TAO_Transport *transport,
                                                            TAO_ORB_Core *orb_core)
{
  // Set the upcall thread
  orb_core->leader_follower ().set_upcall_thread ();

  // We know we have some request message. Check whether it is a
  // GIOP_REQUEST or GIOP_LOCATE_REQUEST to take action.

  // Once we send the InputCDR stream we need to just forget about
  // the stream and never touch that again for anything. We basically
  // loose ownership of the data_block.

  switch (this->message_handler_.message_state ().message_type)
    {
    case TAO_GIOP_REQUEST:
      // Should be taken care by the state specific invocations. They
      // could raise an exception or write things in the output CDR
      // stream
      return this->process_request (transport,
                                    orb_core,
                                    this->message_handler_.input_cdr ());

    case TAO_GIOP_LOCATEREQUEST:
      return this->process_locate_request (transport,
                                           orb_core,
                                           this->message_handler_.input_cdr ());
    default:
      return -1;
    }
}

int
TAO_GIOP_Message_NonReactive_Base::process_reply_message (
    TAO_Pluggable_Reply_Params &params
  )
{
  // We know we have some reply message. Check whether it is a
  // GIOP_REPLY or GIOP_LOCATE_REPLY to take action.

  // Once we send the InputCDR stream we need to just forget about
  // the stream and never touch that again for anything. We basically
  // loose ownership of the data_block.

  switch (this->message_handler_.message_state ().message_type)
    {
    case TAO_GIOP_REPLY:
      // Should be taken care by the state specific parsing
      return this->generator_parser_->parse_reply (this->message_handler_.input_cdr (),
                                                   params);

    case TAO_GIOP_LOCATEREPLY:
      return this->generator_parser_->parse_locate_reply (this->message_handler_.input_cdr (),
                                                          params);
      default:
        return -1;
    }
}


int
TAO_GIOP_Message_NonReactive_Base::more_messages (void)
{
  if (this->message_handler_.message_state ().message_fragmented ())
    {
      // We have fragmented message
      // Get the message block from the handler
      ACE_Message_Block *mesg_block =
        this->message_handler_.input_cdr ().steal_contents ();

      // Send the message block for patching with the right info.
      int retval =
        this->message_handler_.message_state ().is_complete (*mesg_block);

      // Get the read and write pointers of the message block
      size_t rd_pos =
        mesg_block->rd_ptr () - mesg_block->base ();

      size_t wr_pos =
        mesg_block->wr_ptr () - mesg_block->base ();

      // Create a new InputCDR stream
      TAO_InputCDR cdr (mesg_block->data_block (),
                        0, 
                        rd_pos,
                        wr_pos);

      this->message_handler_.input_cdr ().exchange_data_blocks (cdr);

      return retval;
    }



  ///@@@
  return 1;
}
