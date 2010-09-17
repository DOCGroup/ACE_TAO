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

#include "ace/HTBP/HTBP_Stream.h"
#include "ace/HTBP/HTBP_Session.h"
#include "ace/HTBP/HTBP_ID_Requestor.h"
#include "ace/HTBP/HTBP_Environment.h"

#include "ace/Thread.h"
#include "ace/Thread_Manager.h"
#include "ace/SOCK_Connector.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/SOCK_Stream.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_sys_socket.h"
#include "ace/os_include/os_netdb.h"

// Change to non-zero if test fails
static int Test_Result = 0;

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
unsigned port = 8088;
const ACE_TCHAR *notifier_file = 0;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:p:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        notifier_file = get_opts.opt_arg();
        break;
      case 'p':
        port = static_cast<unsigned>(ACE_OS::atoi (get_opts.opt_arg()));
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-p port "
                            "\n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_OS::socket_init (ACE_WSOCK_VERSION);

  if (parse_args(argc, argv) != 0)
    return 1;

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

  ACE::HTBP::Addr addr (port, hostname);

  // Bind listener to any port and then find out what the port was.
  if (peer_acceptor.open (addr) == -1
      || peer_acceptor.get_local_addr (server_addr) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                ACE_TEXT ("(%P|%t) %p\n"),
                ACE_TEXT ("open")), 1);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) starting server at port %d\n"),
              server_addr.get_port_number ()));

  if (notifier_file != 0)
    {
      FILE *f = ACE_OS::fopen (notifier_file,ACE_TEXT("w+"));
      const char *msg = "server ready";
      ACE_OS::fwrite (msg,ACE_OS::strlen(msg),1,f);
      ACE_OS::fclose (f);
    }

  ACE_INET_Addr cli_addr;

  ACE_SOCK_Stream sock_stream[2];
  ACE_DEBUG ((LM_DEBUG,"(%P) server is ready\n"));

  if (peer_acceptor.accept(sock_stream[0],&cli_addr,0) == -1)
    {
        ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%P|%t) %p\n"),
                  ACE_TEXT ("accept")));
      return 1;
    }

  ACE::HTBP::Channel channel1(sock_stream[0]);
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT("(%P) Got sock[0], handle = %d\n"),
                       sock_stream[0].get_handle()));

  if (peer_acceptor.accept(sock_stream[1],&cli_addr,0) == -1)
    {
        ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%P|%t) %p\n"),
                  ACE_TEXT ("accept")));
      return 1;
    }

  ACE::HTBP::Channel channel2(sock_stream[1]);
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT("(%P) Got sock_stream[1], handle = %d\n"),
              sock_stream[1].get_handle()));
  int res = 0;
  while ((res = channel1.pre_recv()) != 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT("(%P)res = %d. waiting 1 sec. %p\n"),
                  res,
                  ACE_TEXT("stream.pre_recv()")));
      ACE_OS::sleep (1);
    }

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT("(%P)Read from channel2\n")));
  while ((res = channel2.pre_recv()) != 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT("(%P)res = %d, waiting 1 sec. %p\n"),res,
                  ACE_TEXT("stream2.pre_recv()")));
      ACE_OS::sleep (1);
    }

  ACE::HTBP::Session *session = channel1.session();
  ACE::HTBP::Stream stream (session);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) client %C connected from %d\n"),
              cli_addr.get_host_name (),
              cli_addr.get_port_number ()));

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT("(%P) *****  server TEST 1 *****\n")));
  //*******************   TEST 1   ******************************
  //
  // Do a iovec recvv - the client should send 255 bytes, which we
  // will be detected and read into a ACE-allocated buffer.  Use a 5
  // second timeout to give the client a chance to send it all.

  ACE_OS::sleep (2);

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

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT("(%P) server, Test 1: recvd len = %d\n"),len));

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

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT("(%P) ***** server  TEST 2 *****\n")));

  //*******************   TEST 2   ******************************
  //
  // Send the buffer back, using send (size_t n, ...) in 3 pieces.

  len = stream.send (buffer, 6);
  len += stream.send (buffer + 6,42);
  len += stream.send (buffer + 48,189);
  len += stream.send (buffer + 237,18);
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT("(%P) server sent len=%d\n"),len));
  //  ACE_OS::sleep(10);
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT("(%P) flushing outbound queue\n")));

  while ((res = channel1.pre_recv()) != 0 &&
         (res = channel2.pre_recv()) != 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT("(%P)res = %d. waiting 1 sec. %p\n"),
                  res,
                  ACE_TEXT("stream.pre_recv()")));
      ACE_OS::sleep (1);
    }
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT("(%P)Read from channel2\n")));

  int result = session->flush_outbound_queue();

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT("(%P) server: shutting down, flush returned %d\n"), result));

  ACE_OS::sleep(1); // prevent test failure on windows when the connection
                    // closes too fast.

  sock_stream[0].close();
  sock_stream[1].close();
  stream.close ();

  peer_acceptor.close ();


  if (len != 255)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT("(%P} server: send result %d != 255\n"), len));
      return 1;
    }

  return Test_Result;
}
