// -*- C++ -*-
// $Id$

#include "tao/PortableServer/LifespanStrategyTransientFactoryImpl.h"
#include "ace/Dynamic_Service.h"
#include "tao/PortableServer/LifespanStrategyTransient.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    LifespanStrategy*
    LifespanStrategyTransientFactoryImpl::create (
      ::PortableServer::LifespanPolicyValue value)
    {
      LifespanStrategy* strategy = 0;

      switch (value)
      {
        case ::PortableServer::PERSISTENT :
        {
          TAOLIB_ERROR ((LM_ERROR, "Incorrect type in LifespanStrategyTransientFactoryImpl"));
          break;
        }
        case ::PortableServer::TRANSIENT :
        {
          ACE_NEW_RETURN (strategy, LifespanStrategyTransient, 0);
          break;
        }
      }

      return strategy;
    }

    void
    LifespanStrategyTransientFactoryImpl::destroy (LifespanStrategy *strategy)
    {
      strategy->strategy_cleanup ();

      delete strategy;
    }
  }
}


ACE_STATIC_SVC_DEFINE (
  LifespanStrategyTransientFactoryImpl,
  ACE_TEXT ("LifespanStrategyTransientFactory"),
  ACE_SVC_OBJ_T,
  &ACE_SVC_NAME (LifespanStrategyTransientFactoryImpl),
  ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
  0)

ACE_FACTORY_NAMESPACE_DEFINE (
  ACE_Local_Service,
  LifespanStrategyTransientFactoryImpl,
  TAO::Portable_Server::LifespanStrategyTransientFactoryImpl)

TAO_END_VERSIONED_NAMESPACE_DECL

