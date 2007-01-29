// $Id$

#include "tao/Reply_Dispatcher.h"

ACE_RCSID (tao, 
           Reply_Dispatcher, 
           "$Id$")

#if !defined (__ACE_INLINE__)
#include "tao/Reply_Dispatcher.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// Constructor.
TAO_Reply_Dispatcher::TAO_Reply_Dispatcher (void)
  // Just an invalid reply status.
  : reply_status_ (100)
{
}

// Destructor.
TAO_Reply_Dispatcher::~TAO_Reply_Dispatcher (void)
{
}

TAO_END_VERSIONED_NAMESPACE_DECL
