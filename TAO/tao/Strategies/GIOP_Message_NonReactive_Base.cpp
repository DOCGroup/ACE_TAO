#include "GIOP_Message_NonReactive_Base.h"
#include "tao/Leader_Follower.h"

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
  if (retval == -1)
    return -1;

  // Get the message state
  TAO_GIOP_Message_State &state =
    this->message_handler_.message_state ();

  // Set the state internally for parsing and generating messages
  this->set_state (state.giop_version.major,
                   state.giop_version.minor);

  // We return 2, it is ugly. But the reactor semantics has made us to
  // limp :(
  return 2;
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
  ///@@@
  return 0;
}
