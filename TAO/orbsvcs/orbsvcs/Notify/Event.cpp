// $Id$

#include "Event.h"
#include "tao/debug.h"

#if ! defined (__ACE_INLINE__)
#include "Event.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(RT_Notify, TAO_NS_Event, "$Id$")

TAO_NS_Event::TAO_NS_Event (void)
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,"event:%x  created\n", this ));
}

TAO_NS_Event::~TAO_NS_Event ()
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,"event:%x  destroyed\n", this ));
}

