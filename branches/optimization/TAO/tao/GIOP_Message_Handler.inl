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
  if (this->supp_buffer_.length () >0)
    return this->supp_buffer_.data_block ()->duplicate ();

  return this->current_buffer_.data_block ()->duplicate ();
}


ACE_INLINE void
TAO_GIOP_Message_Handler::reset (int /*reset_flag*/)
{
  // Reset the contents of the message state
  this->message_state_.reset (0);

  // Reset the current buffer
  if (this->message_status_ != TAO_GIOP_MULTIPLE_MESSAGES)
    this->current_buffer_.reset ();

  this->supp_buffer_.reset ();

}

ACE_INLINE char *
TAO_GIOP_Message_Handler::rd_ptr (void) const
{
  if (this->supp_buffer_.length () > 0)
    return this->supp_buffer_.rd_ptr ();

  return this->current_buffer_.rd_ptr ();
}

ACE_INLINE size_t
TAO_GIOP_Message_Handler::rd_pos (void) const
{
  if (this->supp_buffer_.length () > 0)
    {
      return
        this->supp_buffer_.rd_ptr () - this->supp_buffer_.base ();
    }

  return
    this->current_buffer_.rd_ptr () - this->current_buffer_.base ();
}

ACE_INLINE size_t
TAO_GIOP_Message_Handler::wr_pos (void) const
{
  if (this->supp_buffer_.length () > 0)
    {
      return
        this->supp_buffer_.wr_ptr () - this->supp_buffer_.base ();
    }

  return
    this->current_buffer_.wr_ptr () - this->current_buffer_.base ();
}
