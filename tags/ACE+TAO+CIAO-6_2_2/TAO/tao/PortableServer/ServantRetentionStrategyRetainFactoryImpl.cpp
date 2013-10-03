// -*- C++ -*-
// $Id$

#include "tao/PortableServer/ServantRetentionStrategyRetainFactoryImpl.h"
#include "tao/PortableServer/ServantRetentionStrategy.h"
#include "tao/PortableServer/ServantRetentionStrategyRetain.h"
#include "ace/Dynamic_Service.h"
#include "ace/Log_Msg.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    ServantRetentionStrategy*
    ServantRetentionStrategyRetainFactoryImpl::create (
      ::PortableServer::ServantRetentionPolicyValue value)
    {
      ServantRetentionStrategy* strategy = 0;

      switch (value)
      {
        case ::PortableServer::RETAIN :
        {
          ACE_NEW_RETURN (strategy, ServantRetentionStrategyRetain, 0);
          break;
        }
        case ::PortableServer::NON_RETAIN :
        {
          TAOLIB_ERROR ((LM_ERROR, "Incorrect type in ServantRetentionStrategyNonRetainFactoryImpl"));
          break;
        }
      }

      return strategy;
    }

    void
    ServantRetentionStrategyRetainFactoryImpl::destroy (
      ServantRetentionStrategy *strategy
      )
    {
      strategy->strategy_cleanup ();

      delete strategy;
    }
  }
}


ACE_STATIC_SVC_DEFINE (
  ServantRetentionStrategyRetainFactoryImpl,
  ACE_TEXT ("ServantRetentionStrategyRetainFactory"),
  ACE_SVC_OBJ_T,
  &ACE_SVC_NAME (ServantRetentionStrategyRetainFactoryImpl),
  ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
  0)

ACE_FACTORY_NAMESPACE_DEFINE (
  ACE_Local_Service,
  ServantRetentionStrategyRetainFactoryImpl,
  TAO::Portable_Server::ServantRetentionStrategyRetainFactoryImpl)
TAO_END_VERSIONED_NAMESPACE_DECL
