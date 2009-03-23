// $Id$

#include "tao/Reply_Dispatcher.h"
#include "tao/Pluggable_Messaging_Utils.h"
#include "tao/ORB_Core.h"
#include "tao/debug.h"
#include "tao/Transport.h"

#if !defined (__ACE_INLINE__)
#include "tao/Reply_Dispatcher.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID (tao,
           Reply_Dispatcher,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// Default Constructor.
TAO_Reply_Dispatcher::TAO_Reply_Dispatcher (void)
  : transport_ (0)
  , db_ ()
  , reply_cdr_ (&db_)
  , lock_ (0)
  , refcount_ (1)
  , is_reply_dispatched_ (false)
  , allocator_ (0)
  , locate_reply_status_ (GIOP::UNKNOWN_OBJECT)
  , reply_status_ (GIOP::NO_EXCEPTION)
{
}

// Copy Constructor.
TAO_Reply_Dispatcher::TAO_Reply_Dispatcher (
    TAO_ORB_Core *orb_core,
    ACE_Allocator *allocator
  )
  : db_ (sizeof buf_,
         ACE_Message_Block::MB_DATA,
         this->buf_,
         orb_core->input_cdr_buffer_allocator (),
         orb_core->locking_strategy (),
         ACE_Message_Block::DONT_DELETE,
         orb_core->input_cdr_dblock_allocator ())
  ,  reply_cdr_ (&db_,
                ACE_Message_Block::MB_DATA,
                TAO_ENCAP_BYTE_ORDER,
                TAO_DEF_GIOP_MAJOR,
                TAO_DEF_GIOP_MINOR,
                orb_core)
  , transport_ (0)
  , lock_ (0)
  , refcount_ (1)
  , is_reply_dispatched_ (false)
  , allocator_ (allocator)
  , locate_reply_status_ (GIOP::UNKNOWN_OBJECT)
  , reply_status_ (GIOP::NO_EXCEPTION)

{
  // @@ NOTE: Need a seperate option for this..
  this->lock_ =
    orb_core->resource_factory ()->create_cached_connection_lock ();
}

// Destructor.
TAO_Reply_Dispatcher::~TAO_Reply_Dispatcher (void)
{
  // Release the transport that we own
  if (this->transport_ != 0)
    this->transport_->remove_reference ();

  if (this->lock_)
    delete this->lock_;
}

void
TAO_Reply_Dispatcher::transport (TAO_Transport *t)
{
  if (this->transport_ != 0)
    this->transport_->remove_reference ();

  this->transport_ = t;

  this->transport_->add_reference ();
}

void
TAO_Reply_Dispatcher::incr_refcount (void)
{
  ACE_GUARD (ACE_Lock,
             mutex,
             *this->lock_);
  ++this->refcount_;
}

void
TAO_Reply_Dispatcher::decr_refcount (void)
{
  {
    ACE_GUARD (ACE_Lock,
               mutex,
               *this->lock_);
    --this->refcount_;

    if (this->refcount_ > 0)
      return;
  }

  if (this->allocator_)
    {
      ACE_DES_FREE (this,
                    this->allocator_->free,
                    TAO_Reply_Dispatcher);
    }
  else
    {
      delete this;
    }

  return;
}

bool
TAO_Reply_Dispatcher::try_dispatch_reply (void)
{
  if (this->is_reply_dispatched_)
    {
      return false;
    }
  else
    {
      ACE_GUARD_RETURN (ACE_Lock,
                        mutex,
                        *this->lock_,
                        false);

      if (!this->is_reply_dispatched_)
        {
          this->is_reply_dispatched_ = true;
          return true;
        }
    }

  return false;
}

TAO_END_VERSIONED_NAMESPACE_DECL
