// -*- C++ -*-
//
// $Id$

ACE_INLINE int
TAO_LB_Replica_Equal_To::operator() (
  const CORBA::Object_ptr &lhs,
  const CORBA::Object_ptr &rhs) const
{
  // As with all CORBA objects, the
  // CORBA::Object::_is_equivalent() returns 1 if two
  // references refer to the same object.  However, it is possible
  // that is_equivalent() may return 0 even if both references refer
  // to the same object.

  return lhs->_is_equivalent (rhs);
}


ACE_INLINE int
TAO_LB_Location_Equal_To::operator() (
  const TAO_LoadBalancing::Location &lhs,
  const TAO_LoadBalancing::Location &rhs) const
{
  size_t lhs_length = lhs.length ();
  size_t rhs_length = rhs.length ();

  if (lhs_length != rhs_length)
    return 0;

  for (size_t i = 0; i < lhs_length; ++i)
    if (ACE_OS::strcmp (lhs[i].id, rhs[i].id) != 0
        || ACE_OS::strcmp (lhs[i].kind), rhs[i].kind != 0)
      return 0;

  return 1;
}
