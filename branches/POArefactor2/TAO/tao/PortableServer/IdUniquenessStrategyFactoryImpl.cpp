// $Id$

#include "IdUniquenessStrategyFactoryImpl.h"
#include "IdUniquenessStrategy.h"
#include "IdUniquenessStrategyUnique.h"
#include "IdUniquenessStrategyMultiple.h"
#include "ace/Dynamic_Service.h"

ACE_RCSID (PortableServer,
           IdUniquenessStrategyFactoryImpl,
           "$Id$")

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
          strategy =
            ACE_Dynamic_Service<IdUniquenessStrategy>::instance ("IdUniquenessStrategyMultiple");

          if (strategy == 0)
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("(%P|%t) %p\n"),
                        ACE_TEXT ("Unable to get ")
                        ACE_TEXT ("IdUniquenessStrategyMultiple")));

          break;
        }
        case ::PortableServer::UNIQUE_ID :
        {
          // @@Johnny, why is there a difference between these
          // multiple and unique.

/*          strategy =
            ACE_Dynamic_Service<IdUniquenessStrategy>::instance ("IdUniquenessStrategyUnique");

          if (strategy == 0)
            {
              ACE_Service_Config::process_directive (
                ACE_TEXT("dynamic IdUniquenessStrategyUnique Service_Object *")
                ACE_TEXT("TAO_PortableServer:_make_IdUniquenessStrategyUnique()"));

              strategy =
                ACE_Dynamic_Service<IdUniquenessStrategy>::instance ("IdUniquenessStrategyUnique");
            }*/
          ACE_NEW_RETURN (strategy, IdUniquenessStrategyUnique, 0);
          break;
        }
      }

      return strategy;
    }

    ACE_STATIC_SVC_DEFINE (
        IdUniquenessStrategyFactoryImpl,
        ACE_TEXT ("IdUniquenessStrategyFactory"),
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
