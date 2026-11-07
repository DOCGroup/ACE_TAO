#include "ace/FIFO_Send_Msg.h"
#include "ace/OS_main.h"

#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <ctime>


#if defined (ACE_HAS_STREAM_PIPES)

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_FIFO_Send_Msg client (ACE_DEFAULT_RENDEZVOUS);

  char buf[BUFSIZ] {};
  ACE_Str_Buf msg (buf);

  std::srand (static_cast<unsigned> (std::time (nullptr)));

  while (std::fgets (buf, sizeof buf, stdin) != nullptr)
    {
      msg.len = std::strlen (buf) + 1;
      if (client.send (std::rand () % 11, &msg) == -1)
        std::perror ("send");
    }

  if (client.close () == -1)
    {
      std::perror ("close");
      std::exit (1);
    }

  return 0;
}
#else
int ACE_TMAIN (int, ACE_TCHAR *[])
{
  std::fprintf (stderr, "This feature is not supported\n");
  return 0;
}
#endif /* ACE_HAS_STREAM_PIPES */
