#include "ace/FIFO_Send.h"
#include "ace/Log_Msg.h"
#include "ace/Truncate.h"
#include "ace/OS_main.h"

#include <cstdio>
#include <cstring>

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_FIFO_Send client (ACE_DEFAULT_RENDEZVOUS);
  char buf[BUFSIZ] {};

  while (std::fgets (buf, sizeof buf, stdin) != nullptr)
    {
      ssize_t const n = ACE_Utils::truncate_cast<ssize_t> (std::strlen (buf));

      if (client.send (buf, n) != n)
      {
        ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "send"), 1);
      }
    }

  if (client.close () == -1)
  {
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "close"), 1);
  }

  return 0;
}
