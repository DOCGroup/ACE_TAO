// $Id$

#include "IdUniquenessStrategyFactoryImpl.h"
#include "IdUniquenessStrategyUnique.h"
#include "IdUniquenessStrategyMultiple.h"

ACE_RCSID (PortableServer,
           IdUniquenessStrategyFactoryImpl,
           "$Id$")

#if (TAO_HAS_MINIMUM_POA == 0)

namespace TAO
{
  namespace Portable_Server
  {
    IdUniquenessStrategyFactoryImpl::~IdUniquenessStrategyFactoryImpl (void)
    {
    }
    IdUniquenessStrategy*
    IdUniquenessStrategyFactoryImpl::create (
      ::PortableServer::IdUniquenessPolicyValue value)
    {
      IdUniquenessStrategy* strategy = 0;

      switch (value)
      {
        case ::PortableServer::MULTIPLE_ID :
        {
          ACE_NEW_RETURN (strategy, Multiple_Id_Uniqueness_Strategy, 0);
          break;
        }
        case ::PortableServer::UNIQUE_ID :
        {
          ACE_NEW_RETURN (strategy, Unique_Id_Uniqueness_Strategy, 0);
          break;
        }
      }

      return strategy;
    }

    ACE_STATIC_SVC_DEFINE (
        IdUniquenessStrategyFactoryImpl,
        ACE_TEXT ("IdUniquenessStrategyFactoryImpl"),
        ACE_SVC_OBJ_T,
        &ACE_SVC_NAME (IdUniquenessStrategyFactoryImpl),
        ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
        0
      )

    ACE_FACTORY_DEFINE (TAO_PortableServer, IdUniquenessStrategyFactoryImpl)

    #if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

    template class ACE_Dynamic_Service<IdUniquenessStrategyFactoryImpl>;

    #elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

    #pragma instantiate ACE_Dynamic_Service<IdUniquenessStrategyFactoryImpl>

    #endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */

