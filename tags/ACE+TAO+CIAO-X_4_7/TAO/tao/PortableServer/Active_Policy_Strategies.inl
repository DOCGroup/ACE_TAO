// @(#) $Id$

namespace TAO
{
  namespace Portable_Server
  {
    ACE_INLINE
    ThreadStrategy*
    Active_Policy_Strategies::thread_strategy (void) const
    {
      return thread_strategy_;
    }

    ACE_INLINE
    RequestProcessingStrategy*
    Active_Policy_Strategies::request_processing_strategy (void) const
    {
      return request_processing_strategy_;
    }

    ACE_INLINE
    IdAssignmentStrategy *
    Active_Policy_Strategies::id_assignment_strategy (void) const
    {
      return id_assignment_strategy_;
    }

    ACE_INLINE
    IdUniquenessStrategy *
    Active_Policy_Strategies::id_uniqueness_strategy (void) const
    {
      return id_uniqueness_strategy_;
    }

    ACE_INLINE
    LifespanStrategy*
    Active_Policy_Strategies::lifespan_strategy (void) const
    {
      return lifespan_strategy_;
    }

    ACE_INLINE
    ImplicitActivationStrategy*
    Active_Policy_Strategies::implicit_activation_strategy (void) const
    {
      return implicit_activation_strategy_;
    }

    ACE_INLINE
    ServantRetentionStrategy*
    Active_Policy_Strategies::servant_retention_strategy (void) const
    {
      return servant_retention_strategy_;
    }
  }
}
