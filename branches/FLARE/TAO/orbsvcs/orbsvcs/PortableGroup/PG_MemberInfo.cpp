#include "orbsvcs/PortableGroup/PG_MemberInfo.h"
#include "orbsvcs/PortableGroup/PG_Operators.h"

ACE_RCSID (PortableGroup,
           PG_MemberInfo,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

bool
TAO_PG_MemberInfo::operator== (const TAO_PG_MemberInfo & rhs)
{
  // For the purposes of the member info set, only the location is
  // important.
  return location == rhs.location;
}

TAO_END_VERSIONED_NAMESPACE_DECL
