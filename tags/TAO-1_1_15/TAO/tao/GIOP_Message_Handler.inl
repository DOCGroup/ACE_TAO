// -*- C++ -*-
// $Id$

ACE_INLINE int
TAO_GIOP_Message_Handler::parse_message_header (void)
{
  // Check what message are we waiting for and take suitable action
  if (this->message_status_ == TAO_GIOP_WAITING_FOR_HEADER)
    {
      if (this->current_buffer_.length () >=
          TAO_GIOP_MESSAGE_HEADER_LEN)
        {
          return this->parse_message_header_i ();
        }

      // We dont have suffcient information to decipher the GIOP
      // header. Make sure that the reactor calls us back.
      return -1;
    }

  // The last read just "read" left-over messages
  return 0;
}

ACE_INLINE TAO_GIOP_Message_State &
TAO_GIOP_Message_Handler::message_state (void)
{
  return this->message_state_;
}

ACE_INLINE ACE_Data_Block *
TAO_GIOP_Message_Handler::data_block (void) const
{
  return this->current_buffer_.data_block ();
}

ACE_INLINE size_t
TAO_GIOP_Message_Handler::rd_pos (void) const
{
  return
    this->current_buffer_.rd_ptr () - this->current_buffer_.base ();
}

ACE_INLINE size_t
TAO_GIOP_Message_Handler::wr_pos (void) const
{
  return
    this->current_buffer_.wr_ptr () - this->current_buffer_.base ();
}
