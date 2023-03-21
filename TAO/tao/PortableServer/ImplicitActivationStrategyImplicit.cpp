#include "tao/PortableServer/ImplicitActivationStrategyImplicit.h"

#if !defined (CORBA_E_MICRO) && !defined (CORBA_E_COMPACT)

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    bool
    ImplicitActivationStrategyImplicit::allow_implicit_activation () const
    {
      return true;
    }
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif
