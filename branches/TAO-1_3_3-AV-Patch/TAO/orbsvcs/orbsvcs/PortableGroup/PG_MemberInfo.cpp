#include "PG_MemberInfo.h"
#include "PG_Operators.h"

ACE_RCSID (PortableGroup,
           PG_MemberInfo,
           "$Id$")


int
TAO_PG_MemberInfo::operator== (const TAO_PG_MemberInfo & rhs)
{
  // For the purposes of the member info set, only the location is
  // important.
  return location == rhs.location;
}


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Node<TAO_PG_MemberInfo>;
template class ACE_Unbounded_Set<TAO_PG_MemberInfo>;
template class ACE_Unbounded_Set_Iterator<TAO_PG_MemberInfo>;

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Node<TAO_PG_MemberInfo>
#pragma instantiate ACE_Unbounded_Set<TAO_PG_MemberInfo>
#pragma instantiate ACE_Unbounded_Set_Iterator<TAO_PG_MemberInfo>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
