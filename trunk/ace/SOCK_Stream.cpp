// SOCK_Stream.cpp
// $Id$

#define ACE_BUILD_DLL
#include "ace/SOCK_Stream.h"

ACE_ALLOC_HOOK_DEFINE(ACE_SOCK_Stream)

void
ACE_SOCK_Stream::dump (void) const
{
  ACE_TRACE ("ACE_SOCK_Stream::dump");
}

int
ACE_SOCK_Stream::close (void)
{
#if defined (ACE_WIN32)
  // We need the following call to make things work correctly on
  // Win32, which requires use to do a <close_writer> before doing the
  // close in order to avoid losing data.  Note that we don't need to
  // do this on UNIX since it doesn't have this "feature".  Moreover,
  // this will cause subtle problems on UNIX due to the way that
  // fork() works.
  if (this->get_handle () != ACE_INVALID_HANDLE)
    this->close_writer ();
#endif /* ACE_WIN32 */
  // Close down the socket.
  return ACE_SOCK::close ();
}

