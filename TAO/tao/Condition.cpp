#ifndef TAO_CONDITION_CPP
#define TAO_CONDITION_CPP
#include "tao/Condition.h"
#include "tao/debug.h"

#include <memory>

#if !defined (__ACE_INLINE__)
# include "tao/Condition.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template <class MUTEX>
TAO_Condition<MUTEX>::TAO_Condition (MUTEX &m)
  : mutex_ (std::addressof (m))
{
  // @todo: Need to add the allocatore here..
  this->cond_ = std::make_unique<TAO_SYNCH_CONDITION> (*this->mutex_);
}

template <class MUTEX>
TAO_Condition<MUTEX>::TAO_Condition ()
    : mutex_ (nullptr)
{
  // @todo: Need to add the allocatore here..
  this->owned_mutex_ = std::make_unique<MUTEX> ();
  this->mutex_ = this->owned_mutex_.get ();

  this->cond_ = std::make_unique<TAO_SYNCH_CONDITION> (*this->mutex_);
}

template <class MUTEX>
TAO_Condition<MUTEX>::~TAO_Condition ()
{
  if (this->cond_ && this->remove () == -1)
    TAOLIB_ERROR ((LM_ERROR,
                ACE_TEXT ("%p\n"),
                ACE_TEXT ("TAO_Condition::~TAO_Condition")));
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif  /* TAO_CONDITION_CPP */
