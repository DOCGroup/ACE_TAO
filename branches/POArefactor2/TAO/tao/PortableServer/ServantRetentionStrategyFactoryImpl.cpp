// $Id$

#include "ServantRetentionStrategyFactoryImpl.h"
#include "ace/Dynamic_Service.h"
#include "ServantRetentionStrategy.h"
#include "Servant_Retention_Strategy.h"

ACE_RCSID (PortableServer,
           ServantRetentionStrategyFactoryImpl,
           "$Id$")

#if (TAO_HAS_MINIMUM_POA == 0)

namespace TAO
{
  namespace Portable_Server
  {
    ServantRetentionStrategyFactoryImpl::~ServantRetentionStrategyFactoryImpl (void)
    {
    }
    ServantRetentionStrategy*
    ServantRetentionStrategyFactoryImpl::create (
      ::PortableServer::ServantRetentionPolicyValue value)
    {
      ServantRetentionStrategy* strategy = 0;

      switch (value)
      {
        case ::PortableServer::RETAIN :
        {
          ACE_NEW_RETURN (strategy, Retain_Servant_Retention_Strategy, 0);
          break;
        }
        case ::PortableServer::NON_RETAIN :
        {
          ACE_NEW_RETURN (strategy, Non_Retain_Servant_Retention_Strategy, 0);
          break;
        }
      }

      return strategy;
    }

    ACE_STATIC_SVC_DEFINE (
        ServantRetentionStrategyFactoryImpl,
        ACE_TEXT ("ServantRetentionStrategyFactoryImpl"),
        ACE_SVC_OBJ_T,
        &ACE_SVC_NAME (ServantRetentionStrategyFactoryImpl),
        ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
        0
      )

    ACE_FACTORY_DEFINE (TAO_PortableServer, ServantRetentionStrategyFactoryImpl)

    #if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

    template class ACE_Dynamic_Service<ServantRetentionStrategyFactoryImpl>;

    #elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

    #pragma instantiate ACE_Dynamic_Service<ServantRetentionStrategyFactoryImpl>

    #endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */

