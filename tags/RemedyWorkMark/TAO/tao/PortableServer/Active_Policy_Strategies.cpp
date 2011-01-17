// -*- C++ -*-
// $Id$

#include "tao/PortableServer/Active_Policy_Strategies.h"
#include "tao/PortableServer/POA_Cached_Policies.h"

#include "tao/PortableServer/ThreadStrategy.h"
#include "tao/PortableServer/ThreadStrategyFactory.h"
#include "tao/PortableServer/RequestProcessingStrategy.h"
#include "tao/PortableServer/RequestProcessingStrategyFactory.h"
#include "tao/PortableServer/IdAssignmentStrategy.h"
#include "tao/PortableServer/IdAssignmentStrategyFactory.h"
#include "tao/PortableServer/LifespanStrategy.h"
#include "tao/PortableServer/LifespanStrategyFactory.h"
#include "tao/PortableServer/IdUniquenessStrategy.h"
#include "tao/PortableServer/IdUniquenessStrategyFactory.h"
#include "tao/PortableServer/ImplicitActivationStrategy.h"
#include "tao/PortableServer/ImplicitActivationStrategyFactory.h"
#include "tao/PortableServer/ServantRetentionStrategy.h"
#include "tao/PortableServer/ServantRetentionStrategyFactory.h"

#include "tao/PortableServer/IdAssignmentPolicyC.h"
#include "tao/PortableServer/IdUniquenessPolicyC.h"
#include "tao/PortableServer/ImplicitActivationPolicyC.h"
#include "tao/PortableServer/LifespanPolicyC.h"
#include "tao/PortableServer/RequestProcessingPolicyC.h"
#include "tao/PortableServer/ServantRetentionPolicyC.h"
#include "tao/PortableServer/ThreadPolicyC.h"
#include "tao/PortableServer/ServantRetentionPolicyC.h"

#include "ace/Dynamic_Service.h"

#if !defined (__ACE_INLINE__)
# include "tao/PortableServer/Active_Policy_Strategies.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    Active_Policy_Strategies::Active_Policy_Strategies (void)
      : thread_strategy_ (0),
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
                                      ::TAO_Root_POA *poa)
    {
      this->thread_strategy_factory_ =
        ACE_Dynamic_Service<ThreadStrategyFactory>::instance ("ThreadStrategyFactory");

      if (this->thread_strategy_factory_ != 0)
        this->thread_strategy_ =
          this->thread_strategy_factory_->create (policies.thread());

      /**/

      this->id_assignment_strategy_factory_ =
        ACE_Dynamic_Service<IdAssignmentStrategyFactory>::instance ("IdAssignmentStrategyFactory");

      if (this->id_assignment_strategy_factory_ != 0)
        this->id_assignment_strategy_ =
          this->id_assignment_strategy_factory_->create (policies.id_assignment());

      /**/

      this->id_uniqueness_strategy_factory_ =
        ACE_Dynamic_Service<IdUniquenessStrategyFactory>::instance ("IdUniquenessStrategyFactory");

      if (this->id_uniqueness_strategy_factory_ != 0)
        this->id_uniqueness_strategy_ =
          this->id_uniqueness_strategy_factory_->create (policies.id_uniqueness());

      /**/

      this->servant_retention_strategy_factory_ =
        ACE_Dynamic_Service<ServantRetentionStrategyFactory>::instance ("ServantRetentionStrategyFactory");

      if (this->servant_retention_strategy_factory_ != 0)
        this->servant_retention_strategy_ =
          this->servant_retention_strategy_factory_->create (policies.servant_retention());

      /**/

      this->request_processing_strategy_factory_ =
        ACE_Dynamic_Service<RequestProcessingStrategyFactory>::instance ("RequestProcessingStrategyFactory");

      if (this->request_processing_strategy_factory_ != 0)
        this->request_processing_strategy_ =
          this->request_processing_strategy_factory_->create (policies.request_processing(), policies.servant_retention());

      /**/

      this->lifespan_strategy_factory_ =
        ACE_Dynamic_Service<LifespanStrategyFactory>::instance ("LifespanStrategyFactory");

      if (this->lifespan_strategy_factory_ != 0)
        this->lifespan_strategy_ =
          this->lifespan_strategy_factory_->create (policies.lifespan());

      /**/

      this->implicit_activation_strategy_factory_ =
        ACE_Dynamic_Service<ImplicitActivationStrategyFactory>::instance ("ImplicitActivationStrategyFactory");

      if (this->implicit_activation_strategy_factory_ != 0)
        this->implicit_activation_strategy_ =
          this->implicit_activation_strategy_factory_->create (policies.implicit_activation());

      /**/

// @todo, check if all pointers are != 0

      if (this->lifespan_strategy_ != 0)
        {
          this->lifespan_strategy_->strategy_init (poa);
        }

      if (this->request_processing_strategy_ != 0)
        {
          this->request_processing_strategy_->strategy_init (poa, policies.servant_retention());
        }

      if (this->id_uniqueness_strategy_ != 0)
        {
          this->id_uniqueness_strategy_->strategy_init (poa);
        }

      if (this->implicit_activation_strategy_ != 0)
        {
          this->implicit_activation_strategy_->strategy_init (poa);
        }

      if (this->thread_strategy_ != 0)
        {
          this->thread_strategy_->strategy_init (poa);
        }

      if (this->servant_retention_strategy_ != 0)
        {
          this->servant_retention_strategy_->strategy_init (poa);
        }

      if (this->id_assignment_strategy_ != 0)
        {
          this->id_assignment_strategy_->strategy_init (poa);
        }
    }

    void
    Active_Policy_Strategies::cleanup (void)
    {

      if (this->lifespan_strategy_ != 0)
        {
          this->lifespan_strategy_factory_->destroy (lifespan_strategy_);

          this->lifespan_strategy_ = 0;
        }

      if (this->request_processing_strategy_ != 0)
        {
          this->request_processing_strategy_factory_->destroy (request_processing_strategy_);

          this->request_processing_strategy_ = 0;
        }

      if (this->id_uniqueness_strategy_ != 0)
        {
          this->id_uniqueness_strategy_factory_->destroy (id_uniqueness_strategy_);

          this->id_uniqueness_strategy_ = 0;
        }

      if (this->implicit_activation_strategy_ != 0)
        {
          this->implicit_activation_strategy_factory_->destroy (implicit_activation_strategy_);

          this->implicit_activation_strategy_ = 0;
        }

      if (this->thread_strategy_ != 0)
        {
          this->thread_strategy_factory_->destroy (thread_strategy_);

          this->thread_strategy_ = 0;
        }

      if (this->servant_retention_strategy_ != 0)
        {
          this->servant_retention_strategy_factory_->destroy (servant_retention_strategy_);

          this->servant_retention_strategy_ = 0;
        }

      if (this->id_assignment_strategy_ != 0)
        {
          this->id_assignment_strategy_factory_->destroy (id_assignment_strategy_);

          this->id_assignment_strategy_ = 0;
        }
    }
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL
