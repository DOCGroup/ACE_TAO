// $Id$

#include "Properties.h"
#include "tao/debug.h"

#if ! defined (__ACE_INLINE__)
#include "Properties.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(RT_Notify, TAO_NS_Properties, "$Id$")

TAO_NS_Properties::TAO_NS_Properties (void)
  :asynch_updates_ (0)
{
  if (TAO_debug_level > 1)
    ACE_DEBUG ((LM_DEBUG, "in TAO_Properties ctos %x\n", this));
}

TAO_NS_Properties::~TAO_NS_Properties ()
{
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Singleton<TAO_NS_Properties, TAO_SYNCH_MUTEX>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Singleton<TAO_NS_Properties, TAO_SYNCH_MUTEX>

#endif /*ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
