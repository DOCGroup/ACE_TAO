// -*- C++ -*-
//
// $Id$

ACE_INLINE CORBA::ULong
TAO_LB_Replica_Hash::operator() (
  const CORBA::Object_ptr &replica) const
{
  CORBA::ULong max = 4294967295UL;  // Maximum 32-bit unsigned integer
  return replica->_hash (max);
}

ACE_INLINE CORBA::ULong
TAO_LB_Location_Hash::operator() (
  const TAO_LoadBalancing::Location &location) const
{
  CORBA::ULong hash = 0;

  size_t location_length = location.length ();

  for (size_t i = 0; i < location_length; ++i)
    hash += (ACE::hash_pjw (location[i].id) +
             ACE::hash_pjw (location[i].kind));

  return hash;
}
