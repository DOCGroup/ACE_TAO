/**
 *  @file Sendfile_Test.cpp
 *
 *  This is a test of the @c ACE_OS::sendfile() wrapper function.  It
 *  is primarily meant to test the case when ACE_HAS_SENDFILE is not
 *  defined, i.e. when sendfile() support is emulated.
 *
 *  @author
 *    Steve Huston <shuston@riverace.com>
 *    Ossama Othman <ossama@dre.vanderbilt.edu>
 */

#include "test_config.h"
#include "ace/Lib_Find.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_sys_wait.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_fcntl.h"
#include "ace/Thread.h"
#include "ace/Thread_Manager.h"
#include "ace/Time_Value.h"
#include "ace/SOCK_Connector.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/SOCK_Stream.h"
#include "ace/OS_NS_sys_sendfile.h"

// Change to non-zero if test fails
static int Test_Result = 0;

#if !defined (ACE_LACKS_FORK) || defined (ACE_HAS_THREADS)

// This test sends a large amount of data.  The purpose is to overflow the
// TCP send window, causing the sender to block (it's a send_n). This value
// is the amount to send. The assumption is that no implementation has a
// receive window larger than 128K bytes. If one is found, this is the place
// to change it.
// For some odd reason, NT will try to send a single large buffer, but not
// multiple smaller ones that add up to the large size.
const size_t Test3_Send_Size = 4*1024;
const size_t Test3_Loops = 10;
const size_t Test3_Total_Size = Test3_Send_Size * Test3_Loops;


static void *
client (void *arg)
{
  ACE_INET_Addr *remote_addr = reinterpret_cast<ACE_INET_Addr *> (arg);
  ACE_INET_Addr server_addr (remote_addr->get_port_number (),
                             ACE_LOCALHOST);
  ACE_SOCK_Stream cli_stream;
  ACE_SOCK_Connector con;
  ACE_Time_Value timeout (ACE_DEFAULT_TIMEOUT);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) Connecting to port %d\n"),
              server_addr.get_port_number()));

  // Initiate connection with server; don't wait forever
  if (con.connect (cli_stream,
                   server_addr,
                   &timeout) == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%P|%t) %p\n"),
                  ACE_TEXT ("connection failed")));
      Test_Result = 1;
      return 0;
    }

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) connected to %C\n"),
              server_addr.get_host_name ()));

  //*******************   TEST 1   ******************************
  //
  // Send the 255 byte buffer in 5 chunks.  The
  // server will verify that the correct data is sent, and that there
  // is no more and no less.

  u_char buffer[255];
  size_t i;
  ssize_t byte_count = 0;
  ssize_t len = 0;
  off_t offset = 0;

  // The server will verify that this data pattern gets there intact.

  for (i = 0; i < sizeof buffer; ++i)
    buffer[i] = static_cast<u_char> (i);

  const ACE_TCHAR file[] = ACE_TEXT ("Sendfile_Test_File");
  static const size_t file_sz = sizeof (file) / sizeof (file[0]);
  ACE_TCHAR test_file[MAXPATHLEN + 1];
  ACE_HANDLE in_fd = ACE_INVALID_HANDLE;
  if (ACE::get_temp_dir (test_file, MAXPATHLEN - file_sz) == -1
      || ACE_OS::strcat (test_file, file) == 0
      || (in_fd = ACE_OS::open (test_file, O_CREAT | O_RDWR | O_TRUNC,
                                ACE_DEFAULT_FILE_PERMS)) == ACE_INVALID_HANDLE)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%P|%t) open %p\n"), test_file));
      Test_Result = 1;
      goto cleanup;
    }

  ACE_OS::unlink (test_file);

  byte_count = ACE_OS::write (in_fd, buffer, sizeof (buffer));

  if (byte_count != static_cast<ssize_t> (sizeof (buffer)))
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%P|%t) write %p\n"), test_file));
      Test_Result = 1;
    }

  len = ACE_OS::sendfile (cli_stream.get_handle (),
                          in_fd,
                          &offset,
                          byte_count);

  if (len == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%P|%t) %p\n"),
                  ACE_TEXT ("Test 1, sendfile failed")));
      Test_Result = 1;
      goto cleanup;
    }
  else if (len != 255)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%P|%t) sendfile len %b; should be 255\n"),
                len));

  //*******************   TEST 2   ******************************
  //
  // The same data is coming back - receive it using recv (size_t n,
  // ...)  and compare it to the original data.

  u_char buffer2[255];
  // Give it a chance to get here
  ACE_OS::sleep (2);
#ifndef ACE_LACKS_VA_FUNCTIONS
  len = cli_stream.recv (4,
                         buffer2,
                         150,
                         &buffer2[150],
                         105);
#endif

  if (len != 255)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%P|%t) recv len is %b, but should be 255!\n"),
                  len));
    }

  for (i = 0; i < static_cast<size_t>(len); i++)
    if (buffer2[i] != buffer[i])
      {
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("(%P|%t) Test 2, rcvd byte %B is %d, not %d\n"),
                    i, buffer2[i], buffer[i]));
        Test_Result = 1;
      }

cleanup:
  cli_stream.close ();
  if (in_fd != ACE_INVALID_HANDLE)
    (void) ACE_OS::close (in_fd);

  return 0;
}

static void *
server (void *arg)
{
  ACE_SOCK_Acceptor *peer_acceptor = (ACE_SOCK_Acceptor *) arg;
  ACE_SOCK_Stream sock_str;
  ACE_INET_Addr cli_addr;
  ACE_Time_Value timeout (ACE_DEFAULT_TIMEOUT);

  // Accept the connection over which the stream tests will run.
  // Don't lock up if client doesn't connect
  if (peer_acceptor->accept (sock_str,
                             &cli_addr,
                             &timeout) == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%P|%t) %p\n"),
                  ACE_TEXT ("accept")));
      Test_Result = 1;
      return 0;
    }

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) client %C connected from %d\n"),
              cli_addr.get_host_name (),
              cli_addr.get_port_number ()));

  //*******************   TEST 1   ******************************
  //
  // Do a iovec recvv - the client should send 255 bytes, which we
  // will be detected and read into a ACE-allocated buffer.  Use a 5
  // second timeout to give the client a chance to send it all.

  ACE_OS::sleep (5);

  u_char buffer[255];
  ssize_t len;
  int i;

  len = sock_str.recv (buffer, 255);
  if (len == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%P|%t) %p\n"),
                  ACE_TEXT ("Test 1, recv failed")));
      Test_Result = 1;
    }

  if (len != 255)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%P|%t) recv len is %b, but should be 255!\n"),
                  len));
    }

  for (i = 0; i < static_cast<int>(len); i++)
    if (buffer[i] != i)
      {
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("(%P|%t) Test 1, rcvd byte %d is %d, not %d\n"),
                    i,
                    buffer[i],
                    i));
        Test_Result = 1;
      }

  //*******************   TEST 2   ******************************
  //
  // Send the buffer back, using send (size_t n, ...) in 3 pieces.

#ifndef ACE_LACKS_VA_FUNCTIONS
  len = sock_str.send (6,
                       buffer,
                       42,
                       &buffer[42],
                       189,
                       &buffer[231],
                       24);
#endif

  if (len != 255)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%P|%t) send len is %b, but should be 255!\n"),
                  len));
    }

  sock_str.close();

  return 0;
}

#endif /* !ACE_LACKS_FORK || ACE_HAS_THREADS */

void
spawn (void)
{
  // Acceptor
  ACE_SOCK_Acceptor peer_acceptor;

  // Create a server address.
  ACE_INET_Addr server_addr;

  // Bind listener to any port and then find out what the port was.
  if (peer_acceptor.open (ACE_Addr::sap_any) == -1
      || peer_acceptor.get_local_addr (server_addr) == -1)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%P|%t) %p\n"),
                ACE_TEXT ("open")));
  else
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) starting server at port %d\n"),
                  server_addr.get_port_number ()));

#if !defined (ACE_LACKS_FORK)
      switch (ACE_OS::fork (ACE_TEXT ("child")))
        {
        case -1:
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("(%P|%t) %p\n%a"),
                      ACE_TEXT ("fork failed"),
                      1));
          /* NOTREACHED */
        case 0:
          client (&server_addr);
          ACE_OS::exit (0);
          /* NOTREACHED */
        default:
          server (reinterpret_cast<void *> (&peer_acceptor));
          ACE_OS::wait ();
        }
#elif defined (ACE_HAS_THREADS)
      if (ACE_Thread_Manager::instance ()->spawn
          (ACE_THR_FUNC (server),
           reinterpret_cast<void *> (&peer_acceptor),
           THR_NEW_LWP | THR_DETACHED) == -1)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("(%P|%t) %p\n%a"),
                    ACE_TEXT ("thread create failed"),
                    1));

      if (ACE_Thread_Manager::instance ()->spawn
          (ACE_THR_FUNC (client),
           reinterpret_cast<void *> (&server_addr),
           THR_NEW_LWP | THR_DETACHED) == -1)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("(%P|%t) %p\n%a"),
                    ACE_TEXT ("thread create failed"),
                    1));

      // Wait for the threads to exit.
      ACE_Thread_Manager::instance ()->wait ();
#else
      ACE_ERROR ((LM_INFO,
                  ACE_TEXT ("(%P|%t) ")
                  ACE_TEXT ("only one thread may be run ")
                  ACE_TEXT ("in a process on this platform\n")));
#endif /* ACE_HAS_THREADS */

      peer_acceptor.close ();
    }
}

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Sendfile_Test"));

#ifndef ACE_LACKS_ACCEPT
  spawn ();
#endif

  ACE_END_TEST;
  return Test_Result;
}
