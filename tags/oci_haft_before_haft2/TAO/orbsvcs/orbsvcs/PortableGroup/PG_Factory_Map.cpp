// -*- C++ -*-

#include "PG_Factory_Map.h"


ACE_RCSID (PortableGroup,
           PG_Factory_Map,
           "$Id$")


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Hash_Map_Entry<PortableGroup::ObjectGroupId, TAO_PG_Factory_Set>;
template class ACE_Hash_Map_Manager_Ex<PortableGroup::ObjectGroupId, TAO_PG_Factory_Set, ACE_Hash<PortableGroup::ObjectGroupId>, ACE_Equal_To<PortableGroup::ObjectGroupId>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<PortableGroup::ObjectGroupId, TAO_PG_Factory_Set, ACE_Hash<PortableGroup::ObjectGroupId>, ACE_Equal_To<PortableGroup::ObjectGroupId>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<PortableGroup::ObjectGroupId, TAO_PG_Factory_Set, ACE_Hash<PortableGroup::ObjectGroupId>, ACE_Equal_To<PortableGroup::ObjectGroupId>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<PortableGroup::ObjectGroupId, TAO_PG_Factory_Set, ACE_Hash<PortableGroup::ObjectGroupId>, ACE_Equal_To<PortableGroup::ObjectGroupId>, ACE_Null_Mutex>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Hash_Map_Entry<PortableGroup::ObjectGroupId, TAO_PG_Factory_Set>
#pragma instantiate ACE_Hash_Map_Manager_Ex<PortableGroup::ObjectGroupId, TAO_PG_Factory_Set, ACE_Hash<PortableGroup::ObjectGroupId>, ACE_Equal_To<PortableGroup::ObjectGroupId>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<PortableGroup::ObjectGroupId, TAO_PG_Factory_Set, ACE_Hash<PortableGroup::ObjectGroupId>, ACE_Equal_To<PortableGroup::ObjectGroupId>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<PortableGroup::ObjectGroupId, TAO_PG_Factory_Set, ACE_Hash<PortableGroup::ObjectGroupId>, ACE_Equal_To<PortableGroup::ObjectGroupId>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<PortableGroup::ObjectGroupId, TAO_PG_Factory_Set, ACE_Hash<PortableGroup::ObjectGroupId>, ACE_Equal_To<PortableGroup::ObjectGroupId>, ACE_Null_Mutex>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
