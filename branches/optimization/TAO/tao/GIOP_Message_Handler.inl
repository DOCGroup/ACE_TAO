// -*- C++ -*-
// $Id$

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

ACE_INLINE ACE_Data_Block *
TAO_GIOP_Message_Handler::data_block_dup (void)
{
  return this->current_buffer_.data_block ()->duplicate ();
}


ACE_INLINE void
TAO_GIOP_Message_Handler::reset (int /*reset_flag*/)
{
  // Reset the contents of the message state
  this->message_state_.reset (0);

  // Reset the current buffer
  this->current_buffer_.reset ();
}

ACE_INLINE char *
TAO_GIOP_Message_Handler::rd_ptr (void) const
{
  return this->current_buffer_.rd_ptr ();
}
