// $Id$

#include "tao/Context.h"

#if !defined (__ACE_INLINE__)
# include "tao/Context.i"
#endif /* ! __ACE_INLINE__ */

CORBA_Context::CORBA_Context (void)
{
}

CORBA_Context::~CORBA_Context (void)
{
}

CORBA::ULong
CORBA_Context::_incr_refcnt (void)
{
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->refcount_lock_, 0);
  return refcount_++;
}

CORBA::ULong
CORBA_Context::_decr_refcnt (void)
{
  {
    ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->refcount_lock_, 0);
    this->refcount_--;
    if (this->refcount_ != 0)
      return this->refcount_;
  }

  delete this;
  return 0;
}

