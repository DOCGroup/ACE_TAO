// -*- C++ -*-

ACE_INLINE int
TAO_LB_Location_Equal_To::operator () (
  const PortableGroup::Location &lhs,
  const PortableGroup::Location &rhs) const
{
  // @todo Interim implementation until we figure out what to do when a
  //       location has multiple components.
  CORBA::ULong lhs_len = lhs.length ();
  CORBA::ULong rhs_len = rhs.length ();

  if (lhs_len != rhs_len)
    return 0;

  for (CORBA::ULong i = 0; i < lhs_len; ++i)
    if (ACE_OS::strcmp (lhs[i].id.in (), rhs[i].id.in ()) != 0
        || ACE_OS::strcmp (lhs[i].kind.in (), rhs[i].kind.in ()) != 0)
      return 0;

  return 1;
}
