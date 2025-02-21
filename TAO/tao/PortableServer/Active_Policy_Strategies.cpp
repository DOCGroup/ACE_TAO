// -*- C++ -*-
#include "tao/PortableServer/Active_Policy_Strategies.h"
#include "tao/PortableServer/POA_Cached_Policies.h"

#include "tao/PortableServer/ThreadStrategySingle.h"
#include "tao/PortableServer/ThreadStrategyORBControl.h"
#include "tao/PortableServer/IdAssignmentStrategySystem.h"
#include "tao/PortableServer/IdAssignmentStrategyUser.h"
#include "tao/PortableServer/IdUniquenessStrategyMultiple.h"
#include "tao/PortableServer/IdUniquenessStrategyUnique.h"
#include "tao/PortableServer/ImplicitActivationStrategyExplicit.h"
#include "tao/PortableServer/ImplicitActivationStrategyImplicit.h"
#include "tao/PortableServer/LifespanStrategyPersistent.h"
#include "tao/PortableServer/LifespanStrategyTransient.h"
#include "tao/PortableServer/RequestProcessingStrategyAOMOnly.h"
#include "tao/PortableServer/RequestProcessingStrategyDefaultServant.h"
#include "tao/PortableServer/RequestProcessingStrategyServantLocator.h"
#include "tao/PortableServer/RequestProcessingStrategyServantActivator.h"
#include "tao/PortableServer/ServantRetentionStrategyNonRetain.h"
#include "tao/PortableServer/ServantRetentionStrategyRetain.h"

#if !defined (__ACE_INLINE__)
# include "tao/PortableServer/Active_Policy_Strategies.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    void
    Active_Policy_Strategies::update (Cached_Policies &policies,
                                      ::TAO_Root_POA *poa)
    {
      this->create (policies.thread());
      this->create (policies.id_assignment());
      this->create (policies.id_uniqueness());
      this->create (policies.servant_retention());
      this->create (policies.lifespan());
      this->create (policies.implicit_activation());
      this->create (policies.request_processing(), policies.servant_retention());

      /**/
      if (this->lifespan_strategy_)
        {
          this->lifespan_strategy_->strategy_init (poa);
        }

      if (this->request_processing_strategy_)
        {
          this->request_processing_strategy_->strategy_init (poa);
        }

      if (this->id_uniqueness_strategy_)
        {
          this->id_uniqueness_strategy_->strategy_init (poa);
        }

      if (this->servant_retention_strategy_)
        {
          this->servant_retention_strategy_->strategy_init (poa);
        }
    }

    void
    Active_Policy_Strategies::cleanup ()
    {
      if (this->lifespan_strategy_)
        {
          this->lifespan_strategy_->strategy_cleanup ();
        }
      this->lifespan_strategy_.reset (nullptr);
      if (this->request_processing_strategy_)
        {
          this->request_processing_strategy_->strategy_cleanup ();
        }
      this->request_processing_strategy_.reset (nullptr);
      if (this->id_uniqueness_strategy_)
        {
          this->id_uniqueness_strategy_->strategy_cleanup ();
        }
      this->id_uniqueness_strategy_.reset (nullptr);
      this->implicit_activation_strategy_.reset (nullptr);
      this->thread_strategy_.reset (nullptr);
      if (this->servant_retention_strategy_)
        {
          this->servant_retention_strategy_->strategy_cleanup ();
        }
      this->servant_retention_strategy_.reset (nullptr);
      this->id_assignment_strategy_.reset (nullptr);
    }

    void
    Active_Policy_Strategies::create (::PortableServer::ThreadPolicyValue value)
    {
      switch (value)
      {
        case ::PortableServer::SINGLE_THREAD_MODEL :
        {
#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_MICRO) && !defined (CORBA_E_COMPACT)
          this->thread_strategy_ = std::make_unique<ThreadStrategySingle> ();
#endif /* TAO_HAS_MINIMUM_POA == 0 */
          break;
        }
        case ::PortableServer::ORB_CTRL_MODEL :
        {
          this->thread_strategy_ = std::make_unique<ThreadStrategyORBControl> ();
          break;
        }
      }
    }

    void
    Active_Policy_Strategies::create (::PortableServer::IdAssignmentPolicyValue value)
    {
      switch (value)
      {
        case ::PortableServer::SYSTEM_ID :
        {
          this->id_assignment_strategy_ = std::make_unique<IdAssignmentStrategySystem> ();
          break;
        }
        case ::PortableServer::USER_ID :
        {
#if !defined (CORBA_E_MICRO)
          this->id_assignment_strategy_ = std::make_unique<IdAssignmentStrategyUser> ();
#endif /* CORBA_E_MICRO */
          break;
        }
      }
    }

    void
    Active_Policy_Strategies::create (::PortableServer::IdUniquenessPolicyValue value)
    {
      switch (value)
      {
        case ::PortableServer::MULTIPLE_ID :
        {
#if !defined (CORBA_E_MICRO)
          this->id_uniqueness_strategy_ = std::make_unique<IdUniquenessStrategyMultiple> ();
#endif /* CORBA_E_MICRO */
          break;
        }
        case ::PortableServer::UNIQUE_ID :
        {
          this->id_uniqueness_strategy_ = std::make_unique<IdUniquenessStrategyUnique> ();
          break;
        }
      }
    }

    void
    Active_Policy_Strategies::create (::PortableServer::ServantRetentionPolicyValue value)
    {
      switch (value)
      {
        case ::PortableServer::RETAIN :
        {
          this->servant_retention_strategy_ = std::make_unique<ServantRetentionStrategyRetain> ();
          break;
        }
        case ::PortableServer::NON_RETAIN :
        {
#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_MICRO) && !defined (CORBA_E_COMPACT)
          this->servant_retention_strategy_ = std::make_unique<ServantRetentionStrategyNonRetain> ();
#endif /* TAO_HAS_MINIMUM_POA == 0 */
          break;
        }
      }
    }

    void
    Active_Policy_Strategies::create (::PortableServer::LifespanPolicyValue value)
    {
      switch (value)
      {
        case ::PortableServer::PERSISTENT :
        {
#if !defined (CORBA_E_MICRO)
          this->lifespan_strategy_ = std::make_unique<LifespanStrategyPersistent> ();
#endif /* CORBA_E_MICRO */
          break;
        }
        case ::PortableServer::TRANSIENT :
        {
          this->lifespan_strategy_ = std::make_unique<LifespanStrategyTransient> ();
          break;
        }
      }
    }

    void
    Active_Policy_Strategies::create (::PortableServer::ImplicitActivationPolicyValue value)
    {
      switch (value)
      {
        case ::PortableServer::IMPLICIT_ACTIVATION :
        {
#if !defined (CORBA_E_MICRO) && !defined (CORBA_E_COMPACT)
          this->implicit_activation_strategy_= std::make_unique<ImplicitActivationStrategyImplicit> ();
#endif /* CORBA_E_MICRO */
          break;
        }
        case ::PortableServer::NO_IMPLICIT_ACTIVATION :
        {
          this->implicit_activation_strategy_ = std::make_unique<ImplicitActivationStrategyExplicit> ();
          break;
        }
      }
    }

    void
    Active_Policy_Strategies::create (
        ::PortableServer::RequestProcessingPolicyValue value,
        ::PortableServer::ServantRetentionPolicyValue srvalue)
    {
      switch (value)
      {
        case ::PortableServer::USE_ACTIVE_OBJECT_MAP_ONLY :
        {
          this->request_processing_strategy_ = std::make_unique<RequestProcessingStrategyAOMOnly> ();
          break;
        }
        case ::PortableServer::USE_DEFAULT_SERVANT :
        {
#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)
          this->request_processing_strategy_ = std::make_unique<RequestProcessingStrategyDefaultServant> ();
#endif /* TAO_HAS_MINIMUM_POA == 0 */
          break;
        }
        case ::PortableServer::USE_SERVANT_MANAGER :
        {
          switch (srvalue)
          {
            case ::PortableServer::RETAIN :
            {
#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)
              this->request_processing_strategy_ = std::make_unique<RequestProcessingStrategyServantActivator> ();
#endif /* TAO_HAS_MINIMUM_POA == 0 */
              break;
            }
            case ::PortableServer::NON_RETAIN :
            {
#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)
              this->request_processing_strategy_ = std::make_unique<RequestProcessingStrategyServantLocator> ();
#endif /* TAO_HAS_MINIMUM_POA == 0 */
              break;
            }
          }
          break;
        }
      }
    }
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL
