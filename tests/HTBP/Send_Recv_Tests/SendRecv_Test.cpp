// $Id$

// ===========================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Send_Recv_Test.cpp
//
// = DESCRIPTION This is a test of the <ACE_SOCK>'s various send and
// receive methods, over HTBP.  The test forks two processes or spawns
// two threads (depending upon the platform) and then executes client
// and server allowing them to connect and exchange data in ways
// designed to exercise the send and recv functions.
//
//     Right now, it primarily tests the iov-like send and recv
//     functions, but others should be added to completely cover the
//     possible scenarios.
//
// = AUTHOR
//    Steve Huston <shuston@riverace.com>
//
// ============================================================================

#include "tests/test_config.h"

#include "ace/HTBP/HTBP_Stream.h"
#include "ace/HTBP/HTBP_Session.h"
#include "ace/HTBP/HTBP_ID_Requestor.h"
#include "ace/HTBP/HTBP_Environment.h"

#include "ace/OS.h"
#include "ace/Thread.h"
#include "ace/Thread_Manager.h"
#include "ace/SOCK_Connector.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/SOCK_Stream.h"

ACE_RCSID(tests,
          SOCK_Send_Recv_Test,
          "$Id$")

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
  ACE_OS::socket_init (ACE_WSOCK_VERSION);
  ACE::HTBP::Environment ht_env;

#if 0 // this needs to take the config file name as an argument.
  ht_env.import_config ("../HTBP_Config.conf");
#endif /* 0 */

  ACE::HTBP::ID_Requestor req (&ht_env);
  ACE::HTBP::Addr local(ACE_TEXT_ALWAYS_CHAR(req.get_HTID()));

  char hostname [128];

  if (ACE_OS::hostname (hostname,128) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "Could not get the host name\n"),0);

  ACE_INET_Addr *remote_addr = reinterpret_cast<ACE_INET_Addr *> (arg);
  ACE::HTBP::Addr remote (remote_addr->get_port_number (),hostname);

  unsigned pport;
  ACE_TString phost;
  ht_env.get_proxy_port(pport);
  ht_env.get_proxy_host(phost);

  ACE_INET_Addr proxy(pport,phost.c_str());
  ACE::HTBP::Session session(remote,local,ACE::HTBP::Session::next_session_id(),&proxy);
  ACE::HTBP::Stream stream(&session);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) Connecting to port %d\n"),
              remote.get_port_number()));

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) connected to %s\n"),
              ACE_TEXT_CHAR_TO_TCHAR(remote.get_host_name ())));

  //*******************   TEST 1   ******************************
  //
  // Do a iovec sendv - send the 255 byte buffer in 5 chunks.  The
  // server will verify that the correct data is sent, and that there
  // is no more and no less.

  u_char buffer[255];
  size_t  i;
  ssize_t len;

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

  len = stream.sendv (iov, 5);
  if (len == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%P|%t) %p\n"),
                  ACE_TEXT ("Test 1, sendv failed")));
      Test_Result = 1;
    }
  else
    ACE_ASSERT (len == 255);


  ACE_DEBUG ((LM_DEBUG, "*****  client TEST 2 ***** \n"));

  //*******************   TEST 2   ******************************
  //
  // The same data is coming back - receive it using recv (size_t n,
  // ...)  and compare it to the original data.

  u_char buffer2[255];
  // Give it a chance to get here
  ACE_OS::sleep (2);

  len = stream.recv_n (buffer2,
                       155);


  len = stream.recv_n (buffer2,
                       105);

  for (i = 0; i < 255; i++)
    if (buffer2[i] != buffer[i])
      {
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("(%P|%t) Test 2, rcvd byte %d is %d, not %d\n"),
                    i, buffer2[i], buffer[i]));
        Test_Result = 1;
      }


  stream.close ();

  return 0;
}

static void *
server (void *arg)
{

  ACE_OS::socket_init (ACE_WSOCK_VERSION);
  ACE_SOCK_Acceptor *peer_acceptor = (ACE_SOCK_Acceptor *) arg;
  ACE_INET_Addr cli_addr;

  ACE_Time_Value timeout (ACE_DEFAULT_TIMEOUT);

  ACE_SOCK_Stream sock[2];
  ACE_DEBUG ((LM_DEBUG,"server is ready\n"));

  if (peer_acceptor->accept(sock[0],&cli_addr,&timeout) == -1)
    {
        ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%P|%t) %p\n"),
                  ACE_TEXT ("accept")));
      Test_Result = 1;
      return 0;
    }

  ACE::HTBP::Channel channel1(sock[0]);
  ACE_DEBUG ((LM_DEBUG,"Got sock[0], handle = %d\n",sock[0].get_handle()));

  if (peer_acceptor->accept(sock[0],&cli_addr,&timeout) == -1)
    {
        ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%P|%t) %p\n"),
                  ACE_TEXT ("accept")));
      Test_Result = 1;
      return 0;
    }

  ACE::HTBP::Channel channel2(sock[1]);
  ACE_DEBUG ((LM_DEBUG,"Got sock[1], handle = %d\n",sock[1].get_handle()));
  int res = 0;
  while ((res = channel1.pre_recv()) != 0)
    {
      ACE_DEBUG ((LM_DEBUG,"res = %d. waiting 1 sec. %p\n",res,
                  "stream.pre_recv()"));
      ACE_OS::sleep (1);
    }

  ACE_DEBUG ((LM_DEBUG,"Read from channel2\n"));
  while ((res = channel2.pre_recv()) != 0)
    {
      ACE_DEBUG ((LM_DEBUG,"res = %d, waiting 1 sec. %p\n",res,
                  "stream2.pre_recv()"));
      ACE_OS::sleep (1);
    }

  ACE::HTBP::Session *session = channel1.session();
  ACE::HTBP::Stream stream (session);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) client %s connected from %d\n"),
              ACE_TEXT_CHAR_TO_TCHAR(cli_addr.get_host_name ()),
              cli_addr.get_port_number ()));

  //*******************   TEST 1   ******************************
  //
  // Do a iovec recvv - the client should send 255 bytes, which we
  // will be detected and read into a ACE-allocated buffer.  Use a 5
  // second timeout to give the client a chance to send it all.

  ACE_OS::sleep (5);

  u_char buffer[255];

  iovec iov[3];

  ssize_t len;
  int i;

  iov[0].iov_base = reinterpret_cast<char *> (&buffer[0]);
  iov[0].iov_len = 75;

  iov[1].iov_base = reinterpret_cast<char *> (&buffer[75]);
  iov[1].iov_len = 100;

  iov[2].iov_base = reinterpret_cast<char *> (&buffer[175]);
  iov[2].iov_len = 80;

  len = stream.recvv (iov, 3);
  if (len == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%P|%t) %p\n"),
                  ACE_TEXT ("Test 1, recvv failed")));
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

  ACE_DEBUG ((LM_DEBUG, "*****  TEST 2 ***** \n"));

  //*******************   TEST 2   ******************************
  //
  // Send the buffer back, using send (size_t n, ...) in 3 pieces.

  len = stream.send (buffer, 6);
  len += stream.send (buffer,42);
  len += stream.send (buffer,189);
  len += stream.send_n (buffer,18);
  ACE_ASSERT (len == 255);
  sock[0].close();
  sock[1].close();
  stream.close ();
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

  char hostname[BUFSIZ];

  if (ACE_OS::hostname (hostname, BUFSIZ) != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Could not get the hostname\n")));
    }

  ACE::HTBP::Addr addr (8088, hostname);

  // Bind listener to any port and then find out what the port was.
  if (peer_acceptor.open (addr) == -1
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
      switch (ACE_OS::fork (ACE_TEXT("child")))
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
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("SOCK_Send_Recv_Test"));

  spawn ();

  ACE_END_TEST;
  return Test_Result;
}
