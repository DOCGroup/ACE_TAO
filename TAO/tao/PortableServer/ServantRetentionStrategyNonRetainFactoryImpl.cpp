// $Id$

#include "tao/orbconf.h"

#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)

#include "tao/PortableServer/ServantRetentionStrategyNonRetainFactoryImpl.h"
#include "tao/PortableServer/ServantRetentionStrategy.h"
#include "tao/PortableServer/ServantRetentionStrategyNonRetain.h"
#include "ace/Dynamic_Service.h"
#include "ace/Log_Msg.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    ServantRetentionStrategy*
    ServantRetentionStrategyNonRetainFactoryImpl::create (
      ::PortableServer::ServantRetentionPolicyValue value)
    {
      ServantRetentionStrategy* strategy = 0;

      switch (value)
      {
        case ::PortableServer::NON_RETAIN :
        {
          ACE_NEW_RETURN (strategy, ServantRetentionStrategyNonRetain, 0);
          break;
        }
        case ::PortableServer::RETAIN :
        {
          TAOLIB_ERROR ((LM_ERROR, "Incorrect type in ServantRetentionStrategyNonRetainFactoryImpl"));
          break;
        }
      }

      return strategy;
    }

    void
    ServantRetentionStrategyNonRetainFactoryImpl::destroy (
      ServantRetentionStrategy *strategy
      )
    {
      strategy->strategy_cleanup ();

      delete strategy;
    }
  }
}


ACE_STATIC_SVC_DEFINE (
  ServantRetentionStrategyNonRetainFactoryImpl,
  ACE_TEXT ("ServantRetentionStrategyNonRetainFactory"),
  ACE_SVC_OBJ_T,
  &ACE_SVC_NAME (ServantRetentionStrategyNonRetainFactoryImpl),
  ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
  0)

ACE_FACTORY_NAMESPACE_DEFINE (
  ACE_Local_Service,
  ServantRetentionStrategyNonRetainFactoryImpl,
  TAO::Portable_Server::ServantRetentionStrategyNonRetainFactoryImpl)
TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_MINIMUM_POA == 0 && !CORBA_E_COMPACT && !CORBA_E_MICRO */
