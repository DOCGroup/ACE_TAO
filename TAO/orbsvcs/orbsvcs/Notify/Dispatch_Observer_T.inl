// $Id$

template <class PEER> ACE_INLINE TAO_NS_Dispatch_Observer_T<PEER>::PEER_COLLECTION*
TAO_NS_Dispatch_Observer_T<PEER>::peer_collection (void)
{
  return this->peer_collection_;
}

template <class PEER> ACE_INLINE void
TAO_NS_Dispatch_Observer_T<PEER>::max_retry_attempts (int max_retry_attempts)
{
  this->max_retry_attempts_ = max_retry_attempts;
}
