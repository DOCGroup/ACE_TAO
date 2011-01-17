// $Id$

#include "tao/PortableServer/IdUniquenessStrategyUniqueFactoryImpl.h"
#include "tao/PortableServer/IdUniquenessStrategyUnique.h"
#include "ace/Dynamic_Service.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    IdUniquenessStrategy*
    IdUniquenessStrategyUniqueFactoryImpl::create (
      ::PortableServer::IdUniquenessPolicyValue value)
    {
      IdUniquenessStrategy* strategy = 0;

      switch (value)
      {
        case ::PortableServer::UNIQUE_ID :
        {
          ACE_NEW_RETURN (strategy, IdUniquenessStrategyUnique, 0);
          break;
        }
        case ::PortableServer::MULTIPLE_ID :
        {
          ACE_ERROR ((LM_ERROR, "Incorrect type in IdUniquenessStrategyUniqueFactoryImpl"));
          break;
        }
      }

      return strategy;
    }

    void
    IdUniquenessStrategyUniqueFactoryImpl::destroy (
      IdUniquenessStrategy *strategy
      )
    {
      strategy->strategy_cleanup ();

      delete strategy;
    }

  }
}


ACE_STATIC_SVC_DEFINE (
  IdUniquenessStrategyUniqueFactoryImpl,
  ACE_TEXT ("IdUniquenessStrategyUniqueFactory"),
  ACE_SVC_OBJ_T,
  &ACE_SVC_NAME (IdUniquenessStrategyUniqueFactoryImpl),
  ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
  0)

ACE_FACTORY_NAMESPACE_DEFINE (
  ACE_Local_Service,
  IdUniquenessStrategyUniqueFactoryImpl,
  TAO::Portable_Server::IdUniquenessStrategyUniqueFactoryImpl)

TAO_END_VERSIONED_NAMESPACE_DECL
