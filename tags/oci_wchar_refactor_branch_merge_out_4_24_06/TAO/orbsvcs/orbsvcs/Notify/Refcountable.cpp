// $Id$

#include "Refcountable.h"
#include "tao/debug.h"
#include "ace/Log_Msg.h"
#include "ace/Guard_T.h"

ACE_RCSID(Notify, TAO_Notify_Refcountable, "$Id$")

TAO_Notify_Refcountable::TAO_Notify_Refcountable (void)
{
}

TAO_Notify_Refcountable::~TAO_Notify_Refcountable ()
{
  Counter refcount = this->refcount_.value();
  if ( refcount != 0 )
  {
    ACE_ERROR ((LM_ERROR,"ERROR: object:%x delete with non-zero refcount = %d\n", this, refcount ));
    ACE_ASSERT( refcount == 0 );
  }
}

CORBA::ULong
TAO_Notify_Refcountable::_incr_refcnt (void)
{
  Counter refcount = ++this->refcount_;
  if (TAO_debug_level > 1 )
  {
    ACE_DEBUG ((LM_DEBUG,"object:%x incr refcount = %d\n", this, refcount ));
  }
  return static_cast< CORBA::ULong >( refcount );
}

CORBA::ULong
TAO_Notify_Refcountable::_decr_refcnt (void)
{
  Counter refcount = --this->refcount_;

    if (TAO_debug_level > 1 )
  {
    ACE_DEBUG ((LM_DEBUG,"object:%x  decr refcount = %d\n", this, refcount ));
  }

  // If error
  if ( refcount < 0 )
  {
    ACE_ERROR ((LM_ERROR,"ERROR: object:%x _decr_refcnt (%d < 0)\n", this, refcount ));
    ACE_ASSERT( refcount >= 0 );
  }

  // Release if count is zero
  else if ( refcount == 0 )
  {
  this->release ();
  }

  return static_cast< CORBA::ULong >( refcount );
}
