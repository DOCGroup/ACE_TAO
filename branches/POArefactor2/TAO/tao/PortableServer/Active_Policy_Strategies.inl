// @(#) $Id$

namespace TAO
{
  namespace Portable_Server
  {
    ACE_INLINE
    Thread_Strategy*
    Active_Policy_Strategies::thread_strategy (void) const
    {
      return thread_strategy_;
    }

    ACE_INLINE
    Request_Processing_Strategy*
    Active_Policy_Strategies::request_processing_strategy (void) const
    {
      return request_processing_strategy_;
    }

    ACE_INLINE
    Id_Assignment_Strategy *
    Active_Policy_Strategies::id_assignment_strategy (void) const
    {
      return id_assignment_strategy_;
    }

    ACE_INLINE
    Id_Uniqueness_Strategy *
    Active_Policy_Strategies::id_uniqueness_strategy (void) const
    {
      return id_uniqueness_strategy_;
    }

    ACE_INLINE
    Lifespan_Strategy*
    Active_Policy_Strategies::lifespan_strategy (void) const
    {
      return lifespan_strategy_;
    }

    ACE_INLINE
    Activation_Strategy*
    Active_Policy_Strategies::activation_strategy (void) const
    {
      return activation_strategy_;
    }

    ACE_INLINE
    ServantRetentionStrategy*
    Active_Policy_Strategies::servant_retention_strategy (void) const
    {
      return servant_retention_strategy_;
    }
  }
}
