// $Id$

ACE_INLINE CORBA::ULong
TAO_Reply_Dispatcher::reply_status (void) const
{
  return this->reply_status_;
}

ACE_INLINE void
TAO_Reply_Dispatcher::has_timeout (CORBA::Boolean t)
{
  this->timeout_ = t;
}
