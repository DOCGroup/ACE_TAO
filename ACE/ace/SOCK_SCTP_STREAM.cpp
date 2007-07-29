// $Id$

#include "ace/SOCK_SCTP_STREAM.h"

#include "ace/Auto_Ptr.h"
#include "ace/Log_Msg.h"
#include "ace/OS_Memory.h"
#include "ace/OS_NS_string.h"

#if !defined (__ACE_INLINE__)
#include "ace/SOCK_SCTP_STREAM.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(ace, SOCK_SCTP_STREAM, "$Id$")

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_ALLOC_HOOK_DEFINE(ACE_SOCK_SCTP_STREAM)

// Developed according to the API discussed in 7.1.4 of
// draft-ietf-tsvwg-sctpsocket-09.txt to abruptly free a transport
// transport association's resources.
int
ACE_SOCK_SCTP_STREAM::abort (void)
{
  //
  // setsockopt() SO_LINGER configures socket to reap immediately.
  // Normal close then aborts the association.
  //
  linger slinger;

  slinger.l_onoff = 1;
  slinger.l_linger = 0;

  if (-1 == ACE_OS::setsockopt (this->get_handle (),
                                SOL_SOCKET,
                                SO_LINGER,
                                reinterpret_cast<const char *> (&slinger),
                                sizeof (linger)))
  {
    return -1;
  }

  return this->close ();
}


ACE_END_VERSIONED_NAMESPACE_DECL
