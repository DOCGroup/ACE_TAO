// $Id$

#include "Refcountable.h"
#include "Destroy_Callback.h"

#include "tao/debug.h"
#include "ace/Log_Msg.h"

#if ! defined (__ACE_INLINE__)
#include "Refcountable.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(RT_Notify, TAO_NS_Refcountable, "$Id$")

TAO_NS_Refcountable::TAO_NS_Refcountable (void)
  :refcount_ (1)
{
}

TAO_NS_Refcountable::~TAO_NS_Refcountable ()
{
}

CORBA::ULong
TAO_NS_Refcountable::_incr_refcnt (void)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->lock_, 0);

  if (TAO_debug_level > 1 )
    ACE_DEBUG ((LM_DEBUG,"object:%x  incr refcount = %d\n", this, refcount_+1 ));


  return this->refcount_++;
}

CORBA::ULong
TAO_NS_Refcountable::_decr_refcnt (void)
{
  {
    ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->lock_, 0);

    if (TAO_debug_level > 1 )
      ACE_DEBUG ((LM_DEBUG,"object:%x  decr refcount = %d\n", this, refcount_-1 ));

    this->refcount_--;
    if (this->refcount_ != 0)
      return this->refcount_;
  }

  if (this->destroy_callback_ != 0)
    this->destroy_callback_->release ();

  return 0;
}

void
TAO_NS_Refcountable::destroy_callback (TAO_NS_Destroy_Callback* destroy_callback)
{
  destroy_callback_ = destroy_callback;
}
