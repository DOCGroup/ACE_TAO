#include "PG_Operators.h"

ACE_RCSID (PortableGroup,
           PG_Operators,
           "$Id$")

int
operator== (const CosNaming::Name & lhs, const CosNaming::Name & rhs)
{
  const CORBA::ULong lhs_len = lhs.length ();
  const CORBA::ULong rhs_len = rhs.length ();

  if (lhs_len != rhs_len)
    return 0;

  for (CORBA::ULong i = 0; i < lhs_len; ++i)
    if (ACE_OS::strcmp (lhs[i].id.in (), rhs[i].id.in ()) != 0
        || ACE_OS::strcmp (lhs[i].kind.in (), rhs[i].kind.in ()) != 0)
      return 0;

  return 1;
}

int
operator!= (const CosNaming::Name & lhs, const CosNaming::Name & rhs)
{
  return !(lhs == rhs);
}
