// -*- C++ -*-

#include "LB_Location_Index_Map.h"

ACE_RCSID (LoadBalancing,
           LB_Location_Index,
           "$Id$")


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Hash_Map_Entry<PortableGroup::ObjectGroupId, CORBA::ULong>;
template class ACE_Hash_Map_Manager_Ex<PortableGroup::ObjectGroupId, CORBA::ULong, ACE_Hash<ACE_UINT64>, ACE_Equal_To<ACE_UINT64>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<PortableGroup::ObjectGroupId, CORBA::ULong, ACE_Hash<ACE_UINT64>, ACE_Equal_To<ACE_UINT64>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<PortableGroup::ObjectGroupId, CORBA::ULong, ACE_Hash<ACE_UINT64>, ACE_Equal_To<ACE_UINT64>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<PortableGroup::ObjectGroupId, CORBA::ULong, ACE_Hash<ACE_UINT64>, ACE_Equal_To<ACE_UINT64>, ACE_Null_Mutex>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Hash_Map_Entry<PortableGroup::ObjectGroupId, CORBA::ULong>
#pragma instantiate ACE_Hash_Map_Manager_Ex<PortableGroup::ObjectGroupId, CORBA::ULong, ACE_Hash<ACE_UINT64>, ACE_Equal_To<ACE_UINT64>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<PortableGroup::ObjectGroupId, CORBA::ULong, ACE_Hash<ACE_UINT64>, ACE_Equal_To<ACE_UINT64>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<PortableGroup::ObjectGroupId, CORBA::ULong, ACE_Hash<ACE_UINT64>, ACE_Equal_To<ACE_UINT64>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<PortableGroup::ObjectGroupId, CORBA::ULong, ACE_Hash<ACE_UINT64>, ACE_Equal_To<ACE_UINT64>, ACE_Null_Mutex>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
