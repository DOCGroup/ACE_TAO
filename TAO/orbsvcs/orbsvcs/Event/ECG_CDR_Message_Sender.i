// $Id$

ACE_INLINE
TAO_ECG_CDR_Message_Sender::TAO_ECG_CDR_Message_Sender (void)
  :  endpoint_rptr_ (),
     mtu_ (TAO_ECG_CDR_Message_Sender::ECG_DEFAULT_MTU)
{
}

ACE_INLINE void
TAO_ECG_CDR_Message_Sender::shutdown (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  // Release the endpoint.
  TAO_ECG_Refcounted_Endpoint empty_endpoint_rptr;
  this->endpoint_rptr_ = empty_endpoint_rptr;
}

ACE_INLINE ACE_SOCK_Dgram&
TAO_ECG_CDR_Message_Sender::dgram (void)
{
  ACE_ASSERT (this->endpoint_rptr_.get ());
  return this->endpoint_rptr_->dgram ();
}

ACE_INLINE int
TAO_ECG_CDR_Message_Sender::get_local_addr (ACE_INET_Addr& addr)
{
  if (this->endpoint_rptr_.get () == 0)
    return -1;
  return this->dgram ().get_local_addr (addr);
}

ACE_INLINE CORBA::ULong
TAO_ECG_CDR_Message_Sender::mtu (void) const
{
  return this->mtu_;
}

ACE_INLINE int
TAO_ECG_CDR_Message_Sender::mtu (CORBA::ULong new_mtu)
{
  if (new_mtu < TAO_ECG_CDR_Message_Sender::ECG_MIN_MTU
      || new_mtu >= TAO_ECG_CDR_Message_Sender::ECG_MAX_MTU)
    return -1;
  this->mtu_ = new_mtu;
  return 0;
}
