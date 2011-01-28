// $Id$

#include "tao/PortableServer/IdAssignmentStrategyFactoryImpl.h"
#include "tao/PortableServer/IdAssignmentStrategy.h"
#include "ace/Dynamic_Service.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    IdAssignmentStrategy *
    IdAssignmentStrategyFactoryImpl::create (
      ::PortableServer::IdAssignmentPolicyValue value)
    {
      IdAssignmentStrategy *strategy = 0;
      const char *strategy_name = 0;

      switch (value)
      {
        case ::PortableServer::SYSTEM_ID :
        {
          strategy_name = "IdAssignmentStrategySystem";
          break;
        }
        case ::PortableServer::USER_ID :
        {
          strategy_name = "IdAssignmentStrategyUser";
          break;
        }
      }

      strategy =
        ACE_Dynamic_Service <IdAssignmentStrategy>::instance (strategy_name);

      if (strategy == 0)
        ACE_ERROR ((LM_ERROR,
                   ACE_TEXT ("(%P|%t) ERROR, Unable to get %s\n"),
                   strategy_name));

      return strategy;
    }

    void
    IdAssignmentStrategyFactoryImpl::destroy (IdAssignmentStrategy *)
    {
      // Noop because both types are singletons
    }

  }
}



ACE_STATIC_SVC_DEFINE (
  IdAssignmentStrategyFactoryImpl,
  ACE_TEXT ("IdAssignmentStrategyFactory"),
  ACE_SVC_OBJ_T,
  &ACE_SVC_NAME (IdAssignmentStrategyFactoryImpl),
  ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
  0)

ACE_FACTORY_NAMESPACE_DEFINE (
  ACE_Local_Service,
  IdAssignmentStrategyFactoryImpl,
  TAO::Portable_Server::IdAssignmentStrategyFactoryImpl)
TAO_END_VERSIONED_NAMESPACE_DECL
