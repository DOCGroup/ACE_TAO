// -*- C++ -*-

#include "LB_ObjectGroup_Map.h"

ACE_RCSID (LoadBalancing,
           LB_ObjectGroup_Map,
           "$Id$")


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Hash_Map_Entry<PortableServer::ObjectId, TAO_LB_ObjectGroup_Map_Entry *>;
template class ACE_Hash_Map_Manager<PortableServer::ObjectId, TAO_LB_ObjectGroup_Map_Entry *, TAO_SYNCH_MUTEX>;
template class ACE_Hash_Map_Iterator<PortableServer::ObjectId, TAO_LB_ObjectGroup_Map_Entry *, TAO_SYNCH_MUTEX>;
template class ACE_Hash_Map_Reverse_Iterator<PortableServer::ObjectId, TAO_LB_ObjectGroup_Map_Entry *, TAO_SYNCH_MUTEX>;
template class ACE_Hash_Map_Manager_Ex<PortableServer::ObjectId, TAO_LB_ObjectGroup_Map_Entry *, ACE_Hash<PortableServer::ObjectId>, ACE_Equal_To<PortableServer::ObjectId>, TAO_SYNCH_MUTEX>;
template class ACE_Hash_Map_Iterator_Base_Ex<PortableServer::ObjectId, TAO_LB_ObjectGroup_Map_Entry *, ACE_Hash<PortableServer::ObjectId>, ACE_Equal_To<PortableServer::ObjectId>, TAO_SYNCH_MUTEX>;
template class ACE_Hash_Map_Iterator_Ex<PortableServer::ObjectId, TAO_LB_ObjectGroup_Map_Entry *, ACE_Hash<PortableServer::ObjectId>, ACE_Equal_To<PortableServer::ObjectId>, TAO_SYNCH_MUTEX>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<PortableServer::ObjectId, TAO_LB_ObjectGroup_Map_Entry *, ACE_Hash<PortableServer::ObjectId>, ACE_Equal_To<PortableServer::ObjectId>, TAO_SYNCH_MUTEX>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Hash_Map_Entry<PortableServer::ObjectId, TAO_LB_ObjectGroup_Map_Entry *>
#pragma instantiate ACE_Hash_Map_Manager<PortableServer::ObjectId, TAO_LB_ObjectGroup_Map_Entry *, TAO_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator<PortableServer::ObjectId, TAO_LB_ObjectGroup_Map_Entry *, TAO_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<PortableServer::ObjectId, TAO_LB_ObjectGroup_Map_Entry *, TAO_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Manager_Ex<PortableServer::ObjectId, TAO_LB_ObjectGroup_Map_Entry *, ACE_Hash<PortableServer::ObjectId>, ACE_Equal_To<PortableServer::ObjectId>, TAO_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<PortableServer::ObjectId, TAO_LB_ObjectGroup_Map_Entry *, ACE_Hash<PortableServer::ObjectId>, ACE_Equal_To<PortableServer::ObjectId>, TAO_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<PortableServer::ObjectId, TAO_LB_ObjectGroup_Map_Entry *, ACE_Hash<PortableServer::ObjectId>, ACE_Equal_To<PortableServer::ObjectId>, TAO_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<PortableServer::ObjectId, TAO_LB_ObjectGroup_Map_Entry *, ACE_Hash<PortableServer::ObjectId>, ACE_Equal_To<PortableServer::ObjectId>, TAO_SYNCH_MUTEX>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
