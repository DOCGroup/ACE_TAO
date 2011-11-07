// $Id$

#include "ace/SPIPE_Addr.h"
#include "ace/SPIPE_Acceptor.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_unistd.h"



#if defined (ACE_WIN32)
#define MAKE_PIPE_NAME(X) ACE_TEXT ("\\\\.\\pipe\\") ACE_TEXT (X)
#else
#define MAKE_PIPE_NAME(X) ACE_TEXT (X)
#endif

int
ACE_TMAIN (int /* argc */, ACE_TCHAR * /* argv */ [])
{
  ACE_SPIPE_Acceptor acceptor;
  ACE_SPIPE_Stream new_stream;
  char buf[BUFSIZ];
  int  n;
  const ACE_TCHAR *rendezvous = MAKE_PIPE_NAME ("acepipe");

  // Initialize named pipe listener.

  if (acceptor.open (ACE_SPIPE_Addr (rendezvous)) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("open")), 1);

  for (;;)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("waiting for connection\n")));

      // Accept a client connection.
      if (acceptor.accept (new_stream, 0) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("%p\n"),
                           ACE_TEXT ("accept")),
                          1);

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Accepted connection\n")));

      while ((n = new_stream.recv (buf, sizeof buf)) > 0)
        {
          ACE_OS::fprintf (stderr,
                           "%s\n",
                           buf);
          ACE_OS::write (ACE_STDOUT,
                         buf,
                         n);
        }

      if (n == -1)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("End of connection. Closing handle\n")));
          new_stream.close ();
        }
    }

  ACE_NOTREACHED(return 0);
}
