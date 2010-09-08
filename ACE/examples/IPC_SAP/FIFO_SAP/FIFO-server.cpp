// $Id$

#include "ace/FIFO_Recv.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_main.h"



int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_OS::unlink (ACE_DEFAULT_RENDEZVOUS);
  ACE_FIFO_Recv server (ACE_DEFAULT_RENDEZVOUS);
  char buf[BUFSIZ];
  int n;

  while ((n = server.recv (buf, sizeof buf)) > 0)
    {
      ACE_OS::printf ("%4d: ", n);
      ACE_OS::fflush (stdout);
      ACE_OS::write (ACE_STDOUT, buf, n);
    }

  if (n == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "recv"), 1);

  return 0;
}
