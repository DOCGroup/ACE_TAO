// -*- C++ -*-

// $Id$


#include "LoadBalancing_Strategy.h"
#include "ace/Containers.h"

#if defined (__ACE_INLINE__)
#include "LoadBalancing_Strategy.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(orbsvcs, LoadBalancing_Strategy, "$Id$")

Load_Balancing_Strategy::~Load_Balancing_Strategy (void)
{
}

void
Load_Balancing_Strategy::load_changed (ReplicaProxy_Impl *,
                                       CORBA::Environment &)
{
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Node<ReplicaProxy_Impl*>;
template class ACE_Unbounded_Set<ReplicaProxy_Impl*>;
template class ACE_Unbounded_Set_Iterator<ReplicaProxy_Impl*>;

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Node<ReplicaProxy_Impl*>
#pragma instantiate ACE_Unbounded_Set<ReplicaProxy_Impl*>
#pragma instantiate ACE_Unbounded_Set_Iterator<ReplicaProxy_Impl*>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
