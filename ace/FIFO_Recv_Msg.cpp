// FIFO_Recv_Msg.cpp
// $Id$

/* -*- C++ -*- */

#define ACE_BUILD_DLL
#include "ace/FIFO_Recv_Msg.h"

ACE_ALLOC_HOOK_DEFINE(ACE_FIFO_Recv_Msg)

void
ACE_FIFO_Recv_Msg::dump (void) const
{
  ACE_TRACE ("ACE_FIFO_Recv_Msg::dump");
  ACE_FIFO_Recv::dump ();
}

// Note that persistent means "open FIFO for writing, as well as
// reading."  This ensures that the FIFO never gets EOF, even if there
// aren't any writers at the moment!

int 
ACE_FIFO_Recv_Msg::open (const char *fifo_name,
			 int flags,
			 int perms,
			 int persistent)
{
  ACE_TRACE ("ACE_FIFO_Recv_Msg::open");

  return ACE_FIFO_Recv::open (fifo_name,
			      flags,
			      perms,
			      persistent);
}

ACE_FIFO_Recv_Msg::ACE_FIFO_Recv_Msg (void)
{
  ACE_TRACE ("ACE_FIFO_Recv_Msg::ACE_FIFO_Recv_Msg");
}

ACE_FIFO_Recv_Msg::ACE_FIFO_Recv_Msg (const char *fifo_name,
				      int flags,
				      int perms,
				      int persistent)
{
  ACE_TRACE ("ACE_FIFO_Recv_Msg::ACE_FIFO_Recv_Msg");

  if (this->ACE_FIFO_Recv_Msg::open (fifo_name,
				     flags,
				     perms, 
				     persistent) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "ACE_FIFO_Recv_Msg"));
}
