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

#if defined (TAO_HAS_CORBA_MESSAGING) && defined (TAO_POLLER)

ACE_INLINE CORBA::ULong
TAO_Asynch_Reply_Dispatcher::reply_status (void) const
{
  return this->reply_status_;
}

ACE_INLINE const TAO_GIOP_Version&
TAO_Asynch_Reply_Dispatcher::version (void) const
{
  return this->version_;
}

#endif /* TAO_HAS_CORBA_MESSAGING && TAO_POLLER */

//*********************************************************************

#if !defined (TAO_HAS_MINIMUM_CORBA)

ACE_INLINE CORBA::ULong
TAO_DII_Deferred_Reply_Dispatcher::reply_status (void) const
{
  return this->reply_status_;
}

ACE_INLINE const TAO_GIOP_Version&
TAO_DII_Deferred_Reply_Dispatcher::version (void) const
{
  return this->version_;
}

#endif /* TAO_HAS_MINIMUM_CORBA */

