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
//    Prashant Jain <pjain@cs.wustl.edu>
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

#if defined (ACE_HAS_STREAM_PIPES) \
    || (defined (ACE_WIN32) && defined(ACE_HAS_WINNT4) \
        && (ACE_HAS_WINNT4 !=0))
# define TEST_HAS_STREAM_PIPES
#endif

#if defined (TEST_HAS_STREAM_PIPES)

static const char ACE_ALPHABET[] = "abcdefghijklmnopqrstuvwxyz";

// pipe name to use
static const ACE_TCHAR *PIPE_NAME = ACE_TEXT ("ace_pipe_name");

static void *
client (void *)
{
  const ACE_TCHAR *rendezvous = PIPE_NAME;
  ACE_SPIPE_Stream cli_stream;
  ACE_SPIPE_Connector con;

  ACE_OS::sleep (10);

  if (con.connect (cli_stream, ACE_SPIPE_Addr (rendezvous)) == -1)
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n%a"), rendezvous, 1));

  for (const char *c = ACE_ALPHABET; *c != '\0'; c++)
    if (cli_stream.send (c, 1) == -1)
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("send_n")));

  if (cli_stream.close () == -1)
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("close")));

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
  const char *t = ACE_ALPHABET;

  const ACE_TCHAR *rendezvous = PIPE_NAME;

  // Initialize named pipe listener.

  if (acceptor.open (ACE_SPIPE_Addr (rendezvous)) == -1)
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n%a"), ACE_TEXT ("open"), 1));

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("waiting for connection\n")));

  // Accept a client connection
  if (acceptor.accept (new_stream, 0) == -1)
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n%a"), ACE_TEXT ("accept"), 1));

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Accepted connection\n")));

  while (new_stream.recv (buf, 1) > 0)
    {
      ACE_ASSERT (*t == buf[0]);
      t++;
    }
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("End of connection. Closing handle\n")));
  new_stream.close ();
  acceptor.close ();
  return 0;
}
#endif /* TEST_HAS_STREAM_PIPES */

int
main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("SPIPE_Test"));

#if defined (TEST_HAS_STREAM_PIPES)
#if !defined (ACE_LACKS_FORK)
  switch (ACE_OS::fork ())
    {
    case -1:
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n%a"), ACE_TEXT ("fork failed")));
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
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n%a"), ACE_TEXT ("thread create failed")));

  if (ACE_Thread_Manager::instance ()->spawn (ACE_THR_FUNC (server),
                                              (void *) 0,
                                              THR_NEW_LWP | THR_DETACHED) == -1)
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n%a"), ACE_TEXT ("thread create failed")));

  ACE_Thread_Manager::instance ()->wait ();
#endif /* !ACE_LACKS_EXEC */
#else
  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("SPIPE is not supported on this platform\n")));
#endif /* TEST_HAS_STREAM_PIPES */
  ACE_END_TEST;
  return 0;
}
