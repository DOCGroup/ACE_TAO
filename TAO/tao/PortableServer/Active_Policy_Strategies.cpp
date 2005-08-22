// @(#) $Id$

#include "Active_Policy_Strategies.h"
#include "POA_Cached_Policies.h"

#include "ThreadStrategy.h"
#include "ThreadStrategyFactory.h"
#include "RequestProcessingStrategy.h"
#include "RequestProcessingStrategyFactory.h"
#include "IdAssignmentStrategy.h"
#include "IdAssignmentStrategyFactory.h"
#include "LifespanStrategy.h"
#include "LifespanStrategyFactory.h"
#include "IdUniquenessStrategy.h"
#include "IdUniquenessStrategyFactory.h"
#include "ImplicitActivationStrategy.h"
#include "ImplicitActivationStrategyFactory.h"
#include "ServantRetentionStrategy.h"
#include "ServantRetentionStrategyFactory.h"

#include "IdAssignmentPolicyC.h"
#include "IdUniquenessPolicyC.h"
#include "ImplicitActivationPolicyC.h"
#include "LifespanPolicyC.h"
#include "RequestProcessingPolicyC.h"
#include "ServantRetentionPolicyC.h"
#include "ThreadPolicyC.h"
#include "ServantRetentionPolicyC.h"

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
      servant_retention_strategy_ (0),
      thread_strategy_factory_ (0),
      servant_retention_strategy_factory_ (0),
      request_processing_strategy_factory_ (0),
      lifespan_strategy_factory_ (0),
      implicit_activation_strategy_factory_ (0),
      id_uniqueness_strategy_factory_ (0),
      id_assignment_strategy_factory_ (0)
    {
    }

    void
    Active_Policy_Strategies::update (Cached_Policies &policies,
                                      ::TAO_Root_POA *poa
                                      ACE_ENV_ARG_DECL)
    {
      thread_strategy_factory_ =
        ACE_Dynamic_Service<ThreadStrategyFactory>::instance ("ThreadStrategyFactory");

      if (thread_strategy_factory_ != 0)
        thread_strategy_ = thread_strategy_factory_->create (policies.thread());

      /**/

      id_assignment_strategy_factory_ =
        ACE_Dynamic_Service<IdAssignmentStrategyFactory>::instance ("IdAssignmentStrategyFactory");

      if (id_assignment_strategy_factory_ != 0)
        id_assignment_strategy_ = id_assignment_strategy_factory_->create (policies.id_assignment());

      /**/

      id_uniqueness_strategy_factory_ =
        ACE_Dynamic_Service<IdUniquenessStrategyFactory>::instance ("IdUniquenessStrategyFactory");

      if (id_uniqueness_strategy_factory_ != 0)
        id_uniqueness_strategy_ = id_uniqueness_strategy_factory_->create (policies.id_uniqueness());

      /**/

      servant_retention_strategy_factory_ =
        ACE_Dynamic_Service<ServantRetentionStrategyFactory>::instance ("ServantRetentionStrategyFactory");

      if (servant_retention_strategy_factory_ != 0)
        servant_retention_strategy_ =
          servant_retention_strategy_factory_->create (policies.servant_retention());

      /**/

      request_processing_strategy_factory_ =
        ACE_Dynamic_Service<RequestProcessingStrategyFactory>::instance ("RequestProcessingStrategyFactory");

      if (request_processing_strategy_factory_ != 0)
        request_processing_strategy_ = request_processing_strategy_factory_->create (policies.request_processing(), policies.servant_retention());

      /**/

      lifespan_strategy_factory_ =
        ACE_Dynamic_Service<LifespanStrategyFactory>::instance ("LifespanStrategyFactory");

      if (lifespan_strategy_factory_ != 0)
        lifespan_strategy_ = lifespan_strategy_factory_->create (policies.lifespan());

      /**/

      implicit_activation_strategy_factory_ =
        ACE_Dynamic_Service<ImplicitActivationStrategyFactory>::instance ("ImplicitActivationStrategyFactory");

      if (implicit_activation_strategy_factory_ != 0)
        implicit_activation_strategy_ = implicit_activation_strategy_factory_->create (policies.implicit_activation());

      /**/

// @todo, check if all pointers are != 0

      if (lifespan_strategy_ != 0)
        {
          lifespan_strategy_->strategy_init (poa ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;
        }

      if (request_processing_strategy_ != 0)
        {
          request_processing_strategy_->strategy_init (poa, policies.servant_retention() ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;
        }

      if (id_uniqueness_strategy_ != 0)
        {
          id_uniqueness_strategy_->strategy_init (poa ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;
        }

      if (implicit_activation_strategy_ != 0)
        {
          implicit_activation_strategy_->strategy_init (poa ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;
        }

      if (thread_strategy_ != 0)
        {
          thread_strategy_->strategy_init (poa ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;
        }

      if (servant_retention_strategy_ != 0)
        {
          servant_retention_strategy_->strategy_init (poa ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;
        }

      if (id_assignment_strategy_ != 0)
        {
          id_assignment_strategy_->strategy_init (poa ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;
        }
    }

    void
    Active_Policy_Strategies::cleanup (ACE_ENV_SINGLE_ARG_DECL)
    {

      if (lifespan_strategy_ != 0)
        {
          lifespan_strategy_factory_->destroy (lifespan_strategy_ ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;

          lifespan_strategy_ = 0;
        }

      if (request_processing_strategy_ != 0)
        {
          request_processing_strategy_factory_->destroy (request_processing_strategy_ ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;

          request_processing_strategy_ = 0;
        }

      if (id_uniqueness_strategy_ != 0)
        {
          id_uniqueness_strategy_factory_->destroy (id_uniqueness_strategy_ ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;

          id_uniqueness_strategy_ = 0;
        }

      if (implicit_activation_strategy_ != 0)
        {
          implicit_activation_strategy_factory_->destroy (implicit_activation_strategy_ ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;

          implicit_activation_strategy_ = 0;
        }

      if (thread_strategy_ != 0)
        {
          thread_strategy_factory_->destroy (thread_strategy_ ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;

          thread_strategy_ = 0;
        }

      if (servant_retention_strategy_ != 0)
        {
          servant_retention_strategy_factory_->destroy (servant_retention_strategy_ ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;

          servant_retention_strategy_ = 0;
        }

      if (id_assignment_strategy_ != 0)
        {
          id_assignment_strategy_factory_->destroy (id_assignment_strategy_ ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;

          id_assignment_strategy_ = 0;
        }
    }
  }
}
