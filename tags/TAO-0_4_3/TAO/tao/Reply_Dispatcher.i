// $Id$

ACE_INLINE CORBA::ULong
TAO_Synch_Reply_Dispatcher::reply_status (void) const
{
  return this->reply_status_;
}

ACE_INLINE const TAO_GIOP_Version&
TAO_Synch_Reply_Dispatcher::version (void) const
{
  return this->version_;
}

ACE_INLINE TAO_GIOP_ServiceContextList&
TAO_Synch_Reply_Dispatcher::reply_ctx (void)
{
  return this->reply_ctx_;
}
