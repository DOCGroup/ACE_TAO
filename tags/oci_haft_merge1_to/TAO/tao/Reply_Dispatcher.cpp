// $Id$

#include "tao/Reply_Dispatcher.h"
#include "ace/Synch_T.h"

ACE_RCSID(tao, Reply_Dispatcher, "$Id$")

#if !defined (__ACE_INLINE__)
#include "tao/Reply_Dispatcher.i"
#endif /* __ACE_INLINE__ */

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
