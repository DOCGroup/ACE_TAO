#include "PG_ObjectGroup_Map.h"


ACE_RCSID (PortableGroup,
           PG_ObjectGroup_Map,
           "$Id$")


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Hash_Map_Entry<PortableServer::ObjectGroupId, TAO_PG_ObjectGroup_Map_Entry *>;
template class ACE_Hash_Map_Manager_Ex<PortableServer::ObjectGroupId, TAO_PG_ObjectGroup_Map_Entry *, ACE_Hash<ACE_UINT64>, ACE_Equal_To<ACE_UINT64>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<PortableServer::ObjectGroupId, TAO_PG_ObjectGroup_Map_Entry *, ACE_Hash<ACE_UINT64>, ACE_Equal_To<ACE_UINT64>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<PortableServer::ObjectGroupId, TAO_PG_ObjectGroup_Map_Entry *, ACE_Hash<ACE_UINT64>, ACE_Equal_To<ACE_UINT64>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<PortableServer::ObjectGroupId, TAO_PG_ObjectGroup_Map_Entry *, ACE_Hash<ACE_UINT64>, ACE_Equal_To<ACE_UINT64>, ACE_Null_Mutex>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Hash_Map_Entry<PortableServer::ObjectGroupId, TAO_PG_ObjectGroup_Map_Entry *>
#pragma instantiate ACE_Hash_Map_Manager_Ex<PortableServer::ObjectGroupId, TAO_PG_ObjectGroup_Map_Entry *, ACE_Hash<ACE_UINT64>, ACE_Equal_To<ACE_UINT64>, ACE_Null_Mutex>;
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<PortableServer::ObjectGroupId, TAO_PG_ObjectGroup_Map_Entry *, ACE_Hash<ACE_UINT64>, ACE_Equal_To<ACE_UINT64>, ACE_Null_Mutex>;
#pragma instantiate ACE_Hash_Map_Iterator_Ex<PortableServer::ObjectGroupId, TAO_PG_ObjectGroup_Map_Entry *, ACE_Hash<ACE_UINT64>, ACE_Equal_To<ACE_UINT64>, ACE_Null_Mutex>;
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<PortableServer::ObjectGroupId, TAO_PG_ObjectGroup_Map_Entry *, ACE_Hash<ACE_UINT64>, ACE_Equal_To<ACE_UINT64>, ACE_Null_Mutex>;

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
