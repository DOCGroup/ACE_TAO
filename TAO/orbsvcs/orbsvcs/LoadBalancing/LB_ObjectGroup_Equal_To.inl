// -*- C++ -*-
//
// $Id$

ACE_INLINE int
TAO_LB_ObjectGroup_Equal_To::operator() (
  const TAO_LoadBalancing::ObjectGroup_ptr &lhs,
  const TAO_LoadBalancing::ObjectGroup_ptr &rhs) const
{
  // As with all CORBA objects, the
  // TAO_LoadBalancing::ObjectGroup::is_equivalent() returns 1 if two
  // references refer to the same object.  However, it is possible
  // that is_equivalent() may return 0 even if both references refer
  // to the same object.

  return lhs->_is_equivalent (rhs);
}


ACE_INLINE int
TAO_LB_FactoryCreationId_Equal_To::operator() (
  const TAO_LoadBalancing::FactoryCreationId &lhs,
  const TAO_LoadBalancing::FactoryCreationId &rhs) const
{
  // A Load Balancer FactoryCreationId is simply a CORBA::ULong.

  CORBA::ULong lhs_id, rhs_id;

  lhs >>= lhs_id;
  rhs >>= rhs_id;

  return (lhs_id == rhs_id);
}
