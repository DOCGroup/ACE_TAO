// $Id$

#include "Dispatch_Observer_T.h"

template <class PEER> ACE_INLINE TAO_NS_Dispatch_Observer_T<PEER>::PEER_COLLECTION*
TAO_NS_Dispatch_Observer_T<PEER>::peer_collection (void)
{
  return this->peer_collection_;
}
