// -*- C++ -*-

#include "LB_LoadAlertMap.h"


ACE_RCSID (LoadBalancing,
           LB_LoadAlertMap,
           "$Id$")


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Hash_Map_Entry<PortableGroup::Location, TAO_LB_LoadAlertInfo>;
template class ACE_Hash_Map_Manager_Ex<PortableGroup::Location, TAO_LB_LoadAlertInfo, TAO_PG_Location_Hash, TAO_PG_Location_Equal_To, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<PortableGroup::Location, TAO_LB_LoadAlertInfo, TAO_PG_Location_Hash, TAO_PG_Location_Equal_To, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<PortableGroup::Location, TAO_LB_LoadAlertInfo, TAO_PG_Location_Hash, TAO_PG_Location_Equal_To, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<PortableGroup::Location, TAO_LB_LoadAlertInfo, TAO_PG_Location_Hash, TAO_PG_Location_Equal_To, ACE_Null_Mutex>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Hash_Map_Entry<PortableGroup::Location, TAO_LB_LoadAlertInfo>
#pragma instantiate ACE_Hash_Map_Manager_Ex<PortableGroup::Location, TAO_LB_LoadAlertInfo, TAO_PG_Location_Hash, TAO_PG_Location_Equal_To, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<PortableGroup::Location, TAO_LB_LoadAlertInfo, TAO_PG_Location_Hash, TAO_PG_Location_Equal_To, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<PortableGroup::Location, TAO_LB_LoadAlertInfo, TAO_PG_Location_Hash, TAO_PG_Location_Equal_To, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<PortableGroup::Location, TAO_LB_LoadAlertInfo, TAO_PG_Location_Hash, TAO_PG_Location_Equal_To, ACE_Null_Mutex>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
