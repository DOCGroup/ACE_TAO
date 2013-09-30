
//=============================================================================
/**
 *  @file    SOCK_Send_Recv_Test.cpp
 *
 *  $Id$
 *
 *   This is a test of the <ACE_SOCK>'s various send and receive
 *   methods.  The test forks two processes or spawns two threads
 *   (depending upon the platform) and then executes client and
 *   server allowing them to connect and exchange data in ways
 *   designed to exercise the send and recv functions.
 *
 *   Right now, it primarily tests the iov-like send and recv
 *   functions, but others should be added to completely cover the
 *   possible scenarios.
 *
 *
 *  @author Steve Huston <shuston@riverace.com>
 */
//=============================================================================


#include "test_config.h"
#include "ace/OS_NS_sys_wait.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Thread.h"
#include "ace/Thread_Manager.h"
#include "ace/Time_Value.h"
#include "ace/SOCK_Connector.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/SOCK_Stream.h"



// Change to non-zero if test fails
static int Test_Result = 0;

#if !defined (ACE_LACKS_FORK) || defined (ACE_HAS_THREADS)

// In test 3, a large amount of data is sent. The purpose is to overflow the
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

  //*******************   TEST 0   ******************************
  //
  // First make sure that non-blocking receive works as intended.
  // Set the socket to non-blocking and do a recv() - the server side
  // will send a short piece of data after sleeping for a few seconds.
  // This means we should get a EWOULDBLOCK first; then change to
  // blocking for the rest of the tests.

  u_char buffer[255];
  size_t  i;
  ssize_t len;

  if (-1 == cli_stream.enable (ACE_NONBLOCK))
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%P|%t) %p\n"),
                ACE_TEXT ("Can't enable test 0 nonblocking")));

  len = cli_stream.recv (buffer, sizeof (buffer));
  cli_stream.disable (ACE_NONBLOCK);
  if (len == -1)
    {
      if (errno == EWOULDBLOCK)
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) Nonblocking recv ok!\n")));
      else
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("(%P|%t) %p\n"),
                      ACE_TEXT ("Nonblocking recv")));
          Test_Result = 1;
        }
      cli_stream.recv (buffer, sizeof (buffer));    // Drain the sent data
    }
  else
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%P|%t) Nonblocking recv blocked\n")));
      Test_Result = 1;
    }

  //*******************   TEST 1   ******************************
  //
  // Do a iovec sendv - send the 255 byte buffer in 5 chunks.  The
  // server will verify that the correct data is sent, and that there
  // is no more and no less.

  // The server will verify that this data pattern gets there intact.

  for (i = 0; i < sizeof buffer; ++i)
    buffer[i] = static_cast<u_char> (i);

  iovec iov[5];

  iov[0].iov_base = reinterpret_cast<char *> (&buffer[0]);
  iov[0].iov_len = 50;

  iov[1].iov_base = reinterpret_cast<char *> (&buffer[50]);
  iov[1].iov_len = 25;

  iov[2].iov_base = reinterpret_cast<char *> (&buffer[75]);
  iov[2].iov_len = 150;

  iov[3].iov_base = reinterpret_cast<char *> (&buffer[225]);
  iov[3].iov_len = 29;

  iov[4].iov_base = reinterpret_cast<char *> (&buffer[254]);
  iov[4].iov_len = 1;

  len = cli_stream.sendv (iov, 5);
  if (len == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%P|%t) %p\n"),
                  ACE_TEXT ("Test 1, sendv failed")));
      Test_Result = 1;
    }
  else if (len != 255)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%P|%t) test 1 recvd %d; should be 255\n"),
                static_cast<int> (len)));

  //*******************   TEST 2   ******************************
  //
  // The same data is coming back - receive it using recv (size_t n,
  // ...)  and compare it to the original data.

  u_char buffer2[255];
  // Give it a chance to get here
  ACE_OS::sleep (2);
  len = cli_stream.recv (4,
                         buffer2,
                         150,
                         &buffer2[150],
                         105);
  if (len != 255)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%P|%t) %p; len is %d, but should be 255!\n"),
                  len));
    }

  for (i = 0; i < 255; i++)
    if (buffer2[i] != buffer[i])
      {
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("(%P|%t) Test 2, rcvd byte %d is %d, not %d\n"),
                    i, buffer2[i], buffer[i]));
        Test_Result = 1;
      }

  //*******************   TEST 3   ******************************
  //
  // Do a send_n of a large size. The receive should sleep some to
  // cause the data reception to be delayed, which will fill up the
  // TCP window and cause send_n to block at some point. The particular
  // case this tests only needs to be exercised if the socket is
  // non-blocking, so set that first.

  ssize_t sent;
  char buff[Test3_Send_Size];
  if (-1 == cli_stream.enable (ACE_NONBLOCK))
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%P|%t) %p\n"),
                ACE_TEXT ("Can't enable test 3 nonblocking")));
  for (i = 0; i < Test3_Loops; ++i)
    {
      errno = 0;
      sent = cli_stream.send_n (buff, sizeof (buff));
      if (sent != sizeof (buff) && errno != 0)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("(%P|%t) Test 3, pass %d, sent %d, %p\n"),
                      i, sent, ACE_TEXT ("error")));
          Test_Result = 1;   // Fail
        }
    }

  cli_stream.close ();

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
  // The client will be expecting to not see data for a few seconds.
  // Wait a bit then send a short blurb to complete the client's recv.

  ACE_OS::sleep (3);
  sock_str.send ("abc", 3);

  //*******************   TEST 1   ******************************
  //
  // Do a iovec recvv - the client should send 255 bytes, which we
  // will be detected and read into a ACE-allocated buffer.  Use a 5
  // second timeout to give the client a chance to send it all.

  ACE_OS::sleep (5);

  iovec iov[3];
  u_char buffer[255];
  ssize_t len;
  int i;

  iov[0].iov_base = reinterpret_cast<char *> (&buffer[0]);
  iov[0].iov_len = 75;

  iov[1].iov_base = reinterpret_cast<char *> (&buffer[75]);
  iov[1].iov_len = 100;

  iov[2].iov_base = reinterpret_cast<char *> (&buffer[175]);
  iov[2].iov_len = 80;

  len = sock_str.recvv_n (iov, 3);
  if (len == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%P|%t) %p\n"),
                  ACE_TEXT ("Test 1, recvv failed")));
      Test_Result = 1;
    }

  if (len != 255)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%P|%t) Test 1 recvd %d; should be 255\n"),
                  static_cast<int> (len)));
      Test_Result = 1;
    }
  for (i = 0; i < 255; i++)
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

  len = sock_str.send (6,
                       buffer,
                       42,
                       &buffer[42],
                       189,
                       &buffer[231],
                       24);
  if (len != 255)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%P|%t) Test 2 sent %d; should be 255\n"),
                  static_cast<int> (len)));
      Test_Result = 1;
    }

  //*******************   TEST 3   ******************************
  //
  // The sender is testing send_n to make sure it blocks if the TCP
  // window fills. So sleep here for a bit to avoid getting the data
  // yet. Then just read and empty out the received data.
  ACE_OS::sleep (8);
  // Keep reading until the peer closes.
  sock_str.disable (ACE_NONBLOCK);
  ssize_t got = 1;
  size_t total_recv = 0;
  while (got != 0)
    {
      errno = 0;
      got = sock_str.recv (buffer, sizeof (buffer));
      if (got < 0)
          break;
      total_recv += got;
    }

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) Test 3 received %d bytes\n"),
                total_recv));

  if (total_recv == Test3_Total_Size)
    {
      if (got != 0 || errno != 0)
        {
          ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%P|%t) Test 3 final recv status %d, expected 0\n"),
                got));
          ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%P|%t) %p\n"),
                      ACE_TEXT ("expected errno == 0, instead")));
          Test_Result = 1;    // Fail
        }
    }
  else
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%P|%t) Test 3 expected %d %p\n"),
                  Test3_Total_Size, ACE_TEXT ("bytes")));
      Test_Result = 1;
    }

  sock_str.close();

  return 0;
}

#endif /* !ACE_LACKS_FORK || ACE_HAS_THREADS */

static void
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
  ACE_START_TEST (ACE_TEXT ("SOCK_Send_Recv_Test"));

  spawn ();

  ACE_END_TEST;
  return Test_Result;
}
