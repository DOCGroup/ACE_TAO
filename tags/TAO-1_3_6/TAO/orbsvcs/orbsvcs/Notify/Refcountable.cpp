// $Id$

#include "Refcountable.h"
#include "tao/debug.h"
#include "ace/Log_Msg.h"

#if ! defined (__ACE_INLINE__)
#include "Refcountable.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Notify, TAO_Notify_Refcountable, "$Id$")

TAO_Notify_Refcountable::TAO_Notify_Refcountable (void)
  :refcount_ (1)
{
}

TAO_Notify_Refcountable::~TAO_Notify_Refcountable ()
{
}

CORBA::ULong
TAO_Notify_Refcountable::_incr_refcnt (void)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->lock_, 0);

  if (TAO_debug_level > 1 )
    ACE_DEBUG ((LM_DEBUG,"object:%x  incr refcount = %d\n", this, refcount_+1 ));


  return this->refcount_++;
}

CORBA::ULong
TAO_Notify_Refcountable::_decr_refcnt (void)
{
  {
    ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->lock_, 0);

    if (TAO_debug_level > 1 )
      ACE_DEBUG ((LM_DEBUG,"object:%x  decr refcount = %d\n", this, refcount_-1 ));

    this->refcount_--;
    if (this->refcount_ != 0)
      return this->refcount_;
  }

  this->release ();

  return 0;
}
