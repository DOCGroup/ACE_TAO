// $Id$

#include "Types.h"
#include "orbsvcs/NotifyExtC.h"
#include "orbsvcs/ESF/ESF_RefCount_Guard.h"

#if ! defined (__ACE_INLINE__)
#include "Types.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(RT_Notify, TAO_NS_Types, "$Id$")

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class TAO_ESF_RefCount_Guard<CORBA::ULong>; 
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate TAO_ESF_RefCount_Guard<CORBA::ULong> 
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
