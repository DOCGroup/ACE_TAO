// $Id$

ACE_INLINE ACE_Message_Block *
TAO_Queued_Message::mb (void) const
{
  return this->current_block_;
}

ACE_INLINE int
TAO_Queued_Message::done (void) const
{
  return this->current_block_ == 0;
}

ACE_INLINE TAO_Queued_Message *
TAO_Queued_Message::next (void) const
{
  return this->next_;
}

ACE_INLINE TAO_Queued_Message *
TAO_Queued_Message::prev (void) const
{
  return this->prev_;
}
