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

#if (TAO_HAS_CORBA_MESSAGING == 1) && (TAO_HAS_AMI_POLLER == 1)

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

#endif /* TAO_HAS_CORBA_MESSAGING == 1 && TAO_HAS_AMI_POLLER == 1 */

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
