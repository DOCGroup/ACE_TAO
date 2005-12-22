#include "IORInterceptor_Details.h"

#if !defined (__ACE_INLINE__)
#include "IORInterceptor_Details.inl"
#endif /* defined INLINE */

ACE_RCSID (PI,
           IORInterceptor_Details,
           "$Id$")

#include "tao/SystemException.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  void
  IORInterceptor_Details::apply_policies (
      const CORBA::PolicyList &policies
      ACE_ENV_ARG_DECL)
  {
    // There are currently no policies that apply to IOR Interceptors.
    ACE_UNUSED_ARG (policies);
    ACE_THROW (CORBA::INV_POLICY ());
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL
