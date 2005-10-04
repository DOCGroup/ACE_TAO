#include "PG_MemberInfo.h"
#include "PG_Operators.h"

ACE_RCSID (PortableGroup,
           PG_MemberInfo,
           "$Id$")


bool
TAO_PG_MemberInfo::operator== (const TAO_PG_MemberInfo & rhs)
{
  // For the purposes of the member info set, only the location is
  // important.
  return location == rhs.location;
}
