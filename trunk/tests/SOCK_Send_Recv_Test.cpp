// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    SOCK_Send_Recv_Test.cpp
//
// = DESCRIPTION
//     This is a test of the <ACE_SOCK>'s various send and receive methods.
//     The test forks two processes or spawns two threads (depending upon
//     the platform) and then executes client and server allowing them to
//     connect and exchange data in ways designed to exercise the send
//     and recv functions.
//
//     Right now, it primarily tests the iov-like send and recv functions,
//     but others should be added to completely cover the possible
//     scenarios.
//
// = AUTHOR
//    Steve Huston
//
// ============================================================================

#include "test_config.h"
#include "ace/OS.h"
#include "ace/Thread.h"
#include "ace/Thread_Manager.h"
#include "ace/SOCK_Connector.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/SOCK_Stream.h"

ACE_RCSID(tests, SOCK_Send_Recv_Test, "$Id$")

#if defined(__BORLANDC__) && __BORLANDC__ >= 0x0530
USELIB("..\ace\aced.lib");
//---------------------------------------------------------------------------
#endif /* defined(__BORLANDC__) && __BORLANDC__ >= 0x0530 */

static int Test_Result = 0;        // Change to non-zero if test fails


static void *
client (void *arg)
{
  ACE_INET_Addr *    remote_addr = (ACE_INET_Addr *) arg;
  ACE_INET_Addr      server_addr (remote_addr->get_port_number (),
                                  ASYS_TEXT ("localhost"));
  ACE_SOCK_Stream    cli_stream;
  ACE_SOCK_Connector con;
  ACE_Time_Value     timeout (ACE_DEFAULT_TIMEOUT);

  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("(%P|%t) Connecting to port %d\n"),
              server_addr.get_port_number()));

  // Initiate connection with server; don't wait forever
  if (con.connect (cli_stream, server_addr, &timeout) == -1)
    {
      ACE_ERROR ((LM_ERROR, ASYS_TEXT ("(%P|%t) %p\n"),
                  ASYS_TEXT ("connection failed")));
      Test_Result = 1;
      return 0;
    }

  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("(%P|%t) connected to %s\n"),
              server_addr.get_host_name ()));

  //*******************   TEST 1   ******************************
  //
  // Do a iovec sendv - send the 255 byte buffer in 5 chunks.
  // The server will verify that the correct data is sent, and that
  // there is no more and no less.

  unsigned char buff[255];
  unsigned int i;
  ssize_t len;

  // The server will verify that this data pattern gets there intact.
  for (i = 0; i < sizeof(buff); buff[i] = i, ++i)
    ;

  iovec iov[5];

  iov[0].iov_base = ACE_static_cast(char *, &buff[0]);
  iov[0].iov_len = 50;

  iov[1].iov_base = ACE_static_cast(char *, &buff[50]);
  iov[1].iov_len = 25;

  iov[2].iov_base = ACE_static_cast(char *, &buff[75]);
  iov[2].iov_len = 150;

  iov[3].iov_base = ACE_static_cast(char *, &buff[225]);
  iov[3].iov_len = 29;

  iov[4].iov_base = ACE_static_cast(char *, &buff[254]);
  iov[4].iov_len = 1;

  len = cli_stream.sendv (iov, 5);
  if (len == -1)
    {
      ACE_ERROR ((LM_ERROR, ASYS_TEXT ("(%P|%t) %p\n"),
                  ASYS_TEXT ("Test 1, sendv failed")));
      Test_Result = 1;
    }
  else
    ACE_ASSERT (len == 255);

  //*******************   TEST 2   ******************************
  //
  // The same data is coming back - receive it using recv (size_t n, ...)
  // and compare it to the original data.

  unsigned char buff2[255];
  ACE_OS::sleep(2);         // Give it a chance to get here
  len = cli_stream.recv (4, buff2, 150, &buff2[150], 105);
  ACE_ASSERT (len == 255);
  for (i = 0; i < 255; i++)
    if (buff2[i] != buff[i]) {
      ACE_ERROR ((LM_ERROR,
                  ASYS_TEXT ("(%P|%t) Test 2, rcvd byte %d is %d, not %d\n"),
                  i, buff2[i], buff[i]));
      Test_Result = 1;
    }

  cli_stream.close ();

  return 0;
}

static void *
server (void *arg)
{
  ACE_SOCK_Acceptor *peer_acceptor = (ACE_SOCK_Acceptor *) arg;
  ACE_SOCK_Stream    sock_str;
  ACE_INET_Addr      cli_addr;
  ACE_Time_Value     timeout (ACE_DEFAULT_TIMEOUT);

  // Accept the connection over which the stream tests will run.  Don't
  // lock up if client doesn't connect
  if (peer_acceptor->accept (sock_str, &cli_addr, &timeout) == -1)
    {
      ACE_ERROR ((LM_ERROR, ASYS_TEXT ("(%P|%t) %p\n"), ASYS_TEXT ("accept")));
      Test_Result = 1;
      return 0;
    }

  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("(%P|%t) client %s connected from %d\n"),
              cli_addr.get_host_name (), cli_addr.get_port_number ()));

  //*******************   TEST 1   ******************************
  //
  // Do a iovec recvv - the client should send 255 bytes, which we
  // will be detected and read into a ACE-allocated buffer.  Use a
  // 5 second timeout to give the client a chance to send it all.

  iovec iov[3];
  unsigned char buff[255];
  ssize_t len;
  int i;

  iov[0].iov_base = ACE_static_cast(char *, &buff[0]);
  iov[0].iov_len = 75;

  iov[1].iov_base = ACE_static_cast(char *, &buff[75]);
  iov[1].iov_len = 100;

  iov[2].iov_base = ACE_static_cast(char *, &buff[175]);
  iov[2].iov_len = 80;
  
  len = sock_str.recvv_n (iov, 3);
  if (len == -1)
    {
      ACE_ERROR ((LM_ERROR, ASYS_TEXT ("(%P|%t) %p\n"),
                  ASYS_TEXT ("Test 1, recvv failed")));
      Test_Result = 1;
    }
  ACE_ASSERT (len == 255);
  for (i = 0; i < 255; i++)
    if (buff[i] != i)
      {
        ACE_ERROR ((LM_ERROR,
                    ASYS_TEXT ("(%P|%t) Test 1, rcvd byte %d is %d, not %d\n"),
                    i, buff[i], i));
        Test_Result = 1;
      }

  //*******************   TEST 2   ******************************
  //
  // Send the buffer back, using send (size_t n, ...) in 3 pieces.

  len = sock_str.send (6, buff, 42, &buff[42], 189, &buff[231], 24);
  ACE_ASSERT (len == 255);

  sock_str.close();

  return 0;
}

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
    ACE_ERROR ((LM_ERROR, ASYS_TEXT ("(%P|%t) %p\n"), ASYS_TEXT ("open")));
  else
    {
      ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("(%P|%t) starting server at port %d\n"),
                  server_addr.get_port_number ()));

#if !defined (ACE_LACKS_FORK)
      switch (ACE_OS::fork ("child"))
        {
        case -1:
          ACE_ERROR ((LM_ERROR, ASYS_TEXT ("(%P|%t) %p\n%a"), ASYS_TEXT ("fork failed")));
          /* NOTREACHED */
        case 0:
          client (&server_addr);
          exit (0);
          /* NOTREACHED */
        default:
          server ((void *) &peer_acceptor);
          ACE_OS::wait ();
        }
#elif defined (ACE_HAS_THREADS)
      if (ACE_Thread_Manager::instance ()->spawn
          (ACE_THR_FUNC (server), (void *) &peer_acceptor, THR_NEW_LWP | THR_DETACHED) == -1)
        ACE_ERROR ((LM_ERROR, ASYS_TEXT ("(%P|%t) %p\n%a"), ASYS_TEXT ("thread create failed")));

      if (ACE_Thread_Manager::instance ()->spawn
          (ACE_THR_FUNC (client), (void *) &server_addr, THR_NEW_LWP | THR_DETACHED) == -1)
        ACE_ERROR ((LM_ERROR, ASYS_TEXT ("(%P|%t) %p\n%a"), ASYS_TEXT ("thread create failed")));

      // Wait for the threads to exit.
      ACE_Thread_Manager::instance ()->wait ();
#else
      ACE_ERROR ((LM_ERROR, ASYS_TEXT ("(%P|%t) only one thread may be run in a process on this platform\n%a"), 1));
#endif /* ACE_HAS_THREADS */

      peer_acceptor.close ();
    }
}

int
main (int, ASYS_TCHAR *[])
{
  ACE_START_TEST (ASYS_TEXT ("SOCK_Send_Recv_Test"));

  spawn ();

  ACE_END_TEST;
  return Test_Result;
}
