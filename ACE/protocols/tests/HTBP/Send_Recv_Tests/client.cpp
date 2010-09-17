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
#include "ace/OS_NS_sys_socket.h"

// Change to non-zero if test fails
static int Test_Result = 0;

const size_t Test3_Send_Size = 4*1024;
const size_t Test3_Loops = 10;
const size_t Test3_Total_Size = Test3_Send_Size * Test3_Loops;

const ACE_TCHAR * remote_host = 0;
const ACE_TCHAR * config_file = 0;
unsigned remote_port = 8088;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("p:h:c:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'p':
        remote_port = static_cast<unsigned>(ACE_OS::atoi (get_opts.opt_arg()));
        break;
      case 'h':
        remote_host = get_opts.opt_arg ();
        break;
      case 'c':
        config_file = get_opts.opt_arg ();
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage:  %s ")
                           ACE_TEXT ("-h remote_host ")
                           ACE_TEXT ("-p remote_port ")
                           ACE_TEXT ("-c config_file ")
                           ACE_TEXT ("\n"),
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
  if (remote_host == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Client: No remote host specified\n")),1);

  ACE::HTBP::Environment env;
  if (config_file != 0)
    env.import_config (config_file);

  ACE::HTBP::ID_Requestor req (&env);
  ACE::HTBP::Addr local(ACE_TEXT_ALWAYS_CHAR(req.get_HTID()));

  unsigned proxy_port = 0;
  ACE_TString proxy_host;

  if (env.get_proxy_port(proxy_port) != 0 ||
      env.get_proxy_host(proxy_host) != 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT("(%P|%t) Client: ")
                  ACE_TEXT("no proxy address in ")
                  ACE_TEXT("config, using direct connect\n")));
      proxy_port = remote_port;
      proxy_host = remote_host;
    }

  ACE_INET_Addr proxy(proxy_port,proxy_host.c_str());
  ACE::HTBP::Addr remote (remote_port,
                          ACE_TEXT_ALWAYS_CHAR(remote_host));

  ACE::HTBP::Session session(remote,
                             local,
                             ACE::HTBP::Session::next_session_id(),
                             &proxy);

  ACE::HTBP::Stream stream(&session);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) Connecting to port %d\n"),
              remote.get_port_number()));

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) connected to %C\n"),
              remote.get_host_name ()));

   ACE_DEBUG ((LM_DEBUG, "(%P) *****  client TEST 1 *****\n"));

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
  ACE_DEBUG ((LM_DEBUG,"(%P) after send, len = %d\n"));
  if (len == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%P|%t) %p\n"),
                  ACE_TEXT ("Test 1, sendv failed")));
      Test_Result = 1;
    }
  else
    if (len != 255)
      {
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("(%P|%t) %p\n"),
                    ACE_TEXT ("Test 1, len = %d != 255\n"), len));
          Test_Result = 1;
      }

  //  ACE_OS::sleep (10);
  ACE_DEBUG ((LM_DEBUG, "(%P) *****  client TEST 2 *****\n"));

  //*******************   TEST 2   ******************************
  //
  // The same data is coming back - receive it using recv (size_t n,
  // ...)  and compare it to the original data.

  u_char buffer2[255];

  ssize_t total = 0;
  do {
    len = stream.recv (buffer2+total, 145 - total);
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT("(%P) Test 2: want %d bytes, got %d\n"),
                145 - total,len));
    if (len == -1 || errno == EWOULDBLOCK)
      ACE_OS::sleep (1);
    else
      total += len;
  } while ((len == -1 &&  errno == EWOULDBLOCK) || total < 145);

    if (total != 145)
      Test_Result = 1;

  len = stream.recv (buffer2 + total, 110);
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT("(%P) Test 2: second read want 110 bytes, got %d\n"),
              len));

  if (len != 110)
    Test_Result = 1;

  for (i = 0; Test_Result == 0 && i < 255; i++)
    if (buffer2[i] != buffer[i])
      {
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("(%P|%t) Test 2, rcvd byte %d is %d, not %d\n"),
                    i, buffer2[i], buffer[i]));
        Test_Result = 1;
      }


  stream.close ();

  return Test_Result;
}
