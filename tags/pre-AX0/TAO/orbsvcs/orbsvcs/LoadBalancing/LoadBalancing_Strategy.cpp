// -*- C++ -*-

// $Id$


#include "LoadBalancing_Strategy.h"
#include "ace/Containers.h"

#if defined (__ACE_INLINE__)
#include "LoadBalancing_Strategy.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(orbsvcs, LoadBalancing_Strategy, "$Id$")

TAO_LB_LoadBalancing_Strategy::~TAO_LB_LoadBalancing_Strategy (void)
{
}

void
TAO_LB_LoadBalancing_Strategy::load_changed (TAO_LB_ReplicaProxy *
                                              ACE_ENV_ARG_DECL_NOT_USED)
{
}

// @@ Ossama: i had to instantiate the template here because it is
// used in two strategies, but it sound kind of silly...
#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Node<TAO_LB_ReplicaProxy *>;
template class ACE_Unbounded_Set<TAO_LB_ReplicaProxy *>;
template class ACE_Unbounded_Set_Iterator<TAO_LB_ReplicaProxy *>;

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Node<TAO_LB_ReplicaProxy *>
#pragma instantiate ACE_Unbounded_Set<TAO_LB_ReplicaProxy *>
#pragma instantiate ACE_Unbounded_Set_Iterator<TAO_LB_ReplicaProxy *>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
