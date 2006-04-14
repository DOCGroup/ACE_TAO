// SOCK_Stream.cpp
// $Id$

#include "ace/SOCK_Stream.h"

#if !defined (__ACE_INLINE__)
#include "ace/SOCK_Stream.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(ace, SOCK_Stream, "$Id$")

ACE_ALLOC_HOOK_DEFINE(ACE_SOCK_Stream)

void
ACE_SOCK_Stream::dump (void) const
{
#if defined (ACE_HAS_DUMP)
  ACE_TRACE ("ACE_SOCK_Stream::dump");
#endif /* ACE_HAS_DUMP */
}

int
ACE_SOCK_Stream::close (void)
{
  // Close down the socket.
  return ACE_SOCK::close ();
}

