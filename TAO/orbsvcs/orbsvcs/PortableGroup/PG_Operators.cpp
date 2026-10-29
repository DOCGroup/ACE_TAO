#include "orbsvcs/PortableGroup/PG_Operators.h"
#include "ace/OS_NS_string.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

bool
operator== (CosNaming::Name const &lhs, CosNaming::Name const &rhs)
{
  CORBA::ULong const lhs_len = lhs.length ();
  CORBA::ULong const rhs_len = rhs.length ();

  if (lhs_len != rhs_len)
    return false;

  for (CORBA::ULong i = 0; i < lhs_len; ++i)
    if (ACE_OS::strcmp (lhs[i].id.in (), rhs[i].id.in ()) != 0
        || ACE_OS::strcmp (lhs[i].kind.in (), rhs[i].kind.in ()) != 0)
      return false;

  return true;
}

bool
operator!= (CosNaming::Name const &lhs, CosNaming::Name const &rhs)
{
  return !(lhs == rhs);
}

TAO_END_VERSIONED_NAMESPACE_DECL
