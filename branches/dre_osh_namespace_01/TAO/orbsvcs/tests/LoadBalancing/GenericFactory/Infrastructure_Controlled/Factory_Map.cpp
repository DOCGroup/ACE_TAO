// -*- C++ -*-

#include "Factory_Map.h"


ACE_RCSID (Infrastructure_Controlled,
           Factory_Map,
           "$Id$");


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Hash_Map_Entry<ACE_UINT32, Factory_Struct>;
template class ACE_Hash_Map_Manager_Ex<ACE_UINT32, Factory_Struct, ACE_Hash<ACE_UINT32>, ACE_Equal_To<ACE_UINT32>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<ACE_UINT32, Factory_Struct, ACE_Hash<ACE_UINT32>, ACE_Equal_To<ACE_UINT32>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<ACE_UINT32, Factory_Struct, ACE_Hash<ACE_UINT32>, ACE_Equal_To<ACE_UINT32>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<ACE_UINT32, Factory_Struct, ACE_Hash<ACE_UINT32>, ACE_Equal_To<ACE_UINT32>, ACE_Null_Mutex>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Hash_Map_Entry<ACE_UINT32, Factory_Struct>
#pragma instantiate ACE_Hash_Map_Manager_Ex<ACE_UINT32, Factory_Struct, ACE_Hash<ACE_UINT32>, ACE_Equal_To<ACE_UINT32>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<ACE_UINT32, Factory_Struct, ACE_Hash<ACE_UINT32>, ACE_Equal_To<ACE_UINT32>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<ACE_UINT32, Factory_Struct, ACE_Hash<ACE_UINT32>, ACE_Equal_To<ACE_UINT32>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<ACE_UINT32, Factory_Struct, ACE_Hash<ACE_UINT32>, ACE_Equal_To<ACE_UINT32>, ACE_Null_Mutex>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
