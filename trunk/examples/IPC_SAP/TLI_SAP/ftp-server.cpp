// $Id$
/* Simple file transfer example */

#include "ace/Thread_Manager.h"
#include "ace/TLI_Acceptor.h"

#if defined (ACE_HAS_THREADS) && defined (ACE_HAS_TLI)

ACE_Thread_Manager thr_mgr;

void *
read_file (void *fd)
{
  ACE_TLI_Stream stream;
  char       buf[BUFSIZ];
  int        flags = 0;
  int        n;

#if defined (ACE_HAS_64BIT_LONGS)
  stream.set_handle (long (fd));
#else /* ! ACE_HAS_64BIT_LONGS */
  stream.set_handle (int (fd));
#endif /* ! ACE_HAS_64BIT_LONGS */

  ACE_OS::printf ("start  (tid = %d, fd = %d)\n", ACE_OS::thr_self (), stream.get_handle ());
  ACE_OS::fflush (stdout);

  while ((n = stream.recv (buf, sizeof buf, &flags)) > 0)
    continue;

  ACE_UNUSED_ARG (n);

  ACE_OS::printf ("finish (tid = %d, fd = %d)\n", ACE_OS::thr_self (), stream.get_handle ());

  if (stream.close () == -1)
    ACE_OS::t_error ("stream.close error");

  return 0;
}

int
main (int argc, char *argv[])
{
  u_short port = argc > 1 ? ACE_OS::atoi (argv[1]) : ACE_DEFAULT_SERVER_PORT;
  ACE_TLI_Acceptor   server;
  ACE_TLI_Stream     new_stream;

  /* Allow up to 100 simultaneous threads */
  if (thr_mgr.open (100) == -1)
    ACE_OS::perror ("thr_mgr.open"), ACE_OS::exit (1);

  // Open the server and reuse the address if in use...
  if (server.open (ACE_INET_Addr (port), 1) == -1)
    ACE_OS::t_error ("server.open"), ACE_OS::exit (1);

  /* Wait for a connection from a client.  This is an example of a concurrent server */

  for (int count = 1; ; count++)
    {
      ACE_OS::fprintf (stderr, "thread %d, blocking for accept #%d\n",
                       ACE_OS::thr_self (), count);

      if (server.accept (new_stream) == -1)
        ACE_OS::t_error ("server.accept error");

      else if (thr_mgr.spawn (ACE_THR_FUNC (read_file),
                              (void *) new_stream.get_handle (),
                              THR_DETACHED | THR_BOUND) == -1)
        ACE_OS::perror ("can't create worker thread\n");
    }

  ACE_NOTREACHED (return 0);
}
#else
#include <stdio.h>
int main (int, char *[])
{
  ACE_ERROR_RETURN ((LM_ERROR, "your platform must support ACE_TLI\n"), 1);
}
#endif /* ACE_HAS_THREADS */
