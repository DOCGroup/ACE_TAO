// -*- C++ -*-
// $Id$



ACE_INLINE TAO_GIOP_Message_State &
TAO_GIOP_Message_Reactive_Handler::message_state (void)
{
  return this->message_state_;
}

ACE_INLINE ACE_Data_Block *
TAO_GIOP_Message_Reactive_Handler::data_block (void) const
{
  return this->current_buffer_.data_block ();
}

ACE_INLINE size_t
TAO_GIOP_Message_Reactive_Handler::rd_pos (void) const
{
  return
    this->current_buffer_.rd_ptr () - this->current_buffer_.base ();
}

ACE_INLINE size_t
TAO_GIOP_Message_Reactive_Handler::wr_pos (void) const
{
  return
    this->current_buffer_.wr_ptr () - this->current_buffer_.base ();
}
