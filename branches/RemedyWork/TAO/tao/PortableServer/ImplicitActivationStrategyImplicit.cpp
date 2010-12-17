// $Id$

#include "tao/PortableServer/ImplicitActivationStrategyImplicit.h"

#if !defined (CORBA_E_MICRO) && !defined (CORBA_E_COMPACT)

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    bool
    ImplicitActivationStrategyImplicit::allow_implicit_activation (void) const
    {
      return true;
    }
  }
}


ACE_FACTORY_NAMESPACE_DEFINE (
  ACE_Local_Service,
  ImplicitActivationStrategyImplicit,
  TAO::Portable_Server::ImplicitActivationStrategyImplicit)

ACE_STATIC_SVC_DEFINE (
  ImplicitActivationStrategyImplicit,
  ACE_TEXT ("ImplicitActivationStrategyImplicit"),
  ACE_SVC_OBJ_T,
  &ACE_SVC_NAME (ImplicitActivationStrategyImplicit),
  ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
  0)

TAO_END_VERSIONED_NAMESPACE_DECL

#endif
