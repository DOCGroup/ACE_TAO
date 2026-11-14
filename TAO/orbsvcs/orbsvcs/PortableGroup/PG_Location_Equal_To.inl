// -*- C++ -*-
TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE int
TAO_PG_Location_Equal_To::operator () (
  PortableGroup::Location const & lhs,
  PortableGroup::Location const & rhs) const
{
  return lhs == rhs;
}

TAO_END_VERSIONED_NAMESPACE_DECL
