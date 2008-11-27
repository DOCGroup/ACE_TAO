// -*- C++ -*-

//=============================================================================
/**
 *  @file Active_Policy_Strategies.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_PORTABLESERVER_ACTIVE_POLICY_STRATEGIES_H
#define TAO_PORTABLESERVER_ACTIVE_POLICY_STRATEGIES_H

#include /**/ "ace/pre.h"

#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Root_POA;

namespace TAO
{
  namespace Portable_Server
  {
    class Cached_Policies;
    class ThreadStrategy;
    class RequestProcessingStrategy;
    class IdAssignmentStrategy;
    class LifespanStrategy;
    class IdUniquenessStrategy;
    class ImplicitActivationStrategy;
    class ServantRetentionStrategy;

    class ThreadStrategyFactory;
    class ServantRetentionStrategyFactory;
    class RequestProcessingStrategyFactory;
    class LifespanStrategyFactory;
    class ImplicitActivationStrategyFactory;
    class IdUniquenessStrategyFactory;
    class IdAssignmentStrategyFactory;

    /**
     * This class stores the active policy strategies used for a certain POA.
     */
    class Active_Policy_Strategies
    {
    public:
      Active_Policy_Strategies (void);

      void update (Cached_Policies &policies,
                   TAO_Root_POA* poa
                  );

      void cleanup (void);

      ThreadStrategy *thread_strategy (void) const;

      RequestProcessingStrategy *request_processing_strategy (void) const;

      IdAssignmentStrategy *id_assignment_strategy (void) const;

      IdUniquenessStrategy *id_uniqueness_strategy (void) const;

      LifespanStrategy *lifespan_strategy (void) const;

      ImplicitActivationStrategy *implicit_activation_strategy (void) const;

      ServantRetentionStrategy *servant_retention_strategy (void) const;

    private:
      ThreadStrategy *thread_strategy_;
      RequestProcessingStrategy *request_processing_strategy_;
      IdAssignmentStrategy *id_assignment_strategy_;
      LifespanStrategy *lifespan_strategy_;
      IdUniquenessStrategy *id_uniqueness_strategy_;
      ImplicitActivationStrategy *implicit_activation_strategy_;
      ServantRetentionStrategy *servant_retention_strategy_;

      ThreadStrategyFactory *thread_strategy_factory_;
      ServantRetentionStrategyFactory *servant_retention_strategy_factory_;
      RequestProcessingStrategyFactory *request_processing_strategy_factory_;
      LifespanStrategyFactory *lifespan_strategy_factory_;
      ImplicitActivationStrategyFactory *implicit_activation_strategy_factory_;
      IdUniquenessStrategyFactory *id_uniqueness_strategy_factory_;
      IdAssignmentStrategyFactory *id_assignment_strategy_factory_;
    };

    /**
     * This class quards the cleanup of strategies if something went wrong
     * in the code that called Active_Policy_Strategies::update().
     */
    class Active_Policy_Strategies_Cleanup_Guard
    {
    public:
      Active_Policy_Strategies_Cleanup_Guard (Active_Policy_Strategies *p);
      ~Active_Policy_Strategies_Cleanup_Guard (void);

      Active_Policy_Strategies *_retn (void);

    private:
      Active_Policy_Strategies *ptr_;
    };
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "tao/PortableServer/Active_Policy_Strategies.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_PORTABLESERVER_ACTIVE_POLICY_STRATEGIES_H */
