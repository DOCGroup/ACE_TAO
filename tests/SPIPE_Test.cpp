// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    SPIPE_Test.cpp
//
// = DESCRIPTION
//      This is a simple test of Named Pipes that uses
//      ACE_SPIPE_Acceptor and ACE_SPIPE_Connector classes. The test
//      forks two processes or spawns two threads (depending upon the
//      platform) and then executes the client and server allowing
//      them to use the named pipe to exchange data. No user input is
//      required as far as command line arguments are concerned.
//
// = AUTHOR
//    Prashant Jain
//
// ============================================================================

#include "test_config.h"
#include "ace/OS.h"
#include "ace/Thread.h"
#include "ace/Thread_Manager.h"
#include "ace/SPIPE_Addr.h"
#include "ace/SPIPE_Connector.h"
#include "ace/SPIPE_Acceptor.h"

ACE_RCSID(tests, SPIPE_Test, "$Id$")

#if defined(__BORLANDC__) && __BORLANDC__ >= 0x0530
USELIB("..\ace\aced.lib");
//---------------------------------------------------------------------------
#endif /* defined(__BORLANDC__) && __BORLANDC__ >= 0x0530 */

#if defined (ACE_HAS_STREAM_PIPES) || defined (ACE_WIN32)
// pipe name to use
static const char *PIPE_NAME = "ace_pipe_name";

static void *
client (void *)
{
  const char *rendezvous = PIPE_NAME;
  ACE_SPIPE_Stream cli_stream;
  ACE_SPIPE_Connector con;

  ACE_OS::sleep (10);

  if (con.connect (cli_stream, ACE_SPIPE_Addr (ACE_WIDE_STRING (rendezvous))) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", rendezvous));

  for (char *c = ACE_ALPHABET; *c != '\0'; c++)
    if (cli_stream.send (c, 1) == -1)
      ACE_ERROR ((LM_ERROR, "%p\n", "send_n"));

  if (cli_stream.close () == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "close"));

#if !defined (ACE_WIN32)
  ACE_OS::exit (0);
#endif
  return 0;
}

static void *
server (void *)
{
  ACE_SPIPE_Acceptor acceptor;
  ACE_SPIPE_Stream new_stream;
  char buf[BUFSIZ];
  char *t = ACE_ALPHABET;

  const char *rendezvous = PIPE_NAME;

  // Initialize named pipe listener.

  if (acceptor.open (ACE_SPIPE_Addr (ACE_WIDE_STRING (rendezvous))) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "open"));

  ACE_DEBUG ((LM_DEBUG, "waiting for connection\n"));

  // Accept a client connection
  if (acceptor.accept (new_stream, 0) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "accept"));

  ACE_DEBUG ((LM_DEBUG, "Accepted connection\n"));

  while (new_stream.recv (buf, 1) > 0)
    {
      ACE_ASSERT (*t == buf[0]);
      t++;
    }
  ACE_DEBUG ((LM_DEBUG, "End of connection. Closing handle\n"));
  new_stream.close ();
  acceptor.close ();
  return 0;
}
#endif /* ACE_HAS_STREAM_PIPES || ACE_WIN32 */

int
main (int, char *[])
{
  ACE_START_TEST ("SPIPE_Test");

#if defined (ACE_HAS_STREAM_PIPES) || defined (ACE_WIN32)
#if !defined (ACE_LACKS_FORK)
  switch (ACE_OS::fork ())
    {
    case -1:
      ACE_ERROR ((LM_ERROR, "%p\n%a", "fork failed"));
      exit (-1);
    case 0:
      client (0);
    default:
      server (0);
    }
#elif defined (ACE_HAS_THREADS)
  if (ACE_Thread_Manager::instance ()->spawn (ACE_THR_FUNC (client),
                                              (void *) 0,
                                              THR_NEW_LWP | THR_DETACHED) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n%a", "thread create failed"));

  if (ACE_Thread_Manager::instance ()->spawn (ACE_THR_FUNC (server),
                                              (void *) 0,
                                              THR_NEW_LWP | THR_DETACHED) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n%a", "thread create failed"));

  ACE_Thread_Manager::instance ()->wait ();
#endif /* !ACE_LACKS_EXEC */
#else
  ACE_DEBUG ((LM_INFO,
              "SPIPE is not supported on this platform\n"));
#endif /* ACE_HAS_STREAM_PIPES || ACE_WIN32 */
  ACE_END_TEST;
  return 0;
}
