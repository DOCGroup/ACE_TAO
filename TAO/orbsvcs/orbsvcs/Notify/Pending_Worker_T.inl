// $Id$

#include "Pending_Worker_T.h"

ACE_INLINE template <class PEER> void
TAO_NS_Pending_Worker_T<PEER>::shutdown (void)
{
  this->done_ = 1;
  this->wait ();
}

ACE_INLINE template <class PEER> void
TAO_NS_Pending_Worker_T<PEER>::worker_suspend (void)
{
  this->suspended_ = 1;
}

ACE_INLINE template <class PEER> void
TAO_NS_Pending_Worker_T<PEER>::worker_resume (void)
{
  this->suspended_ = 0;
}
