// -*- C++ -*-
//
// $Id$


#include "SSL_SOCK.h"

#if defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "SSL_SOCK.i"
#endif

ACE_RCSID (ACE_SSL,
           SSL_SOCK,
           "$Id$")


ACE_SSL_SOCK::ACE_SSL_SOCK (void)
{
  ACE_TRACE ("ACE_SSL_SOCK::ACE_SSL_SOCK");
}

ACE_SSL_SOCK::~ACE_SSL_SOCK (void)
{
  ACE_TRACE ("ACE_SSL_SOCK::~ACE_SSL_SOCK");
}

int
ACE_SSL_SOCK::enable (int value) const
{
  ACE_TRACE ("ACE_SSL_SOCK::enable");

  switch (value)
    {
#ifdef SIGURG
    case SIGURG:
    case ACE_SIGURG:
#endif  /* SIGURG */
    case SIGIO:
    case ACE_SIGIO:
    case ACE_CLOEXEC:
      ACE_NOTSUP_RETURN (-1);
    case ACE_NONBLOCK:
      return ACE_IPC_SAP::enable (value);
    default:
      return -1;
    }
}

int
ACE_SSL_SOCK::disable (int value) const
{
  ACE_TRACE("ACE_SSL_SOCK::disable");
  switch (value)
    {
#ifdef SIGURG
    case SIGURG:
    case ACE_SIGURG:
#endif  /* SIGURG */
    case SIGIO:
    case ACE_SIGIO:
    case ACE_CLOEXEC:
      ACE_NOTSUP_RETURN (-1);
    case ACE_NONBLOCK:
      return ACE_IPC_SAP::disable (value);
    default:
      return -1;
    }
}
