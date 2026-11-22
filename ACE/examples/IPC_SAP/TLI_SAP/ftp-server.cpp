// Simple file transfer example

#include "ace/OS_NS_stdio.h"
#include "ace/Thread_Manager.h"
#include "ace/TLI_Acceptor.h"
#include <cstdint>

#if defined (ACE_HAS_THREADS) && defined (ACE_HAS_TLI)

ACE_Thread_Manager thr_mgr;

ACE_THR_FUNC_RETURN
read_file (void *fd)
{
  ACE_TLI_Stream stream;
  char buf[BUFSIZ];
  int flags = 0;
  ssize_t n;

  ACE_HANDLE const handle = static_cast<ACE_HANDLE> (
    reinterpret_cast<std::intptr_t> (fd));
  stream.set_handle (handle);

  ACE_DEBUG ((LM_DEBUG, "start (tid = %t, fd = %H)\n",
              stream.get_handle ()));

  while ((n = stream.recv (buf, sizeof buf, &flags)) > 0)
    {
    }

  ACE_UNUSED_ARG (n);

  ACE_DEBUG ((LM_DEBUG, "finish (tid = %t, fd = %H)\n",
              stream.get_handle ()));

  if (stream.close () == -1)
    ACE_OS::t_error ("stream.close error");

  return ACE_THR_FUNC_RETURN_NULL;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  u_short port = argc > 1 ? ACE_OS::atoi (argv[1]) : ACE_DEFAULT_SERVER_PORT;
  ACE_TLI_Acceptor server;
  ACE_TLI_Stream new_stream;

  // Open the server and reuse the address if in use...
  if (server.open (ACE_INET_Addr (port), 1) == -1)
    {
      ACE_OS::t_error ("server.open");
      ACE_OS::exit (1);
    }

  // Wait for a connection from a client.  This is an example of a
  // concurrent server.

  for (int count = 1; ; ++count)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "thread %t, blocking for accept #%d\n",
                  count));

      if (server.accept (new_stream) == -1)
        {
          ACE_OS::t_error ("server.accept error");
        }

      else if (thr_mgr.spawn (read_file,
                              reinterpret_cast<void *> (
                                static_cast<std::intptr_t> (
                                  new_stream.get_handle ())),
                              THR_DETACHED | THR_BOUND) == -1)
        {
          ACE_OS::perror ("can't create worker thread\n");
        }
    }

  ACE_NOTREACHED (return 0);
}
#else
#include <stdio.h>
int ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_ERROR_RETURN ((LM_ERROR,
                     "your platform isn't configured to support TLI\n"),
                    1);
}
#endif /* ACE_HAS_THREADS */
