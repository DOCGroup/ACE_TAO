#include "tao/Reply_Dispatcher.h"

#if !defined (__ACE_INLINE__)
#include "tao/Reply_Dispatcher.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// Constructor.
TAO_Reply_Dispatcher::TAO_Reply_Dispatcher (ACE_Allocator *allocator)
  // Just an invalid reply status.
  : locate_reply_status_ (GIOP::UNKNOWN_OBJECT)
  , reply_status_ (GIOP::NO_EXCEPTION)
  , refcount_ (1)
  , allocator_(allocator)
{
}

// Destructor.
TAO_Reply_Dispatcher::~TAO_Reply_Dispatcher (void)
{
}


void
TAO_Reply_Dispatcher::intrusive_add_ref (TAO_Reply_Dispatcher* rd)
{
  if (rd != 0)
    {
      ++rd->refcount_;
    }
}

void
TAO_Reply_Dispatcher::intrusive_remove_ref (TAO_Reply_Dispatcher* rd)
{
  if (rd != 0)
    {
      long const tmp = --rd->refcount_;
      if (tmp <= 0)
        {
          if (rd->allocator_)
            {
              ACE_DES_FREE (rd,
                            rd->allocator_->free,
                            TAO_Reply_Dispatcher);
            }
          else
            {
              delete rd;
            }
        }
    }
}

TAO_END_VERSIONED_NAMESPACE_DECL
