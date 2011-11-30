// $Id$

#include "tao/TAO_Singleton.h"
#include "orbsvcs/Notify/RT_Properties.h"

#if ! defined (__ACE_INLINE__)
#include "orbsvcs/Notify/RT_Properties.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Notify_RT_Properties::TAO_Notify_RT_Properties (void)
{
}

TAO_Notify_RT_Properties::~TAO_Notify_RT_Properties ()
{
}

TAO_Notify_RT_Properties *
TAO_Notify_RT_Properties::instance (void)
{
  // Hide the template instantiation to prevent multiple instances
  // from being created.

  return
    TAO_Singleton<TAO_Notify_RT_Properties, TAO_SYNCH_MUTEX>::instance ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
