// FIFO_Send_Msg.cpp
// $Id$

/* -*- C++ -*- */

#define ACE_BUILD_DLL
#include "ace/FIFO_Send_Msg.h"

#if defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "ace/FIFO_Send_Msg.i"
#endif

ACE_RCSID(ace, FIFO_Send_Msg, "$Id$")

ACE_ALLOC_HOOK_DEFINE(ACE_FIFO_Send_Msg)

void
ACE_FIFO_Send_Msg::dump (void) const
{
  ACE_TRACE ("ACE_FIFO_Send_Msg::dump");
  ACE_FIFO_Send::dump ();
}

ssize_t
ACE_FIFO_Send_Msg::send (const ACE_Str_Buf &send_msg)
{
  // ACE_TRACE ("ACE_FIFO_Send_Msg::send");
#if defined (ACE_HAS_STREAM_PIPES)
  return ACE_OS::putmsg (this->get_handle (), 
			 (strbuf *) 0, 
			 (strbuf *) &send_msg, 
			 0);
#else
  iovec iov[2];

  iov[0].iov_base = (char *) &send_msg.len;
  iov[0].iov_len  = sizeof send_msg.len;

  iov[1].iov_base = (char *) send_msg.buf;
  iov[1].iov_len  =  int (send_msg.len);

  return ACE_OS::writev (this->get_handle (), iov, 2);
#endif /* ACE_HAS_STREAM_PIPES */
}

ACE_FIFO_Send_Msg::ACE_FIFO_Send_Msg (void)
{
//  ACE_TRACE ("ACE_FIFO_Send_Msg::ACE_FIFO_Send_Msg");
}

int
ACE_FIFO_Send_Msg::open (const char *fifo_name, 
			 int flags, 
			 int perms,
                         LPSECURITY_ATTRIBUTES sa)
{
  ACE_TRACE ("ACE_FIFO_Send_Msg::open");
  return ACE_FIFO_Send::open (fifo_name, flags | O_WRONLY, perms, sa);
}

ACE_FIFO_Send_Msg::ACE_FIFO_Send_Msg (const char *fifo_name, 
				      int flags, 
				      int perms,
                                      LPSECURITY_ATTRIBUTES sa)
{
  ACE_TRACE ("ACE_FIFO_Send_Msg::ACE_FIFO_Send_Msg");
  if (this->ACE_FIFO_Send_Msg::open (fifo_name, flags, perms, sa) == -1)
    ACE_ERROR ((LM_ERROR,  ASYS_TEXT ("%p\n"),  ASYS_TEXT ("ACE_FIFO_Send_Msg")));
}
