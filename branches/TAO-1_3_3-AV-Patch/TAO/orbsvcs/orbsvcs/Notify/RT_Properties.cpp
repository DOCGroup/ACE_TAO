// $Id$

#include "RT_Properties.h"

#if ! defined (__ACE_INLINE__)
#include "RT_Properties.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(RT_Notify, TAO_Notify_RT_Properties, "$Id$")

TAO_Notify_RT_Properties::TAO_Notify_RT_Properties (void)
{
}

TAO_Notify_RT_Properties::~TAO_Notify_RT_Properties ()
{
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class TAO_Singleton<TAO_Notify_RT_Properties, TAO_SYNCH_MUTEX>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate TAO_Singleton<TAO_Notify_RT_Properties, TAO_SYNCH_MUTEX>

#endif /*ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
