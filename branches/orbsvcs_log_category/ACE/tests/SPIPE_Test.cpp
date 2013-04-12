
//=============================================================================
/**
 *  @file    SPIPE_Test.cpp
 *
 *  $Id$
 *
 *    This is a simple test of Named Pipes that uses
 *    ACE_SPIPE_Acceptor and ACE_SPIPE_Connector classes. The test
 *    forks two processes or spawns two threads (depending upon the
 *    platform) and then executes the client and server allowing
 *    them to use the named pipe to exchange data. No user input is
 *    required as far as command line arguments are concerned.
 *
 *
 *  @author Prashant Jain <pjain@cs.wustl.edu>
 */
//=============================================================================


#include "test_config.h"
#include "ace/Thread.h"
#include "ace/Thread_Manager.h"
#include "ace/SPIPE_Addr.h"
#include "ace/SPIPE_Connector.h"
#include "ace/SPIPE_Acceptor.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_unistd.h"



#if defined (ACE_HAS_STREAM_PIPES) || defined (ACE_HAS_WIN32_NAMED_PIPES)
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

  ACE_OS::sleep (5);

  if (con.connect (cli_stream, ACE_SPIPE_Addr (rendezvous)) == -1)
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n%a"), rendezvous, 1));

  for (const char *c = ACE_ALPHABET; *c != '\0'; c++)
    if (cli_stream.send (c, 1) == -1)
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("send")));

  if (cli_stream.close () == -1)
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("close")));

#if defined (ACE_HAS_WIN32_NAMED_PIPES)

  // Wait for server to get ready...
  ACE_OS::sleep (1);

  // Connect in bytestream-oriented mode.
  if (con.connect (cli_stream,
                   ACE_SPIPE_Addr (rendezvous),
                   0,
                   ACE_Addr::sap_any,
                   0,
                   O_RDWR,
                   0,
                   0,
                   PIPE_READMODE_BYTE) == -1)
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n%a"), rendezvous, 1));


  // Write out the alphabet all at once.
  if (cli_stream.send_n (ACE_ALPHABET,
                         ACE_OS::strlen (ACE_ALPHABET)) != (ssize_t) ACE_OS::strlen (ACE_ALPHABET))
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("send_n")));

  // Write out the alphabet one byte at a time
  for (const char *d = ACE_ALPHABET; *d != '\0'; d++)
    if (cli_stream.send (d, 1) == -1)
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("send")));

  if (cli_stream.close () == -1)
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("close")));
#endif

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
      ACE_TEST_ASSERT (*t == buf[0]);
      t++;
    }
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("End of connection. Closing handle\n")));
  new_stream.close ();
  acceptor.close ();

#if defined (ACE_HAS_WIN32_NAMED_PIPES)
  // Initialize an NT bytestream named pipe listener.
  if (acceptor.open (ACE_SPIPE_Addr (rendezvous),
                     1,
                     0,
                     0,
                     PIPE_TYPE_BYTE | PIPE_READMODE_BYTE) == -1)
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n%a"), ACE_TEXT ("open"), 1));

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("waiting for connection\n")));

  // Accept a client connection
  if (acceptor.accept (new_stream, 0) == -1)
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n%a"), ACE_TEXT ("accept"), 1));

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Accepted connection\n")));

  // The client will write the entire buffer at once, verify that we
  // can stream it in one byte at a time.
  for (t = ACE_ALPHABET; *t; t++)
    {
      if (new_stream.recv (buf, 1) <= 0)
        {
          ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n%a"), ACE_TEXT ("recv"), 1));
          break;
        }
      ACE_TEST_ASSERT (*t == buf[0]);
    }

  // Wait for the client to stream in the buffer one byte at a time.

  ACE_OS::sleep (1);

  // Verify that we can read the stream of individual bytes all at
  // once.
  if (new_stream.recv (buf, sizeof(buf)) != (ssize_t) ACE_OS::strlen (ACE_ALPHABET))
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n%a"), ACE_TEXT ("recv"), 1));
  else
    ACE_TEST_ASSERT(ACE_OS::memcmp(ACE_ALPHABET, buf, ACE_OS::strlen (ACE_ALPHABET)) == 0);

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("End of connection. Closing handle\n")));
  new_stream.close ();
  acceptor.close ();
#endif /* defined (ACE_HAS_WIN32NAMED_PIPES) */

  return 0;
}
#endif /* TEST_HAS_STREAM_PIPES */

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("SPIPE_Test"));

#if defined (TEST_HAS_STREAM_PIPES)
#if !defined (ACE_LACKS_FORK)
  switch (ACE_OS::fork ())
    {
    case -1:
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n%a"), ACE_TEXT ("fork failed")));
      ACE_OS::exit (-1);
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
