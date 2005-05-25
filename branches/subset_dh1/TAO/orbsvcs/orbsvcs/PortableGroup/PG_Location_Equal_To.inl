// -*- C++ -*-
//
// $Id$

ACE_INLINE int
TAO_PG_Location_Equal_To::operator () (
  const PortableGroup::Location &lhs,
  const PortableGroup::Location &rhs) const
{
  return lhs == rhs;
}
