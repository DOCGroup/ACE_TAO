// $Id$

ACE_INLINE int
TAO_Queued_Message::done (void) const
{
  // @@ Actually we should have a status() method that returns not
  //    only if there is more data, but also indicates if there was a
  //    failure.
  return (this->data_sent_successfully_ == 1
          || this->connection_closed_ == 1);
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
