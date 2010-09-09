// $Id$

#include "tao/Utils/Synch_Refcountable.h"

#if !defined (__ACE_INLINE__)
# include "tao/Utils/Synch_Refcountable.inl"
#endif /* __ACE_INLINE__ */

#include "ace/Log_Msg.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Synch_Refcountable::TAO_Synch_Refcountable (ACE_Lock *lock, int refcount)
  : ACE_Refcountable_T<ACE_Null_Mutex> (refcount)
  , refcount_lock_ (lock)
{
}

TAO_Synch_Refcountable::~TAO_Synch_Refcountable (void)
{
  ACE_ASSERT (this->refcount_ == 0);
  delete this->refcount_lock_;
}

TAO_END_VERSIONED_NAMESPACE_DECL
