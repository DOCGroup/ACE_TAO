#include "ace/Log_Msg.h"
// $Id$

#include "ace/SPIPE_Addr.h"
#include "ace/Time_Value.h"
#include "ace/SPIPE_Acceptor.h"

#if defined (ACE_WIN32)
#define MAKE_PIPE_NAME(X) \\\\.\\pipe\\#X
#else
#define MAKE_PIPE_NAME(X) X
#endif

int
main (int argc, char *argv[])
{
  ACE_SPIPE_Acceptor acceptor;
  ACE_SPIPE_Stream	 new_stream;
  char buf[BUFSIZ];
  int  n;
  //  char *pipe_name = ACE_DEFAULT_RENDEZVOUS;
  char *pipe_name = "acepipe";

  char *rendezvous;
  rendezvous = MAKE_PIPE_NAME (pipe_name);

  /* Initialize named pipe listener */

  if (acceptor.open (ACE_SPIPE_Addr (rendezvous)) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "open"), 1);

  for (;;)
    {
      ACE_DEBUG ((LM_DEBUG, "waiting for connection\n"));
      
      /* Accept a client connection */
      if (acceptor.accept (new_stream, 0) == -1)
	ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "accept"), 1);

      ACE_DEBUG ((LM_DEBUG, "Accepted connection\n"));

      while ((n = new_stream.recv (buf, sizeof buf)) > 0)
	{
	  cerr << buf << endl;
	  ACE_OS::write (ACE_STDOUT, buf, n);
	}
      if (n == -1)
	{
	  ACE_DEBUG ((LM_DEBUG, "End of connection. Closing handle\n"));
	  new_stream.close ();
	}
    }
  return 0;
}
