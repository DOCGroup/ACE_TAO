// @(#) $Id$

#include "Active_Policy_Strategies.h"
#include "POA_Cached_Policies.h"
#include "IdAssignmentPolicyC.h"
#include "IdUniquenessPolicyC.h"
#include "ImplicitActivationPolicyC.h"
#include "LifespanPolicyC.h"
#include "RequestProcessingPolicyC.h"
#include "ServantRetentionPolicyC.h"
#include "ThreadPolicyC.h"
#include "ServantRetentionPolicyC.h"

#include "ThreadStrategy.h"
#include "ThreadStrategyFactory.h"
#include "RequestProcessingStrategy.h"
#include "IdAssignmentStrategy.h"
#include "LifespanStrategy.h"
#include "LifespanStrategyFactory.h"
#include "IdUniquenessStrategy.h"
#include "ImplicitActivationStrategy.h"
#include "ImplicitActivationStrategyFactory.h"
#include "ServantRetentionStrategy.h"
#include "ServantRetentionStrategyFactory.h"

#include "ace/Dynamic_Service.h"

#if !defined (__ACE_INLINE__)
# include "Active_Policy_Strategies.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(PortableServer,
          Active_Policy_Strategies,
          "$Id$")

namespace TAO
{
  namespace Portable_Server
  {
    Active_Policy_Strategies::Active_Policy_Strategies() :
      thread_strategy_ (0),
      request_processing_strategy_ (0),
      id_assignment_strategy_ (0),
      lifespan_strategy_ (0),
      id_uniqueness_strategy_ (0),
      implicit_activation_strategy_ (0),
      servant_retention_strategy_ (0)
    {
    }

    void
    Active_Policy_Strategies::update (Cached_Policies &policies,
                                      TAO_POA* poa
                                      ACE_ENV_ARG_DECL)
    {
      ThreadStrategyFactory *thread_strategy_factory =
        ACE_Dynamic_Service<ThreadStrategyFactory>::instance ("ThreadStrategyFactory");

      if (thread_strategy_factory == 0)
        {
          ACE_Service_Config::process_directive (ACE_TEXT("dynamic ThreadStrategyFactory Service_Object *")
                                                 ACE_TEXT("TAO_PortableServer:_make_ThreadStrategyFactoryImpl()"));
          thread_strategy_factory =
            ACE_Dynamic_Service<ThreadStrategyFactory>::instance ("ThreadStrategyFactory");
        }

      if (thread_strategy_factory != 0)
        thread_strategy_ = thread_strategy_factory->create (policies.thread());

      if (thread_strategy_ != 0)
        thread_strategy_->strategy_init (poa);

      /**/

      switch (policies.request_processing())
      {
        case ::PortableServer::USE_ACTIVE_OBJECT_MAP_ONLY :
        {
          ACE_NEW (request_processing_strategy_, AOM_Only_Request_Processing_Strategy);
          break;
        }
        case ::PortableServer::USE_DEFAULT_SERVANT :
        {
          ACE_NEW (request_processing_strategy_, Default_Servant_Request_Processing_Strategy);
          break;
        }
        case ::PortableServer::USE_SERVANT_MANAGER :
        {
          switch (policies.servant_retention())
          {
            case ::PortableServer::RETAIN :
            {
              ACE_NEW (request_processing_strategy_, Servant_Activator_Request_Processing_Strategy);
              break;
            }
            case ::PortableServer::NON_RETAIN :
            {
              ACE_NEW (request_processing_strategy_, Servant_Locator_Request_Processing_Strategy);
              break;
            }
          }
          break;
        }
      }

      request_processing_strategy_->strategy_init (poa);

      switch (policies.id_assignment())
      {
        case ::PortableServer::USER_ID :
        {
          ACE_NEW (id_assignment_strategy_, User_Id_Assignment_Strategy);
          break;
        }
        case ::PortableServer::SYSTEM_ID :
        {
          ACE_NEW (id_assignment_strategy_, System_Id_Assignment_Strategy);
          break;
        }
      }

      id_assignment_strategy_->strategy_init (poa);

      switch (policies.id_uniqueness())
      {
        case ::PortableServer::UNIQUE_ID :
        {
          ACE_NEW (id_uniqueness_strategy_, Unique_Id_Uniqueness_Strategy);
          break;
        }
        case ::PortableServer::MULTIPLE_ID :
        {
          ACE_NEW (id_uniqueness_strategy_, Multiple_Id_Uniqueness_Strategy);
          break;
        }
      }

      id_uniqueness_strategy_->strategy_init (poa);

      /**/

      LifespanStrategyFactory *lifespan_strategy_factory =
        ACE_Dynamic_Service<LifespanStrategyFactory>::instance ("LifespanStrategyFactory");

      if (lifespan_strategy_factory == 0)
        {
          ACE_Service_Config::process_directive (ACE_TEXT("dynamic LifespanStrategyFactory Service_Object *")
                                                 ACE_TEXT("TAO_PortableServer:_make_LifespanStrategyFactoryImpl()"));
          lifespan_strategy_factory =
            ACE_Dynamic_Service<LifespanStrategyFactory>::instance ("LifespanStrategyFactory");
        }

      if (lifespan_strategy_factory != 0)
        lifespan_strategy_ = lifespan_strategy_factory->create (policies.lifespan());

      if (lifespan_strategy_ != 0)
        lifespan_strategy_->strategy_init (poa);

      /**/

      ImplicitActivationStrategyFactory *implicit_activation_strategy_factory =
        ACE_Dynamic_Service<ImplicitActivationStrategyFactory>::instance ("ImplicitActivationStrategyFactory");

      if (implicit_activation_strategy_factory == 0)
        {
          ACE_Service_Config::process_directive (ACE_TEXT("dynamic ImplicitActivationStrategyFactory Service_Object *")
                                                 ACE_TEXT("TAO_PortableServer:_make_ImplicitActivationFactoryImpl()"));
          implicit_activation_strategy_factory =
            ACE_Dynamic_Service<ImplicitActivationStrategyFactory>::instance ("ImplicitActivationStrategyFactory");
        }

      if (implicit_activation_strategy_factory != 0)
        implicit_activation_strategy_ = implicit_activation_strategy_factory->create (policies.implicit_activation());

      if (implicit_activation_strategy_ != 0)
        implicit_activation_strategy_->strategy_init (poa);

      /**/

      ServantRetentionStrategyFactory *servant_retention_strategy_factory =
        ACE_Dynamic_Service<ServantRetentionStrategyFactory>::instance ("ServantRetentionStrategyFactory");

      if (servant_retention_strategy_factory == 0)
        {
          ACE_Service_Config::process_directive (ACE_TEXT("dynamic ServantRetentionStrategyFactory Service_Object *")
                                                 ACE_TEXT("TAO_PortableServer:_make_ServantRetentionStrategyFactoryImpl()"));
          servant_retention_strategy_factory =
            ACE_Dynamic_Service<ServantRetentionStrategyFactory>::instance ("ServantRetentionStrategyFactory");
        }


      if (servant_retention_strategy_factory != 0)
        servant_retention_strategy_ =
          servant_retention_strategy_factory->create (policies.servant_retention());

      if (servant_retention_strategy_ != 0)
      servant_retention_strategy_->strategy_init (poa);
    }
  }
}
