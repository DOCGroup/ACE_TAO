// $Id$

#include "ace/FIFO_Send.h"
#include "ace/Log_Msg.h"
#include "ace/Truncate.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_main.h"



int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_FIFO_Send client (ACE_DEFAULT_RENDEZVOUS);
  ACE_TCHAR     buf[BUFSIZ];

  while (ACE_OS::fgets (buf, sizeof buf, stdin) != 0)
    {
      ssize_t n = ACE_Utils::truncate_cast<ssize_t> (ACE_OS::strlen (buf));

      if (client.send (buf, n) != n)
        ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "send"), 1);
    }

  if (client.close () == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "close"), 1);

  return 0;
}
