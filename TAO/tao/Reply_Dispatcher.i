// $Id$


ACE_INLINE CORBA::ULong
TAO_Synch_Reply_Dispatcher::reply_status (void) const
{
  return this->reply_status_;
}

#if 0
ACE_INLINE const TAO_GIOP_Version&
TAO_Synch_Reply_Dispatcher::version (void) const
{
  return this->version_;
}
#endif /* if 0 */

#if (TAO_HAS_AMI_CALLBACK == 1) || (TAO_HAS_AMI_POLLER == 1)

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

#endif /* TAO_HAS_AMI_CALLBACK == 1 || TAO_HAS_AMI_POLLER == 1 */

//*********************************************************************

#if (TAO_HAS_MINIMUM_CORBA == 0)

ACE_INLINE CORBA::ULong
TAO_DII_Deferred_Reply_Dispatcher::reply_status (void) const
{
  return this->reply_status_;
}

#if 0
ACE_INLINE const TAO_GIOP_Version&
TAO_DII_Deferred_Reply_Dispatcher::version (void) const
{
  return this->version_;
}

#endif /*If 0 */

ACE_INLINE void
TAO_DII_Deferred_Reply_Dispatcher::transport (TAO_Transport *t)
{
  this->transport_ = t;
}

#endif /* TAO_HAS_MINIMUM_CORBA */
