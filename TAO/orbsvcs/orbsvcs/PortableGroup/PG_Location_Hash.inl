// -*- C++ -*-
TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE CORBA::ULong
TAO_PG_Location_Hash::operator() (
  PortableGroup::Location const & location) const
{
  // @todo Interim implementation until we figure out what to do when a
  //       location has multiple components.

  CORBA::ULong const len = location.length ();
  CORBA::ULong hash {};

  for (CORBA::ULong i = 0; i < len; ++i)
    hash +=
      ACE::hash_pjw (location[i].id.in ()) +
      ACE::hash_pjw (location[i].kind.in ());

  return hash;
}

TAO_END_VERSIONED_NAMESPACE_DECL
