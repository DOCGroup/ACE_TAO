// -*- C++ -*-

#include "LB_Location_Map.h"

ACE_RCSID (LoadBalancing,
           LB_Location_Map,
           "$Id$")

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Hash_Map_Entry<LoadBalancing::Location, TAO_LB_ReplicaInfo_Set>;
template class ACE_Hash_Map_Manager_Ex<LoadBalancing::Location, TAO_LB_ReplicaInfo_Set, ACE_Hash<LoadBalancing::Location>, ACE_Equal_To<LoadBalancing::Location>, TAO_SYNCH_MUTEX>;
template class ACE_Hash_Map_Iterator_Base_Ex<LoadBalancing::Location, TAO_LB_ReplicaInfo_Set, ACE_Hash<LoadBalancing::Location>, ACE_Equal_To<LoadBalancing::Location>, TAO_SYNCH_MUTEX>;
template class ACE_Hash_Map_Iterator_Ex<LoadBalancing::Location, TAO_LB_ReplicaInfo_Set, ACE_Hash<LoadBalancing::Location>, ACE_Equal_To<LoadBalancing::Location>, TAO_SYNCH_MUTEX>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<LoadBalancing::Location, TAO_LB_ReplicaInfo_Set, ACE_Hash<LoadBalancing::Location>, ACE_Equal_To<LoadBalancing::Location>, TAO_SYNCH_MUTEX>;

template class ACE_Hash<LoadBalancing::Location>;
template class ACE_Equal_To<LoadBalancing::Location>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Hash_Map_Entry<LoadBalancing::Location, TAO_LB_ReplicaInfo_Set>
#pragma instantiate ACE_Hash_Map_Manager_Ex<LoadBalancing::Location, TAO_LB_ReplicaInfo_Set, ACE_Hash<LoadBalancing::Location>, ACE_Equal_To<LoadBalancing::Location>, TAO_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<LoadBalancing::Location, TAO_LB_ReplicaInfo_Set, ACE_Hash<LoadBalancing::Location>, ACE_Equal_To<LoadBalancing::Location>, TAO_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<LoadBalancing::Location, TAO_LB_ReplicaInfo_Set, ACE_Hash<LoadBalancing::Location>, ACE_Equal_To<LoadBalancing::Location>, TAO_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<LoadBalancing::Location, TAO_LB_ReplicaInfo_Set, ACE_Hash<LoadBalancing::Location>, ACE_Equal_To<LoadBalancing::Location>, TAO_SYNCH_MUTEX>

#pragma instantiate  ACE_Hash<LoadBalancing::Location>
#pragma instantiate  ACE_Equal_To<LoadBalancing::Location>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
