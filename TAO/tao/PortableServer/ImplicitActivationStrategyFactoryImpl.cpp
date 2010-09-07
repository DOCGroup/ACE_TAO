// -*- C++ -*-
// $Id$

#include "tao/PortableServer/ImplicitActivationStrategyFactoryImpl.h"
#include "ace/Dynamic_Service.h"
#include "tao/PortableServer/ImplicitActivationStrategyImplicit.h"
#include "tao/PortableServer/ImplicitActivationStrategyExplicit.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    ImplicitActivationStrategy*
    ImplicitActivationStrategyFactoryImpl::create (
      ::PortableServer::ImplicitActivationPolicyValue value)
    {
      ImplicitActivationStrategy* strategy = 0;
      const char * strategy_name = 0;

      switch (value)
      {
        case ::PortableServer::IMPLICIT_ACTIVATION :
        {
          strategy_name = "ImplicitActivationStrategyImplicit";
          break;
        }
        case ::PortableServer::NO_IMPLICIT_ACTIVATION :
        {
          strategy_name = "ImplicitActivationStrategyExplicit";
          break;
        }
      }

      strategy =
        ACE_Dynamic_Service<ImplicitActivationStrategy>::instance (strategy_name);

      if (strategy == 0)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("(%P|%t) ERROR, Unable to get %s\n"),
                    strategy_name));

      return strategy;
    }

    void
    ImplicitActivationStrategyFactoryImpl::destroy (
      ImplicitActivationStrategy * /*strategy*/
      )
    {
      // Noop because both types are singletons
    }
  }
}


ACE_FACTORY_NAMESPACE_DEFINE (
  ACE_Local_Service,
  ImplicitActivationStrategyFactoryImpl,
  TAO_VERSIONED_NAMESPACE_NAME::TAO::Portable_Server::ImplicitActivationStrategyFactoryImpl)


ACE_STATIC_SVC_DEFINE (
  ImplicitActivationStrategyFactoryImpl,
  ACE_TEXT ("ImplicitActivationStrategyFactory"),
  ACE_SVC_OBJ_T,
  &ACE_SVC_NAME (ImplicitActivationStrategyFactoryImpl),
  ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
  0)

TAO_END_VERSIONED_NAMESPACE_DECL
