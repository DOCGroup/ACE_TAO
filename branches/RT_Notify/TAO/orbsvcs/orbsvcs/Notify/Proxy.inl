// $Id$

#include "Proxy.h"

ACE_INLINE TAO_NS_EventTypeSeq&
TAO_NS_Proxy::subscribed_types (void)
{
  return this->subscribed_types_;
}
