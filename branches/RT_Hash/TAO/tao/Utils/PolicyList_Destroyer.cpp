#include "PolicyList_Destroyer.h"
#include "tao/Environment.h"

#if !defined (__ACE_INLINE__)
# include "PolicyList_Destroyer.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID (Utils,
           PolicyList_Deactivator,
           "$Id$")

TAO::Utils::PolicyList_Destroyer::~PolicyList_Destroyer()
  ACE_THROW_SPEC (())
{
  ACE_DECLARE_NEW_CORBA_ENV;

  for (CORBA::ULong i = 0; i != length(); ++i)
    {
      CORBA::Policy_ptr policy = (*this)[i];
      if (CORBA::is_nil (policy))
        {
          continue;
        }

      ACE_TRY
        {
          policy->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      ACE_CATCHALL
        {
        }
      ACE_ENDTRY;

      (*this)[i] = CORBA::Policy::_nil();
    }
}
