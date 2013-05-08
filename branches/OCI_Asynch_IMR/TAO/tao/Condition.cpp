// $Id$

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

  : mutex_ (&m),
    delete_lock_ (false),
    cond_ (0)
{
  // @todo: Need to add the allocatore here..
  ACE_NEW (this->cond_,
           TAO_SYNCH_CONDITION (*this->mutex_));
}

template <class MUTEX>
TAO_Condition<MUTEX>::TAO_Condition (void)
    : mutex_ (0),
      delete_lock_ (false),
      cond_ (0)

{
  // @todo: Need to add the allocatore here..

  ACE_NEW (this->mutex_,
           MUTEX);

  this->delete_lock_ = true;

  ACE_NEW (this->cond_,
           TAO_SYNCH_CONDITION (*this->mutex_));
}


template <class MUTEX>
TAO_Condition<MUTEX>::~TAO_Condition (void)
{
  if (this->remove () == -1)
    TAOLIB_ERROR ((LM_ERROR,
                ACE_TEXT ("%p\n"),
                ACE_TEXT ("TAO_Condition::~TAO_Condition")));

  delete this->cond_;

  if (this->delete_lock_)
    delete this->mutex_;
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif  /* TAO_CONDITION_CPP */
