#include "Synch_Refcountable.h"


#if !defined (__ACE_INLINE__)
# include "Synch_Refcountable.inl"
#endif /* __ACE_INLINE__ */

#include "ace/Log_Msg.h"

ACE_RCSID (tao,
           Synch_Refcountable,
           "$Id$")

TAO_Synch_Refcountable::TAO_Synch_Refcountable (ACE_Lock *lock,
                                                int refcount)
  : ACE_Refcountable (refcount)
  , refcount_lock_ (lock)
{
}

TAO_Synch_Refcountable::~TAO_Synch_Refcountable (void)
{
  ACE_ASSERT (this->refcount_ == 0);
  delete this->refcount_lock_;
}
