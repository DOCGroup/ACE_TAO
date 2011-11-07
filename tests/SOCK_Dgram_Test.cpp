//=============================================================================
/**
 *  @file    SOCK_Dgram_Test.cpp
 *
 *  $Id$
 *
 *   Tests that a call to open with an any address binds to the any address
 *   for the protocol passed in.
 *
 *   This test uses the same test setup as SOCK_Test.
 *
 *
 *  @author Brian Buesker (bbuesker@qualcomm.com)
 */
//=============================================================================


#include "test_config.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_sys_wait.h"
#include "ace/Thread.h"
#include "ace/Thread_Manager.h"
#include "ace/SOCK_Dgram.h"
#include "ace/Log_Msg.h"
#include "ace/Time_Value.h"
#include "ace/OS_NS_unistd.h"

#define SERVER_PORT 20000
#define TEST_DATA ACE_TEXT ("UDP Open Test")

static void *
client (void *arg)
{
  ACE_INET_Addr *remote_addr = (ACE_INET_Addr *) arg;
  ACE_INET_Addr server_addr, peer_addr;
  ACE_SOCK_Dgram cli_dgram;
  ACE_Time_Value timeout (1);

  ACE_TCHAR buf[20];
  ACE_TCHAR hostname_string[100];

  if (remote_addr->get_type () == AF_INET)
    {
      server_addr.set (remote_addr->get_port_number (),
                       ACE_LOCALHOST,
                       1,
                       remote_addr->get_type ());
    }
#if defined (ACE_HAS_IPV6)
  else
    {
      server_addr.set (remote_addr->get_port_number(),
                       ACE_IPV6_LOCALHOST,
                       1,
                       remote_addr->get_type ());
    }
#endif /* ACE_HAS_IPV6 */

  server_addr.addr_to_string (hostname_string, 100);
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) starting client UDP send to %s\n"),
              hostname_string));

  if (cli_dgram.open (ACE_Addr::sap_any, server_addr.get_type ()) == -1)
    {
      ACE_ERROR((LM_ERROR,
                 ACE_TEXT("(%P|%t) protocol %d, %p\n"),
                 server_addr.get_type (),
                 ACE_TEXT ("SOCK_Dgram open")));
    }
  else if (cli_dgram.send (TEST_DATA, sizeof (TEST_DATA), server_addr) == -1)
    {
      ACE_ERROR((LM_ERROR,
                 ACE_TEXT("(%P|%t) UDP send to %s %p\n"),
                 hostname_string,
                 ACE_TEXT ("failed")));
    }
  else
    {
      ssize_t rcv_cnt = cli_dgram.recv (buf,
                                        sizeof (buf),
                                        peer_addr,
                                        0,
                                        &timeout);
      if (rcv_cnt == -1)
        {
          if (errno == ETIME)
          {
            ACE_ERROR((LM_ERROR,
                       ACE_TEXT("(%P|%t) UDP recv on proto %d timed out\n"),
                       server_addr.get_type ()));
          }
          else
          {
            ACE_ERROR((LM_ERROR,
                       ACE_TEXT("(%P|%t) UDP recv on proto %d %p\n"),
                       server_addr.get_type (),
                       ACE_TEXT("failed")));
          }
        }
      else
        {
          // recv() ok, check data and 'from' address
          size_t rcv_siz = static_cast<size_t> (rcv_cnt);
          if (rcv_siz != sizeof (TEST_DATA))
            {
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT ("(%P|%t) recv cnt %B; should be %B\n"),
                          rcv_siz,
                          sizeof (TEST_DATA)));
            }
          else
            {
              buf[rcv_siz] = '\0';
              if (ACE_OS::strncmp (buf, TEST_DATA, ACE_OS::strlen (TEST_DATA)))
                ACE_ERROR ((LM_ERROR,
                            ACE_TEXT ("(%P|%t) recv string should be %s; ")
                            ACE_TEXT (" but is %s\n"),
                            TEST_DATA, buf));
            }
          if (peer_addr.get_type () != server_addr.get_type ())
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("(%P|%t) recv addr type %d; should be %d\n"),
                        peer_addr.get_type (),
                        server_addr.get_type ()));
          if (peer_addr.get_size () != server_addr.get_size ())
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("(%P|%t) recv addr size %d; should be %d\n"),
                        peer_addr.get_size (),
                        server_addr.get_size ()));
        }
    }

  cli_dgram.close();

  return 0;
}

static void *
server (void *arg)
{
  ACE_SOCK_Dgram *server_dgram = (ACE_SOCK_Dgram *) arg;
  ACE_INET_Addr peer_addr;

  ACE_TCHAR buf[20];
  ACE_TCHAR hostname_string[100];
  ssize_t rcv_cnt;

  if ((rcv_cnt = server_dgram->recv (buf, sizeof (buf), peer_addr, 0)) == -1)
    {
      ACE_ERROR((LM_ERROR, ACE_TEXT("(%P|%t) %p\n"), ACE_TEXT("server recv")));
    }
  else
    {
      // recv() ok, check data and 'from' address
      size_t rcv_siz = static_cast<size_t> (rcv_cnt);
      if (rcv_siz != sizeof (TEST_DATA))
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("(%P|%t) recv cnt %B; should be %B\n"),
                      rcv_siz,
                      sizeof (TEST_DATA)));
        }
      else
        {
          buf[rcv_siz / sizeof (ACE_TCHAR)] = '\0';
          if (ACE_OS::strncmp (buf, TEST_DATA, ACE_OS::strlen (TEST_DATA)))
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("(%P|%t) recv string should be %s; ")
                        ACE_TEXT (" but is %s\n"),
                        TEST_DATA, buf));
        }

      if (0 == peer_addr.addr_to_string (hostname_string,
          sizeof (hostname_string)))
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%P|%t) Echoing data to %s\n"),
                    hostname_string));
      else
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("(%P|%t) %p\n"),
                    ACE_TEXT ("addr_to_string")));
      if (server_dgram->send (TEST_DATA, sizeof (TEST_DATA),
                              peer_addr, 0) == -1)
        {
          ACE_ERROR((LM_ERROR,
                     ACE_TEXT("(%P|%t) %p\n"),
                     ACE_TEXT("Server UDP send failed")));
        }
    }
  server_dgram->close ();

  return 0;
}

static int
spawn (int proto)
{
  ACE_SOCK_Dgram server_dgram;
  ACE_INET_Addr server_addr;

  if (proto == AF_INET)
    {
      server_addr.set (SERVER_PORT, ACE_LOCALHOST, 1, proto);
    }
#if defined (ACE_HAS_IPV6)
  else
    {
      server_addr.set (SERVER_PORT, ACE_IPV6_LOCALHOST, 1, proto);
    }
#endif /* ACE_HAS_IPV6 */

  // Bind UDP server to the appropriate port
  if (server_dgram.open (server_addr, proto) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%P|%t) %p\n"),
                         ACE_TEXT ("server dgram open")),
                         1);
    }
  else
    {
      if (server_addr.get_port_number() != SERVER_PORT)
        {
          ACE_TCHAR hostname_string[100];
          server_addr.addr_to_string (hostname_string, 100);
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT("(%P|%t) Portnumber has unexpected value of %d on host %s\n"),
                             server_addr.get_port_number(), hostname_string), 1);
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) started server at proto %d, port %d\n"),
                      proto,
                      server_addr.get_port_number ()));
#if !defined (ACE_LACKS_FORK)
          switch (ACE_OS::fork (ACE_TEXT ("child")))
            {
            case -1:
              ACE_ERROR_BREAK ((LM_ERROR,
                                ACE_TEXT ("(%P|%t) %p\n"),
                                ACE_TEXT ("fork failed")));
              /* NOTREACHED */
            case 0:
              client (&server_addr);
              ACE_OS::exit (0);
              /* NOTREACHED */
            default:
              server ((void *) &server_dgram);
              ACE_OS::wait ();
            }
#elif defined (ACE_HAS_THREADS)
          if (ACE_Thread_Manager::instance ()->spawn
              (ACE_THR_FUNC (server),
               (void *) &server_dgram,
               THR_NEW_LWP | THR_DETACHED) == -1)
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("(%P|%t) %p\n"),
                               ACE_TEXT ("thread create failed")),
                               1);

          if (ACE_Thread_Manager::instance ()->spawn
              (ACE_THR_FUNC (client),
               (void *) &server_addr,
               THR_NEW_LWP | THR_DETACHED) == -1)
            ACE_ERROR_RETURN ((LM_ERROR,
                        ACE_TEXT ("(%P|%t) %p\n"),
                        ACE_TEXT ("thread create failed")),
                        1);

          // Wait for the threads to exit.
          ACE_Thread_Manager::instance ()->wait ();
#else
          ACE_ERROR ((LM_INFO,
                      ACE_TEXT ("(%P|%t) ")
                      ACE_TEXT ("only one thread may be run ")
                      ACE_TEXT ("in a process on this platform\n")));
#endif /* ACE_HAS_THREADS */
      }

      server_dgram.close ();
    }

  return 0;
}

int run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("SOCK_Dgram_Test"));

  int retval = spawn (AF_INET);

#if defined (ACE_HAS_IPV6)

  if (retval == 0)
    {
      retval = spawn (AF_INET6);
    }

#endif /* ACE_HAS_IPV6 */

  ACE_END_TEST;
  return retval;
}
