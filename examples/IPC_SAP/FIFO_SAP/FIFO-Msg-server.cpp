#include "ace/FIFO_Recv_Msg.h"
// $Id$


#if defined (ACE_HAS_STREAM_PIPES)

int 
main (int, char *[])
{
  ACE_OS::unlink (ACE_DEFAULT_RENDEZVOUS);
  ACE_FIFO_Recv_Msg server (ACE_DEFAULT_RENDEZVOUS);
  char buf[BUFSIZ];
  ACE_Str_Buf msg (buf, 0, sizeof buf);
  int flags = MSG_ANY;
  int band  = 0;
  int n;

  while ((n = server.recv (&band, &msg, (ACE_Str_Buf *) 0, &flags)) >= 0)
    {
      if (msg.len == 0)
	break;
      else
	printf ("%4d (%4d): %*s", msg.len, band, msg.len, msg.buf);
      flags = MSG_ANY;
      band  = 0;
    }

  if (n == -1)
    ACE_OS::perror ("recv"), ACE_OS::exit (1);

  return 0;
}
#else
#include <stdio.h>
int main (void)
{
  ACE_OS::fprintf (stderr, "This feature is not supported\n");
  return 0;
}
#endif /* ACE_HAS_STREAM_PIPES */
