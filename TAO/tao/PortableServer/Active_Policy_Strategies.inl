// -*- C++ -*-
TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    ACE_INLINE
    ThreadStrategy*
    Active_Policy_Strategies::thread_strategy () const
    {
      return this->thread_strategy_.get ();
    }

    ACE_INLINE
    RequestProcessingStrategy*
    Active_Policy_Strategies::request_processing_strategy () const
    {
      return this->request_processing_strategy_.get ();
    }

    ACE_INLINE
    IdAssignmentStrategy *
    Active_Policy_Strategies::id_assignment_strategy () const
    {
      return this->id_assignment_strategy_. get ();
    }

    ACE_INLINE
    IdUniquenessStrategy *
    Active_Policy_Strategies::id_uniqueness_strategy () const
    {
      return this->id_uniqueness_strategy_.get ();
    }

    ACE_INLINE
    LifespanStrategy*
    Active_Policy_Strategies::lifespan_strategy () const
    {
      return this->lifespan_strategy_. get();
    }

    ACE_INLINE
    ImplicitActivationStrategy*
    Active_Policy_Strategies::implicit_activation_strategy () const
    {
      return this->implicit_activation_strategy_.get ();
    }

    ACE_INLINE
    ServantRetentionStrategy*
    Active_Policy_Strategies::servant_retention_strategy () const
    {
      return this->servant_retention_strategy_.get ();
    }

    ACE_INLINE
    Active_Policy_Strategies_Cleanup_Guard
      ::Active_Policy_Strategies_Cleanup_Guard (Active_Policy_Strategies *p)
      : ptr_ (p)
    {
    }

    ACE_INLINE
    Active_Policy_Strategies_Cleanup_Guard::~Active_Policy_Strategies_Cleanup_Guard ()
    {
      if (this->ptr_)
        {
          this->ptr_->cleanup ();
        }
    }

    ACE_INLINE
    Active_Policy_Strategies *
    Active_Policy_Strategies_Cleanup_Guard::_retn ()
    {
      Active_Policy_Strategies *temp = this->ptr_;
      this->ptr_ = nullptr;
      return temp;
    }
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL
