// FIFO_Send.cpp
// $Id$

#include "ace/IPC/FIFO_Send.h"

#ifdef ACE_SUBSET_0
#include "ace/Logging/Log_Msg.h"
#endif

#if defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "ace/IPC/FIFO_Send.i"
#endif

ACE_RCSID(ace, FIFO_Send, "$Id$")

ACE_ALLOC_HOOK_DEFINE(ACE_FIFO_Send)

void
ACE_FIFO_Send::dump (void) const
{
  ACE_TRACE ("ACE_FIFO_Send::dump");
  ACE_FIFO::dump ();
}

ACE_FIFO_Send::ACE_FIFO_Send (void)
{
//  ACE_TRACE ("ACE_FIFO_Send::ACE_FIFO_Send");
}

int
ACE_FIFO_Send::open (const ACE_TCHAR *rendezvous_name,
                     int flags,
                     int perms,
                     LPSECURITY_ATTRIBUTES sa)
{
  ACE_TRACE ("ACE_FIFO_Send::open");
  return ACE_FIFO::open (rendezvous_name,
                         flags | O_WRONLY,
                         perms,
                         sa);
}

ACE_FIFO_Send::ACE_FIFO_Send (const ACE_TCHAR *fifo_name,
                              int flags,
                              int perms,
                              LPSECURITY_ATTRIBUTES sa)
{
  ACE_TRACE ("ACE_FIFO_Send::ACE_FIFO_Send");

#ifdef ACE_SUBSET_0
  if (this->ACE_FIFO_Send::open (fifo_name,
                                 flags,
                                 perms,
                                 sa) == -1)
    ACE_ERROR ((LM_ERROR,
                ACE_LIB_TEXT ("%p\n"),
                ACE_LIB_TEXT ("ACE_FIFO_Send::ACE_FIFO_Send")));
#else
  this->ACE_FIFO_Send::open (fifo_name,
                                 flags,
                                 perms,
                                 sa);
#endif
}
