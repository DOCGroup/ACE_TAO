// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    FIFO_Test.cpp
//
// = DESCRIPTION
//     This is a test of basic ACE_FIFO_* class functionality.
//     The test forks two processes or spawns two threads (depending
//     upon the platform) and then tests various data exchange
//     scenarios.
//
// = AUTHOR
//    Steve Huston <shuston@riverace.com>
//
// ============================================================================

#include "test_config.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_sys_stat.h"
#include "ace/OS_NS_sys_wait.h"
#include "ace/OS_NS_unistd.h"
#include "ace/ACE.h"
#include "ace/FIFO_Send_Msg.h"
#include "ace/FIFO_Recv_Msg.h"
#include "ace/Handle_Set.h"
#include "ace/Lib_Find.h"
#include "ace/Thread.h"
#include "ace/Thread_Manager.h"



#if !defined (ACE_LACKS_MKFIFO)

static const char ACE_ALPHABET[] = "abcdefghijklmnopqrstuvwxyz";

// This length is used for the "big buffer" send/receive.
static const size_t big_size = (BUFSIZ * 4);

static void *
client (void *arg)
{
  ACE_TCHAR *fifo_path = reinterpret_cast <ACE_TCHAR *> (arg);
  ACE_FIFO_Send_Msg fifo;
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) client opening %s\n"), fifo_path));
  if (fifo.open (fifo_path) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("(%P|%t) %p\n"), fifo_path), 0);

  // Try some transfers - the server part is expecting this data.
  // First, try a nice, easy send.
  ssize_t send_count;
  ssize_t expect = static_cast <ssize_t> (ACE_OS::strlen (ACE_ALPHABET));
  send_count = fifo.send (ACE_ALPHABET, ACE_OS::strlen (ACE_ALPHABET));
  if (send_count == expect)
    {
      // Ok, so far so good. Now try one that will overflow the reader
      // side to be sure it properly tosses the overflow. Then send another
      // to be sure it finds the start of the next message ok.
      char big[big_size];
      for (size_t i = 0; i < big_size; ++i)
        big[i] = (i % 2) ? 0x05 : 0x0A;  // Make nice pattern in blown stack
      expect = static_cast <ssize_t> (big_size);
      send_count = fifo.send (big, big_size);
      if (send_count == expect)
        {
          expect = static_cast <ssize_t> (ACE_OS::strlen (ACE_ALPHABET));
          send_count = fifo.send (ACE_ALPHABET, ACE_OS::strlen (ACE_ALPHABET));
          if (send_count != expect)
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("(%P|%t) Final send; sent %d, expected %d")
                        ACE_TEXT ("%p\n"),
                        send_count, expect, ACE_TEXT ("send")));
          else
            ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) All sends ok\n")));
        }
      else
        {
          ACE_ERROR
            ((LM_ERROR,
              ACE_TEXT ("(%P|%t) Second send expected %d; sent %d. %p\n"),
              expect, send_count, ACE_TEXT ("send")));
        }
    }
  else
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%P|%t) First send expected %d; sent %d. %p\n"),
                  expect, send_count, ACE_TEXT ("send")));
    }

  if (fifo.close () != 0)
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%P|%t) %p\n"), ACE_TEXT ("fifo close")));

  return 0;
}

static void *
server (void *arg)
{
  ACE_FIFO_Recv_Msg *fifo = reinterpret_cast <ACE_FIFO_Recv_Msg *> (arg);

  // Wait for the client to get going and open the FIFO.
  errno = 0;
  ACE_Handle_Set h;
  ACE_Time_Value delay (10);
  h.set_bit (fifo->get_handle ());
  if (ACE::select (h.max_set () + 1, h, &delay) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("(%P|%t) server %p\n"),
                       ACE_TEXT ("select")),
                      0);

  // On AIX, select() always seems to select a fifo handle as a normal file,
  // always readable. Just wait a second...
# if defined (AIX) || defined (HPUX) || defined (__osf__)
  ACE_OS::sleep (1);
# endif /* AIX || HPUX */

  // Read the things the client is sending; alphabet, huge overflow, then
  // alphabet.

  char buf[BUFSIZ];
  ssize_t recv_count;
  ssize_t expect = static_cast <ssize_t> (ACE_OS::strlen (ACE_ALPHABET));
  recv_count = fifo->recv (buf, sizeof (buf));
  if (recv_count != expect)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("(%P|%t) Recv 1 expected %d, got %d. %p\n"),
                       expect, recv_count, ACE_TEXT ("recv")),
                      0);
  buf[recv_count] = '\0';
  if (ACE_OS::strcmp (buf, ACE_ALPHABET) != 0)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%P|%t) Recv 1 expected alphabet; got %s\n"),
                buf));

  // See documented return values for ACE_FIFO_Recv_Msg...
  // We are being sent a message much longer than BUFSIZ.
  // If this platform has STREAM pipes, the entire message will come
  // through and we can grab it all. If not, then ACE_FIFO_Recv_Msg ditches
  // the part of the message we don't read. This is rather a pain in the
  // neck, but the API doesn't return info that more data is in the message
  // (for STREAM pipes). When non-ACE_HAS_STREAM_PIPES discards data, the
  // returned length will be larger than requested, though only the requested
  // number of bytes are written to the buffer.
#if defined (ACE_HAS_STREAM_PIPES)
  for (size_t remaining = big_size;
       remaining > 0;
       remaining -= recv_count)
    {
#endif /* ACE_HAS_STREAM_PIPES */

      // recv_count is sizeof(buf) on ACE_HAS_STREAM_PIPES; big_size on others.
#if defined (ACE_HAS_STREAM_PIPES)
      expect = static_cast <ssize_t> (sizeof (buf));
#else
      expect = static_cast <ssize_t> (big_size);
#endif /* ACE_HAS_STREAM_PIPES */
      recv_count = fifo->recv (buf, sizeof (buf));
      if (recv_count != expect)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("(%P|%t) Recv 2 expected %d, ")
                           ACE_TEXT ("got %d. %p\n"),
                           expect, recv_count, ACE_TEXT ("recv")),
                          0);
#if defined (ACE_HAS_STREAM_PIPES)
    }
#endif /* ACE_HAS_STREAM_PIPES */

  expect = static_cast <ssize_t> (ACE_OS::strlen (ACE_ALPHABET));
  recv_count = fifo->recv (buf, sizeof (buf));
  if (recv_count != expect)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("(%P|%t) Recv 3 expected %d, got %d. %p\n"),
                       expect, recv_count, ACE_TEXT ("recv")),
                      0);
  buf[recv_count] = '\0';
  if (ACE_OS::strcmp (buf, ACE_ALPHABET) != 0)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%P|%t) Recv 3 expected alphabet; got %s\n"),
                buf));
  else
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) All receives ok\n")));
  return 0;
}

static int
test_fifo_msg (void)
{
  // Reader side opens first - it may fail if fifo not supported on this
  // platform.
  ACE_TCHAR fifo_path[MAXPATHLEN];
  if (ACE::get_temp_dir (fifo_path, MAXPATHLEN) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"),
                       ACE_TEXT ("get_temp_dir")), 1);
  ACE_OS::strcat (fifo_path, ACE_TEXT ("FIFO_Test"));
  ACE_FIFO_Recv_Msg read_side;
  // Open read only, not persistent (4th arg is 0)
  if (-1 == read_side.open (fifo_path,
                            O_CREAT | O_RDONLY,
                            ACE_DEFAULT_FILE_PERMS,
                            0))
    {
#if defined (ACE_WIN32)
      if (errno == ENOTSUP)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("FIFO not supported on Win32; ")
                      ACE_TEXT ("this is correct.\n")));
          return 0;
        }
#endif /* ACE_WIN32 */
      ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"),
                         ACE_TEXT ("ACE_FIFO_Recv_Msg::open")),
                        1);
    }

  // Ok, the FIFO opened clean for read. Now it's safe to spawn a
  // process/thread and test some transfers.

  int status = 0;    // Test status; innocent until proven guilty.

#if !defined (ACE_LACKS_FORK)
  switch (ACE_OS::fork (ACE_TEXT ("child")))
    {
    case -1:
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("fork failed")));
      status = 1;
      break;
    case 0:
      client (fifo_path);
      ACE_OS::exit (0);
      /* NOTREACHED */
    default:
      server (&read_side);
      ACE_OS::wait ();
    }
#elif defined (ACE_HAS_THREADS)
  if (ACE_Thread_Manager::instance ()->spawn
        (ACE_THR_FUNC (server),
         &read_side,
         THR_NEW_LWP | THR_DETACHED) == -1)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("server spawn")));
      ++status;
    }

  if (ACE_Thread_Manager::instance ()->spawn
        (ACE_THR_FUNC (client),
         fifo_path,
         THR_NEW_LWP | THR_DETACHED) == -1)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("client spawn")));
      ++status;
    }

  // Wait for the threads to exit.
  ACE_Thread_Manager::instance ()->wait ();
#else
  ACE_ERROR ((LM_INFO,
              ACE_TEXT ("This test requires multiple threads ")
              ACE_TEXT ("or processes.\n")));
#endif /* ACE_HAS_THREADS */

  if (read_side.remove () != 0)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("fifo remove")));
      ++status;
    }

  ACE_stat fifo_stat;
  if (ACE_OS::stat (fifo_path, &fifo_stat) == 0)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("FIFO %s should be gone but isn't\n"),
                  fifo_path));
      ++status;
      ACE_OS::unlink (fifo_path);   // Try to get rid of it.
    }

  return status;
}

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("FIFO_Test"));

  int errors = 0;
  errors += test_fifo_msg ();

  ACE_END_TEST;
  return errors;
}

#else

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("FIFO_Test"));

  ACE_ERROR ((LM_INFO, ACE_TEXT ("FIFOs are not supported on this platform\n")));

  ACE_END_TEST;
  return 0;
}

#endif /* !ACE_LACKS_MKFIFO */
