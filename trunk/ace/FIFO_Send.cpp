// FIFO_Send.cpp
// $Id$

/* -*- C++ -*- */

#define ACE_BUILD_DLL
#include "ace/FIFO_Send.h"

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
ACE_FIFO_Send::open (const char *rendezvous_name, int flags, int perms,
                     LPSECURITY_ATTRIBUTES sa)
{
  ACE_TRACE ("ACE_FIFO_Send::open");
  return ACE_FIFO::open (rendezvous_name, flags | O_WRONLY, perms, sa);
}

ACE_FIFO_Send::ACE_FIFO_Send (const char *fifo_name, int flags, int perms,
                              LPSECURITY_ATTRIBUTES sa)
{
  ACE_TRACE ("ACE_FIFO_Send::ACE_FIFO_Send");
  if (this->ACE_FIFO_Send::open (fifo_name, flags, perms, sa) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "ACE_FIFO_Send::ACE_FIFO_Send"));
}
