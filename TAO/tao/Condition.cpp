#include "Condition.h"


#if !defined (__ACE_INLINE__)
# include "tao/Condition.inl"
#endif /* __ACE_INLINE__ */


ACE_RCSID (TAO,
           Condition,
           "$Id$")

template <class MUTEX>
TAO_Condition<MUTEX>::TAO_Condition (MUTEX &m)

  : mutex_ (&m),
    delete_lock_ (0),
    cond_ (0)
{
  // @@todo: Need to add the allocatore here..
  ACE_NEW (this->cond_,
           TAO_SYNCH_CONDITION (*this->mutex_));
}

template <class MUTEX>
TAO_Condition<MUTEX>::TAO_Condition (void)
    : mutex_ (0),
      delete_lock_ (0),
      cond_ (0)

{
  // @@todo: Need to add the allocatore here..

  ACE_NEW (this->mutex_,
           MUTEX);

  this->delete_lock_ = 1;

  ACE_NEW (this->cond_,
           TAO_SYNCH_CONDITION (*this->mutex_));
}


template <class MUTEX>
TAO_Condition<MUTEX>::~TAO_Condition (void)
{
  if (this->remove () == -1)
    ACE_ERROR ((LM_ERROR,
                ACE_LIB_TEXT ("%p\n"),
                ACE_LIB_TEXT ("TAO_Condition::~TAO_Condition")));

  delete this->cond_;

  if (this->delete_lock_)
    delete this->mutex_;
}
