TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE TAO_Queued_Message *
TAO_Queued_Message::next () const
{
  return this->next_;
}

ACE_INLINE TAO_Queued_Message *
TAO_Queued_Message::prev () const
{
  return this->prev_;
}

TAO_END_VERSIONED_NAMESPACE_DECL
