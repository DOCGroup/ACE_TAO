// $Id$

#include "tao/Reply_Dispatcher.h"

ACE_RCSID(tao, Reply_Dispatcher, "$Id$")

#if !defined (__ACE_INLINE__)
#include "tao/Reply_Dispatcher.i"
#endif /* __ACE_INLINE__ */

// Constructor.
TAO_Reply_Dispatcher::TAO_Reply_Dispatcher (void)
  : reply_status_ (100) // Just an invalid reply status.
{
}

// Destructor.
TAO_Reply_Dispatcher::~TAO_Reply_Dispatcher (void)
{
}

