// $Id$

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
