/**
 * @file PolicyList_Destroyer.cpp
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@atdesk.com>
 */
#include "PolicyList_Destroyer.h"

#if !defined (__ACE_INLINE__)
# include "PolicyList_Destroyer.inl"
#endif /* __ACE_INLINE__ */

TAO::Utils::PolicyList_Destroyer::
~PolicyList_Destroyer()
{
  for (CORBA::ULong i = 0; i != length(); ++i)
  {
    CORBA::Policy_ptr policy = (*this)[i];
    if (CORBA::is_nil(policy))
    {
      continue;
    }
    try
    {
      policy->destroy();
    }
    catch (...)
    {
    }
    (*this)[i] = CORBA::Policy::_nil();
  }
}
