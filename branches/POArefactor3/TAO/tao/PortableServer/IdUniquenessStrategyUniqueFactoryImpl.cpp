// $Id$

#include "IdUniquenessStrategyUniqueFactoryImpl.h"
#include "IdUniquenessStrategyUnique.h"
#include "ace/Dynamic_Service.h"

ACE_RCSID (PortableServer,
           IdUniquenessStrategyUniqueFactoryImpl,
           "$Id$")

namespace TAO
{
  namespace Portable_Server
  {
    IdUniquenessStrategyUniqueFactoryImpl::~IdUniquenessStrategyUniqueFactoryImpl (void)
    {
    }

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
      ACE_ENV_ARG_DECL)
    {
      strategy->strategy_cleanup (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      delete strategy;
    }

    ACE_STATIC_SVC_DEFINE (
        IdUniquenessStrategyUniqueFactoryImpl,
        ACE_TEXT ("IdUniquenessStrategyUniqueFactory"),
        ACE_SVC_OBJ_T,
        &ACE_SVC_NAME (IdUniquenessStrategyUniqueFactoryImpl),
        ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
        0
      )

    ACE_FACTORY_DEFINE (TAO_PortableServer, IdUniquenessStrategyUniqueFactoryImpl)

    #if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

    template class ACE_Dynamic_Service<IdUniquenessStrategyUniqueFactoryImpl>;

    #elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

    #pragma instantiate ACE_Dynamic_Service<IdUniquenessStrategyUniqueFactoryImpl>

    #endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
  }
}

