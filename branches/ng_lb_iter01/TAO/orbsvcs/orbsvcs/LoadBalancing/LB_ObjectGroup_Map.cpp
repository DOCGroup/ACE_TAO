// -*- C++ -*-

#include "LB_ObjectGroup_Map.h"


ACE_RCSID (LoadBalancing,
           LB_ObjectGroup_Map,
           "$Id$")


#if !defined (__ACE_INLINE__)
#include "LB_ObjectGroup_Map.inl"
#endif  /* __ACE_INLINE__ */


TAO_LB_ObjectGroup_Map::TAO_LB_ObjectGroup_Map (void)
  : poa_ (),
    table_ ()
{
}

int
TAO_LB_ObjectGroup_Map::bind (const PortableServer::ObjectId &oid,
                              TAO_LB_ObjectGroup_Map_Entry *entry)
{
  return this->table_.bind (oid, entry);
}

int
TAO_LB_ObjectGroup_Map::find (const PortableServer::ObjectId &oid,
                              TAO_LB_ObjectGroup_Map_Entry *&entry)
{
  return this->table_.find (oid, entry);
}

int
TAO_LB_ObjectGroup_Map::unbind (const PortableServer::ObjectId &oid)
{
  return this->table_.unbind (oid);
}

void
TAO_LB_ObjectGroup_Map::poa (PortableServer::POA_ptr poa)
{
  this->poa_ = PortableServer::POA::_duplicate (poa);
}

TAO_LB_ObjectGroup_Map_Entry *
TAO_LB_ObjectGroup_Map::get_group_entry (
    CORBA::Object_ptr object_group,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::ObjectGroupNotFound))
{
  if (CORBA::is_nil (this->poa_.in ()))
    ACE_THROW_RETURN (CORBA::INTERNAL (), 0);

  PortableServer::ObjectId_var oid =
    this->poa_->reference_to_id (object_group, ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  TAO_LB_ObjectGroup_Map_Entry *group_entry = 0;
  if (this->table_.find (oid.in (), group_entry) != 0)
    ACE_THROW_RETURN (PortableGroup::ObjectGroupNotFound (),
                      0);

  return group_entry;
}


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Hash_Map_Entry<PortableServer::ObjectId, TAO_LB_ObjectGroup_Map_Entry *>;
template class ACE_Hash_Map_Manager_Ex<PortableServer::ObjectId, TAO_LB_ObjectGroup_Map_Entry *, TAO_ObjectId_Hash, ACE_Equal_To<PortableServer::ObjectId>, TAO_SYNCH_MUTEX>;
template class ACE_Hash_Map_Iterator_Base_Ex<PortableServer::ObjectId, TAO_LB_ObjectGroup_Map_Entry *, TAO_ObjectId_Hash, ACE_Equal_To<PortableServer::ObjectId>, TAO_SYNCH_MUTEX>;
template class ACE_Hash_Map_Iterator_Ex<PortableServer::ObjectId, TAO_LB_ObjectGroup_Map_Entry *, TAO_ObjectId_Hash, ACE_Equal_To<PortableServer::ObjectId>, TAO_SYNCH_MUTEX>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<PortableServer::ObjectId, TAO_LB_ObjectGroup_Map_Entry *, TAO_ObjectId_Hash, ACE_Equal_To<PortableServer::ObjectId>, TAO_SYNCH_MUTEX>;

template class ACE_Equal_To<PortableServer::ObjectId>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Hash_Map_Entry<PortableServer::ObjectId, TAO_LB_ObjectGroup_Map_Entry *>
#pragma instantiate ACE_Hash_Map_Manager_Ex<PortableServer::ObjectId, TAO_LB_ObjectGroup_Map_Entry *, TAO_ObjectId_Hash, ACE_Equal_To<PortableServer::ObjectId>, TAO_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<PortableServer::ObjectId, TAO_LB_ObjectGroup_Map_Entry *, TAO_ObjectId_Hash, ACE_Equal_To<PortableServer::ObjectId>, TAO_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<PortableServer::ObjectId, TAO_LB_ObjectGroup_Map_Entry *, TAO_ObjectId_Hash, ACE_Equal_To<PortableServer::ObjectId>, TAO_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<PortableServer::ObjectId, TAO_LB_ObjectGroup_Map_Entry *, TAO_ObjectId_Hash, ACE_Equal_To<PortableServer::ObjectId>, TAO_SYNCH_MUTEX>

#pragma instantiate  ACE_Equal_To<PortableServer::ObjectId>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
