#ifndef TAO_CONDITION_CPP
#define TAO_CONDITION_CPP
#include "tao/Condition.h"
#include "tao/debug.h"

#if !defined (__ACE_INLINE__)
# include "tao/Condition.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template <class MUTEX>
TAO_Condition<MUTEX>::TAO_Condition (MUTEX &m)
  : mutex_ (&m)
{
  // @todo: Need to add the allocatore here..
  TAO_SYNCH_CONDITION *cond {};
  ACE_NEW (cond, TAO_SYNCH_CONDITION (*this->mutex_));
  this->cond_.reset (cond);
}

template <class MUTEX>
TAO_Condition<MUTEX>::TAO_Condition ()
    : mutex_ (nullptr)
{
  // @todo: Need to add the allocatore here..
  MUTEX *mutex {};
  ACE_NEW (mutex, MUTEX);
  this->owned_mutex_.reset (mutex);
  this->mutex_ = this->owned_mutex_.get ();

  TAO_SYNCH_CONDITION *cond {};
  ACE_NEW (cond, TAO_SYNCH_CONDITION (*this->mutex_));
  this->cond_.reset (cond);
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
