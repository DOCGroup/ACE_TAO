// -*- C++ -*-
//
// $Id$

ACE_INLINE CORBA::ULong
TAO_LB_ObjectGroup_Hash::operator() (
  const TAO_LoadBalancing::ObjectGroup_ptr &object_group) const
{
  CORBA::ULong max = 4294967295UL;  // Maximum 32-bit unsigned integer
  return object_group->_hash (max);
}

// -------------------------------------------------------------------

ACE_INLINE CORBA::ULong
TAO_LB_FactoryCreationId_Hash::operator() (
  const TAO_LoadBalancing::FactoryCreationId &factory_creation_id) const
{
  // A Load Balancer FactoryCreationId is simply a CORBA::ULong.

  CORBA::ULong id;
  factory_creation_id >>= id;

  return id;
}
