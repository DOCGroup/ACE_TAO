// -*- C++ -*-

#include "LB_Location_Map.h"

ACE_RCSID (LoadBalancing,
           LB_Location_Map,
           "$Id$")


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Hash_Map_Entry<PortableGroup::Location, TAO_LB_Location_Map_Entry *>;
template class ACE_Hash_Map_Manager_Ex<PortableGroup::Location, TAO_LB_Location_Map_Entry *, TAO_LB_Location_Hash, TAO_LB_Location_Equal_To, TAO_SYNCH_MUTEX>;
template class ACE_Hash_Map_Iterator_Base_Ex<PortableGroup::Location, TAO_LB_Location_Map_Entry *, TAO_LB_Location_Hash, TAO_LB_Location_Equal_To, TAO_SYNCH_MUTEX>;
template class ACE_Hash_Map_Iterator_Ex<PortableGroup::Location, TAO_LB_Location_Map_Entry *, TAO_LB_Location_Hash, TAO_LB_Location_Equal_To, TAO_SYNCH_MUTEX>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<PortableGroup::Location, TAO_LB_Location_Map_Entry *, TAO_LB_Location_Hash, TAO_LB_Location_Equal_To, TAO_SYNCH_MUTEX>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Hash_Map_Entry<PortableGroup::Location, TAO_LB_Location_Map_Entry *>
#pragma instantiate ACE_Hash_Map_Manager_Ex<PortableGroup::Location, TAO_LB_Location_Map_Entry *, TAO_LB_Location_Hash, TAO_LB_Location_Equal_To, TAO_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<PortableGroup::Location, TAO_LB_Location_Map_Entry *, TAO_LB_Location_Hash, TAO_LB_Location_Equal_To, TAO_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<PortableGroup::Location, TAO_LB_Location_Map_Entry *, TAO_LB_Location_Hash, TAO_LB_Location_Equal_To, TAO_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<PortableGroup::Location, TAO_LB_Location_Map_Entry *, TAO_LB_Location_Hash, TAO_LB_Location_Equal_To, TAO_SYNCH_MUTEX>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
