// $Id$

ACE_INLINE int
ACE_RMCast_Partial_Message::is_complete (void) const
{
  return (this->hole_count_ == 0)
    || (this->hole_count_ == 1
        && this->hole_list_[0].start == this->hole_list_[0].end);
}

ACE_INLINE ACE_Message_Block *
ACE_RMCast_Partial_Message::message_body (void)
{
  return &this->message_body_;
}
