// $Id$


ACE_INLINE CORBA::ULong
TAO_Asynch_Reply_Dispatcher::reply_status (void) const
{
  return this->reply_status_;
}

#if 0
ACE_INLINE const TAO_GIOP_Version &
TAO_Asynch_Reply_Dispatcher::version (void) const
{
  return this->version_;
}
#endif /*if 0*/
ACE_INLINE void
TAO_Asynch_Reply_Dispatcher::transport (TAO_Transport *t)
{
  this->transport_ = t;
}
