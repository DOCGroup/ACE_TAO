// $Id$

template <class PEER> ACE_INLINE void
TAO_NS_Pending_Worker_T<PEER>::shutdown (void)
{
  this->done_ = 1;
  this->wait ();
}

template <class PEER> ACE_INLINE void
TAO_NS_Pending_Worker_T<PEER>::worker_suspend (void)
{
  this->suspended_ = 1;
}

template <class PEER> ACE_INLINE void
TAO_NS_Pending_Worker_T<PEER>::worker_resume (void)
{
  this->suspended_ = 0;
}
