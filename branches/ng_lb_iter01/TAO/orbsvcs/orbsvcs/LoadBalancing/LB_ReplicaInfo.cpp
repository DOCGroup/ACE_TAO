// -*- C++ -*-

#include "LB_ReplicaInfo.h"

ACE_RCSID (LoadBalancing,
           LB_ReplicaInfo,
           "$Id$")

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Node<TAO_LB_ReplicaInfo *>;
template class ACE_Unbounded_Set<TAO_LB_ReplicaInfo *>;
template class ACE_Unbounded_Set_Iterator<TAO_LB_ReplicaInfo *>;

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Node<TAO_LB_ReplicaInfo *>
#pragma instantiate ACE_Unbounded_Set<TAO_LB_ReplicaInfo *>
#pragma instantiate ACE_Unbounded_Set_Iterator<TAO_LB_ReplicaInfo *>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
