/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    RTCosScheduling_ServerScheduler_i.i
 *
 *  $Id$
 *
 *  @author Matt Murphy <murphym@cs.uri.edu>
 */
//=============================================================================

ACE_INLINE int
ACE_Equal_To< COS_SCHEDULING_OBJECT_NAME_KEY >::operator () (
  COS_SCHEDULING_OBJECT_NAME_KEY lhs,
  COS_SCHEDULING_OBJECT_NAME_KEY rhs) const
{
     return lhs->_is_equivalent(rhs.in());
}

ACE_INLINE u_long
TAO::CosSchedulingObjectMap_Hash_Key::operator () (
  const COS_SCHEDULING_OBJECT_NAME_KEY &key) const
{
      u_long value;
      value = key->_hash(LONG_MAX);
      return value;
}

ACE_INLINE u_long
TAO::CosSchedulingInvocation_Hash_key::operator () (
  const COS_SCHEDULING_INVOCATION_KEY &key) const
{
  return ACE_static_cast(u_long, key);
}
