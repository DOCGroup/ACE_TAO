// $Id$

#include "ace/FIFO_Send.h"
#include "ace/Log_Msg.h"

ACE_RCSID(FIFO_SAP, FIFO_client, "$Id$")

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_FIFO_Send client (ACE_DEFAULT_RENDEZVOUS);
  ACE_TCHAR     buf[BUFSIZ];

  while (ACE_OS::fgets (buf, sizeof buf, stdin) != 0)
    {
      ssize_t n = ACE_OS::strlen (buf);

      if (client.send (buf, n) != n)
	ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "send"), 1);
    }

  if (client.close () == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "close"), 1);

  return 0;
}
