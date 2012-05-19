// -*- C++ -*-
// $Id$

#include "tao/PortableServer/ImplicitActivationStrategyExplicit.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    bool
    ImplicitActivationStrategyExplicit::allow_implicit_activation (void) const
    {
      return false;
    }
  }
}

ACE_FACTORY_NAMESPACE_DEFINE (
  ACE_Local_Service,
  ImplicitActivationStrategyExplicit,
  TAO::Portable_Server::ImplicitActivationStrategyExplicit)

ACE_STATIC_SVC_DEFINE (
  ImplicitActivationStrategyExplicit,
  ACE_TEXT ("ImplicitActivationStrategyExplicit"),
  ACE_SVC_OBJ_T,
  &ACE_SVC_NAME (ImplicitActivationStrategyExplicit),
  ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
  0)

TAO_END_VERSIONED_NAMESPACE_DECL
